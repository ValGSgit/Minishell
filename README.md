I'll create a comprehensive list of failed tests by examining the actual test files to understand what each failing test is doing. Here's the list of all failed tests with their file lines and commands:

### PARSING HELL Section
1. **Test #3**: (`10_parsing_hell.sh:10`) - `>> '$USER'` followed by `cat $USER` (STD_ERR issue)
   - Creating a file named '$USER' (with quotes) but trying to cat it without quotes

2. **Test #12**: (`10_parsing_hell.sh:32`) - `"ECho" -n -nnn"" "-"nnnnn` (STD_ERR and EXIT_CODE issues)
   - Non-standard echo command with complex flag combinations

3. **Test #14**: (`10_parsing_hell.sh:40`) - `ECHO '''''-''n'` (STD_ERR and EXIT_CODE issues)
   - Echo with mixed quotes around flags

4. **Test #15**: (`10_parsing_hell.sh:45`) - `echo '' -n` (STD_ERR and EXIT_CODE issues)
   - Echo with empty string and -n flag

5. **Test #16**: (`10_parsing_hell.sh:52`) - `echo "" "" "" -n -n -n -n` (EXIT_CODE issue)
   - Echo with multiple empty strings and multiple -n flags

6. **Test #58**: (`10_parsing_hell.sh:170`) - `cd "$H"O"ME"/Desktop/` followed by `echo $?` (STD_ERR and EXIT_CODE issues)
   - Complex path construction with environment variable fragments

7. **Test #77**: (`10_parsing_hell.sh:216`) - `export T="|" followed by echo segfault $T grep segfault` (STD_ERR, EXIT_CODE and CRASH issues)
   - Exporting a pipe symbol and using it in a command

8. **Test #78**: (`10_parsing_hell.sh:219`) - `export T='|' followed by echo segfault $T grep segfault` (STD_OUT issue)
   - Similar to #77 but with single quotes

9. **Test #101**: (`10_parsing_hell.sh:272`) - `export T='|' followed by echo $T echo lala $T echo $T echo $?` (STD_ERR and EXIT_CODE issues)
   - Complex pipe symbol expansion in a command

10. **Test #127**: (`10_parsing_hell.sh:327`) - `echo |< echo segf` (STD_ERR and EXIT_CODE issues)
    - Pipe followed by input redirection

### EXPANSION Section
1. **Test #3**: (`11_expansion.sh:9`) - `$notexists echo "haha"` (STD_OUT issue)
   - Trying to execute a command with a non-existent environment variable

2. **Test #4**: (`11_expansion.sh:11`) - `> $notexists echo "haha"` (STD_ERR issue)
   - Output redirection to a non-existent environment variable

### CD Section
1. **Test #22**: (`1_builtins_cd.sh:68`) - `cd "?"` (STD_ERR issue)
   - Changing to a directory with special character

2. **Test #34**: (`1_builtins_cd.sh:113`) - `cd $OLDPWD/something` (STD_ERR issue)
   - CD to a path combined with $OLDPWD

3. **Test #38**: (`1_builtins_cd.sh:129`) - `cd $HOME asd w dd q asd` (STD_OUT and STD_ERR issues)
   - CD with $HOME followed by multiple arguments

4. **Test #39**: (`1_builtins_cd.sh:135`) - `cd -` (STD_ERR issue)
   - CD to previous directory

5. **Test #40**: (`1_builtins_cd.sh:139`) - `cd -` (STD_OUT and STD_ERR issues)
   - Second CD to previous directory (different behavior expected)

6. **Test #45**: (`1_builtins_cd.sh:162`) - `cd ../libft ojid iowjiojwoid joijd owjoid jwojd` (STD_ERR issue)
   - CD with relative path and additional arguments

The most severe issue is with test #77, which results in a SIGABRT crash when trying to handle special characters in commands with environment variables.
