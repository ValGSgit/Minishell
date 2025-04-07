Left to fix:

Rework export and the +11 issue
Work on echo for multiple -n

Val: Handle echo $"HOME"$USER (empty expansion remove first $)
     Write updater for PWD, OLDPWD, SHLVL
     Handle SIGTERM in the middle of heredoc
     SHLVL idea (if ./minishell or bash then SHLVL++)
     Fix conditional jump when syntax error and echo $?
     Remake syntax check for case ">| echo something"
  
