#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'
YELLOW='\033[1;33m'

MINISHELL="./minishell"

# Track results
PASS=0
FAIL=0

make re
run_test() {
    local cmd="$1"
    
    # Run command in Bash
    echo "$cmd" | bash > /dev/null 2>&1
    local bash_exit=$?
    
    # Run command in Minishell
    echo "$cmd" | $MINISHELL > /dev/null 2>&1
    local mini_exit=$?
    
    if [ "$bash_exit" -eq "$mini_exit" ]; then
        echo -e "${GREEN}[PASS]${RESET} Command: \"$cmd\" (Exit: $mini_exit)"
        ((PASS++))
    else
        echo -e "${RED}[FAIL]${RESET} Command: \"$cmd\""
        echo -e "       Expected: $bash_exit | Got: $mini_exit"
        ((FAIL++))
    fi
}

echo -e "${YELLOW}--- Starting Exit Code Tests ---${RESET}"

# 1. Basic Success & Failure
run_test "ls"
run_test "not_a_command"

# 2. Builtins
run_test "exit 42"
run_test "exit 259" # Testing wrap-around (259 % 256)
run_test "cd non_existent_dir"

# 3. Syntax & Path Errors
run_test "ls /root/no_permission"
run_test "cat non_existent_file"

# 4. Logical Operators (if implemented)
run_test "ls && false"
run_test "false || true"

# 5. Pipes and Redirects
run_test "ls | grep nothing"
run_test "cat < non_existent_file"

# New function specifically for signals
run_signal_test() {
    local label="$1"
    local cmd="$2"
    
    # We use perl to trigger specific signals reliably
    echo "$cmd" | bash > /dev/null 2>&1
    local bash_exit=$?
    
    echo "$cmd" | $MINISHELL > /dev/null 2>&1
    local mini_exit=$?
    
    if [ "$bash_exit" -eq "$mini_exit" ]; then
        echo -e "${GREEN}[PASS]${RESET} Signal: $label (Exit: $mini_exit)"
        ((PASS++))
    else
        echo -e "${RED}[FAIL]${RESET} Signal: $label"
        echo -e "       Expected: $bash_exit | Got: $mini_exit"
        ((FAIL++))
    fi
}

echo -e "${YELLOW}--- Starting Signal Tests ---${RESET}"

# SIGINT (Signal 2) -> Expected 130
run_signal_test "SIGINT (Ctrl+C)" "perl -e 'kill 2, \$$'"

# SIGQUIT (Signal 3) -> Expected 131
run_signal_test "SIGQUIT (Ctrl+\\)" "perl -e 'kill 3, \$$'"

# SIGKILL (Signal 9) -> Expected 137
run_signal_test "SIGKILL" "perl -e 'kill 9, \$$'"

# SIGTERM (Signal 15) -> Expected 143
run_signal_test "SIGTERM" "perl -e 'kill 15, \$$'"

# Complex: Signal inside a pipe
# Only the last command's exit code should be captured
run_test "ls | perl -e 'kill 9, \$$'"


echo -e "${YELLOW}--- Starting Builtin Tests ---${RESET}"

# 1. Exit (The most critical builtin)
run_test "exit 0"
run_test "exit 255"
run_test "exit 42"
run_test "exit -1"         # Should wrap to 255
run_test "exit 256"        # Should wrap to 0
run_test "exit abc"        # Non-numeric: Expected 2 in Bash (Numeric argument required)
run_test "exit abc 123"    # Non-numeric with extra args: Expected 2 in Bash
run_test "exit 1 2"        # Too many arguments: Expected 1 in Bash

# 2. CD (Change Directory)
run_test "cd /"            # Success: 0
run_test "cd /nonexistent" # Error: 1
run_test "cd .."           # Success: 0
run_test "cd /bin/ls"      # Not a directory: 1

# 3. Export & Unset
run_test "export VAR=val"  # Success: 0
run_test "export 123VAR"   # Invalid identifier: 1
run_test "unset 123VAR"    # Invalid identifier: 1
run_test "unset PATH"      # Success: 0

