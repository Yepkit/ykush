/*******************************************************************************
Copyright 2017 Yepkit Lda (www.yepkit.com)

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

#include "ykush_help.h"
#include <iostream>

#include <stdio.h>




Help::Help(char *exec_name)
{
	app_name = exec_name;
}

void Help::print_usage(void)
{
	std::cout << "Usage:\t" << app_name << " [board_name] [OPTION]...\n";
	std::cout << "Control Yepkit YKUSH family boards.\n\n";
}

void Help::print_board_names(void)
{
	std::cout << "The board_name specifies the board model being addressed.";
	std::cout << "If no board_name is provided the board is assumed to be a YKUSH board (board_name = ykush).\n";
	std::cout << "Board names:\n";
	std::cout << " ykush\t\tYKUSH board.\n";
	std::cout << " ykush3\t\tYKUSH3 board.\n";
	std::cout << " ykushxs\tYKUSHXS board.\n";
	std::cout << "The [board_name] must be provided for commands addressed for a specific board.";
	std::cout << "For general or common commands, applicable to all board models, the [board_name] is to ommited from the command.";
}

void Help::print_common_options(void)
{
	

	
}

void Help::print_ykush(void)
{

}

void Help::print_ykush2(void)
{

}

void Help::print_ykush3(void)
{

}

void Help::print_ykushxs(void)
{

}


void Help::print_all(void) 
{
	print_usage();
	print_board_names();
}




