# The Khalang Language

*version 0.1?*

## Definting constants:
```
(talk {const} "Constant")
```

## Defining and changing varialbes
```
(bless var "Variable")
```
Here `var` is initially `"Variable"`
```
(bless var "Test")
```
Now, `var` is `"Test"`

## Basic operations
```
(* (+ 1 1) (- 3 1))
Lion: 4
```

## Defining functions with lambda calculus
```
(fun {factorial x} {if (== x 0) {1} {(* x (factorial (- x 1)))}})
(factorial 5) -> 120
```

## REPL Usage

To quit, just type `played-myself`