# 4. PWD (Print Working Directory)
run_test "pwd"             # Success: 0
run_test "pwd extra_arg"   # Bash ignores args for pwd: 0

# 5. Echo
run_test "echo hello"      # Success: 0
run_test "echo -n test"    # Success: 0

echo -e "${YELLOW}--- Starting Advanced Builtin Error Tests ---${RESET}"

# 1. CD (Change Directory) Errors
run_test "cd"                   # Home not set? (If you unset HOME, this should error)
run_test "cd /tmp/permission"   # No search permission (chmod 000 a dir to test)

# 2. EXIT Errors (The 1 vs 2 distinction)
run_test "exit 9223372036854775808" # Out of range (Long Long Max + 1): Expected 2
run_test "exit --"                  # No arg: Expected 0
run_test "exit -- 123"              # Valid numeric with --: Expected 123
run_test "exit -- 123 456"          # Too many args with --: Expected 1
run_test "exit -- abc"              # Non-numeric with --: Expected 2
run_test "exit +5"                  # Valid numeric: Expected 5
run_test "exit 1 2 3"               # Too many args: Expected 1

# 3. EXPORT / UNSET (Invalid Identifiers)
run_test "export ="                 # Invalid: 1
run_test "export VAR-NAME=val"      # Hyphens not allowed: 1
run_test "export 9VAR=val"          # Starts with digit: 1
run_test "unset VAR!NAME"           # Invalid char: 1

# 4. PWD (Environment interference)
# In Bash, if you unset PWD, pwd still works by looking at the system.
run_test "unset PWD && pwd"         # Expected: 0

# 5. ECHO (Edge cases)
run_test "echo -nnnnnnnn"           # Bash treats multiple 'n's as -n: 0
run_test "echo -n -n -n hello"      # Multiple flags: 0

echo -e "${YELLOW}--- Starting Pipe Tests ---${RESET}"

# Basic pipe success and failure
run_test "echo hello | cat"                      # Success: 0
run_test "ls | grep test"                        # Exit code from grep: 1 if not found
run_test "echo test | grep test"                 # Success: 0
run_test "cat /nonexistent | wc -l"              # First fails, but pipe continues: 0
run_test "echo hello | cat | cat | cat"          # Multiple pipes: 0
run_test "false | true"                          # Last command succeeds: 0
run_test "true | false"                          # Last command fails: 1
run_test "exit 42 | echo test"                   # Exit in pipe: 0 (echo succeeds)
run_test "echo test | exit 42"                   # Exit as last: 42
run_test "ls | not_a_command"                    # Command not found in pipe: 127
run_test "not_a_command | ls"                    # First fails, second succeeds: 0
run_test "echo hello | cat | grep bye"           # Chain ending in failure: 1
run_test "ls /nonexistent 2>&1 | grep 'No such'" # Piping stderr: 0 if found
run_test "pwd | cd .."                           # Builtin in pipe: 0 (cd succeeds)
run_test "cd /nonexistent | pwd"                 # Failed builtin then success: 0

echo -e "${YELLOW}--- Starting Redirect Tests ---${RESET}"

# Output redirects
run_test "echo hello > /tmp/test_out"            # Success: 0
run_test "echo hello > /root/noperm"             # Permission denied: 1
run_test "ls > /tmp/test_out"                    # Success: 0
run_test "cat < /nonexistent"                    # Input redirect fail: 1
run_test "echo test >> /tmp/test_append"         # Append success: 0
run_test "echo test >> /root/noperm"             # Append permission denied: 1

# Input redirects
run_test "cat < /etc/passwd"                     # Success: 0
run_test "cat < /nonexistent_file"               # File not found: 1
run_test "wc -l < /etc/passwd"                   # Success: 0

