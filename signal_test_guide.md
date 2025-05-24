# Signal Testing for Minishell

## Ctrl+C (SIGINT) and Ctrl+D (EOF) Test Cases

### Test 1: Ctrl+C at the Prompt
1. Start minishell
2. Wait at the prompt
3. Press Ctrl+C
4. Expected behavior:
   - Should clear the current line
   - Display a new prompt on a new line
   - Not exit the shell

### Test 2: Ctrl+D at Empty Prompt
1. Start minishell
2. Wait at empty prompt
3. Press Ctrl+D
4. Expected behavior:
   - Should exit the shell gracefully
   - Display "exit" message

### Test 3: Ctrl+C during Command Execution
1. Run a command that doesn't terminate immediately: `sleep 10`
2. Press Ctrl+C while it's running
3. Expected behavior:
   - Command should terminate
   - Return to prompt
   - Not exit the shell

### Test 4: Ctrl+D with Text at Prompt
1. Start minishell
2. Type some text but don't press Enter (e.g., type "echo hello")
3. Press Ctrl+D
4. Expected behavior:
   - Should either do nothing or execute the line
   - Should not exit the shell

### Test 5: Ctrl+C during Heredoc Input
1. Start a heredoc: `cat << EOF`
2. Type a few lines
3. Press Ctrl+C before typing EOF
4. Expected behavior:
   - Heredoc should be interrupted
   - Return to prompt (may show error)
   - Shell should not exit

### Test 6: Multiple Ctrl+C Presses
1. Press Ctrl+C multiple times quickly at the prompt
2. Expected behavior:
   - Should display a new prompt for each Ctrl+C
   - Shell should remain running

### Test 7: Ctrl+D during Command Input
1. Run: `cat`  (to read from stdin)
2. Type some text and press Enter
3. Press Ctrl+D on an empty line
4. Expected behavior:
   - `cat` should terminate
   - Return to prompt
   - Shell should not exit

### Test 8: Ctrl+C during Pipe Sequence
1. Run: `cat | grep hello`
2. Type some text
3. Press Ctrl+C
4. Expected behavior:
   - Both processes should terminate
   - Return to prompt
   - Shell should not exit

## Testing Instructions

For each test case:
1. Run the specified commands in your minishell
2. Perform the signal action (Ctrl+C or Ctrl+D)
3. Compare the behavior with what you expect
4. If possible, also test the same scenario in bash to compare behavior

Compare your implementation against the standard bash behavior as a reference.
Remember: in a properly implemented shell:
- Ctrl+C (SIGINT) interrupts the current foreground process but doesn't exit the shell
- Ctrl+D (EOF) at an empty prompt exits the shell
- Signal handling should properly clean up resources

## Implementation Notes

In your signal.c, I can see you've implemented:
1. `signal_handler` - Main handler for the prompt
2. `signal_handler_heredoc` - Special handler for heredoc input
3. `signal_handler_input` - Handler for child processes
4. `setup_signals` - Sets up the initial signal handlers
5. `set_signal_handler` - Changes handlers based on context

These should cover the required signal handling if implemented correctly.
