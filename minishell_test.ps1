# Minishell Test Script (PowerShell version)
# This script generates test commands for minishell

Write-Host "=== Minishell Test Commands ==="
Write-Host "Copy and paste these commands into your minishell to test functionality."
Write-Host ""

# Basic Commands
Write-Host "-- Basic Commands --"
Write-Host "echo Hello World"
Write-Host "echo -n Hello World"
Write-Host ""

# Environment Variables
Write-Host "-- Environment Variables --"
Write-Host "export TEST_VAR=variable_content"
Write-Host "echo This is a \$TEST_VAR test"
Write-Host "echo This is a '\$TEST_VAR' test"
Write-Host "echo This is a \"\$TEST_VAR\" test"
Write-Host ""

# Exit Status
Write-Host "-- Exit Status --"
Write-Host "echo hello"
Write-Host "echo \$?"
Write-Host "not_existing_command"
Write-Host "echo \$?"
Write-Host ""

# Redirections
Write-Host "-- Redirections --"
Write-Host "echo Test redirection > test_redirect.txt"
Write-Host "cat test_redirect.txt"
Write-Host "echo Appended line >> test_redirect.txt"
Write-Host "cat test_redirect.txt"
Write-Host "cat < test_redirect.txt"
Write-Host ""

# Here-document
Write-Host "-- Here-document --"
Write-Host "cat << EOF"
Write-Host "This is a heredoc test"
Write-Host "EOF"
Write-Host ""

# Pipes
Write-Host "-- Pipes --"
Write-Host "echo hello world test | wc -w"
Write-Host "echo hello world test | grep hello | wc -l"
Write-Host ""

# Builtins
Write-Host "-- Builtins --"
Write-Host "pwd"
Write-Host "export TEST_EXPORT=exported_value"
Write-Host "env | grep TEST_EXPORT"
Write-Host "echo \$TEST_EXPORT"
Write-Host ""

# Cleanup
Write-Host "-- Cleanup --"
Write-Host "rm test_redirect.txt"
Write-Host ""

# Complex tests
Write-Host "-- Complex Commands --"
Write-Host "echo hello > file1 && cat file1 | wc -c > file2 && cat file2"
Write-Host "ls -la | grep \".txt\" | wc -l"
Write-Host ""

Write-Host "To compare with bash behavior, try these same commands in bash or WSL."