# Multiple redirects
run_test "echo hello > /tmp/out1 > /tmp/out2"    # Multiple outputs: 0
run_test "cat < /etc/passwd > /tmp/passwd_copy"  # Input and output: 0
run_test "< /etc/passwd cat > /tmp/out"          # Redirects before command: 0
run_test "> /tmp/out echo hello"                 # Output before command: 0

# Redirect with pipes
run_test "echo hello | cat > /tmp/pipe_out"      # Pipe with output redirect: 0
run_test "cat < /etc/passwd | grep root > /tmp/grep_out" # Full chain: 0

# Failed redirects should prevent command execution
run_test "echo test > /root/noperm && echo success" # First fails: 1
run_test "cat < /nonexistent > /tmp/out"         # Input fail: 1

# stderr redirects (2>)
run_test "ls /nonexistent 2> /tmp/err"           # Redirect stderr: 0 (ls fails but redirect works)
run_test "cat /nonexistent 2> /tmp/err"          # Command fails: 1
run_test "ls /nonexistent 2> /root/noperm"       # stderr redirect fails: 1

echo -e "${YELLOW}--- Starting Heredoc Tests ---${RESET}"

# Basic heredoc (Note: These are tricky to test in this format)
# We'll use a different approach for heredocs
run_heredoc_test() {
    local label="$1"
    local expected="$2"
    
    # For bash
    bash << 'TESTEOF' > /dev/null 2>&1
cat << EOF
test
EOF
TESTEOF
    local bash_exit=$?
    
    # For minishell
    $MINISHELL << 'TESTEOF' > /dev/null 2>&1
cat << EOF
test
EOF
TESTEOF
    local mini_exit=$?
    
    if [ "$bash_exit" -eq "$mini_exit" ]; then
        echo -e "${GREEN}[PASS]${RESET} Heredoc: $label (Exit: $mini_exit)"
        ((PASS++))
    else
        echo -e "${RED}[FAIL]${RESET} Heredoc: $label"
        echo -e "       Expected: $bash_exit | Got: $mini_exit"
        ((FAIL++))
    fi
}

run_heredoc_test "Basic heredoc" 0

# Simpler heredoc tests using run_test
run_test "cat << EOF
hello
EOF"

run_test "grep test << EOF
test
nothing
EOF"

run_test "grep missing << EOF
test
nothing
EOF"

echo -e "${YELLOW}--- Starting Logical Operator Tests (&&) ---${RESET}"

# Basic && (AND) tests
run_test "true && true"                          # Both succeed: 0
run_test "true && false"                         # Second fails: 1
run_test "false && true"                         # First fails, second not executed: 1
run_test "false && false"                        # First fails: 1
run_test "echo hello && echo world"              # Both succeed: 0
run_test "ls && pwd"                             # Both succeed: 0
run_test "ls /nonexistent && echo test"          # First fails: 1 (echo not executed)
run_test "echo test && ls /nonexistent"          # First succeeds, second fails: 1
run_test "not_a_command && echo test"            # Command not found: 127

# Chain of && operators
run_test "true && true && true"                  # All succeed: 0
run_test "true && false && true"                 # Second fails, third not executed: 1
run_test "echo a && echo b && echo c"            # All succeed: 0
run_test "exit 0 && echo test"                   # Exit then echo: depends on implementation

# && with builtins
run_test "cd / && pwd"                           # Both succeed: 0
run_test "cd /nonexistent && pwd"                # cd fails, pwd not executed: 1
run_test "export VAR=test && echo \$VAR"         # Both succeed: 0

# && with redirects
run_test "echo test > /tmp/out && cat /tmp/out"  # Both succeed: 0
run_test "echo test > /root/noperm && echo fail" # First fails: 1

echo -e "${YELLOW}--- Starting Logical Operator Tests (||) ---${RESET}"

# Basic || (OR) tests
run_test "true || true"                          # First succeeds, second not executed: 0
run_test "true || false"                         # First succeeds: 0
run_test "false || true"                         # First fails, second succeeds: 0
run_test "false || false"                        # Both fail: 1
run_test "ls || echo fallback"                   # First succeeds: 0
run_test "ls /nonexistent || echo fallback"      # First fails, second succeeds: 0
run_test "not_a_command || echo fallback"        # First fails (127), second succeeds: 0
run_test "false || not_a_command"                # First fails, second fails: 127

