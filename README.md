# Flow

Flow is a statically typed, compiled toy language I created to learn about writing a compiler. With the lexer/parser portion being heavily based on the
[how to write an interpreter in Go bool](https://interpreterbook.com/) by Thorsten Ball.

This is a work in progress, but eventually I plan on supporting all features listed here, using an llvm backend to compile to static executable.


# Language spec
## Variable declarations
Flow uses Rust-style variable declarations using the let keyword
```
let a: int = 10;
```
## Builtin types
```
// integers(64 bit)
let a: int = 10;

// strings
let s: str = "hello world";

// arrays
let arr: [int, 2] = [1, 5];
```
## Functions
```
fn add(a: int, b: int) -> int {
    return a + b;
}
```
Flow also supports lambda style functions with the following syntax
```
let add: lambda = fn(a: int, b: int) -> int {
    return a + b;
}
```
### Invoking a function
The interesting bit comes here when it comes to invoking functions, Flow supports two different syntaxes for invoking a
function, one of which being the namesake of the language.

First the usual syntax
```
let a: int = add(10, 12);
```
#### Flow operator
Flow takes inspiration from the stream operator in C++ to create a new syntax for invoking a function using the "Flow"(stream) operator
this allows you to stream function invokations together without needing to either deal with deeply nested function calls, or separate each
call into its own line, using intermediate variables to caches the results of each invokation
```
let doubleNums: lambda = fn(a: int, b: int) {
    return [a*2, b*2];
}
let result: [int, 2] = [10, 20] >> doubleNums() >> doubleNums();
// result will be (40, 80);
```
In this example, the array [10, 20] is matched to the expected arguments of the "doubleNums" function, and the result of the
call is fed into another call to the function, and then the result is assigned to the result var

# Documentation TODO
- Loops
- Inline blocks
- Example full program

# Implementation TODO
- Proper type checking at the parser level
- Arrays
- Flow operator function calls
- Lambdas
- Strings

