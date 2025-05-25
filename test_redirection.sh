#!/bin/bash

# Test script for minishell redirection and variable handling

echo "===== Testing Variable Handling and Redirection in Minishell ====="

# Clean up any test files from previous runs
rm -f test_out.txt "$USER" "$" 2>/dev/null

echo "Testing 1: Simple redirection with literal $"
echo "Command: ls >> $"
echo "Expected: Should create a file named '$' with ls output"
echo

echo "Testing 2: Variable expansion in redirection"
echo "Command: echo hello >> \$USER"
echo "Expected: Should create a file named with your username containing 'hello'"
echo

echo "Testing 3: Multiple redirections with variables"
echo "Command: ls -la >> \$USER >> test_out.txt"
echo "Expected: Should append ls output to both files"
echo

echo "Testing 4: Heredoc with variable expansion"
echo "Command: cat << EOF (with \$USER inside)"
echo "Expected: Should show the expanded username"
echo

echo "===== Running the tests in minishell ====="
echo "Please run the following commands in your minishell:"
echo "1. ls >> $"
echo "2. echo hello >> \$USER"
echo "3. ls -la >> \$USER >> test_out.txt"
echo "4. cat << EOF"
echo "   This is a test with \$USER"
echo "   EOF"
echo
echo "After running these tests, check the contents of the files:"
echo "- File named '$'"
echo "- File named with your username"
echo "- File named 'test_out.txt'"
echo

echo "Press Enter to verify the results after running the tests..."
read

# Verify the results
echo "===== Verification ====="
if [ -f "$" ]; then
    echo "File '$' exists: SUCCESS"
    echo "Contents:"
    cat "$"
else
    echo "File '$' does not exist: FAIL"
fi
echo

if [ -f "$USER" ]; then
    echo "File '$USER' exists: SUCCESS"
    echo "Contents:"
    cat "$USER"
else
    echo "File '$USER' does not exist: FAIL"
fi
echo

if [ -f "test_out.txt" ]; then
    echo "File 'test_out.txt' exists: SUCCESS"
    echo "Contents:"
    cat "test_out.txt"
else
    echo "File 'test_out.txt' does not exist: FAIL"
fi
