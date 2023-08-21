# Neoawk Scripting Language

**Neoawk** is a strongly typed scripting language designed to optimize text parsing tasks. It combines the syntax characteristics of C, JavaScript and Bash while maintaining the simplicity of Python. Neoawk is currently operational and Turing complete.

> Please note that Neoawk's development is ongoing, with continuous efforts to expand its capabilities.

## Features

- **Text Parsing Optimization:** Neoawk is engineered to excel at text parsing tasks, making it a powerful tool for data manipulation and processing.

- **C-JavaScript-Bash Hybrid Syntax:** The language's syntax draws inspiration from C, JavaScript, and Bash providing a familiar yet expressive coding experience.

- **Simplicity:** Despite its powerful capabilities, Neoawk remains simple to learn and use, allowing users to quickly get started with text processing tasks.

## Examples

Check out some examples of what you can achieve with Neoawk:

- 1) Powerful Regular Expression Matching
- 2) Text extraction and manipulation
- 3) Functional Programming with first class functions

 ##### file.txt
```nawk
Contact us at 111-222-333 or 444-555-6666 or 777-888-9999
```

#### Example 1
###### regex.nawk
```nawk
string file = read("file.txt");

int i = 1;

<(file)> => {

    /* Every match of this regex in the file string will trigger the code in the following scope to run */
    
    # [0-9]{3}-[0-9]{3}-[0-9]{4} # { 
    
    	/* Regex is broken into sub rules which can be used as identifiers*/
        
        println("Match number: " + i);
        
        println("Area Code: " + $[0-9]{3}$[0]);
        
        println("Remaining phone number: " + $[0-9]{3}$[1] + "-" + $[0-9]{4}$);
        
        i++;
        
    }
}
```
##### Note: If a subrule appears more than once in the regex, the identifier associated with it becomes an array instead of a string. In this case, [0-9]{3} appears twice in the regex so each submatch has its own array subscript.
###### output
```nawk
Match number: 1
Area Code: 111
Remaining phone number: 222-3333

Match number: 2
Area Code: 444
Remaining phone number: 555-6666

Match number: 3
Area Code: 777
Remaining phone number: 888-9999
```
#### Example 2
###### splitAndReplace.nawk
```nawk
string file = read("file.txt");

/* Split on spaces */
string[] fileStrings = split(file, " ");

for (i:size(fileStrings)) {

    string current = fileStrings[i];

    <(current)> => {

        # [0-9]{3}-[0-9]{3}-[0-9]{4} # {

			/* Remove all dashes and print */
            println(
                replace($[0-9]{3}-[0-9]{3}-[0-9]{4}$, "-", "")
            );

        }

    }

}
```
###### output
```nawk
1112223333
4445556666
7778889999
```
#### Example 3
###### firstClassFunctions.nawk
```nawk
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

println(fib(10)); /* 55 */

println(fibonacciTimesTwo(fib, fib, 10));
```
###### output
```nawk
55 
110
```

## License

Neoawk is distributed under the [MIT License](LICENSE). Feel free to use and modify it according to your needs.
