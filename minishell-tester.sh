#!/bin/bash
# =============================================
# Advanced Minishell Tester
# Comprehensive test suite for 42 Minishell project
# Includes Valgrind memory leak detection
# =============================================

MINISHELL="${1:-./minishell}"
VALGRIND_CHECK="${2:-no}"  # Pass "valgrind" as second arg to enable

# Temp files
BASH_OUT=$(mktemp)
MINI_OUT=$(mktemp)
BASH_ERR=$(mktemp)
MINI_ERR=$(mktemp)
BASH_EXIT=$(mktemp)
MINI_EXIT=$(mktemp)
VALGRIND_LOG=$(mktemp)

# Counters
TEST_COUNT=0
PASS_COUNT=0
FAIL_COUNT=0
VALGRIND_FAIL=0

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

# -------------------------
# Run a single test
# -------------------------
run_test() {
    local name="$1"
    local cmd="$2"
    local skip_valgrind="${3:-no}"
    TEST_COUNT=$((TEST_COUNT+1))
    
    echo -e "\n${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${YELLOW}Test #${TEST_COUNT}: $name${NC}"
    echo -e "${BLUE}Command:${NC} $cmd"

    # Run in Bash
    bash -c "$cmd" > "$BASH_OUT" 2> "$BASH_ERR"
    echo $? > "$BASH_EXIT"

    # Run in Minishell
    if [ "$VALGRIND_CHECK" = "valgrind" ] && [ "$skip_valgrind" = "no" ]; then
        echo "$cmd" | valgrind --leak-check=full --show-leak-kinds=all \
            --track-fds=yes --errors-for-leak-kinds=all \
            --error-exitcode=42 --log-file="$VALGRIND_LOG" \
            $MINISHELL > "$MINI_OUT" 2> "$MINI_ERR"
        local valgrind_exit=$?
    else
        echo "$cmd" | $MINISHELL > "$MINI_OUT" 2> "$MINI_ERR"
    fi
    echo $? > "$MINI_EXIT"
    
    # Strip prompts
    sed -i '/^minishell[>$] /d' "$MINI_OUT"
    sed -i '/^MS\$ /d' "$MINI_OUT"
    sed -i '/^MS\$ exit/d' "$MINI_OUT"

    # Compare
    local stdout_match=0
    local stderr_match=0
    local exit_match=0
    diff -q "$BASH_OUT" "$MINI_OUT" >/dev/null && stdout_match=1
    diff -q "$BASH_ERR" "$MINI_ERR" >/dev/null && stderr_match=1
    diff -q "$BASH_EXIT" "$MINI_EXIT" >/dev/null && exit_match=1

    # Always print expected vs actual
    echo -e "${YELLOW}Expected stdout (bash):${NC}"; cat "$BASH_OUT"
    echo -e "${YELLOW}Actual stdout (minishell):${NC}"; cat "$MINI_OUT"
    echo -e "${YELLOW}Expected stderr (bash):${NC}"; cat "$BASH_ERR"
    echo -e "${YELLOW}Actual stderr (minishell):${NC}"; cat "$MINI_ERR"
    echo -e "${YELLOW}Expected exit code (bash):${NC} $(cat $BASH_EXIT)"
    echo -e "${YELLOW}Actual exit code (minishell):${NC} $(cat $MINI_EXIT)"

    # Check valgrind results
    if [ "$VALGRIND_CHECK" = "valgrind" ] && [ "$skip_valgrind" = "no" ]; then
        if [ $valgrind_exit -eq 42 ]; then
            echo -e "${RED}⚠ Memory leak detected!${NC}"
            grep -A 5 "LEAK SUMMARY" "$VALGRIND_LOG"
            VALGRIND_FAIL=$((VALGRIND_FAIL+1))
        else
            echo -e "${GREEN}✓ No memory leaks${NC}"
        fi
    fi

    if [ $stdout_match -eq 1 ] && [ $stderr_match -eq 1 ] && [ $exit_match -eq 1 ]; then
        echo -e "${GREEN}✓ Passed${NC}"
        PASS_COUNT=$((PASS_COUNT+1))
    else
        echo -e "${RED}✗ Failed${NC}"
        FAIL_COUNT=$((FAIL_COUNT+1))
    fi
}

# -------------------------
# Environment Setup
# -------------------------
export TESTVAR="value"
export EMPTY_VAR=""
export MULTI_WORD="hello world"
export PATH_TEST="/usr/bin:/bin"
touch file.txt in.txt out.txt err.txt
echo "Hello World" > file.txt
echo "Input data" > in.txt
echo "Line 1" > multi_line.txt
echo "Line 2" >> multi_line.txt
echo "Line 3" >> multi_line.txt

