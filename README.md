# Minishell

A robust UNIX shell implementation with built-in command execution, pipeline handling, and variable expansion.

## Overview

Minishell is a simplified shell implementation inspired by bash. It provides a command-line interface where users can execute commands, use pipes for command chaining, leverage redirections, and utilize environment variables - all with proper error handling and signal management.

## Features

### Command Execution
- Run both built-in commands and external executables

### Built-in Commands
- `echo`: Display text with `-n` flag support
- `cd`: Change directory with path or relative navigation
- `pwd`: Print working directory
- `export`: Set environment variables
- `unset`: Remove environment variables
- `env`: Display environment variables
- `exit`: Exit the shell with status code

### Shell Operators
- **Pipes** (`|`): Connect command outputs to inputs
- **Redirections**:
  - Input redirection (`<`)
  - Output redirection (`>`)
  - Append output (`>>`)
  - Heredoc (`<<`)
- **Logical operators**:
  - AND (`&&`)
  - OR (`||`)

### Advanced Features
- Environment variable expansion (`$VAR`)
- Signal handling (Ctrl+C, Ctrl+D, Ctrl+\)
- Subshell execution with parentheses
- Command history navigation
- Custom memory management

## Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/minishell.git

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

1. **Tokenization**: Parse input into tokens (commands, arguments, operators)
2. **Parsing**: Create an Abstract Syntax Tree (AST) from tokens
3. **Expansion**: Handle environment variable expansion
4. **Execution**: Execute commands according to the parsed structure
5. **Memory Management**: Custom memory allocation and deallocation

## Dependencies

- GCC Compiler
- GNU Make
- Standard C Library

## Contributors

- [Yassir Aykhlf](https://github.com/Yassir-aykhlf)
- [Amine Rajma](https://github.com/st-ameen)

## Contributing
Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a feature branch (`git checkout -b feature-name`).
3. Commit your changes (`git commit -m "Add some feature"`).
4. Push to the branch (`git push origin feature-name`).
5. Open a pull request.

## Acknowledgements

- Inspired by the Bash shell
- Developed as part of the 42 School curriculum
