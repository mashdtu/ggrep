#!/bin/bash

GGREP="./build/ggrep"
PASS=0
FAIL=0

pass() {
    PASS=$((PASS + 1))
}

fail() {
    FAIL=$((FAIL + 1))
    echo "  FAIL: $1"
}

assert_output() {
    local desc="$1"
    local expected="$2"
    local actual="$3"

    if [ "$actual" = "$expected" ]; then
        pass
    else
        fail "$desc"
        echo "    expected: $(echo "$expected" | head -5)"
        echo "    actual:   $(echo "$actual" | head -5)"
    fi
}

assert_exit() {
    local desc="$1"
    local expected="$2"
    local actual="$3"

    if [ "$actual" -eq "$expected" ]; then
        pass
    else
        fail "$desc (expected exit $expected, got $actual)"
    fi
}

echo "Running ggrep tests..."
echo ""


echo "Basic matching"

out=$($GGREP "hello" tests/test.txt)
assert_output "basic match" "hello again
a line with hello in the middle" "$out"

out=$($GGREP "Hello" tests/test.txt)
assert_output "case sensitive" "Hello world" "$out"

out=$($GGREP "HELLO" tests/test.txt)
assert_output "uppercase match" "HELLO" "$out"

out=$($GGREP "Something" tests/test.txt)
assert_output "pattern at start" "Something else" "$out"

out=$($GGREP "middle" tests/test.txt)
assert_output "pattern at end" "a line with hello in the middle" "$out"

$GGREP "nonexistent pattern" tests/test.txt > /dev/null 2>&1
assert_exit "no match exits 1" 1 $?

out=$($GGREP "" tests/test.txt | wc -l | tr -d ' ')
assert_output "empty pattern matches all" "6" "$out"


echo "Regex matching"

out=$(echo "hello world" | $GGREP -P 'h')
assert_output "regex matches within line" "hello world" "$out"


echo "Case insensitive"

out=$($GGREP -i "hello" tests/test.txt)
assert_output "-i matches all cases" "Hello world
hello again
HELLO
a line with hello in the middle" "$out"

out=$($GGREP -i "HELLO" tests/test.txt)
assert_output "-i with uppercase pattern" "Hello world
hello again
HELLO
a line with hello in the middle" "$out"


echo "Line numbers"

out=$($GGREP -n "hello" tests/test.txt)
assert_output "-n basic" "3:hello again
6:a line with hello in the middle" "$out"

out=$($GGREP -in "hello" tests/test.txt)
assert_output "-in combined" "1:Hello world
3:hello again
5:HELLO
6:a line with hello in the middle" "$out"


echo "Invert match"

out=$($GGREP -v "hello" tests/test.txt)
assert_output "-v basic" "Hello world
This is a test
Something else
HELLO" "$out"

out=$($GGREP -inv "hello" tests/test.txt)
assert_output "-inv combined" "2:This is a test
4:Something else" "$out"


echo "Combined options"

out=$($GGREP -vn "hello" tests/test.txt)
assert_output "-vn" "1:Hello world
2:This is a test
4:Something else
5:HELLO" "$out"


echo "Stdin"

out=$(echo "hello world" | $GGREP "hello")
assert_output "stdin basic" "hello world" "$out"

out=$(printf "one\ntwo\nthree\n" | $GGREP "two")
assert_output "stdin multiline" "two" "$out"

out=$(printf "Hello\nhello\nHELLO\n" | $GGREP -in "hello")
assert_output "stdin with -in" "1:Hello
2:hello
3:HELLO" "$out"

echo "no match" | $GGREP "zzz" > /dev/null 2>&1
assert_exit "stdin no match exits 1" 1 $?

echo "hello" | $GGREP "hello" > /dev/null 2>&1
assert_exit "stdin match exits 0" 0 $?


echo "Multiple files"

tmp_other=$(mktemp)
echo "hello from other" > "$tmp_other"

out=$($GGREP "hello" tests/test.txt "$tmp_other")
assert_output "multiple files" "tests/test.txt:hello again
tests/test.txt:a line with hello in the middle
${tmp_other}:hello from other" "$out"

out=$($GGREP -n "hello" tests/test.txt "$tmp_other")
assert_output "multiple files with -n" "tests/test.txt:3:hello again
tests/test.txt:6:a line with hello in the middle
${tmp_other}:1:hello from other" "$out"

rm -f "$tmp_other"


echo "Exit codes"

$GGREP "hello" tests/test.txt > /dev/null 2>&1
assert_exit "match exits 0" 0 $?

$GGREP "zzzzz" tests/test.txt > /dev/null 2>&1
assert_exit "no match exits 1" 1 $?

$GGREP "hello" nonexistent_file.txt > /dev/null 2>&1
assert_exit "missing file exits 2" 2 $?

$GGREP -z "hello" tests/test.txt > /dev/null 2>&1
assert_exit "invalid option exits 2" 2 $?

$GGREP > /dev/null 2>&1
assert_exit "missing pattern exits 2" 2 $?


echo "Error handling"

out=$($GGREP "hello" nonexistent_file.txt 2>&1)
echo "$out" | grep -q "cannot open"
assert_exit "missing file prints error" 0 $?

out=$($GGREP -z "test" 2>&1)
echo "$out" | grep -q "unknown option"
assert_exit "unknown option prints error" 0 $?

out=$($GGREP 2>&1)
echo "$out" | grep -q "missing pattern"
assert_exit "missing pattern prints error" 0 $?


echo "Edge cases"

out=$(printf "" | $GGREP "hello"); ec=$?
assert_exit "empty input no match" 1 $ec

out=$($GGREP "this is way longer than any line" tests/test.txt); ec=$?
assert_exit "pattern longer than lines" 1 $ec

printf "no trailing newline" | $GGREP "trailing" > /dev/null
assert_exit "line without trailing newline" 0 $?

out=$(echo "hello hello hello" | $GGREP "hello")
assert_output "multiple occurrences on line" "hello hello hello" "$out"

out=$($GGREP -- "-v" tests/test.txt); ec=$?
assert_exit "-- separator allows dash pattern" 1 $ec


echo ""
TOTAL=$((PASS + FAIL))
echo "Results: $PASS/$TOTAL passed"
if [ "$FAIL" -gt 0 ]; then
    echo "$FAIL test(s) FAILED"
    exit 1
fi
echo "All tests passed!"
exit 0
