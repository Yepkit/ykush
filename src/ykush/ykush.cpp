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
	Ykush ykush( 0xF2F7 );
	Ykush ykushLegacy( 0x0042 );
	char port;
	char status_response = 0;

	
	if((argv[1][0]=='-') && (argv[1][1]=='s'))
	{
		if(argc < 5)
		{
		//ykush_help(argv[0]);
		ykush.print_help(argv[0]);
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
		ykush.print_help(argv[0]);
		return;
		}

	} 
	else if((argv[1][0]=='-') && (argv[1][1]=='u'))
	{
		if(argc < 3)
		{
		ykush.print_help(argv[0]);
		return;
		}

		action = YKUSH_PORT_UP;
		port = argv[2][0];
	}
	else if((argv[1][0]=='-') && (argv[1][1]=='d'))
	{
		if(argc < 3)
		{
		ykush.print_help(argv[0]);
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
		ykush.print_help(argv[0]);
		return;
		}
		action = YKUSH_GET_STATUS;
		port = argv[2][0];
	}
	else
	{
		//ykush_help(argv[0]);
		ykush.print_help(argv[0]);
		return;
	}




	switch (action)
	{
		case YKUSH_PORT_UP:
		if(bySerialFlag)
		{
		if(ykush.port_up(argv[2], port)==-1)
		{
			//try legacy
			ykushLegacy.port_up(argv[2], port);
		}
		}
		else
		{
			if(ykush.port_up(NULL, port)==-1)
			{
			//try legacy
			ykushLegacy.port_up(NULL, port);
			}
		}
		break;

		case YKUSH_PORT_DOWN:
		if(bySerialFlag)
		{
			if(ykush.port_down(argv[2], port)==-1)
			{
			//try legacy
			ykushLegacy.port_down(argv[2], port);
			}
		}
		else
		{
			if(ykush.port_down(NULL, port)==-1)
			{
			//try legacy
			ykushLegacy.port_down(NULL, port);
			}
		}
		break;

		case YKUSH_LIST_BOARDS:
		ykush_list_attached(); 
		break;

		case YKUSH_GET_STATUS:
		if(bySerialFlag)
		{
			status_response = ykush.get_port_status(argv[2], port);
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
			status_response = ykush.get_port_status(NULL, port);
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
		ykush.print_help(argv[0]);
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



void ykush_list_attached()
{
	Ykush ykush( 0xF2F7 );
	Ykush ykushLegacy( 0x0042 );

	printf("\n\nAttached YKUSH Boards:\n");        
	if(ykush.listConnected()==0)
	{
		//try legacy
		if(ykushLegacy.listConnected()==0)
		{
		printf("\n\nNo YKUSH boards found.");
		}
	} 
	else
	{
		//list legacy boards if attached 
		ykushLegacy.listConnected();
	}

	printf("\n\n");

}



void Ykush::print_help(char *exec_name)
{
	Help help( exec_name );

	help.print_version();
	help.print_usage();
	help.print_ykush();

}






