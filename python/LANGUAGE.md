# The Khalang Language

*version 0.1?*

## Definting constants:
```
(talk const "Constant")
```

## Defining and changing varialbes
```
(bless var "Variable")
```
Here `var` is initially `"Variable"`
```
(switchup var "Test")
```
Now, `var` is `"Test"`

## Basic operations
```
(* (+ 1 1) (- 3 1))
Lion: 4
```

## Defining functions with lambda calculus
```
(bless factorial (lambda (x) (if (<= x 1) 1 (* x (factorial (- x 1))))))
(factorial 5) -> 120
```

## Abstract list functions
```
(bless lst (list 1 2 3))
(member? 2 lst) -> True
(member? 7 lst) -> False
(map (lambda (elem) (+ 1 elem)) lst) -> (2 3 4)
(foldr + 0 lst) -> 6
(foldr (lambda (x rest) (cons x rest)) (list) lst) -> (1 2 3)
(foldl (lambda (x acc) (cons acc x)) (list) lst) -> (3 2 1)
(filter (lambda (x) (>= x 2)) lst) -> (2 3)
```

## Structs
```
(struct posn (x y z))
(bless-posn coord_a "x" "y" "z") -> ("x" "y" "z")
(bless-posn coord_f 2 4) -> TypeError
(posn? coord_a) -> True
(define x posn-x-pos)
(posn-pos coord_a x) -> "x"
```

## REPL Usage

To quit, just type `played-myself`
