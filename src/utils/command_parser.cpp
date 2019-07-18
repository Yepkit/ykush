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
#include <ykush_help.h>



/**
 * CommandLine class constructor
 * 
 */

CommandLine::CommandLine() 
{
	error = false;
	command.n_options = 0;
	for( int i = 0; i < 10; i++ ) {
		command.option[i].n_parameters = 0;
	} 
}


int CommandLine::parse(int argc, char **argv)
{
	command.app_name = argv[0];
	
	if ( argc < 2 ) 
		return -1;
	
	if ( set_board(argv[1]) )
		return -1;
	
	int i = 1;
	bool exit_while;
	while ( i < ( argc - 1 ) ) {

		exit_while = false;

		if ( strlen(argv[i]) > 1 ) {
			if ( argv[i][0] == '-') { //is an option
				//add new option to command.option
				command.option[command.n_options].name = argv[i];
				i++;
				//get option has parameters if they exist
				while ( ( i < ( argc - 1 ) ) && !exit_while ) {
					if ( argv[i][0] == '-' ) { //is another option
						i--;
						exit_while = true;
					} else { //is a parameter
						//add parameter to option
						command.option[command.n_options].parameter[command.option[command.n_options].n_parameters] = argv[i];						
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
