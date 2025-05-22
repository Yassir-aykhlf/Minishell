# Minishell

A UNIX shell implementation with command execution, pipeline handling, and variable expansion.

## Overview

Minishell is a minimalistic shell implementation based on bash. It provides a command-line interface where users can execute commands, use pipes for command chaining, use redirections, and utilize environment variables, all with proper error handling and signal management.

## Features

### Command Execution
- Run both built-in commands and external executables
- Accurate **exit status** reporting

### Built-in Commands
- `echo`: Display text with `-n` flag support
- `cd`: Change directory with path or relative navigation
- `pwd`: Print working directory
- `export`: Set or display environment variables
- `unset`: Remove environment variables
- `env`: Display environment variables
- `exit`: Exit the shell with a specified status code

### Shell Operators
- **Pipes** (`|`): Connect command outputs to inputs, allowing complex command chains.
- **Redirections**:
  - Input redirection (`<`): Read input from a file.
  - Output redirection (`>`): Write output to a file, overwriting existing content.
  - Append output (`>>`): Append output to a file.
  - Heredoc (`<<`): Provide multi-line input to a command, with optional delimiter quoting to control expansion.
- **Logical operators**:
  - AND (`&&`): Execute the next command only if the previous one succeeds.
  - OR (`||`): Execute the next command only if the previous one fails.

### Advanced Features
- **Environment Variable Expansion**:
    - Environment variable expansion.
    - Special variable `$?` to display the exit status of the last command.
- **Signal Handling**:
    - `Ctrl+C` (SIGINT): Interrupts the current foreground process or displays a new prompt if no process is running.
    - `Ctrl+D` (EOF): Exits the shell if the input line is empty.
    - `Ctrl+\` (SIGQUIT): Ignored by the shell to prevent accidental termination.
- **Subshell Execution**: Execute commands in a subshell environment using parentheses `(...)`.
- **Command History**: Navigate through previously entered commands using arrow keys.
- **Custom Memory Management**: Utilizes a custom memory allocator for efficient resource handling and memory leak prevention.
- **Robust Error Handling**: Provides informative error messages for syntax errors, command not found, permission issues, etc.

## Project Structure

The project is organized into the following main directories:
- `includes/`: Contains header files, including `minishell.h` and library headers.
- `lib/`: Contains external libraries like `libft`.
- `src/`: Contains the source code for Minishell, categorized into subdirectories:
    - `main/`: Main shell loop and initialization.
    - `ast/`: Abstract Syntax Tree construction and manipulation.
    - `environment/`: Environment variable management.
    - `execution/`: Command execution logic, including builtins and external commands.
    - `expansion/`: Variable and token expansion.
    - `heredoc/`: Heredoc processing.
    - `memory/`: Custom memory management utilities.
    - `scan/`: Input scanning and validation.
    - `signal_handeling/`: Signal handling logic.
    - `tokenization/`: Input string tokenization.
    - `utils/`: Utility functions used across the project.
- `Makefile`: Defines build rules for compiling the project.
- `README.md`: This file.

## Installation

```bash
# Clone the repository
git clone https://github.com/Yassir-aykhlf/minishell.git minishell

# Navigate to the project directory
cd minishell

# Compile the project
make
```

## Usage

```bash
# Launch the shell
./minishell
```

Once running, you can use the shell like any standard UNIX shell:

```bash
# Simple command
echo Hello, World!

# Command with pipes
ls -la | grep "\.c$" | wc -l

# Environment variable usage
echo $HOME

# Redirections
echo test > file.txt
cat < file.txt

# Heredoc
cat << EOF
This is a multi-line
heredoc input
EOF

# Logical operators
cd /tmp && echo "Changed directory successfully"
cd /nonexistentdir || echo "Directory change failed"
```

## Implementation

The project implementation follows these key steps:

1.  **Input Reading & History**: Uses `readline` to read user input and manage command history.
2.  **Scanning**: Validates the input for early syntax errors handling.
3.  **Tokenization**: Parses the input string into a list of tokens (commands, arguments, operators, redirections, etc). Each token is identified by its type and value, and a character mask is generated to handle quoting for expansion.
4.  **Parsing (AST Construction)**: Builds an Abstract Syntax Tree (AST) data structure from the token list. The AST represents the command structure, including pipelines, logical operations, subshells, and redirections.
5.  **Expansion**: Traverses the AST to perform variable expansion (`$VAR`, `$?`) and removes quotes. Field splitting is handled for unquoted expansions.
6.  **Heredoc Processing**: Handles `<<` heredoc redirections by reading input until the delimiter is found, expanding variables within the heredoc.
7.  **Execution**: Traverses the AST to execute commands.
    - Handles built-in commands directly.
    - Executes external commands by forking a child process and using the `execve` syscall.
    - Manages pipes and redirections by manipulating file descriptors.
    - Implements logical operators (`&&`, `||`) based on the exit status of commands.
    - Executes subshells in a separate child process.
8.  **Signal Handling**: Manages signals like SIGINT and SIGQUIT appropriately during command execution and while waiting for input.
9.  **Memory Management**: Employs a custom memory allocation wrapper to track allocations and facilitate easy cleanup, preventing memory leaks.

## Dependencies

- GCC Compiler
- GNU Make
- Standard C Library

## Creators

- [Yassir Aykhlf](https://github.com/Yassir-aykhlf)
- [Amine Rajma](https://github.com/st-ameen)

## Contributing
Contributions are very welcome! Please follow these steps:
1. Fork the repository.
2. Create a feature branch (`git checkout -b feature-name`).
3. Commit your changes (`git commit -m "Add some feature"`).
4. Push to the branch (`git push origin feature-name`).
5. Open a pull request.

## Acknowledgements

- Inspired by the [Bash](https://github.com/gitGNU/gnu_bash) shell
- Developed as part of the [42 School](https://www.42network.org) curriculum
