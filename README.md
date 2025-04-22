# Minishell To-Do List

## Phase 1: Core Infrastructure

### Environment Variable System
1. Create a proper environment variable management system ✅
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

### Builtin Commands
11. Implement `echo` with `-n` option support ✅
12. Implement `pwd` without options ✅
13. Implement `env` without options ✅
14. Implement `exit` without options ✅
15. Implement `cd` with support for relative and absolute paths ✅
16. Implement `export` without options ✅
17. Implement `unset` without options ✅

### Pipeline and Redirection
18. Finish the `execute_pipeline` function to properly connect commands with pipes ✅
19. Add proper pipe creation and management for connecting command outputs to inputs ✅
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

---------------------------------------------------------------------------------------------------
