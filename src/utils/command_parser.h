// SPDX-License-Identifier: Apache-2.0

#ifndef _COMMAND_PARSER_H_
#define _COMMAND_PARSER_H_

#include <string>

struct command_parameter {
	char *value;
	struct command_parameter *next;
};

struct command_option {
	char *name;
	struct command_parameter *parameters;
	struct command_option *next;
};

struct command_line {
	char *app_name;
	struct command_option *options;
};


/*
 * Parses the command line and sets the command object.
 * 
 */
class CommandLine {

	public:
		CommandLine();
		~CommandLine();

		/*
		 * Processes the command line and sets the command property.
		 */
		int parse(int argc, char **argv);

		/*
		 * Iterates through the command_line object and looks for a board option.
		 * 
		 * Input parameters:
		 * 	
		 * 	board_name 	char array with the board name.
		 * 
		 * Return values:
		 *
		 * 	0	A valid board name was provided.
		 * 	1	Unknown error.
		 * 	2	Invalid board name provided.
		 */
		int is_board(std::string board_name);


		struct command_line command;
		
	protected:

	private:
};

#endif

