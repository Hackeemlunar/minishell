#!/bin/bash
# Minishell Test Script
# This script tests various shell features and compares behavior with bash

# Create a log file for results
echo "=== Minishell vs Bash Test Results ===" > test_results.log
echo "Started at $(date)" >> test_results.log
echo "" >> test_results.log

# Test basic commands
echo "=== Testing Basic Commands ===" >> test_results.log

# Echo tests
echo "1. Echo test with quotes" >> test_results.log
echo "Expected: Hello World" >> test_results.log
echo "Bash: $(echo "Hello World")" >> test_results.log
echo "" >> test_results.log

echo "2. Echo test with option -n" >> test_results.log
echo "Expected: Hello World (no newline)" >> test_results.log
echo -n "Bash: $(echo -n "Hello World")" >> test_results.log
echo "" >> test_results.log
echo "" >> test_results.log

# Environment variable tests
echo "=== Testing Environment Variables ===" >> test_results.log

echo "3. Simple variable expansion" >> test_results.log
TEST_VAR="variable_content"
echo "Expected: This is a variable_content test" >> test_results.log
echo "Bash: This is a $TEST_VAR test" >> test_results.log
echo "" >> test_results.log

echo "4. Variable expansion with quotes" >> test_results.log
echo "Expected: This is a 'variable_content' test" >> test_results.log
echo "Bash: This is a '$TEST_VAR' test" >> test_results.log
echo "" >> test_results.log

echo "5. Variable expansion with double quotes" >> test_results.log
echo "Expected: This is a variable_content test" >> test_results.log
echo "Bash: This is a \"$TEST_VAR\" test" >> test_results.log
echo "" >> test_results.log

# Exit status test
echo "=== Testing Exit Status ===" >> test_results.log
echo "6. Testing \$? after successful command" >> test_results.log
echo "hello" > /dev/null
echo "Expected: 0" >> test_results.log
echo "Bash: $?" >> test_results.log
echo "" >> test_results.log

echo "7. Testing \$? after failed command" >> test_results.log
not_existing_command 2> /dev/null || true
echo "Expected: non-zero" >> test_results.log
echo "Bash: $?" >> test_results.log
echo "" >> test_results.log

# Redirection tests
echo "=== Testing Redirections ===" >> test_results.log

echo "8. Output redirection '>'" >> test_results.log
echo "Test redirection" > test_redirect.txt
echo "Expected: file created with 'Test redirection'" >> test_results.log
echo "Bash: $(cat test_redirect.txt)" >> test_results.log
echo "" >> test_results.log

echo "9. Append redirection '>>'" >> test_results.log
echo "Appended line" >> test_redirect.txt
echo "Expected: file with both lines" >> test_results.log
echo "Bash: $(cat test_redirect.txt)" >> test_results.log
echo "" >> test_results.log

echo "10. Input redirection '<'" >> test_results.log
echo "Expected: contents of test_redirect.txt" >> test_results.log
echo -n "Bash: " >> test_results.log
cat < test_redirect.txt >> test_results.log
echo "" >> test_results.log

# Here-document test
echo "=== Testing Here-document ===" >> test_results.log
echo "11. Simple heredoc" >> test_results.log
echo "Expected: This is a heredoc test" >> test_results.log
echo -n "Bash: " >> test_results.log
cat << EOF >> test_results.log
This is a heredoc test
EOF
echo "" >> test_results.log

# Pipe tests
echo "=== Testing Pipes ===" >> test_results.log
echo "12. Simple pipe" >> test_results.log
echo "Expected: 3 (number of words in 'hello world test')" >> test_results.log
echo -n "Bash: " >> test_results.log
echo "hello world test" | wc -w >> test_results.log
echo "" >> test_results.log

echo "13. Multiple pipes" >> test_results.log
echo "Expected: 1 (line count of 'hello')" >> test_results.log
echo -n "Bash: " >> test_results.log
echo "hello world test" | grep "hello" | wc -l >> test_results.log
echo "" >> test_results.log

# Builtin tests
echo "=== Testing Builtins ===" >> test_results.log

echo "14. pwd builtin" >> test_results.log
echo "Expected: current directory" >> test_results.log
echo "Bash: $(pwd)" >> test_results.log
echo "" >> test_results.log

echo "15. export and env test" >> test_results.log
export TEST_EXPORT="exported_value"
echo "Expected: variable is set to exported_value" >> test_results.log
echo "Bash: $TEST_EXPORT" >> test_results.log
echo "" >> test_results.log

# Cleanup
rm test_redirect.txt

echo "" >> test_results.log
echo "Tests completed at $(date)" >> test_results.log
echo "Remember to run these tests manually in your minishell to compare results" >> test_results.log

echo "Tests completed! Check test_results.log for bash output to compare with your minishell."
