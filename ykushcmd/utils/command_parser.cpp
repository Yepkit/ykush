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

#include <command_parser.h>
#include <string>
#include <cstring>
using namespace std;

/**
 * CommandLine class constructor
 * 
 * This constructor will parse the command line a 
 * set the "command" property.
 * 
 * Inputs:
 * 	Command line arguments
 */
CommandLine::CommandLine(int argc, char **argv)
{
	error = false;
	if (argc < 2) {
		error = true;
		return;
	}
	if (set_board(argv[1])) {
		error = true;
		return;
	}
	int i = 0;
	CommandOption option;
	bool exit_while = false:
	while (i < (argv - 2)) {
		exit_while = false;
		if (strlen(argv[i+2]) > 2) {
			if ((argv[i+2][0] == "-") && (argv[i+2][1] == "-")) {
				option.name = new char[strlen(board)];
				strcpy(option.name, &argv[i+2][2]);
				i++;
				while ((i < (argv - 2)) && !exit_while) {
					if (argv[i+2][0] == "-") {
						i--;
						exit_while = true;
					} else {
						add_parameter(argv[i+2], &option);
					}
				}
			}
		}
	}

}


/**
 * Adds a parameter to CommandOption option.
 */
char add_parameter(char *parameter, CommandOption *option)
{
	
}


char CommandLine::set_board(char *board)
{
	string str_board ("ykush");
	if (str_board.compare(board) == 0 ) {
		command.board = new char[strlen(board)];
		strcpy(command.board, board);
		return 0;
	}
	str_board = "ykush3";
	if (str_board.compare(board) == 0 ) {
		command.board = new char[strlen(board)];
		strcpy(command.board, board);
		return 0;
	}
	str_board = "ykushxs";
	if (str_board.compare(board) == 0 ) {
		command.board = new char[strlen(board)];
		strcpy(command.board, board);
		return 0;
	}
	str_board = "ykush2";
	if (str_board.compare(board) == 0 ) {
		command.board = new char[strlen(board)];
		strcpy(command.board, board);
		return 0;
	}
	str_board = "ykush3xl";
	if (str_board.compare(board) == 0 ) {
		command.board = new char[strlen(board)];
		strcpy(command.board, board);
		return 0;
	}
	str_board = "ykushxl";
	if (str_board.compare(board) == 0 ) {
		command.board = new char[strlen(board)];
		strcpy(command.board, board);
		return 0;
	}

	return 1;
}


YkushCommand CommandLine::get_command(void)
{
	return command;
}
