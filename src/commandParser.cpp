/****************************************************************************
 FileName:      commandParser.cpp
 Dependencies:  See INCLUDES section
 Compiler:      Visual Studio Community 2015
 Company:       Yepkit, Lda.

 Software License Agreement:

 Copyright (c) 2015 Yepkit Lda

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*****************************************************************************
 File Description:

    Change History:
        Rev     Date            Description
        ----    -----------     -----------------------------------------
        1.0     2015-09-11      First Release


 ****************************************************************************
 *  Summary:
 *      Main program function
 *
 *
 *
*****************************************************************************/


// INCLUDES ---------------------------------------------------------------

#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include "stdafx.h"
#include "commandParser.h"
#include <ykush.h>
#include <ykushxs.h>
#include <ykush_help.h>
#include <ykush2.h>
#include <ykush3.h>




using namespace std;

extern unsigned int PRODUCT_ID;




enum Board {
    YKUSH,
    YKUSHXS,
    YKUSH2,
    YKUSH3,
};


int commandParser(int argc, char** argv) {

	
    Help *help = new Help(argv[0]);



  	if ( argc <= 1){

		//printUsage(argv[0]);
        help->print_all();
		return 0;
    	}



    //Parse input options and define action
    if(argc >= 2) 
    {
        if((argv[1][0] == 'y') && (argv[1][1]=='k') && (argv[1][2]=='u') && (argv[1][3]=='s') && (argv[1][4]=='h') && (argv[1][5]=='x') && (argv[1][6]=='s') ) 
        {
            //YKUSHXS
            ykushxs_cmd_parser(argc, argv);
            return 0;
        } 
        else if ((argv[1][0] == 'y') && (argv[1][1]=='k') && (argv[1][2]=='u') && (argv[1][3]=='s') && (argv[1][4]=='h') && (argv[1][5]=='2'))
        {
            //YKUSH2
            ykush2_cmd_parser(argc, argv);
            return 0;
        } 
        else if ((argv[1][0] == 'y') && (argv[1][1]=='k') && (argv[1][2]=='u') && (argv[1][3]=='s') && (argv[1][4]=='h') && (argv[1][5]=='3'))
        {
            //YKUSH3
            ykush3_cmd_parser(argc, argv);
            return 0;
        }
        else
        {
            //YKUSH
            ykush_cmd_parser(argc, argv);
            return 0;
        }
    }

	 
    return 0;
}



int printUsage(char* execName){

    printf("\n-------------------");
    printf("\n\tUsage:\n");
    printf("-------------------\n");
    printf("\n%s -d downstream_number \t\tTurns DOWN the downstream port with the number downstream_number\n", execName);
    printf("\n%s -u downstream_number \t\tTurns UP the downstream port with the number downstream_number\n", execName);
    printf("\n%s -g downstream_number \t\tObtains the switching status of port with the number downstream_number\n", execName);
    printf("\n%s -l \t\t\t\tLists all currently attached YKUSH boards\n", execName);
    printf("\n%s -s serial_number -d downstream_number \tTurns DOWN the downstream port with the number downstream_number for the board with the specified serial number\n", execName);
    printf("\n%s -s serial_number -u downstream_number \tTurns UP the downstream port with the number downstream_number for the board with the specified serial number\n\n\n", execName);
    printf("\n%s -s serial_number -g downstream_number \tObtains the switching status of port with the number downstream_number for the board with the specified serial number\n\n\n", execName);

    return 0;
}
