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


struct cmdOption {
	char *name;
	char **parameters;
} CommandOption;

typedef struct ykushCmd {
	char *board;
	CommandOption *options;
} YkushCommand;

class CommandLine {

	public:
		/**
		 * CommandParser class constructor
		 * 
		 * It takes the command line arqguments as input.
		 */ 
		CommandLine(int argc, char **argv);

		YkushCommand get_command(void);
		
	protected:
		/**
		 * Sets the board in command property if it is valid.
		 * 
		 * Return values:
		 * 	0	- Board valid and set.
		 * 	1	- Error, board is not valid.
		 */
		char set_board(char *board);

		char add_option(char *option);

		/**
		 * Adds a parameter to CommandOption option.
		 */
		char add_parameter(char *parameter, CommandOption *option);

	private:
		bool error;
		YkushCommand command;
};


#endif

