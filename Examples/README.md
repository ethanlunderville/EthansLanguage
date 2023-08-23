# How To
### Declarations

##### Number Declarations
###### Note: The int keyword indicates a number type which is actually double under the hood.
```nawk
int i = 0;

int[] i = [1,2,3];

int() i = (int x) => {

	return x;
    
}
```
##### String Declarations
```nawk
string i = "Hello World";

string[] i = ["Hello", "World"];

string() i = (string x) => {

	return x;
    
}
```
### Comments
##### Comment
```nawk
// THIS IS A COMMENT
/* THIS IS A COMMENT */
```
### Control Flow
##### If-Else Statements
###### Note: This langauge does not contain booleans. Numbers are used instead.
```nawk
if (1) {

	println("Hello World");
    
} else if (2) {

	println("Hello World");
    
} else {

	println("This won't run");
    
}
```
##### While Statements
```nawk
int i = 0;

while (i < 5) {

	println(i);
    
    i++;
    
}
```
##### For Statements
```nawk
for (i : 5) {
	print(i + " ");
}

// Outputs: 1 2 3 4 5

string[] helloArray = ["Hello", "World"];

for (i : size(helloArray)) {
	print(i + " ");
}

// Outputs: Hello World
```
### Built-in Library Functions
##### Files
###### Note: There is a string internally that can act as a default argument to the file functions

```nawk
point(); // Returns an integer and accepts a single string argument. The string passed will set the default file pointer argument.

read(); // Returns a string containing file contents. Takes an optional string argument representing the file path. If this string is not provided, the internal file string will be used.

write(); // Returns an int. Takes an optional string argument representing the file path. If this string is not provided, the internal file string will be used.

append(); // Returns an int. Takes an optional string argument representing the file path. If this string is not provided, the internal file string will be used.
```

##### Text Manipulation

```nawk
// Returns string and accepts three string arguments.
// The first argument is the string to be operated on. 
// The second argument is the substring to be replaced and the third argument is the replacement string.
replace();

// Returns string[] and accepts two string arguments. 
// The first argument is the string to split and the second is the substring to split on.
split();
```

##### Regex Sections

```nawk
string testString = "Contact us at 111-222-333 or 444-555-6666 or 777-888-9999";

<(testString)> => {

	# [0-9]{3}-[0-9]{3}-[0-9]{4} # { 
    
    	/* Regex is broken into sub rules which can be used as identifiers */
        
        println("Match number: " + i);
        
        println("Full phone number: " + $ [0-9]{3}-[0-9]{3}-[0-9]{4} $);
        
        println("Area Code: " + $[0-9]{3}$[0]);
        
        println("Remaining phone number: " + $[0-9]{3}$[1] + "-" + $[0-9]{4}$);
        
        i++;
        
    }

}
```
###### Output

```nawk
Match number: 1
Full phone number: 111-222-3333
Area Code: 111
Remaining phone number: 222-3333

Match number: 2
Full phone number: 444-555-6666
Area Code: 444
Remaining phone number: 555-6666

Match number: 3
Full phone number: 777-888-9999
Area Code: 777
Remaining phone number: 888-9999
```


##### Miscellaneous
```nawk
ls(); // Returns string[].

ls("r"); // Returns string[] containg all files in the current directory as well as all sub-directories.

size(); // Returns an integer and accepts a single argument that can be either a string or an integer.

print(); // Returns an integer and accepts a single argument that can be of any type.

println(); // Returns an integer and accepts a single argument that can be of any type.

intcast(); // Returns an integer and accepts a single argument that can be either a string or an integer.

stringcast(); // Returns a string and accepts a single argument that can be either a string or an integer.

input(); // Returns a string. Program is blocked and user input is retrieved.

system(); // Returns an integer and accepts a single string argument. The passed string will be executed by the system
```