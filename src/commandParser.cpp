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
#include <string>


extern unsigned int PRODUCT_ID;

enum Board {
	YKUSH,
	YKUSHXS,
	YKUSH2,
	YKUSH3,
};


int commandParser(int argc, char** argv) 
{	
    	Help help(argv[0]);
	std::string str_ykush ("ykush");
	std::string str_ykush3 ("ykush3");
	std::string str_ykush2 ("ykush2");
	std::string str_ykushxs ("ykushxs");

  	if ( argc <= 1) {
        	help.print_all();
		return 0;
    	}

	//Parse input options and define action
	if( argc >= 2) {
		if ( str_ykushxs.compare( argv[1]) == 0 ) 
		{
			//YKUSHXS
			ykushxs_cmd_parser(argc, argv);
			return 0;
		} else if ( str_ykush2.compare( argv[1]) == 0 ) {
			//YKUSH2
			ykush2_cmd_parser(argc, argv);
			return 0;
		} else if ( str_ykush3.compare( argv[1]) == 0) {
			//YKUSH3
			ykush3_cmd_parser(argc, argv);
			return 0;
		} else if ( str_ykush.compare( argv[1]) == 0 ) {
			//YKUSH
			if ( argc >= 3 )
				ykush_cmd_parser(argc - 1 , &argv[1]);
			else
				help.print_all();
			
			return 0;
		} else {
			//YKUSH
			ykush_cmd_parser(argc, argv);
			return 0;
		}
	}
	
	return 0;
}



