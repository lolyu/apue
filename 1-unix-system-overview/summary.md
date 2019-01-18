# summary.md

## `extern`
`extern` changes the linkage that the function/variable is assumed to be available somewhere else and the resolving is deferred to the linker.

### function
 By default, the declaration and definition of a C function have “extern” prepended with them.

```c
int foo(int arg1, char arg2);
 ```
the compiler will treats it as below:
```c
extern int foo(int arg1, char arg2);
```
thus, for function, declaration could be done any number of times, all refer to the same function.

### variable
`errno` is historically defined as:

```c
extern int errno;
```

thus, `errno` in different files could point to the same `errno`. But if there were, then two threads could do things at the same time that both set the `errno` value, confusion would ensue.

**SOLUTION**
```c
extern int *__errno_location(void);
#define errno (*__errno_location())
```


# references
https://stackoverflow.com/questions/496448/how-to-correctly-use-the-extern-keyword-in-c
https://www.geeksforgeeks.org/understanding-extern-keyword-in-c/


