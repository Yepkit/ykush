/*******************************************************************************
Copyright 2019 Yepkit Lda (www.yepkit.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/


#ifndef _COMMAND_PARSER_H_
#define _COMMAND_PARSER_H_


struct command_parameter {
	char *name;
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

/**
 * \defgroup command_line Command line
 */

/**
 * \ingroup command_line
 * \brief Parses the command line and sets the command object.
 * 
 */
class CommandLine {

	public:
		CommandLine();

		/**
		 * \brief Processes the command line and sets the command property.
		 */
		int parse(int argc, char **argv);

		/**
		 * \brief Iterates through the command_line object and looks for a board option.
		 * 
		 * \retval 0 board found
		 * \retval -1 no board option provided
		 * 
		 */
		int get_board(char *board_name);


		struct command_line get_command(void);
		
	protected:
		/**
		 * Sets the board in command property if it is valid.
		 * 
		 * Return values:
		 * 	0	- Board valid and set.
		 * 	1	- Error, board is not valid.
		 */
		char set_board(char *board);

		

	private:
		struct command_line command;

};


#endif