echo -e "${CYAN}╔════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║   MINISHELL COMPREHENSIVE TESTER      ║${NC}"
echo -e "${CYAN}╔════════════════════════════════════════╗${NC}"
if [ "$VALGRIND_CHECK" = "valgrind" ]; then
    echo -e "${MAGENTA}Valgrind memory checking: ENABLED${NC}"
fi

# -------------------------
# BASIC ECHO TESTS
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         ECHO TESTS                     ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "echo simple" "echo hello"
run_test "echo multiple args" "echo hello world test"
run_test "echo with -n flag" "echo -n hello"
run_test "echo with multiple -n" "echo -n -n -n hello"
run_test "echo -n with spaces" "echo -n hello world"
run_test "echo empty string" "echo"
run_test "echo just spaces" "echo '   '"
run_test "echo with tabs" "echo -e 'hello\tworld'"

# -------------------------
# QUOTE HANDLING
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         QUOTE TESTS                    ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "single quotes basic" "echo 'hello world'"
run_test "double quotes basic" "echo \"hello world\""
run_test "single quotes protect dollar" "echo '\$TESTVAR'"
run_test "double quotes expand dollar" "echo \"\$TESTVAR\""
run_test "mixed quotes" "echo 'hello' \"world\""
run_test "quotes in middle" "echo hel'lo'wor\"ld\""
run_test "empty quotes" "echo '' \"\""
run_test "nested quote style" "echo \"it's working\""
run_test "dollar in single quotes" "echo '\$HOME \$USER \$?'"
run_test "dollar in double quotes" "echo \"\$HOME \$TESTVAR\""
run_test "multiple dollar signs" "echo \$TESTVAR\$TESTVAR"

# -------------------------
# VARIABLE EXPANSION
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         VARIABLE EXPANSION             ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "simple variable" "echo \$TESTVAR"
run_test "nonexistent variable" "echo \$NONEXISTENT"
run_test "empty variable" "echo \$EMPTY_VAR"
# run_test "variable with text" "echo prefix\${TESTVAR}suffix"
run_test "multiple variables" "echo \$TESTVAR \$HOME \$PATH"
run_test "variable at start" "echo \$TESTVAR world"
run_test "variable at end" "echo hello \$TESTVAR"
run_test "exit status basic" "ls file.txt && echo \$?"
# run_test "exit status after fail" "ls nonexistent 2>/dev/null; echo \$?"
run_test "exit status multiple" "echo \$? \$? \$?"
run_test "dollar without var" "echo \$"
run_test "dollar with number" "echo \$0 \$1 \$123"
run_test "multi word variable" "echo \$MULTI_WORD"

# -------------------------
# WILDCARDS
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         WILDCARD TESTS                 ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

touch test1.txt test2.txt test3.txt a.txt b.txt
run_test "wildcard all txt" "echo *.txt"
run_test "wildcard with prefix" "echo test*.txt"
run_test "wildcard in middle" "echo t*t"
run_test "wildcard no match" "echo *.nonexistent"
run_test "multiple wildcards" "echo *.txt *.sh"
run_test "wildcard with command" "ls *.txt"

# -------------------------
# REDIRECTIONS - BASIC
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         REDIRECTION TESTS              ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "output redirect" "echo hello > out.txt && cat out.txt"
run_test "input redirect" "cat < file.txt"
run_test "append redirect" "echo first > out.txt && echo second >> out.txt && cat out.txt"
run_test "stderr redirect" "ls nonexistent 2> err.txt && cat err.txt"
run_test "redirect both" "ls file.txt nonexistent > out.txt 2> err.txt && cat out.txt && cat err.txt"
run_test "redirect to /dev/null" "echo hidden > /dev/null && echo visible"
run_test "redirect input and output" "cat < file.txt > out.txt && cat out.txt"

# -------------------------
# REDIRECTIONS - MULTIPLE FILES
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         MULTIPLE FILE REDIRECTIONS    ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "multiple output redirects" "echo test > out1.txt > out2.txt > out3.txt && cat out3.txt"
run_test "multiple input redirects" "cat < file.txt < in.txt"
run_test "chained redirects" "echo data > temp1.txt && cat < temp1.txt > temp2.txt && cat temp2.txt"
run_test "redirect overwrite" "echo first > out.txt && echo second > out.txt && cat out.txt"
run_test "mixed redirect types" "ls file.txt > out.txt 2> err.txt >> out.txt"
run_test "complex multi-redirect" "echo a > f1.txt && echo b > f2.txt && cat f1.txt f2.txt > f3.txt && cat f3.txt"

