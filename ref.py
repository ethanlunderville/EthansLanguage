def fib(n):
    if n <= 0:
        return 0
    elif n == 1:
        return 1
    else:
        f = fib(n - 1) + fib(n - 2)
        return f 
num = 4 # Fibonacci number to calculate
print(fib(num))

