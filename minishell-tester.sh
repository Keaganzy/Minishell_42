#!/bin/bash

# Minishell Comprehensive Tester
# Shows ACTUAL test commands from evalsheet + GitHub readme
# Usage: ./minishell_tester.sh ./minishell

MINISHELL="${1:-.}/minishell"

if [ -n "$1" ] && [ -f "$1" ]; then
    MINISHELL="$1"
fi

if [ ! -f "$MINISHELL" ]; then
    echo "Error: minishell not found at $MINISHELL"
    exit 1
fi

PASS=0
FAIL=0
TESTS=0

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

clean_output() {
    echo "$1" | sed '1d' | sed 's/MS\$ $//g' | sed -e :a -e '/^\s*$/d;N;ba'
}

test_cmd() {
    local command="$1"
    TESTS=$((TESTS + 1))

    echo -e "  ${BLUE}Test:${NC} $command"

    local ms_output_raw=$(echo "$command" | "$MINISHELL" 2>&1)
    local ms_exit=$?
    local ms_output=$(clean_output "$ms_output_raw")

    local bash_output=$(echo "$command" | bash 2>&1)
    local bash_exit=$?

    # Always show expected and actual output
    echo -e "    ${YELLOW}Expected output:${NC} '$bash_output'"
    echo -e "    ${YELLOW}Actual output:${NC}   '$ms_output'"
    echo -e "    ${YELLOW}Expected exit:${NC} $bash_exit"
    echo -e "    ${YELLOW}Actual exit:${NC}   $ms_exit"

    if [ "$ms_output" = "$bash_output" ] && [ "$ms_exit" = "$bash_exit" ]; then
        echo -e "    ${GREEN}✓ PASS${NC}"
        PASS=$((PASS + 1))
    else
        echo -e "    ${RED}✗ FAIL${NC}"
        FAIL=$((FAIL + 1))
    fi
    echo ""
}

echo -e "${BLUE}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║  MINISHELL TESTER - ACTUAL TEST COMMANDS                 ║${NC}"
echo -e "${BLUE}║  From evalsheet + GitHub comprehensive tests             ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════════╝${NC}"
echo ""

# ========== BASIC TESTS ==========
echo -e "${YELLOW}=== BASIC TESTS ===${NC}"
test_cmd "pwd"
test_cmd "echo hello"
test_cmd "ls /tmp"
test_cmd ""

# ========== ECHO VARIATIONS ==========
echo -e "${YELLOW}=== ECHO COMMAND ===${NC}"
test_cmd "echo hello"
test_cmd "echo -n test"
test_cmd "echo -n aaa | cat -e"
test_cmd "echo \"hello world\""
test_cmd "echo 'hello world'"
test_cmd "echo"

# ========== EXIT COMMAND ==========
echo -e "${YELLOW}=== EXIT COMMAND ===${NC}"
test_cmd "exit 0"
test_cmd "exit 42"
test_cmd "exit 255"
test_cmd "exit a a"
test_cmd "exit 9223372036854775808"

# ========== QUOTE TOKENIZATION (FROM GITHUB) ==========
echo -e "${YELLOW}=== QUOTE TOKENIZATION (GitHub 1f-1m) ===${NC}"
test_cmd "echo \"IF YOU TOKENIZED THIS >> > < << OR THIS | || && ' ' you dum\""
test_cmd "echo 'IF YOU TOKENIZED THIS >> > < << OR THIS | || && \" \" ur n \$idot'"
test_cmd "echo \"    THIS SHOULD NOT TOUCH THE LEFT BOUNDARY\""
test_cmd "echo LEFTHAND'            'RIGHTHAND"
test_cmd "ls \"\" ''"
test_cmd "echo \$notinenv"
test_cmd "echo '\$BetterNotHaveAFileWithDollar'"

# ========== CD AND PWD (FROM GITHUB) ==========
echo -e "${YELLOW}=== CD AND PWD NAVIGATION (GitHub 1q-1t) ===${NC}"
test_cmd "cd /tmp && pwd"
test_cmd "cd /nonexistent"
test_cmd "cd ."
test_cmd "cd .."
test_cmd "cd ../../ && pwd"
test_cmd "cd /./././ && pwd"

# ========== REDIRECTIONS (FROM GITHUB) ==========
echo -e "${YELLOW}=== REDIRECTIONS (GitHub 1H-1I) ===${NC}"
test_cmd "echo a 1 > /tmp/ms_h.txt && cat /tmp/ms_h.txt"
test_cmd "echo a 1>/tmp/ms_i.txt && cat /tmp/ms_i.txt"
test_cmd "echo hello > /tmp/ms_out.txt && cat /tmp/ms_out.txt"
test_cmd "cat < /etc/hostname"
test_cmd "echo test >> /tmp/ms_app.txt && cat /tmp/ms_app.txt"

