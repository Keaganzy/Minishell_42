#!/bin/bash

# Minishell Comprehensive Tester - ALL Test Cases + Proper Leak Detection
# Tests from evalsheet + GitHub comprehensive document
# Usage: ./minishell_tester.sh ./minishell

MINISHELL="${1:-.}/minishell"
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
LOG_FILE="${SCRIPT_DIR}/minishell_test_results.log"
TMP_LOG="${SCRIPT_DIR}/.tmp_log"

if [ -n "$1" ] && [ -f "$1" ]; then
    MINISHELL="$1"
fi

if [ ! -f "$MINISHELL" ]; then
    echo "Error: minishell not found at $MINISHELL"
    exit 1
fi

if ! command -v valgrind &> /dev/null; then
    USE_VALGRIND=0
else
    USE_VALGRIND=1
fi

PASS=0
FAIL=0
MEM_LEAK=0
FD_LEAK=0
TESTS=0

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
ORANGE='\033[0;33m'
BOLD='\033[1m'
NC='\033[0m'

write_output() {
    local colored="$1"
    local plain="$2"
    echo -e "$colored"
    echo "$plain" | sed 's/\x1b\[[0-9;]*m//g' >> "$TMP_LOG"
}

clean_output() {
    echo "$1" | sed '1d' | sed 's/MS\$ $//g' | sed -e :a -e '/^\s*$/d;N;ba'
}

check_leaks() {
    local valgrind_output="$1"

    # Extract memory leak amounts
    local mem_lost=$(echo "$valgrind_output" | grep -E "definitely lost:|indirectly lost:" | awk '{print $4}' | grep -v "0" || true)

    # Extract FD count - looking for "FILE DESCRIPTORS: X open at exit"
    local fd_count=$(echo "$valgrind_output" | grep "FILE DESCRIPTORS:" | grep -oE "[0-9]+ open" | grep -oE "^[0-9]+" || echo "3")

    local mem_status=0
    local fd_status=0

    # Check memory leaks
    if [ -n "$mem_lost" ]; then
        mem_status=1
        MEM_LEAK=$((MEM_LEAK + 1))
    fi

    # Check FD leaks (anything more than 3 is a leak)
    if [ "$fd_count" -gt 3 ] 2>/dev/null; then
        fd_status=1
        FD_LEAK=$((FD_LEAK + 1))
    fi

    if [ $mem_status -eq 1 ] || [ $fd_status -eq 1 ]; then
        local leak_msg=""
        [ $mem_status -eq 1 ] && leak_msg="✗ MEMORY LEAK ($mem_lost)"
        [ $fd_status -eq 1 ] && leak_msg="$leak_msg | ✗ FD LEAK (FDs: $fd_count)"
        echo "$leak_msg"
        return 1
    else
        echo "✓ No leaks (FDs: $fd_count)"
        return 0
    fi
}

