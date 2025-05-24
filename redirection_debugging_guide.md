# Understanding Shell Redirection Behavior

## Common Issues in Redirection Implementation

If your minishell has issues with redirections printing to stdout instead of writing to files, here are some common problems to check:

### 1. File Descriptor Handling

In a properly implemented redirection:
```c
command > file
```

The shell should:
1. Open the file for writing (with O_WRONLY, O_CREAT, O_TRUNC flags)
2. Duplicate the file descriptor to stdout (fd 1)
3. Execute the command, which will write to stdout (now redirected to the file)
4. Close the file descriptor after command execution

### 2. Common Implementation Problems

#### Not Actually Redirecting stdout

The most common mistake is opening the file but not redirecting stdout to it:

```c
// INCORRECT approach
int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// Missing: dup2(fd, STDOUT_FILENO);
execute_command();
close(fd);
```

#### Correct Implementation

```c
// CORRECT approach
int saved_stdout = dup(STDOUT_FILENO); // Save original stdout
int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
dup2(fd, STDOUT_FILENO); // Redirect stdout to file
close(fd); // Close original fd after duplication
execute_command();
dup2(saved_stdout, STDOUT_FILENO); // Restore original stdout
close(saved_stdout);
```

#### Using Write Instead of Redirection

Another mistake is manually writing to the file instead of redirecting:

```c
// INCORRECT approach
int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
char *output = execute_command_and_get_output();
write(fd, output, strlen(output));
close(fd);
```

### 3. Append Mode Issues

For append mode (`>>`), make sure you're using O_APPEND:

```c
fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
```

### 4. Piping with Redirection

When combining pipes and redirections, order matters:

```bash
cmd1 | cmd2 > file
```

The stdout of cmd2 should be redirected to the file, not cmd1.

### 5. Checking Your Implementation

To debug redirection issues:
1. Add debug prints before and after redirection setup
2. Verify file descriptors with temporary writes
3. Compare to bash behavior with identical commands
4. Check that file descriptors are being properly duplicated and restored

### 6. Common Code Areas to Check

1. Look at how your shell parses redirection operators
2. Check the execution logic for commands with redirections
3. Verify that file descriptors are being properly saved/restored
4. Ensure builtin commands respect the redirected stdout

Remember: In a shell, stdout redirection is fundamentally about changing where file descriptor 1 points to before executing a command.
