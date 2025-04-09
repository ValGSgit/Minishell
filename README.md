===============================================================================
                                 MINISHELL
===============================================================================

Project: Minishell  
Authors: ValGS / Iris
School: 42  
Branch: Val-Branch  
Language: C  
Compilation: gcc with -Wall -Wextra -Werror  
Libs Used: Readline, Libc, Custom Libft  
License: Educational Use - 42 School
-------------------------------------------------------------------------------
Left to correct:

*Handle heredoc limiter with/without quotes (expansion)
*Review execution process & child process exiting
*Handle edge cases for unset and export (When PATH is unset or export removes spaces(?))
*Rewrite syntax error check
*Val will handle echo "$" and the things that thought were fixed after fixing other fixes

-------------------------------------------------------------------------------
1. PROJECT OVERVIEW
-------------------------------------------------------------------------------

Minishell is a simplified UNIX shell developed in C, following the specifications 
of the 42 School curriculum. Its main purpose is to mimic basic behaviors of 
popular shells (like Bash or Zsh), allowing users to execute commands, 
manipulate the environment, use pipes and redirections, and implement built-in 
functionality — all while demonstrating a solid understanding of system 
programming, memory management, and process control.

This project pushes developers to dive deep into UNIX internals, including 
forking, file descriptors, signal handling, command parsing, and environment 
variable management.

-------------------------------------------------------------------------------
2. OBJECTIVES
-------------------------------------------------------------------------------

- Re-implement a basic command-line interpreter (shell)
- Handle command parsing, argument splitting, and tokenization
- Support input/output redirections, pipes, and environment variables
- Implement essential built-in shell commands
- Properly manage process creation and control with fork/execve/wait
- Handle signal interruptions (e.g., Ctrl+C, Ctrl+\)
- Follow strict memory management — avoid leaks or undefined behavior
- Maintain clean code structure and follow 42 coding norms

-------------------------------------------------------------------------------
3. FEATURES & FUNCTIONALITY
-------------------------------------------------------------------------------

Supported Features:
-------------------
- Custom interactive shell prompt
- Execution of binary files and system commands
- Built-in commands:
  - echo
  - cd
  - pwd
  - export
  - unset
  - env
  - exit
- Input/output redirection:
  - `<`  : Redirect input
  - `>`  : Redirect output (truncate)
  - `>>` : Redirect output (append)
- Pipelines using `|`
- Quoting:
  - Single quotes: prevent expansion
  - Double quotes: allow expansion
- Environment variable management (`$VAR`, `$?`)
- Signal handling (`SIGINT`, `SIGQUIT`)
- Return status tracking

Optional / Bonus (if implemented):
----------------------------------
- Heredocs (`<<`)
- Wildcards/globbing
- Command history navigation (via readline)
- Advanced parsing (nested quotes, subshells, etc.)

-------------------------------------------------------------------------------
4. INSTALLATION
-------------------------------------------------------------------------------

1. Clone the repository:
   git clone --branch Val-Branch https://github.com/ValGSgit/Minishell.git

2. Navigate into the project:
   cd Minishell

3. Build the shell:
   make

Optional:
- make clean   : Remove object files
- make fclean  : Remove object files + binary
- make re      : Rebuild everything from scratch

-------------------------------------------------------------------------------
5. USAGE
-------------------------------------------------------------------------------

To run Minishell:

    ./minishell

You will see a prompt like:

    minishell$

From here you can type in commands, use pipes, redirections, built-ins, and 
environment variables.

Examples:

    minishell$ ls -la | grep .c > files.txt
    minishell$ echo "Hello $USER"
    minishell$ export VAR=test
    minishell$ echo $VAR
    minishell$ cd ..
    minishell$ pwd
    minishell$ exit

-------------------------------------------------------------------------------
6. CODE STRUCTURE
-------------------------------------------------------------------------------

Project Layout:

Minishell/
├── includes/           -> Header files
│   └── minishell.h
├── src/                -> Main source files
│   ├── main.c
│   ├── builtins/
│   ├── parser/
│   ├── executor/
│   ├── signals/
│   └── utils/
├── ShellLibft/         -> Custom Libft library
├── Makefile            -> Build system
└── README.txt          -> Project documentation

-------------------------------------------------------------------------------
7. IMPLEMENTATION DETAILS
-------------------------------------------------------------------------------

Parsing:
--------
- Tokenizer separates input by whitespace, respecting quote rules
- Environment variable expansion occurs after parsing tokens
- Redirections and pipes are identified and managed with structures

Execution:
----------
- Commands are either identified as built-ins or binaries
- Uses fork/execve to launch processes
- Pipes are created using pipe() and managed with dup2()
- Redirections handled by reopening file descriptors

Signals:
--------
- Custom handlers for SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\)
- Prevents shell from exiting or printing default behavior
- Clean line refresh with readline after signal interrupts

Memory Management:
------------------
- Custom garbage collector or manual free per context
- All memory is properly managed to avoid leaks
- Leak checks performed with valgrind

Readline Integration:
---------------------
- Used for input and command history
- rl_on_new_line, rl_redisplay used for proper signal redraws

-------------------------------------------------------------------------------
8. LIMITATIONS
-------------------------------------------------------------------------------

- No support for job control (`&`, `fg`, `bg`)
- No advanced shell scripting features
- No wildcard expansion (unless implemented in bonus)
- Syntax error detection is limited to basic cases

-------------------------------------------------------------------------------
9. TESTING
-------------------------------------------------------------------------------

Manual testing is the primary method:
- Use test scripts with complex pipelines and redirections
- Compare behavior to `/bin/bash`
- Use valgrind for memory leaks:
  
  valgrind --leak-check=full ./minishell

-------------------------------------------------------------------------------
10. CONTRIBUTORS
-------------------------------------------------------------------------------

- ValGS (github.com/ValGSgit)

-------------------------------------------------------------------------------
11. LICENSE
-------------------------------------------------------------------------------

This project is part of the 42 School curriculum and intended for educational 
use only. Not licensed for commercial distribution.

-------------------------------------------------------------------------------
