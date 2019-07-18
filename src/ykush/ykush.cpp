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

#include "ykush.h"
#include <stdio.h>
#include <ykush_help.h>
#include <command_parser.h>
#include <iostream>


/*********************************************************
 *  Function: ykush_cmd_parser
 *
 *  Description:
 *
 *  Does the parsing of the user command and call the
 *  action to be undertaken.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************/
void ykush_cmd_parser(int argc, char** argv)
{
	char bySerialFlag = 0;
	enum ykushAction action = YKUSH_HELP;
	Ykush *ykush = new Ykush(0xF2F7);
	Ykush *ykushLegacy = new Ykush(0x0042);
	char port;
	char status_response = 0;
	Help *help = new Help("../doc/general_help.txt");

	/* 
	CommandLine *cmd_handler = new CommandLine();
	if ( cmd_handler->parse(argc, argv) ) {
		std::cout << "Error parsing command." << std::endl;
		ykush->print_help(argv[0]);
		return;
	}
	YkushCommand  user_command = cmd_handler->get_command();

	//verify if board is a YKUSH
	std::string board_str ("ykush");
	if ( board_str.compare(user_command.board) != 0 ) {
		//board is not YKUSH
		ykush->print_help(argv[0]);
		return;
	}

	//iterate through command options
	std::string option_str;
	for (int i = 0; i < user_command.n_options; i++ ) {
		option_str = user_command.option[i].name;
		
		
		//Option: --non-blocking
		if ( option_str.compare("--non-blocking") ) {	
			if ( user_command.option[i].n_parameters == 1 ) {
				ykush->set_usb_serial(user_command.option[i].parameter[0]);
			}
		}
		
	}
	*/


	//the old way...
	if((argv[1][0]=='-') && (argv[1][1]=='s'))
	{
		if(argc < 5)
		{
		//ykush_help(argv[0]);
		help->print();
		return;
		}
		bySerialFlag = 1;
		if(argv[3][0]=='-' && argv[3][1]=='u') 
		{
		action = YKUSH_PORT_UP;
		port = argv[4][0];

		} else if(argv[3][0]=='-' && argv[3][1]=='d') 
		{
		action = YKUSH_PORT_DOWN;
		port = argv[4][0];
		} else if(argv[3][0]=='-' && argv[3][1]=='l') 
		{
		action = YKUSH_LIST_BOARDS;
		} else if(argv[3][0]=='-' && argv[3][1]=='g') 
		{
		action = YKUSH_GET_STATUS;
		port = argv[4][0];
		} else 
		{
		//ykush_help(argv[0]);
		help->print();
		return;
		}

	} 
	else if((argv[1][0]=='-') && (argv[1][1]=='u'))
	{
		if(argc < 3)
		{
		help->print();
		return;
		}

		action = YKUSH_PORT_UP;
		port = argv[2][0];
	}
	else if((argv[1][0]=='-') && (argv[1][1]=='d'))
	{
		if(argc < 3)
		{
		help->print();
		return;
		}
		action = YKUSH_PORT_DOWN;
		port = argv[2][0];
	}
	else if((argv[1][0]=='-') && (argv[1][1]=='l'))
	{
		action = YKUSH_LIST_BOARDS;

	}
	else if((argv[1][0]=='-') && (argv[1][1]=='g'))
	{
		if(argc < 3)
		{
		help->print();
		return;
		}
		action = YKUSH_GET_STATUS;
		port = argv[2][0];
	}
	else
	{
		//ykush_help(argv[0]);
		help->print();
		return;
	}




	switch (action)
	{
		case YKUSH_PORT_UP:
		if(bySerialFlag)
		{
		if(ykush->port_up(argv[2], port)==-1)
		{
			//try legacy
			ykushLegacy->port_up(argv[2], port);
		}
		}
		else
		{
			if(ykush->port_up(NULL, port)==-1)
			{
			//try legacy
			ykushLegacy->port_up(NULL, port);
			}
		}
		break;

		case YKUSH_PORT_DOWN:
		if(bySerialFlag)
		{
			if(ykush->port_down(argv[2], port)==-1)
			{
			//try legacy
			ykushLegacy->port_down(argv[2], port);
			}
		}
		else
		{
			if(ykush->port_down(NULL, port)==-1)
			{
			//try legacy
			ykushLegacy->port_down(NULL, port);
			}
		}
		break;

		case YKUSH_LIST_BOARDS:
		ykush_list_attached(); 
		break;

		case YKUSH_GET_STATUS:
		if(bySerialFlag)
		{
			status_response = ykush->get_port_status(argv[2], port);
			if (status_response >> 4)
			{
			printf("\n\nDownstream port %d is ON\n\n", status_response & 0x0F );
			} 
			else 
			{
			printf("\n\nDownstream port %d is OFF\n\n", status_response & 0x0F);
			}
		}
		else
		{
			status_response = ykush->get_port_status(NULL, port);
			if (status_response >> 4)
			{
			printf("\n\nDownstream port %d is ON\n\n", status_response & 0x0F);
			} 
			else 
			{
			printf("\n\nDownstream port %d is OFF\n\n", status_response & 0x0F);
			}
		}   
		break;

		default:
		//ykush_help(argv[0]);
		help->print();
		break;

	}

}


