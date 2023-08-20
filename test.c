point("./Scratch/test");
string file = read();
println(file);
<(file)> => {
    #[0-9]{3}-[0-9]{3}-[0-9]{4}# {
        println($[0-9]{3}$[0]);
    }
}
