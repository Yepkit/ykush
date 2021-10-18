// SPDX-License-Identifier: Apache-2.0
/*
 * Board routing command parser.
 */
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "commandParser.h"
#include <ykush.h>
#include <ykushxs.h>
#include <ykush_help.h>
#include <ykush2.h>
#include <ykush3.h>
#include <string>
#include <command_parser.h>

extern unsigned int PRODUCT_ID;

enum Board {
	YKUSH,
	YKUSHXS,
	YKUSH2,
	YKUSH3,
};

int commandParser(int argc, char** argv) 
{	
	CommandLine cmdl;
    	Help help(argv[0]);
	std::string str_ykush ("ykush");
	std::string str_ykush3 ("ykush3");
	std::string str_ykush2 ("ykush2");
	std::string str_ykushxs ("ykushxs");

  	if (argc <= 1) {
        	help.print_all();
		return 0;
    	}

	cmdl.parse(argc, argv);


	if(argc >= 2) {
		if (str_ykushxs.compare(argv[1]) == 0) {
			//YKUSHXS
			return ykushxs_cmd_parser(argc, argv);
		} else if (str_ykush2.compare(argv[1]) == 0) {
			//YKUSH2
			ykush2_cmd_parser(argc, argv);
			return 0;
		} else if (cmdl.is_board("ykush3")) {
			//YKUSH3
			return ykush3_action_parser(&cmdl.command);
			//return ykush3_cmd_parser(argc, argv);
		} else if (str_ykush.compare(argv[1]) == 0) {
			//YKUSH
			if (argc >= 3)
				return ykush_cmd_parser(argc - 1 , &argv[1]);
			else
				help.print_all();
			
			return -1;
		} else {
			//YKUSH
			return ykush_cmd_parser(argc, argv);
		}
	}
	
	return 0;
}



