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
 */
CommandLine::CommandLine() 
{
	error = false;
	command.n_options = 0;
	for ( int i = 0; i < 10; i++ ) {
		command.option[i].n_parameters = 0;
	} 
}

/**
 * CommandLine class constructor
 * 
 * This method will parse the command line and 
 * set the "command" property.
 * 
 * Inputs:
 * 	Command line arguments
 * 
 * Return value:
 *  	0	- OK
 * 	1	- Error
 */
int CommandLine::parse(int argc, char **argv)
{
	if (argc < 2) {
		error = true;
		return 1;
	}
	if (set_board(argv[1])) {
		error = true;
		return 1;
	}
	int i = 0;
	bool exit_while;
	while ( i < (argc - 2) ) {
		exit_while = false;
		if ( strlen(argv[i+2]) > 2 ) {
			if ( (argv[i+2][0] == '-') && (argv[i+2][1] == '-') ) {
				//add new option to command.option
				command.option[command.n_options].name = argv[i+2];
				i++;
				//get option has parameters if they exist
				while ( ( i < (argc - 2) ) && !exit_while ) {
					if ( argv[i+2][0] == '-' ) {
						i--;
						exit_while = true;
					} else {
						//add parameter to option
						command.option[command.n_options].parameter[command.option[command.n_options].n_parameters] = argv[i+2];						
						command.option[command.n_options].n_parameters++;
						i++;
					}
				}
				command.n_options++;
			}
		}
	}
	return 0;
}


char CommandLine::set_board(char *board)
{
	string str_board ("ykush");
	if ( str_board.compare(board) == 0 ) {
		command.board = new char[strlen(board)];
		strcpy(command.board, board);
		return 0;
	}
	str_board = "ykush3";
	if ( str_board.compare(board) == 0 ) {
		command.board = new char[strlen(board)];
		strcpy(command.board, board);
		return 0;
	}
	str_board = "ykushxs";
	if ( str_board.compare(board) == 0 ) {
		command.board = new char[strlen(board)];
		strcpy(command.board, board);
		return 0;
	}
	str_board = "ykush2";
	if ( str_board.compare(board) == 0 ) {
		command.board = new char[strlen(board)];
		strcpy(command.board, board);
		return 0;
	}
	str_board = "ykush3xl";
	if ( str_board.compare(board) == 0 ) {
		command.board = new char[strlen(board)];
		strcpy(command.board, board);
		return 0;
	}
	str_board = "ykushxl";
	if ( str_board.compare(board) == 0 ) {
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
