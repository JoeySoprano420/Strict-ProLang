📘 Strict Language v2 — Expanded Syntax
1. Core Statements
Let x = 5             -- declare variable
Let y = Input         -- read from input
Print "hello"         -- print text
End                   -- close blocks

2. Control Flow
If x > y Then
    Print "x bigger"
End

If y = 1..5 Then
    Print "few"
Else
    Print "other"
End

3. Loops
For i = 0..10 Then
    Print i
End

While x < 100 Then
    x = x + 1
End

4. Functions
Func Add(a, b)
    Return a + b
End

Let z = Call Add(3, 4)
Print z

5. Data Structures (using 80–9B ops)
Let nums = [1,2,3]
List.Append nums, 4
List.Remove nums, 2

6. Matching
Match y
    Case 0: Print "none"
    Case 1..5: Print "few"
    Case 6..8: Print "many"
    Case <0: Print "negative"
End
