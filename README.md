# Khalang
*The DJ Khaled Lisp Language*

This is a Lisp like language which uses various phrases and works that are said by the immaculate DJ Khaled.

## Example Code

Function that sums everything in an int list:
```
(bless {sum} {X} {(if (equal? (length X) 0) 0 (+ (head X) (sum (tail X))))})
```

The SML code that mirrors this is
```SML
fun sum X = if X = [] then 0 else (hd X) + sum (tl X)
```
