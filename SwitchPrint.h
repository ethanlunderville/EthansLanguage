/*

    SWITCH BOARD FOR INTERPRETER OUTPUT.

*/
#pragma once
//#define INTERPRETEROUTPUT (0x1)
#ifdef INTERPRETEROUTPUT 
    #define PRINTTOKENS (0x1)
    #define PRINTTREE (0x1)
    #define PRINTEXPRESSION (0x1)
    #define PRINTSYMBOLS (0X1)
    //#define PRINTDEALLOCATIONADDRESSES (0x1)
    //#define PRINTNODES (0x1)
    //#define PRINTDECSANDASSIGNMENTS (0x1)
    //#define PRINTCALLSTACK (0x1)
    //#define REGEXDEBUG (0x1)
#endif
