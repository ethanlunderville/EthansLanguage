int() printArray = (int [] args, int () printFunction, int () otherPrintFunction) => {
    int i = 0;
    while (i < 5) {
        if (i < 3) {
            otherPrintFunction(args[i]);
        } else {
            printFunction(args[i]);
        }
        i = i + 1;
    }
    return 1;
}

int() printer = (int a) => { 
    print(a); 
}

int() oprint = (int a) => { 
    print("Other print"); 
    print(a); 
}

int[] sd = [1,2,3,4,5];
printArray(sd, printer, oprint);

//<STREAM>::[0-9]{3}-[0-9]{3}-[0-9]{4} -> {
//    print("Phone number: " + [0-9]{3} );
//    <[0-9]{3}>::<DELIMITER> -> {
//    }
//}
//<STREAM>::<DELIMITER> -> {
//}
//int b = 5;
//b = 4;
//
//int[] a = [1,2,3];
//print(a[1]);
//a[1] = 5;
//print(a[1]);



