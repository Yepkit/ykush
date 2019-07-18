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
#include <cstdlib>



/**
 * CommandLine class constructor
 * 
 */

CommandLine::CommandLine() 
{

}

CommandLine::~CommandLine()
{
	//free command line structures
	struct command_option *cur_opt = command.options;
	while (cur_opt) {
		struct command_option *next_opt = cur_opt->next;
		free(cur_opt->name);
		struct command_parameter *cur_param = cur_opt->parameters;
		while ( cur_param ) {
			struct command_parameter *next_param = cur_param->next;
			free( cur_param->name );
			free( cur_param );
			cur_param = next_param;
		}
		free( cur_opt );
		cur_opt = next_opt;
	}
}


int CommandLine::parse(int argc, char **argv)
{
	command.app_name = argv[0];
	
	if ( argc < 2 ) 
		return -1;
	
	
	int i = 1;
	struct command_option *cur_opt = NULL;
	struct command_option *root_opt = NULL;
	
	bool exit_while;
	while ( i < ( argc - 1 ) ) {

		exit_while = false;

		if ( std::strlen(argv[i]) > 1 ) {
			if ( argv[i][0] == '-') { //is an option
				//add new option to command.options
				struct command_option *tmp_opt;
				tmp_opt = (struct command_option *) std::calloc(1, sizeof(struct command_option));
				if (cur_opt)
					cur_opt->next = tmp_opt;
				else
					root_opt = tmp_opt;
				cur_opt = tmp_opt;
				cur_opt->next = NULL;

				std::memcpy( cur_opt->name, argv[i], std::strlen( argv[i] ) + 1 );	//copy string allocating memory for destination

				i++;
				//get option parameters, if they exist
				struct command_parameter *cur_param = NULL;
				struct command_parameter *root_param = NULL;
				while ( ( i < ( argc - 1 ) ) && !exit_while ) {
					if ( argv[i][0] == '-' ) { //is another option
						i--;
						exit_while = true;
					} else { //is a parameter
						//add parameter to option
						struct command_parameter *tmp_param;
						tmp_param = (struct command_parameter *) std::calloc(1, sizeof(struct command_parameter));
						if (cur_param)
							cur_param->next = tmp_param;
						else
							root_param = tmp_param;
						cur_param = tmp_param;
						cur_param->next = NULL;
						std::memcpy( cur_param->name, argv[i], std::strlen( argv[i] ) + 1 );	//copy string allocating memory for destination

						i++;
					}
				}
				cur_opt->parameters = root_param;
			}
		}
	}

	command.options = root_opt;

	return 0;
}


int CommandLine::is_board(char *board_name) 
{
	int i = 0;
	std::string str1 ("-b");
	std::string str2 ("--board");
	std::string str3 (board_name);
	//look for -b or --board option
	struct command_option *cur_opt;
	struct command_parameter *cur_param;

	cur_opt = command.options;
	while ( cur_opt ) {
		if ( (str1.compare( cur_opt->name ) == 0) || (str2.compare( cur_opt->name ) == 0) ) {
			//board option was found.
			//Now let's check if the board name parameter matches the board_name
			cur_param = cur_opt->parameters;
			while ( cur_param ) {
				if ( str3.compare(cur_param->name) )
					return 0;			//board match
				cur_param = cur_param->next;
			}	
		}
		cur_opt = cur_opt->next;
	}
	return -1;
}


