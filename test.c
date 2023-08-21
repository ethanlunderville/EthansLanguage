int() fib = (int n) => {
    if (n == 0) {
        return 0;
    } else if ( n == 1 ) {
        return 1;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}

int() fibonacciTimesTwo = (int() function1, int() function2, int n) => {
    return function1(n) + function2(n);
}

println(fibonacciTimesTwo(fib, fib, 10));