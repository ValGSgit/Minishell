Left to fix:

Rework export and the +11 issue (Val)

Work on echo for multiple -n (Iris)

Handle echo $"HOME"$USER (empty expansion remove first $) (Val bc expansions suck)

Write updater for PWD, OLDPWD, SHLVL (Either)

Handle SIGTERM in the middle of heredoc (Iris)

SHLVL idea (if ./minishell or bash then SHLVL++) (Val/Iris)

Fix conditional jump when syntax error and echo $? (Val)
Remake syntax check for case "(null) > (null) | echo something" (bash says "something: command not found") (should not be syntax error)
bash creates file called echo int this case...
