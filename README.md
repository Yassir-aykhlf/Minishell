# Project Plan

## Phase 1: Basic Shell
- Implement the REPL loop. ✅
- Read user input and print it back (basic prompt). ✅
- Exit when `exit` is typed.
- Handle basic built-in commands without arguments.

## Phase 2: Tokenization & Parsing
- Tokenize commands (`ls -l`, `echo hello`, etc.).
- Handle quotes (`" "` and `' '`).
- Identify operators (`|`, `<`, `>`, `;`, `&&`, `||`, `*`).
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