int Ykush::port_up(char *serial, char port)
{  
	switch(port) {
	case '1':
		hid_report_out[0] = 0x11;
		break;
	case '2':     
		hid_report_out[0] = 0x12;
		break;
	case '3':     
		hid_report_out[0] = 0x13;
		break;
	case 'a':     
		hid_report_out[0] = 0x1a;
		break;
	default:
		return 0;
		break;

	}

	if(is_legacy)
		return sendHidReport(serial, hid_report_out, hid_report_in, 6);
	else
		return sendHidReport(serial, hid_report_out, hid_report_in, 64);
}


/*********************************************************
 * Method: port_down
 *
 * Description:
 *
 * Commands the YKUSH XS board to turn OFF the downstream
 * port.
 *
 *
 *
 *
 *********************************************************/
int Ykush::port_down(char *serial, char port)
{
    switch(port)
    {
        case '1':
            hid_report_out[0] = 0x01;
            break;

        case '2':     
            hid_report_out[0] = 0x02;
            break;
        
        case '3':     
            hid_report_out[0] = 0x03;
            break;

        case 'a':     
            hid_report_out[0] = 0x0a;
            break;

        default:
            return 0;
            break;

    }
  
    //send HID report to board
    if(is_legacy)
    {
        return sendHidReport(serial, hid_report_out, hid_report_in, 6);
    }
    else
    {
        return sendHidReport(serial, hid_report_out, hid_report_in, 64);
    }

}



/*********************************************************
 * Method: get_port_status
 *
 * Description:
 *
 * Commands the YKUSH XS to respond with the port status.
 *
 *  1 -> Port is ON/UP
 *  0 -> PORT is OFF/DOWN
 *
 *
 *
 *
 *********************************************************/
int Ykush::get_port_status(char *serial, char port)
{
    int status;

    switch(port)
    {
        case '1':
            hid_report_out[0] = 0x21;
            break;

        case '2':     
            hid_report_out[0] = 0x22;
            break;
        
        case '3':     
            hid_report_out[0] = 0x23;
            break;

        default:
            return 0;
            break;

    }
  
    //send HID report to board
    if(is_legacy)
    {
        sendHidReport(serial, hid_report_out, hid_report_in, 6);
    }
    else
    {
        sendHidReport(serial, hid_report_out, hid_report_in, 64);
    }

    //handle board response HID report
    status = hid_report_in[1];

    return status;
}



/****************************************************
 *
 *
 *
 *
 ****************************************************/ 
void ykush_list_attached()
{
    Ykush *ykush = new Ykush(0xF2F7);
    Ykush *ykushLegacy = new Ykush(0x0042);

    printf("\n\nAttached YKUSH Boards:\n");        
    if(ykush->listConnected()==0)
    {
        //try legacy
        if(ykushLegacy->listConnected()==0)
        {
            printf("\n\nNo YKUSH boards found.");
        }
    } 
    else
    {
        //list legacy boards if attached 
        ykushLegacy->listConnected();
    }

    printf("\n\n");

}






/***************************************************
 * Function: ykush_help
 *
 * Description:
 *
 * Prints to standard output the command usage help
 * information.
 *
 *
 *
 *
 ***************************************************/
void Ykush::print_help(char *exec_name)
{

    printf("\n-------------------");
    printf("\n\tUsage for YKUSH boards:\n");
    printf("-------------------\n");
    printf("\n%s -d downstream_number \t\tTurns DOWN the downstream port with the number downstream_number\n", exec_name);
    printf("\n%s -u downstream_number \t\tTurns UP the downstream port with the number downstream_number\n", exec_name);
    printf("\n%s -g downstream_number \t\tObtains the switching status of port with the number downstream_number\n", exec_name);
    printf("\n%s -l \t\t\t\tLists all currently attached YKUSH boards\n", exec_name);
    printf("\n%s -s serial_number -d downstream_number \tTurns DOWN the downstream port with the number downstream_number for the board with the specified serial number\n", exec_name);
    printf("\n%s -s serial_number -u downstream_number \tTurns UP the downstream port with the number downstream_number for the board with the specified serial number\n\n\n", exec_name);
    printf("\n%s -s serial_number -g downstream_number \tObtains the switching status of port with the number downstream_number for the board with the specified serial number\n\n\n", exec_name);



}






