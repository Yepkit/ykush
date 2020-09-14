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

#include "ykush_help.h"
#include <iostream>

#include <stdio.h>


Help::Help(char *exec_name)
{
	app_name = exec_name;
}

void Help::print_version(void) 
{
	std::cout << "YKUSHCMD Version 1.2.5\n";
}

void Help::print_usage(void)
{
        std::cout << "Usage:\t" << app_name << " [board_name] [-s serial_number] [OPTION]...\n";
	std::cout << "Control Yepkit YKUSH family boards.\n";
}

void Help::print_board_names(void)
{
	std::cout << "\nThe board_name specifies the board model being addressed.";
	std::cout << "If no board_name is provided the board is assumed to be a YKUSH board (board_name = ykush).\n";
	std::cout << "Board names:\n";
	std::cout << " ykush\t\tYKUSH board.\n";
	std::cout << " ykush3\t\tYKUSH3 board.\n";
	std::cout << " ykushxs\tYKUSHXS board.\n";
	std::cout << "The [board_name] must be provided for commands addressed for a specific board.\n";
	std::cout << "For general or common commands, applicable to all board models, the [board_name] is to ommited from the command.\n";
}

void Help::print_common_options(void)
{



}

void Help::print_ykush(void)
{
	std::cout << "\nYKUSH board options:\n";

	std::cout << "-d 1|2|3|a                Power Down/Off downstream port with the number privided.\n";
	std::cout << "                          If [a] is provided as the port number then all ports will be switched.\n";

	std::cout << "-u 1|2|3|a                Power Up/On downstream port number downstream_number\n";

	std::cout << "-s serial_number          Board serial number to wich the command is addressed.\n";
	std::cout << "                          When multiple YKUSH boards are connected to a host, this option should be used\n";
	std::cout << "                          to specify the board. If more than one board is connected and this option is not\n";
	std::cout << "                          provided the command will be sent to the first board in the USB enumeration list.\n";

	std::cout << "-l                        Lists the serial numbers of the YKUSH boards attached to the host.\n";

	std::cout << "-g 1|2|3                  Switching state of a downstream port.\n";

	std::cout << "-h                        Display help for YKUSH board specific commands.\n";
}

void Help::print_ykush2(void)
{

}

void Help::print_ykush3(void)
{
	std::cout << "\nYKUSH3 board options:\n";

	std::cout << "-d 1|2|3|a                Power Down/Off downstream port with the number privided.\n";
	std::cout << "                          If [a] is provided as the port number then all ports will be switched.\n";

	std::cout << "-u 1|2|3|a                Power Up/On downstream port number downstream_number\n";

	std::cout << "-s serial_number          Board serial number to wich the command is addressed.\n";
	std::cout << "                          When multiple YKUSH boards are connected to a host, this option should be used\n";
	std::cout << "                          to specify the board. If more than one board is connected and this option is not\n";
	std::cout << "                          provided the command will be sent to the first board in the USB enumeration list.\n";

	std::cout << "-l                        Lists the serial numbers of the YKUSH boards attached to the host.\n";

        std::cout << "-g 1|2|3                  Switching state of a downstream port.\n";
        
        std::cout << "-c <port-number> <config-value>       Configure the default state of a downstream port\n";
        std::cout << "                                      (port-number=1|2|3|e) at power-on. The port number <e> refers to exteranl 5V port.\n";
        std::cout << "                                      The default states are off (config-value=0), on (config-value=1)\n";
        std::cout << "                                      and persistent (config-value=2).\n";

	std::cout << "-on                       Switch On the 5V output power port.\n";
	std::cout << "-off                      Switch Off the 5V output power port.\n";

	std::cout << "-r 1|2|3                  Read GPIO with the number provided (1, 2 or 3).\n";
	std::cout << "-w 1|2|3 0|1              Write to the GPIO with the number provided (1, 2 or 3).\n";
	std::cout << "                          Writing a value of 1 or 0 will drive the GPIO to logical high or low, respectively.\n";

        std::cout << "--gpio enable|disable     Enable / Disable GPIO control interface. Will become active on next reset/power-on.\n\n";

	std::cout << "--reset                   Resets (reboot) the YKUSH3 board.\n";

	std::cout << "-h                        Display help for YKUSH board specific commands.\n";

        std::cout << "--bootloader-version      Board bootloader version.\n";
        std::cout << "--firmware-version        Board firmware version.\n";
        
	std::cout << "--boot                    Enter bootloader mode for firmware update.\n";
}

void Help::print_ykushxs(void)
{
	std::cout << "\nYKUSHXS board options:\n";

	std::cout << "-d                        Power Down/Off the downstream port.\n";

	std::cout << "-u                        Power Up/On the downstream port\n";

	std::cout << "-s serial_number          Board serial number to wich the command is addressed.\n";
	std::cout << "                          When multiple YKUSHXS boards are connected to a host, this option should be used\n";
	std::cout << "                          to specify the board. If more than one board is connected and this option is not\n";
	std::cout << "                          provided the command will be sent to the first board in the USB enumeration list.\n";

	std::cout << "-l                        Lists the serial numbers of the YKUSHXS boards attached to the host.\n";

	std::cout << "-g                        Switching state of a downstream port.\n";

	std::cout << "-h                        Display help for YKUSHXS board specific commands.\n";
}


void Help::print_all(void)
{
	print_version();
	print_usage();
	print_board_names();
	print_ykush();
	print_ykush2();
	print_ykush3();
	print_ykushxs();
}




