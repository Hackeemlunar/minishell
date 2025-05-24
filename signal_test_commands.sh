#!/bin/bash
# This script provides test commands for signal handling
# Run these commands one by one in both bash and your minishell to compare behavior

echo "==== Signal Testing Script ===="
echo ""
echo "Instructions:"
echo "1. Run each test in bash to see expected behavior"
echo "2. Then run the same test in your minishell to compare"
echo "3. For Ctrl+C tests, type the command and press Ctrl+C"
echo "4. For Ctrl+D tests, press Ctrl+D at the appropriate time"
echo ""

echo "==== Test 1: Basic Command Execution ===="
echo "Run: echo Hello World"
echo "Expected: Should print 'Hello World'"
echo ""

echo "==== Test 2: Ctrl+C at Prompt ===="
echo "Action: Press Ctrl+C at the prompt"
echo "Expected: Should display new prompt, not exit"
echo ""

echo "==== Test 3: Ctrl+D at Empty Prompt ===="
echo "Action: Press Ctrl+D at empty prompt"
echo "Expected: Should exit shell"
echo ""

echo "==== Test 4: Ctrl+C During Command ===="
echo "Run: sleep 10"
echo "Action: Press Ctrl+C while running"
echo "Expected: Command terminates, return to prompt"
echo ""

echo "==== Test 5: Ctrl+C During Pipe ===="
echo "Run: cat | grep hello"
echo "Action: Type some text, then Ctrl+C"
echo "Expected: Both processes terminate, return to prompt"
echo ""

echo "==== Test 6: Heredoc with Ctrl+C ===="
echo "Run: cat << EOF"
echo "Action: Type a few lines, then press Ctrl+C before typing EOF"
echo "Expected: Heredoc terminates, return to prompt"
echo ""

echo "==== Test 7: Interrupt Command in Background ===="
echo "Run: sleep 100 &"
echo "Run: ps | grep sleep"
echo "Run: kill -INT \$(ps | grep sleep | grep -v grep | awk '{print \$1}')"
echo "Expected: Background process should be killed"
echo ""

echo "==== Test 8: Multiple Ctrl+C ===="
echo "Action: Press Ctrl+C multiple times quickly"
echo "Expected: Should display a new prompt each time"
echo ""

echo "==== Test 9: Ctrl+D with Text at Prompt ===="
echo "Action: Type 'echo hello' (don't press enter), then press Ctrl+D"
echo "Expected: Should execute the command in bash"
echo ""

echo "==== Test 10: Ctrl+D During Input ===="
echo "Run: cat"
echo "Action: Type some text, press Enter, then press Ctrl+D on empty line"
echo "Expected: cat terminates, return to prompt"
echo ""

echo "=== End of Tests ==="
echo "Compare the behavior between bash and your minishell to identify any issues."