# -------------------------
# HEREDOC
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         HEREDOC TESTS                  ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "basic heredoc" "cat << EOF
hello
world
EOF"

run_test "heredoc with variables" "cat << EOF
Value: \$TESTVAR
EOF"

run_test "heredoc with quotes" "cat << EOF
'single' \"double\"
EOF"

run_test "heredoc pipe" "cat << EOF | grep hello
hello world
test
EOF"

run_test "heredoc redirect output" "cat << EOF > out.txt
heredoc content
EOF
cat out.txt"

run_test "multiple heredocs" "cat << EOF1 << EOF2
first
EOF1
second
EOF2"

run_test "heredoc with command" "grep world << EOF
hello
world
test
EOF"

# -------------------------
# PIPES
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         PIPE TESTS                     ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "simple pipe" "echo hello | cat"
run_test "pipe with grep" "echo hello world | grep world"
run_test "pipe chain" "echo hello | cat | cat | cat"
run_test "pipe with wc" "echo hello world test | wc -w"
run_test "pipe with sort" "echo -e 'c\nb\na' | sort"
run_test "pipe with redirects" "echo test | cat > out.txt && cat out.txt"
run_test "complex pipe" "cat file.txt | grep World | cat"
run_test "pipe with heredoc" "cat << EOF | grep hello
hello
world
EOF"

# -------------------------
# LOGICAL OPERATORS
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         LOGICAL OPERATOR TESTS         ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "AND success" "echo first && echo second"
run_test "AND failure" "ls nonexistent 2>/dev/null && echo should_not_print"
run_test "OR success first" "echo success || echo should_not_print"
run_test "OR failure first" "ls nonexistent 2>/dev/null || echo recovered"
run_test "combined AND OR" "echo ok && ls nonexistent 2>/dev/null || echo fallback"
run_test "multiple AND" "echo a && echo b && echo c"
run_test "multiple OR" "ls nope 2>/dev/null || ls nope2 2>/dev/null || echo final"
run_test "complex logic chain" "echo start && echo mid || echo alt && echo end"

# -------------------------
# BONUS: PARENTHESES/SUBSHELLS
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         PARENTHESES (BONUS)            ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "simple parentheses" "(echo hello)"
run_test "nested parentheses" "((echo nested))"
run_test "parentheses with AND" "(echo a && echo b)"
run_test "parentheses with OR" "(ls nope 2>/dev/null || echo ok)"
run_test "parentheses priority" "echo a && (echo b || echo c) && echo d"
run_test "multiple paren groups" "(echo first) && (echo second)"
run_test "complex paren logic" "(echo a && echo b) || (echo c && echo d)"

# -------------------------
# BUILT-IN: CD
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         CD BUILT-IN TESTS              ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "cd to /tmp" "cd /tmp && pwd"
run_test "cd to home" "cd ~ && pwd"
run_test "cd to previous dir" "cd /tmp && cd - && pwd"
run_test "cd invalid path" "cd /nonexistent/path"
run_test "cd with .." "cd /tmp && cd .. && pwd"
run_test "cd relative path" "cd . && pwd"
run_test "cd no args" "cd && pwd"

# -------------------------
# BUILT-IN: PWD
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         PWD BUILT-IN TESTS             ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "pwd basic" "pwd"
run_test "pwd after cd" "cd /tmp && pwd"

# -------------------------
# BUILT-IN: EXPORT
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         EXPORT BUILT-IN TESTS          ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "export new variable" "export NEWVAR=test && echo \$NEWVAR"
run_test "export empty variable" "export EMPTYVAR= && echo \$EMPTYVAR"
run_test "export without value" "export NOVALUE"
run_test "export list all" "export"
run_test "export multiple" "export VAR1=a VAR2=b && echo \$VAR1 \$VAR2"
run_test "export with quotes" "export QUOTED=\"hello world\" && echo \$QUOTED"

# -------------------------
# BUILT-IN: UNSET
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         UNSET BUILT-IN TESTS           ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "unset existing var" "export TMPVAR=value && unset TMPVAR && echo \$TMPVAR"
run_test "unset nonexistent" "unset NONEXISTENT"
run_test "unset multiple" "export A=1 B=2 && unset A B && echo \$A \$B"

# -------------------------
# BUILT-IN: ENV
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         ENV BUILT-IN TESTS             ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "env basic" "env | grep TESTVAR"
run_test "env after export" "export ENVTEST=value && env | grep ENVTEST"

