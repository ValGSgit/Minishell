# Minishell

Tests left to pass:
Based on the provided log and the visible code, here is a list of issues to fix or improve in your Minishell implementation:

---

### **1. Expander Issues**
- **Test 7, 8, 15, 16, 17, 18, 19, 21**:
  - **Problem**: Environment variables (`$PWD`, `$?`, `$USER`, `$HOME`, etc.) are not expanded correctly in some cases.
  - **Fix**: Improve the `expand_variable` function in expander.c:
    - Handle `$?` correctly to return the last exit status.
    - Handle `$` followed by nothing or invalid characters (e.g., `$` or `$123`).
    - Ensure single quotes prevent expansion, while double quotes allow it.
    - Handle cases like `$USER` and `$HOME` properly.

---

### **2. Redirection Issues**
- **Test 57, 59, 60, 61, 62, 63, 68, 69, 70, 90**:
  - **Problem**: Input redirections (`<`) and output redirections (`>`) are not working as expected.
  - **Fix**:
    - Ensure `apply_redirection` in executor.c handles file permissions and file existence correctly.
    - Handle cases where multiple redirections are used in a single command.
    - Fix issues with filenames containing spaces or special characters (e.g., `"file name with spaces"`).
    - Ensure redirections work properly in pipelines.

---

### **3. Pipeline Issues**
- **Test 53, 54, 71, 72, 73, 74, 75**:
  - **Problem**: Pipelines are not functioning correctly, especially when combined with redirections.
  - **Fix**:
    - Ensure `execute_pipeline` in executor.c handles input/output redirections properly for each command in the pipeline.
    - Fix broken pipe errors when commands in the pipeline fail or don't produce output.

---

### **4. Builtin Command Issues**
- **Test 40, 41, 51**:
  - **Problem**: Builtin commands like `cd` and `exit` are not handling arguments correctly.
  - **Fix**:
    - For `cd`, ensure it handles invalid paths and multiple arguments properly.
    - For `exit`, handle cases with too many arguments or invalid numeric arguments.

---

### **5. Output and Error Handling**
- **Test 13, 37, 83, 84, 86, 88, 91, 92, 93, 94, 95**:
  - **Problem**: Output redirections (`>`) are not working as expected, and error messages are inconsistent with Bash.
  - **Fix**:
    - Ensure `apply_redirection` handles output redirections correctly, including overwriting and appending to files.
    - Handle permission errors and invalid file paths gracefully.
    - Ensure error messages match Bash behavior.

---

### **6. Environment Variable Export/Unset**
- **Test 56**:
  - **Problem**: Exported variables are not being passed correctly to child processes.
  - **Fix**:
    - Ensure `export` updates the environment properly and propagates changes to child processes.
    - Fix issues with `env` and `unset` commands.

---

### **7. Quoting and Tokenization**
- **Test 13, 15, 16, 62**:
  - **Problem**: Quoting and tokenization are not handled correctly in some cases.
  - **Fix**:
    - Improve the `lexer` in lexer.c to handle quotes and special characters properly.
    - Ensure single quotes prevent expansion, while double quotes allow it.
    - Handle edge cases like nested quotes or escaped characters.

---

### **8. Exit Code Handling**
- **Test 40, 41, 76, 78, 80**:
  - **Problem**: Exit codes are not consistent with Bash behavior.
  - **Fix**:
    - Ensure exit codes are set correctly for all commands, including builtins, pipelines, and redirections.
    - Handle cases where commands fail due to missing files or permissions.

---

### **9. File and Directory Handling**
- **Test 62, 86**:
  - **Problem**: Filenames with spaces or special characters are not handled correctly.
  - **Fix**:
    - Ensure filenames are parsed and passed correctly to commands.
    - Handle quoted filenames properly in the lexer and parser.

---

### **10. Miscellaneous**
- **Test 37, 85, 88**:
  - **Problem**: Permission errors are not handled consistently.
  - **Fix**:
    - Ensure permission errors are detected and reported correctly for all commands and redirections.

---

### **Summary of Files to Update**
1. **`expander.c`**:
   - Fix environment variable expansion issues.
2. **`executor.c`**:
   - Fix redirection and pipeline handling.
   - Improve error handling and exit code consistency.
3. **`lexer.c`**:
   - Improve quoting and tokenization logic.
4. **`parsing.c`**:
   - Ensure redirections and arguments are parsed correctly.
5. **`builtins` (not shown but implied)**:
   - Fix issues with `cd`, `exit`, `export`, and `unset`.

---

### **Next Steps**
1. Prioritize fixing critical issues like redirections and pipelines.
2. Write unit tests for edge cases (e.g., filenames with spaces, multiple redirections).
3. Compare Minishell behavior with Bash for each test case to ensure consistency.
