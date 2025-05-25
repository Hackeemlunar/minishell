#!/bin/bash

# Test script for minishell

echo "Testing minishell..."

# Compile the shell
make

# Test redirection with variable expansion
echo "Testing redirection with variable expansion..."
echo "Try: ls >> \"\$USER\""
echo "Try: echo hello > \"\$USER\""
echo "Try: cat < \"\$USER\""

# Test heredoc with variable expansion
echo "Testing heredoc with variable expansion..."
echo "Try: cat << EOF"
echo "This is a test with \$USER"
echo "EOF"

# Test signal handling
echo "Testing signal handling..."
echo "Try pressing Ctrl+C at the prompt"
echo "Try running: sleep 5 (and press Ctrl+C during execution)"

echo "Tests completed. Make sure to check the output manually."
