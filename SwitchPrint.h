/*

    File: SwitchPrint.h

    Description:

    Switch board for Interpreter Output.

    
*/
#pragma once
#define INTERPRETEROUTPUT (0x1)
#ifdef INTERPRETEROUTPUT 
    #define PRINTTREE (0x1)
    //#define PRINTTOKENS (0x1)
    //#define PRINTEXPRESSION (0x1)
    //#define PRINTSYMBOLS (0X1)
    //#define PRINTDEALLOCATIONADDRESSES (0x1)
    //#define PRINTNODES (0x1)
#endif
