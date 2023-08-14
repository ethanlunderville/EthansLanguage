string test = "r0cgf r0 this r0is a r0 testr0";
string[] splTest = split(test, "");
string[] other = split(test, "");
println(size(splTest));
for ( i : size(splTest) ) {
    if (other[i] == splTest[i]) {
        println("ERROR " + other[i] + " " + splTest[i] );
    }
}