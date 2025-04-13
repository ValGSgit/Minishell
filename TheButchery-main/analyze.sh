#!/bin/bash
# filepath: /home/vagarcia/Desktop/MinishellCheckpoints/Minishell-Val-Branch/analyze.sh

TEST_DIR="/home/vagarcia/Desktop/Minishell-Val-Branch/mstest_output_2025-04-09_15.14.53"
TEST_FILES_DIR="/home/vagarcia/Desktop/Minishell-Val-Branch/mstest/tests"

# Function to find original test command in test files
find_test_command() {
    local test_name="$1"
    local category="$2"
    
    # Look for test file in the category directory
    local test_file="${TEST_FILES_DIR}/${category}.sh"
    
    if [ -f "$test_file" ]; then
        # Extract line number and command
        local line_info=$(grep -n "^[[:space:]]*test_command.*[\'\"]\($test_name\|$test_name [^\'\"]*\)[\'\"]\|^[[:space:]]*test_command.*$test_name[^\'\"]*[^a-zA-Z0-9_-]" "$test_file")
        
        if [ -n "$line_info" ]; then
            local line_num=$(echo "$line_info" | cut -d':' -f1)
            local command=$(echo "$line_info" | sed 's/.*test_command[^"]*"\([^"]*\)".*/\1/' | sed "s/.*test_command[^']*'\([^']*\)'.*/\1/")
            echo "  Source: ${category}.sh:${line_num}"
            echo "  Command: $command"
        else
            echo "  Source: Command not found in ${category}.sh"
        fi
    else
        echo "  Source: Test file not found for $category"
    fi
}

# Function to compare output files
compare_files() {
    local bash_file="$1"
    local minishell_file="$2"
    
    if ! diff -q "$bash_file" "$minishell_file" > /dev/null; then
        local test_name=$(basename "$bash_file" | sed 's/_stdout_bash\|_stderr_bash//')
        local category=$(basename $(dirname "$bash_file"))
        local type=$(echo "$bash_file" | grep -o "std[a-z]*")
        
        echo "- Test $test_name (${category}): $type mismatch"
        find_test_command "$test_name" "$category"
        echo "  Bash output:"
        cat "$bash_file" | sed 's/^/    /' | head -3
        echo "  Minishell output:"
        cat "$minishell_file" | sed 's/^/    /' | head -3
        echo ""
    fi
}

echo "# Failed Tests Report"
echo "Examining test directory: $TEST_DIR"
echo

# Find and iterate through test categories
find "$TEST_DIR" -type d -not -path "$TEST_DIR" | sort | while read category; do
    if [ -d "$category" ]; then
        # Look for stdout files
        find "$category" -name "*_stdout_bash" | while read bash_out; do
            minishell_out="${bash_out/_stdout_bash/_stdout_minishell}"
            if [ -f "$minishell_out" ]; then
                compare_files "$bash_out" "$minishell_out"
            fi
        done
        
        # Look for stderr files
        find "$category" -name "*_stderr_bash" | while read bash_err; do
            minishell_err="${bash_err/_stderr_bash/_stderr_minishell}"
            if [ -f "$minishell_err" ]; then
                compare_files "$bash_err" "$minishell_err"
            fi
        done
        
        # Check exit code files
        find "$category" -name "*_exit_bash" | while read bash_exit; do
            minishell_exit="${bash_exit/_exit_bash/_exit_minishell}"
            if [ -f "$minishell_exit" ]; then
                if ! diff -q "$bash_exit" "$minishell_exit" > /dev/null; then
                    test_name=$(basename "$bash_exit" | sed 's/_exit_bash//')
                    cat_name=$(basename "$category")
                    
                    echo "- Test $test_name ($cat_name): EXIT_CODE mismatch"
                    find_test_command "$test_name" "$cat_name"
                    echo "  Bash exit code: $(cat "$bash_exit")"
                    echo "  Minishell exit code: $(cat "$minishell_exit")"
                    echo ""
                fi
            fi
        done
    fi
done
