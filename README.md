# Minishell

## Description

This project is a simple shell program, a basic implementation of a command-line interpreter. It provides an interface for users to interact with the operating system by entering commands. This Minishell project focuses on understanding fundamental concepts like processes, file descriptors, and command execution. [cite: 1, 2, 30]

## Features

The following features are implemented in this Minishell:

* **Prompt Display:** Displays a prompt to the user when waiting for a new command. [cite: 30]
* **Command History:** Maintains a history of previously entered commands. [cite: 30, 31]
* **Executable Search and Launch:** Locates and executes commands based on the PATH variable or using relative/absolute paths. [cite: 31, 32, 33]
* **Signal Handling:** Handles `ctrl-C` (displays a new prompt), `ctrl-D` (exits the shell), and `ctrl-\` (does nothing) signals. [cite: 36, 37, 38]
* **Quote Handling:** Correctly interprets single quotes (`'`) and double quotes (`"`) to prevent unwanted meta-character interpretation. Double quotes allow for environment variable expansion. [cite: 36, 37, 38]
* **Redirections:**
    * `<` :  Redirects input from a file. [cite: 36, 37, 38]
    * `>` :  Redirects output to a file. [cite: 36, 37, 38]
    * `<<` :  "Here document" - reads input until a specified delimiter. [cite: 36, 37, 38]
    * `>>` :  Redirects output to a file in append mode. [cite: 38, 39]
* **Pipes:** Supports pipes (`|`) to chain commands, where the output of one command becomes the input of the next. [cite: 38, 39]
* **Environment Variables:** Expands environment variables using the `$VAR_NAME` syntax. [cite: 38, 39]
* **Exit Status:** Handles `$?` to represent the exit status of the last executed foreground pipeline. [cite: 39]
* **Built-in Commands:** Implements the following built-in commands:
    * `echo` with option `-n` [cite: 39]
    * `cd` with relative or absolute paths [cite: 39]
    * `pwd` (no options) [cite: 39]
    * `export` (no options) [cite: 39]
    * `unset` (no options) [cite: 39]
    * `env` (no options or arguments) [cite: 39]
    * `exit` (no options) [cite: 29]

## Compilation

To compile the Minishell, use the provided Makefile. The following commands are available:

* `make`: Compiles the project.
* `make all`:  Compiles the project.
* `make clean`:  Removes object files.
* `make fclean`: Removes object files and the executable.
* `make re`:  Performs `fclean` followed by `make`.

## Usage

To run the Minishell, execute the compiled binary:

```bash
./minishell