test_cmd() {
    local command="$1"
    TESTS=$((TESTS + 1))

    write_output "  ${BLUE}Test:${NC} $command" "Test: $command"

    local ms_output_raw=$(echo "$command" | "$MINISHELL" 2>&1)
    local ms_exit=$?
    local ms_output=$(clean_output "$ms_output_raw")

    local bash_output=$(echo "$command" | bash 2>&1)
    local bash_exit=$?

    local expected_display="$bash_output"
    local actual_display="$ms_output"

    if [ ${#expected_display} -gt 100 ]; then
        expected_display="${expected_display:0:97}..."
    fi
    if [ ${#actual_display} -gt 100 ]; then
        actual_display="${actual_display:0:97}..."
    fi

    write_output "    ${YELLOW}Expected:${NC} '$expected_display'" "    Expected: '$bash_output'"
    write_output "    ${YELLOW}Got:${NC}      '$actual_display'" "    Got:      '$ms_output'"
    write_output "    ${YELLOW}Exit:${NC} Expected $bash_exit | Got $ms_exit" "    Exit: Expected $bash_exit | Got $ms_exit"

    local leak_status="N/A"
    if [ $USE_VALGRIND -eq 1 ]; then
        local supp_file="${SCRIPT_DIR}/readline.supp"
        [ ! -f "$supp_file" ] && supp_file="/dev/null"

        local valgrind_output=$(echo "$command" | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions="$supp_file" "$MINISHELL" 2>&1)
        leak_status=$(check_leaks "$valgrind_output")
    fi

    write_output "    ${YELLOW}Memory/FD:${NC} $leak_status" "    Memory/FD: $leak_status"

    if [ "$ms_output" = "$bash_output" ] && [ "$ms_exit" = "$bash_exit" ]; then
        write_output "    ${GREEN}✓ PASS${NC}" "    ✓ PASS"
        PASS=$((PASS + 1))
    else
        write_output "    ${RED}✗ FAIL${NC}" "    ✗ FAIL"
        FAIL=$((FAIL + 1))
    fi

    write_output "" ""
}

> "$TMP_LOG"

write_output "${BLUE}╔════════════════════════════════════════════════════════════╗${NC}" "═════════════════════════════════════════════════════════════"
write_output "${BLUE}║${NC}  ${BOLD}MINISHELL TESTER - COMPREHENSIVE${NC}${BLUE}                      ║${NC}" "   MINISHELL TESTER - COMPREHENSIVE"
write_output "${BLUE}║${NC}  From evalsheet + GitHub tests (100+ cases)${BLUE}            ║${NC}" "   From evalsheet + GitHub tests (100+ cases)"
if [ $USE_VALGRIND -eq 1 ]; then
    write_output "${BLUE}║${NC}  Valgrind: ${GREEN}ENABLED${NC} (proper leak detection)${BLUE}             ║${NC}" "   Valgrind: ENABLED (proper leak detection)"
else
    write_output "${BLUE}║${NC}  Valgrind: ${RED}DISABLED${NC}${BLUE}                                    ║${NC}" "   Valgrind: DISABLED"
fi
write_output "${BLUE}╚════════════════════════════════════════════════════════════╝${NC}" "═════════════════════════════════════════════════════════════"
write_output "" ""

# ========== BASIC TESTS ==========
write_output "${YELLOW}=== BASIC TESTS ===${NC}" "=== BASIC TESTS ==="
test_cmd "pwd"
test_cmd "echo hello"
test_cmd "ls /tmp"
test_cmd ""

# ========== ECHO COMMAND ==========
write_output "${YELLOW}=== ECHO COMMAND ===${NC}" "=== ECHO COMMAND ==="
test_cmd "echo hello"
test_cmd "echo -n test"
test_cmd "echo -n aaa | cat -e"
test_cmd "echo \"hello world\""
test_cmd "echo 'hello world'"
test_cmd "echo"
test_cmd "echo 1 2 3 4 5"

# ========== EXIT COMMAND ==========
write_output "${YELLOW}=== EXIT COMMAND (Github 1b, 2e-2h) ===${NC}" "=== EXIT COMMAND ==="
test_cmd "exit 0"
test_cmd "exit 42"
test_cmd "exit 255"
test_cmd "exit a a"
test_cmd "exit 1 2 3"
test_cmd "exit -1"

# ========== QUOTE TOKENIZATION (Github 1f-1m) ==========
write_output "${YELLOW}=== QUOTE TOKENIZATION (Github 1f-1m) ===${NC}" "=== QUOTE TOKENIZATION ==="
test_cmd "echo \"IF YOU TOKENIZED THIS >> > < << OR THIS | || && ' ' you dum\""
test_cmd "echo 'IF YOU TOKENIZED THIS >> > < << OR THIS | || && \" \" ur n \$idot'"
test_cmd "echo \"    THIS SHOULD NOT TOUCH THE LEFT BOUNDARY\""
test_cmd "echo LEFTHAND'            'RIGHTHAND"
test_cmd "ls \"\" ''"
test_cmd "echo \$notinenv"
test_cmd "echo '\$BetterNotHaveAFileWithDollar'"

# ========== EXPORT & INVALID KEYS (Github 1n) ==========
write_output "${YELLOW}=== EXPORT & ENVIRONMENT (Github 1n-1p) ===${NC}" "=== EXPORT & ENVIRONMENT ==="
test_cmd "export VALID=value"
test_cmd "export 1=invalid"
test_cmd "export !=invalid"
test_cmd "export"
test_cmd "env | wc -l"

# ========== CD AND PWD (Github 1q-1t) ==========
write_output "${YELLOW}=== CD AND PWD (Github 1q-1t) ===${NC}" "=== CD AND PWD ==="
test_cmd "cd /tmp && pwd"
test_cmd "cd /nonexistent"
test_cmd "cd ."
test_cmd "cd .."
test_cmd "cd ../../ && pwd"
test_cmd "cd /./././ && pwd"

# ========== REDIRECTIONS (Github 1H-1I, 1A-1D) ==========
write_output "${YELLOW}=== REDIRECTIONS (Github 1H-1I, 1A-1D) ===${NC}" "=== REDIRECTIONS ==="
test_cmd "echo a 1 > /tmp/ms_h.txt && cat /tmp/ms_h.txt"
test_cmd "echo a 1>/tmp/ms_i.txt && cat /tmp/ms_i.txt"
test_cmd "echo hello > /tmp/ms_out.txt && cat /tmp/ms_out.txt"
test_cmd "cat < /etc/hostname"
test_cmd "echo line1 >> /tmp/ms_app.txt && echo line2 >> /tmp/ms_app.txt && cat /tmp/ms_app.txt"
test_cmd "echo test > /tmp/ms_a > /tmp/ms_b && cat /tmp/ms_b"

# ========== PIPES (Github 1J-1L, 2b-2d) ==========
write_output "${YELLOW}=== PIPES (Github 1J-2d) ===${NC}" "=== PIPES ==="
test_cmd "echo A | cat"
test_cmd "echo hello | cat | cat"
test_cmd "echo test | grep test"
test_cmd "ls /tmp | grep tmp"
test_cmd "cat | cat | cat"

# ========== LOGICAL OPERATORS (Github 1P-1R) ==========
write_output "${YELLOW}=== LOGICAL OPERATORS (Github 1P-1R) ===${NC}" "=== LOGICAL OPERATORS ==="
test_cmd "echo a || echo b && echo c"
test_cmd "echo a && echo b || echo c"
test_cmd "true && true"
test_cmd "true && false"
test_cmd "false || true"
test_cmd "true || false && false"
test_cmd "false && false || false && true"

# ========== ECHO FLAG VARIANTS (Github 2a) ==========
write_output "${YELLOW}=== ECHO FLAG VARIANTS (Github 2a) ===${NC}" "=== ECHO FLAG VARIANTS ==="
test_cmd "echo -nnnnnn test"
test_cmd "echo -n -n -n test"
test_cmd "echo -nnnnnnn -n -n konichiwa"

# ========== SPACE HANDLING (Github 2w-2z) ==========
write_output "${YELLOW}=== SPACE HANDLING (Github 2w-2z) ===${NC}" "=== SPACE HANDLING ==="
test_cmd "echo a    b"
test_cmd "echo \"  hello  world  \""
test_cmd "echo '   multiple   spaces   '"

# ========== NULL EXPANSION (Github 2A-2B) ==========
write_output "${YELLOW}=== NULL EXPANSION (Github 2A-2B) ===${NC}" "=== NULL EXPANSION ==="
test_cmd "echo a\$notinenvDidThisDisappear?"
test_cmd "echo a\"\$notinenv\"DidThisDisappear?"

# ========== RETURN VALUE TESTS ==========
write_output "${YELLOW}=== RETURN VALUE TESTS ===${NC}" "=== RETURN VALUE TESTS ==="
test_cmd "true"
test_cmd "false"
test_cmd "ls /tmp"
test_cmd "ls /nonexistent"
test_cmd "nonexistent_cmd"

# ========== STDERR REDIRECT ==========
write_output "${YELLOW}=== STDERR REDIRECT ===${NC}" "=== STDERR REDIRECT ==="
test_cmd "ls /nonexistent 2> /tmp/ms_err.txt && cat /tmp/ms_err.txt"
test_cmd "cat < /nonexistent"

# ========== PIPE WITH REDIRECT ==========
write_output "${YELLOW}=== PIPE WITH REDIRECT ===${NC}" "=== PIPE WITH REDIRECT ==="
test_cmd "echo hello | cat > /tmp/ms_pipe.txt && cat /tmp/ms_pipe.txt"
test_cmd "cat < /etc/hostname | cat"

# ========== COMPLEX COMBINATIONS ==========
write_output "${YELLOW}=== COMPLEX COMBINATIONS ===${NC}" "=== COMPLEX COMBINATIONS ==="
test_cmd "echo a && echo b && echo c"
test_cmd "false || false || true"
test_cmd "true && false || true"
test_cmd "ls | grep tmp && echo found"

# ========== SYNTAX ERRORS ==========
write_output "${YELLOW}=== SYNTAX ERRORS ===${NC}" "=== SYNTAX ERRORS ==="
test_cmd "| cat"
test_cmd "&&"
test_cmd "||"
test_cmd "echo test |"
test_cmd "echo test &&"
test_cmd "< > cat"

# ========== HEREDOC ==========
write_output "${YELLOW}=== HEREDOC (Github 1E) ===${NC}" "=== HEREDOC ==="
test_cmd "cat << EOF
hello world
EOF"

# ========== MULTIPLE ARGUMENT VARIATIONS ==========
write_output "${YELLOW}=== MULTIPLE ARGUMENTS ===${NC}" "=== MULTIPLE ARGUMENTS ==="
test_cmd "echo 1 2 3 4 5 6 7 8 9 10"
test_cmd "ls /tmp /etc"

# ========== UNSET ==========
write_output "${YELLOW}=== UNSET ===${NC}" "=== UNSET ==="
test_cmd "unset TESTVAR"
test_cmd "export VAR=test && unset VAR"

# Cleanup
rm -f /tmp/ms_*.txt

# Summary
write_output "${BLUE}╔════════════════════════════════════════════════════════════╗${NC}" "═════════════════════════════════════════════════════════════"
write_output "${BLUE}║${NC}                    ${BOLD}SUMMARY${NC}${BLUE}                             ║${NC}" "                         SUMMARY"
write_output "${BLUE}╠════════════════════════════════════════════════════════════╣${NC}" "═════════════════════════════════════════════════════════════"
write_output "${BLUE}║${NC} Total Passed:     ${GREEN}$PASS${NC}${BLUE}                                        ║${NC}" " Total Passed:     $PASS"
write_output "${BLUE}║${NC} Total Failed:     ${RED}$FAIL${NC}${BLUE}                                        ║${NC}" " Total Failed:     $FAIL"

if [ $USE_VALGRIND -eq 1 ]; then
    write_output "${BLUE}║${NC} Memory Leaks:     ${ORANGE}$MEM_LEAK${NC}${BLUE}                                        ║${NC}" " Memory Leaks:     $MEM_LEAK"
    write_output "${BLUE}║${NC} FD Leaks:        ${ORANGE}$FD_LEAK${NC}${BLUE}                                        ║${NC}" " FD Leaks:        $FD_LEAK"
fi

write_output "${BLUE}║${NC} Total Tests:      $TESTS${BLUE}                                        ║${NC}" " Total Tests:      $TESTS"
write_output "${BLUE}╠════════════════════════════════════════════════════════════╣${NC}" "═════════════════════════════════════════════════════════════"

LEAK_TOTAL=$((MEM_LEAK + FD_LEAK))
if [ $FAIL -eq 0 ] && [ $LEAK_TOTAL -eq 0 ]; then
    write_output "${BLUE}║${NC} Status:           ${GREEN}✓ ALL TESTS PASSED - NO LEAKS${NC}${BLUE}          ║${NC}" " Status:           ✓ ALL TESTS PASSED - NO LEAKS"
elif [ $FAIL -eq 0 ]; then
    write_output "${BLUE}║${NC} Status:           ${GREEN}✓ PASS${NC} - ${ORANGE}$LEAK_TOTAL LEAKS${NC}${BLUE}                        ║${NC}" " Status:           ✓ PASS - $LEAK_TOTAL LEAKS"
else
    PERCENT=$((PASS * 100 / TESTS))
    write_output "${BLUE}║${NC} Status:           ${RED}$PERCENT% - $FAIL FAILED${NC}${BLUE}                              ║${NC}" " Status:           $PERCENT% - $FAIL FAILED"
fi

write_output "${BLUE}╚════════════════════════════════════════════════════════════╝${NC}" "═════════════════════════════════════════════════════════════"
write_output "" ""
write_output "${GREEN}Log saved to: $LOG_FILE${NC}" "Log saved to: $LOG_FILE"

mv "$TMP_LOG" "$LOG_FILE"

exit $FAIL