# Chain of || operators
run_test "false || false || true"                # Last succeeds: 0
run_test "false || false || false"               # All fail: 1
run_test "true || echo not_executed || echo no"  # First succeeds: 0

# || with builtins
run_test "cd /nonexistent || cd /"               # First fails, second succeeds: 0
run_test "cd /nonexistent || cd /alsonotexist"   # Both fail: 1

# || with redirects
run_test "cat /nonexistent || echo error"        # First fails, second succeeds: 0

echo -e "${YELLOW}--- Starting Mixed Logical Operator Tests ---${RESET}"

# Combining && and || (left-to-right evaluation)
run_test "true && true || false"                 # (true && true) succeeds, || not evaluated: 0
run_test "true && false || true"                 # (true && false) fails, || true succeeds: 0
run_test "false && true || true"                 # (false && true) not fully evaluated, || true: 0
run_test "false || true && false"                # (false || true) succeeds, && false fails: 1
run_test "true || false && false"                # true succeeds, rest not evaluated: 0
run_test "false && false || false && true"       # Complex: (false && false) || (false && true): 1

# Real-world patterns
run_test "ls && echo found || echo notfound"     # ls succeeds: 0
run_test "ls /nonexistent && echo found || echo notfound" # ls fails, echo notfound: 0
run_test "cd /tmp && ls || echo failed"          # cd succeeds, ls succeeds: 0
run_test "cat /nonexist 2>/dev/null || echo File not found" # Fallback message: 0

# With pipes
run_test "echo test | grep test && echo match"   # Pipe succeeds, then echo: 0
run_test "echo test | grep fail && echo match"   # grep fails, echo not executed: 1
run_test "echo test | grep fail || echo nomatch" # grep fails, echo executes: 0

echo -e "${YELLOW}--- Starting Combined Complex Tests ---${RESET}"

# Pipes with logical operators
run_test "ls | grep test && echo found"          # Depends on if test is found
run_test "ls | grep test || echo notfound"       # Depends on if test is found
run_test "false | true && echo yes"              # Pipe succeeds (true), echo: 0
run_test "true | false || echo no"               # Pipe fails (false), echo: 0

# Redirects with logical operators
run_test "echo test > /tmp/out && cat /tmp/out"  # Both succeed: 0
run_test "cat /nonexist 2>/dev/null || echo error" # cat fails, echo: 0
run_test "echo a > /tmp/a && echo b > /tmp/b && cat /tmp/a /tmp/b" # Chain: 0

# Pipes with redirects
run_test "cat /etc/passwd | grep root > /tmp/root_lines" # Success: 0
run_test "echo hello | cat > /tmp/hello.txt"     # Success: 0
run_test "ls | cat > /tmp/ls_out 2> /tmp/ls_err" # Success: 0

# All together: pipes, redirects, and logical operators
run_test "echo test | grep test > /tmp/out && cat /tmp/out" # Full chain: 0
run_test "cat /nonexist 2>/dev/null | wc -l || echo zero"   # Complex: 0
run_test "ls /tmp | grep test > /tmp/result || echo not_found" # Depends on content

# Edge cases
run_test "| cat"                                 # Syntax error (if your parser catches it)
run_test "&&"                                    # Syntax error
run_test "||"                                    # Syntax error
run_test "echo test |"                           # Incomplete pipe
run_test "echo test &&"                          # Incomplete logical operator
run_test "< > cat"                               # Invalid redirects

echo -e "${YELLOW}--- Summary ---${RESET}"
echo -e "Total Passed: ${GREEN}$PASS${RESET}"
echo -e "Total Failed: ${RED}$FAIL${RESET}"

make fclean

if [ $FAIL -ne 0 ]; then
    exit 1
fi