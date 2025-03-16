# Minishell To-Do List

## Phase 1: Core Infrastructure (Foundational Elements)

### Environment Variable System
1. Create a proper environment variable management system
2. Enhance the tokenizer to handle `$VAR` expansion
3. Implement special variable handling for `$?` (last exit status)

### Command Execution Fundamentals
4. Update `search_path` function to handle cases when PATH is unset
5. Implement tracking and returning of command exit status
6. Improve error reporting for command execution failures

### Signal Handling
7. Add signal handlers for Ctrl-C (SIGINT) to interrupt current process
8. Implement handling for Ctrl-D (EOF) to exit shell
9. Add signal handler for Ctrl-\ (SIGQUIT) with appropriate behavior
10. Ensure proper signal behavior in interactive mode

## Phase 2: Core Command Execution

### Builtin Commands (Relatively Self-Contained)
11. Implement `echo` with `-n` option support
12. Implement `pwd` without options
13. Implement `env` without options
14. Implement `exit` without options
15. Implement `cd` with support for relative and absolute paths
16. Implement `export` without options
17. Implement `unset` without options

### Pipeline and Redirection
18. Finish the `execute_pipeline` function to properly connect commands with pipes
19. Add proper pipe creation and management for connecting command outputs to inputs
20. Improve the `redirect` function with better error handling
21. Enhance the `handle_heredoc` implementation with robust error handling
22. Add support for multiple redirections in a single command

## Phase 3: Advanced Command Execution

### Command Flow Control
23. Complete the execution flow for logical operators (&&, ||) in the `execute_recursive` function
24. Implement the `execute_subshell` function to handle commands in parentheses
25. Implement command grouping execution

### Quoting and Parsing
26. Enhance the tokenizer to properly handle single quotes (prevent all interpretation)
27. Improve handling of double quotes (prevent interpretation except for `$`)
28. Fix quote processing for variable expansion inside quotes

## Phase 4: Enhancements and Refinements

### Additional Features
31. Add wildcard expansion support for `*` patterns
32. Add standard error messages for various error conditions

### Memory Management
34. Ensure all allocated memory is properly tracked and freed

# Project Plan

## Phase 1: Basic Shell
- Implement the REPL loop. ✅
- Read user input and print it back (basic prompt). ✅
- Exit when `exit` is typed. ✅
- Handle basic built-in commands without arguments.

## Phase 2: Tokenization & Parsing
- Tokenize commands (`ls -l`, `echo hello`, etc.). ✅
- Handle quotes (`" "` and `' '`). ✅
- Identify operators (`|`, `<`, `>`, `&&`, `||`, `*`). 
- Construct a syntax tree (or a structured command array).
- Implement error detection for unmatched quotes and syntax errors.

## Phase 3: Execution
- Implement built-in commands (`cd`, `pwd`, `echo`, etc.).
- Execute external commands (`ls`, `cat`, etc.) via `fork()` and `execve()`.
- Implement pipes (`|`) to chain commands.
- Implement `&&` and `||` logical operators for conditional execution.
- Support parenthesis (`()`) to group commands and control precedence.

## Phase 4: Redirections
- Implement `<`, `>`, `>>`, `<<` (file descriptor management).
- Support handling of multiple redirections in a single command.
- Ensure compatibility with built-ins and external programs.

## Phase 5: Environment Variables & Expansion
- Implement variable expansion (`$HOME`, `$?`).
- Support `export` and `unset`.
- Implement `*` wildcard expansion.
- Add support for `$()` command substitution if feasible.

## Phase 6: Signal Handling
- Implement behavior for `CTRL-C`, `CTRL-D`, and `CTRL-\`.
- Ensure processes started in the background do not interfere with the shell prompt.
- Implement signal masking where necessary to prevent unintended behavior.