# -------------------------
# BUILT-IN: ECHO
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         ECHO BUILT-IN TESTS            ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "echo with -n" "echo -n test"
run_test "echo -n multiple args" "echo -n hello world"
run_test "echo invalid flag" "echo -x test"
run_test "echo -nnn" "echo -nnn test"

# -------------------------
# BUILT-IN: EXIT
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         EXIT BUILT-IN TESTS            ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "exit with code" "exit 42" "skip_valgrind"
run_test "exit no args" "exit" "skip_valgrind"
run_test "exit in pipeline" "echo test | exit 5" "skip_valgrind"

# -------------------------
# PATH AND EXECUTABLES
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         PATH & EXECUTABLE TESTS        ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "absolute path" "/bin/echo hello"
run_test "relative path" "./file.txt 2>/dev/null || echo not executable"
run_test "command not found" "nonexistentcommand 2>&1 | head -1"
run_test "ls basic" "ls file.txt"
run_test "cat existing file" "cat file.txt"
run_test "grep basic" "grep Hello file.txt"

# -------------------------
# EDGE CASES
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         EDGE CASE TESTS                ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "empty command" ""
run_test "only spaces" "   "
run_test "only tabs" "		"
run_test "semicolon not implemented" "; echo test" || true
run_test "backslash not implemented" "echo test\\ ing" || true
run_test "multiple pipes empty" "| | |" || true
run_test "pipe at start" "| echo test" || true
run_test "pipe at end" "echo test |" || true
run_test "redirect without file" "echo test >" || true
run_test "double redirect" "echo test >> > out.txt" || true
run_test "many spaces between" "echo     hello     world"
run_test "dollar at end" "echo test\$"
run_test "question mark alone" "echo \?"
run_test "multiple dollar signs" "echo \$\$\$"

# -------------------------
# STRESS TESTS
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         STRESS TESTS                   ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

run_test "long pipe chain" "echo hello | cat | cat | cat | cat | cat"
run_test "many redirects" "echo test > a.txt > b.txt > c.txt > d.txt && cat d.txt"
run_test "complex combined" "echo start && (cat file.txt | grep World) || echo fallback"
run_test "nested logic" "echo a && echo b || echo c && echo d"
run_test "long command line" "echo word1 word2 word3 word4 word5 word6 word7 word8 word9 word10"

# -------------------------
# SIGNAL HANDLING (Manual verification needed)
# -------------------------
echo -e "\n${CYAN}════════════════════════════════════════${NC}"
echo -e "${CYAN}         SIGNAL TESTS (MANUAL)          ${NC}"
echo -e "${CYAN}════════════════════════════════════════${NC}"

echo -e "${YELLOW}Note: Signal tests (Ctrl-C, Ctrl-D, Ctrl-\\) must be tested manually${NC}"
echo -e "${YELLOW}Expected behavior:${NC}"
echo -e "  - Ctrl-C: Display new prompt on new line"
echo -e "  - Ctrl-D: Exit the shell"
echo -e "  - Ctrl-\\: Do nothing"

# -------------------------
# Summary
# -------------------------
echo -e "\n${CYAN}╔════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║          TEST SUMMARY                  ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════╝${NC}"
echo -e "Total tests: ${BLUE}$TEST_COUNT${NC}"
echo -e "${GREEN}Passed: $PASS_COUNT${NC}"
echo -e "${RED}Failed: $FAIL_COUNT${NC}"

if [ "$VALGRIND_CHECK" = "valgrind" ]; then
    echo -e "${MAGENTA}Memory leak failures: $VALGRIND_FAIL${NC}"
fi

# Pass percentage
if [ $TEST_COUNT -gt 0 ]; then
    PERCENTAGE=$((PASS_COUNT * 100 / TEST_COUNT))
    echo -e "Success rate: ${YELLOW}${PERCENTAGE}%${NC}"
fi

# Cleanup
rm -f file.txt in.txt out.txt err.txt test1.txt test2.txt test3.txt a.txt b.txt
rm -f out1.txt out2.txt out3.txt temp1.txt temp2.txt temp3.txt multi_line.txt
rm -f f1.txt f2.txt f3.txt result.txt outt.txt
rm -f "$BASH_OUT" "$MINI_OUT" "$BASH_ERR" "$MINI_ERR" "$BASH_EXIT" "$MINI_EXIT" "$VALGRIND_LOG"

echo -e "\n${CYAN}Testing complete!${NC}"
if [ $FAIL_COUNT -eq 0 ]; then
    echo -e "${GREEN}All tests passed! 🎉${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed. Review the output above.${NC}"
    exit 1
fi