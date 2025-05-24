# Redirection Test Cases
# This file contains specific tests for file redirections in minishell

# 1. Basic Output Redirection
echo "1. Test basic output redirection with '>'"
echo "This should be written to file1.txt" > file1.txt

# 2. Output Redirection with Append
echo "2. Test output redirection with append '>>'"
echo "First line" > file2.txt
echo "Second line" >> file2.txt
echo "Third line" >> file2.txt

# 3. Multiple Redirections
echo "3. Test multiple redirections"
echo "This goes to file3.txt" > file3.txt > file3a.txt
# In bash, only file3a.txt should receive content

# 4. Redirecting Command Output
echo "4. Test redirecting command output"
ls -la > file4.txt

# 5. Redirect with Pipes
echo "5. Test redirect with pipes"
echo "hello world" | wc -w > file5.txt

# 6. Redirect Stderr
echo "6. Test redirect stderr"
non_existent_command 2> error.txt

# 7. Redirect Both stdout and stderr
echo "7. Test redirect both stdout and stderr"
ls existing_file non_existent_file > out.txt 2> err.txt
ls existing_file non_existent_file &> both.txt

# 8. Here Document with Redirection
echo "8. Test heredoc with redirection"
cat << EOF > heredoc.txt
This is a heredoc
with multiple lines
that should be redirected to a file
EOF

# 9. Input and Output Redirection
echo "9. Test input and output redirection"
cat < file1.txt > file1_copy.txt

# 10. Check if file exists before redirection
echo "10. Test if file exists before redirection"
if [ -f file1.txt ]; then
  echo "file1.txt exists" > status.txt
else
  echo "file1.txt does not exist" > status.txt
fi

# Note: To check the results, use:
# cat file1.txt file2.txt file3.txt file3a.txt file4.txt file5.txt error.txt out.txt err.txt both.txt heredoc.txt file1_copy.txt status.txt
