# Minishell

Note 31.03

Val will improve unset and export and remove empty args;

Iris will look into connecting builtins and redir;

VVV Emergency Notes!VVV 

---------------------------------------

Note 01.04

unset and export work without params but Val will make it work just like the og

Iris fixed quite a few redir issues now ex. test count is 132/146

What is left?

Prevent double execve, checking for fd type with fstat or lstat or just stat

Used functions so far: 
                U access@GLIBC_2.2.5
                 U add_history
                 U chdir@GLIBC_2.2.5
                 U close@GLIBC_2.2.5
                 U dup2@GLIBC_2.2.5
                 U __errno_location@GLIBC_2.2.5
                 U execve@GLIBC_2.2.5
                 U exit@GLIBC_2.2.5
                 U fork@GLIBC_2.2.5
                 U free@GLIBC_2.2.5
                 U getcwd@GLIBC_2.2.5
                 U getenv@GLIBC_2.2.5
                 w __gmon_start__
                 U isatty@GLIBC_2.2.5
                 U __libc_start_main@GLIBC_2.34
                 U malloc@GLIBC_2.2.5
                 U open@GLIBC_2.2.5
                 U perror@GLIBC_2.2.5
                 U pipe@GLIBC_2.2.5
                 U printf@GLIBC_2.2.5
                 U readline
                 U rl_clear_history
                 U rl_on_new_line
                 U rl_redisplay
                 U rl_replace_line
                 U sigaction@GLIBC_2.2.5
                 U sigemptyset@GLIBC_2.2.5
                 U unlink@GLIBC_2.2.5
                 U wait@GLIBC_2.2.5
                 U waitpid@GLIBC_2.2.5
                 U write@GLIBC_2.2.5
                 
Unsed or "Should use":  
                        stat, lstat, fstat, unlink (heredoc but not yet working),
                        dup, opendir, readdir, closedir, strerror, perror, isatty,
                        ttyname, ttyslot, ioctl, tcsetattr, tcgetattr, tgetent, tgetflag,
                        tgetnum, tgetstr, tgoto, tputs