# ========== PIPES (FROM GITHUB) ==========
echo -e "${YELLOW}=== PIPES (Github 1J-2c) ===${NC}"
test_cmd "echo A | cat"
test_cmd "echo hello | cat | cat"
test_cmd "echo test | grep test"
test_cmd "ls /tmp | grep tmp"

# ========== LOGICAL OPERATORS ==========
echo -e "${YELLOW}=== LOGICAL OPERATORS (Github 1P-1R) ===${NC}"
test_cmd "echo a || echo b && echo c"
test_cmd "echo a && echo b || echo c"
test_cmd "true && true"
test_cmd "true && false"
test_cmd "false || true"
test_cmd "true || false && false"

# ========== INVALID COMMANDS ==========
echo -e "${YELLOW}=== INVALID COMMANDS ==="
test_cmd "nonexistent_cmd"
test_cmd "nonexistent_cmd_xyz arg1 arg2"

# ========== EXPORT TESTS ==========
echo -e "${YELLOW}=== EXPORT TESTS ==="
test_cmd "export TESTVAR=value"
test_cmd "export 1=invalid"
test_cmd "export"

# ========== ENV TESTS ==========
echo -e "${YELLOW}=== ENV TESTS ==="
test_cmd "env | wc -l"

# ========== UNSET TESTS ==========
echo -e "${YELLOW}=== UNSET TESTS ==="
test_cmd "unset TESTVAR"

# ========== HEREDOC TESTS ==========
echo -e "${YELLOW}=== HEREDOC TESTS (Github 1E) ==="
test_cmd "cat << EOF
hello world
EOF"

# ========== ECHO FLAG VARIANTS (Github 2a) ==========
echo -e "${YELLOW}=== ECHO FLAG VARIANTS (Github 2a) ==="
test_cmd "echo -nnnnnn test"
test_cmd "echo -n -n -n test"

# ========== SPACE HANDLING (Github 2w-2z) ==========
echo -e "${YELLOW}=== SPACE HANDLING (Github 2w-2z) ==="
test_cmd "echo a    b"
test_cmd "echo \"  hello  world  \""

# ========== NULL EXPANSION (Github 2A-2B) ==========
echo -e "${YELLOW}=== NULL EXPANSION (Github 2A-2B) ==="
test_cmd "echo a\$notinenvDidThisDisappear?"
test_cmd "echo a\"\$notinenv\"DidThisDisappear?"

# ========== SYNTAX ERRORS ==========
echo -e "${YELLOW}=== SYNTAX ERRORS ==="
test_cmd "| cat"
test_cmd "&&"
test_cmd "||"
test_cmd "echo test |"
test_cmd "echo test &&"

# ========== RETURN VALUE TESTS ==========
echo -e "${YELLOW}=== RETURN VALUE TESTS ==="
test_cmd "true"
test_cmd "false"
test_cmd "ls /tmp"
test_cmd "ls /nonexistent"

# ========== STDERR REDIRECT ==========
echo -e "${YELLOW}=== STDERR REDIRECT ==="
test_cmd "ls /nonexistent 2> /tmp/ms_err.txt && cat /tmp/ms_err.txt"

# ========== PIPE WITH REDIRECT ==========
echo -e "${YELLOW}=== PIPE WITH REDIRECT ==="
test_cmd "echo hello | cat > /tmp/ms_pipe.txt && cat /tmp/ms_pipe.txt"

# ========== COMPLEX COMBINATIONS ==========
echo -e "${YELLOW}=== COMPLEX COMBINATIONS ==="
test_cmd "echo a && echo b && echo c"
test_cmd "false || false || true"
test_cmd "true && false || true"

# ========== MULTIPLE ARGS ==========
echo -e "${YELLOW}=== MULTIPLE ARGUMENTS ==="
test_cmd "echo 1 2 3 4 5"
test_cmd "ls /tmp /etc"

# Cleanup
rm -f /tmp/ms_*.txt

echo ""
echo -e "${BLUE}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║                       SUMMARY                             ║${NC}"
echo -e "${BLUE}╠════════════════════════════════════════════════════════════╣${NC}"
echo -e "${BLUE}║${NC} Total Passed:  ${GREEN}$PASS${NC}${BLUE}                                            ║${NC}"
echo -e "${BLUE}║${NC} Total Failed:  ${RED}$FAIL${NC}${BLUE}                                            ║${NC}"
echo -e "${BLUE}║${NC} Total Tests:   $TESTS${BLUE}                                            ║${NC}"

if [ $FAIL -eq 0 ]; then
    echo -e "${BLUE}║${NC} Status:        ${GREEN}✓ ALL TESTS PASSED${NC}${BLUE}                      ║${NC}"
else
    PERCENT=$((PASS * 100 / TESTS))
    echo -e "${BLUE}║${NC} Status:        ${YELLOW}$PERCENT% Passing${NC}${BLUE}                                    ║${NC}"
fi
echo -e "${BLUE}╚════════════════════════════════════════════════════════════╝${NC}"

exit $FAIL
