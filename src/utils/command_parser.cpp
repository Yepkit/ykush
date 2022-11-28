// SPDX-License-Identifier: Apache-2.0
#include <command_parser.h>
#include <string>
#include <cstring>
#include <ykush_help.h>
#include <cstdlib>
#include <iostream>

CommandLine::CommandLine() 
{
	command.options = NULL;
}

CommandLine::~CommandLine()
{
	//free command line structures
	struct command_option *cur_opt = command.options;
	while (cur_opt) {
		struct command_option *next_opt = cur_opt->next;
		free(cur_opt->name);
		struct command_parameter *cur_param = cur_opt->parameters;
		while (cur_param) {
			struct command_parameter *next_param = cur_param->next;
			free(cur_param->value);
			free(cur_param);
			cur_param = next_param;
		}
		free(cur_opt);
		cur_opt = next_opt;
	}
}

int CommandLine::parse(int argc, char **argv)
{
	command.app_name = argv[0];
	
	if (argc < 2) 
		return 1;
	
	int i = 1;
	struct command_option *cur_opt = NULL;
	struct command_option *root_opt = NULL;
	bool exit_while;
	while (i < argc) {
		exit_while = false;
		if (std::strlen(argv[i]) > 1) {
			if ((i == 1) && (argv[1][0] != '-')) {
				/* Board name */
				struct command_option *tmp_opt;
				tmp_opt = (struct command_option *)
					std::calloc(1,
						sizeof(struct command_option));

				if (cur_opt) {
					cur_opt->next = tmp_opt;
					cur_opt = cur_opt->next;
				} else {
					root_opt = tmp_opt;
					cur_opt = root_opt;
				}


				tmp_opt->name = (char *)
					std::calloc(std::strlen(argv[i]),
							sizeof(char));
				std::memcpy(tmp_opt->name,
						argv[i],
						std::strlen(argv[i]));

			}
			if (argv[i][0] == '-') {
				/* Add new option to command.options. */
				struct command_option *tmp_opt;
				tmp_opt = (struct command_option *)
					std::calloc(1,
						sizeof(struct command_option));

				if (cur_opt) {
					cur_opt->next = tmp_opt;
					cur_opt = cur_opt->next;
				} else {
					root_opt = tmp_opt;
					cur_opt = root_opt;
				}

				tmp_opt->name = (char *)
					std::calloc(std::strlen(argv[i]),
							sizeof(char));
				
				std::memcpy(tmp_opt->name,
						argv[i],
						std::strlen(argv[i]));

				/* Get option parameters, if they exist. */
				struct command_parameter *cur_param = NULL;
				struct command_parameter *root_param = NULL;
				while ((i < (argc - 1)) && !exit_while) {
					i++;
					if (argv[i][0] == '-') {
						i--;
						exit_while = true;
					} else {
						/* Parameter. */
						//add parameter to option
						struct command_parameter *tmp_param;
						tmp_param = (struct command_parameter *)
							std::calloc(1,
							sizeof(struct command_parameter));
						if (cur_param) {
							cur_param->next = tmp_param;
							cur_param = cur_param->next;
						} else {
							root_param = tmp_param;
							cur_param = root_param;
						}
						
						cur_param->value = (char *)
							std::calloc(std::strlen(argv[i]),
							sizeof(char));
						std::memcpy(cur_param->value,
							argv[i],
							std::strlen(argv[i]));
					}
				}
				cur_opt->parameters = root_param;
			}
		}
		i++;
	}
	command.options = root_opt;
	return 0;
}

/*
 * Returns 1 if board_name is in the options,
 * and 0 if not.
 */
int CommandLine::is_board(std::string board_name) 
{
	int i = 0;
	struct command_option *cur_opt;
	cur_opt = command.options;
	while (cur_opt) {
		if (board_name.compare(cur_opt->name) == 0)
			return 1;

		cur_opt = cur_opt->next;
	}
	return 0;
}


