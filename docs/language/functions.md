# Functions

We use functions to reuse part of the code.

To declare a function:
`fun name(arg1: type, arg2: type...) -> return_type {code...}`

We also can declare of this way if there are not args:
`fun name -> return type {}`

If we have not return type:
`fun name() {}`

Or if we only declare the function:
`fun name();`

To declare that a funtion is declared externally we use:
`extern fun name(args...);`

To make a function visible in a shared library we use:
`fun name(args...) {code...} pub;`

## Call

To call a function we use:
`function(args...);`

If we not need args we can use:
`function;`

We can also integrate the call in an expression if the call returns a value:
`var x: int = sum(2, 3);`

> By the moment and undefined use of arguments is now allowed, but you can declare a external function wich use undefined parameter os this way: `extern fun name(undefan);`
