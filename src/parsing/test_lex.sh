#!/usr/bin/env bash
#
# test_lex.sh — 

# Path to your lexer binary
LEX="./lex"

# Array of edge-case commands
tests=(
  # 1. Empty string
  ""
  # 2. Only whitespace
  "     "
  # 3. Simple command
  "echo hello"
  # 4. Multiple spaces/tabs/newlines
  $'   ls\t -la   \n'
  # 5. Unclosed single quote
  "echo 'unterminated"
  # 6. Unclosed double quote
  'echo "still open'
  # 7. Escaped spaces
  "mkdir my\\ directory"
  # 8. Mixed quotes and escapes
  $'echo "A \\"quoted\\" word" \'and\\\'more\\\'\''
  # 9. Pipe with no command either side
  "|"
  # 10. Consecutive pipes
  "ls ||| grep foo"
  # 11. Redirections with no target
  "cat < > file"
  # 12. Here-doc delimiter only
  "cat << EOF"
  # 13. Combined redirections
  "grep foo <input.txt >>output.txt"
  # 14. Weird argument names
  "--flag=value" "-opt" "-=broken"
  # 15. Command substitution style (not lexed but ensure word handling)
  "echo \$(uname -r)"
  # 16. Trailing backslash
  "echo hello\\"
  # 17. Nested quotes (should be handled as separate words)
  "echo 'one \"two\" three'"
  # 18. Lots of alternating whitespace
  $'\t \n\t   \t\nls'
  # 19. Null byte embedded (bash will strip it, but test raw)
  $'printf "foo\x00bar"\n'
  # 20. Very long single word
  "$(printf 'a%.0s' {1..1024})"
)

echo "Running $((${#tests[@]})) tests against $LEX"
echo

for i in "${!tests[@]}"; do
  cmd="${tests[$i]}"
  printf "Test %2d: [%s]\n" "$((i+1))" "$cmd"
  # Invoke lexer; if your binary reads from argv[1] do:
  #  $LEX "$cmd"
  # Or if it reads stdin, use:
  printf "%s\n" "$cmd" | $LEX
  echo "----------------------------------------"
done

