string file = read("file.txt");

/* Split on spaces */
string[] fileStrings = split(file, " ");

for ( i : size(fileStrings) ) {

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