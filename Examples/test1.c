string testString = "Contact us at 111-222-3333 or 444-555-6666 or 777-888-9999";

int i = 1;

<(testString)> => {

	# [0-9]{3}-[0-9]{3}-[0-9]{4} # { 
    
    	/* Regex is broken into sub rules which can be used as identifiers */
        
        println("Match number: " + i);
        
        println("Full phone number: " + $ [0-9]{3}-[0-9]{3}-[0-9]{4} $);
        
        println("Area Code: " + $[0-9]{3}$[0]);
        
        println("Remaining phone number: " + $[0-9]{3}$[1] + "-" + $[0-9]{4}$ + "\n");
        
        i++;
        
    }

}