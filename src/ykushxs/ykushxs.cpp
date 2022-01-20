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

#include "ykushxs.h"
#include <stdio.h>
#include <ykush_help.h>


enum ykushxsAction
{
	YKUSHXS_PORT_UP,
	YKUSHXS_PORT_DOWN,
	YKUSHXS_PORT_STATUS,
	YKUSHXS_LIST_BOARDS,
	YKUSHXS_GET_STATUS,
        YKUSHXS_DATALINES_OFF,
        YKUSHXS_DATALINES_ON,
	YKUSHXS_HELP
};


int ykushxs_cmd_parser(int argc, char** argv)
{
	char bySerialFlag = 0;
	enum ykushxsAction action = YKUSHXS_HELP;
	YkushXs ykushxs;
	
	if ( argc < 3 ) {
		ykushxs.ykushxs_help(argv[0]);
		return -1;
	}

	if ( ( argv[2][0] == '-' ) && ( argv[2][1] == 's' ) ) {
		bySerialFlag = 1;
		if ( (argv[4][0] == '-') && (argv[4][1] == 'u') ) {
			action = YKUSHXS_PORT_UP;
		} else if ( (argv[4][0] == '-') && (argv[4][1] == 'd') ) {
			action = YKUSHXS_PORT_DOWN;
		} else if ( (argv[4][0] == '-') && (argv[4][1] == 'l') ) {
			action = YKUSHXS_LIST_BOARDS;
		} else if ( (argv[4][0] == '-') && (argv[4][1] == 'g') ) {
			action = YKUSHXS_GET_STATUS;
		} else if ( (argv[4][0] == '-') && (argv[4][1] == '-') && (argv[4][2] == 'd') && (argv[4][3] == 'a') ) { 
                        if ((argv[5][0] == 'o') && (argv[5][1] == 'f')) {
                                action = YKUSHXS_DATALINES_OFF;
                        } else if ((argv[5][0] == 'o') && (argv[5][1] == 'n')) {
                                action = YKUSHXS_DATALINES_ON;
                        }
                } else {
			ykushxs.ykushxs_help(argv[0]);
			return -1;
		}
	} else if ( (argv[2][0] == '-') && (argv[2][1] == 'u') ) {
		action = YKUSHXS_PORT_UP;
	} else if ( (argv[2][0] == '-') && (argv[2][1]=='d') ) {
		action = YKUSHXS_PORT_DOWN;
	} else if ( (argv[2][0] == '-') && (argv[2][1] == 'l') ) {
		action = YKUSHXS_LIST_BOARDS;
	} else if ( (argv[2][0] == '-') && (argv[2][1] == 'g') ) {
		action = YKUSHXS_GET_STATUS;
	} else if ( (argv[2][0] == '-') && (argv[2][1] == '-') && (argv[2][2] == 'd') && (argv[2][3] == 'a') ) { 
                if ((argv[3][0] == 'o') && (argv[3][1] == 'f')) {
                        action = YKUSHXS_DATALINES_OFF;
                } else if ((argv[3][0] == 'o') && (argv[3][1] == 'n')) {
                        action = YKUSHXS_DATALINES_ON;
                }
        } else {
		ykushxs.ykushxs_help(argv[0]);
		return -1;
	}

	switch ( action ) {
	case YKUSHXS_PORT_UP:
		if ( bySerialFlag ) {
			return ykushxs.port_up( argv[3] );
		} else {
			return ykushxs.port_up(NULL);
		}
		break;
	case YKUSHXS_PORT_DOWN:
		if ( bySerialFlag ) {
			return ykushxs.port_down(argv[3]);
		} else {
			return ykushxs.port_down(NULL);
		}
		break;
	case YKUSHXS_LIST_BOARDS:
		return ykushxs_list_attached(); 
		break;
	case YKUSHXS_GET_STATUS:
		if ( bySerialFlag ){
			if( ykushxs.get_port_status(argv[3]) == 0x11 ) {
				printf("\n\nDownstream port is ON\n\n");
			} else {
				printf("\n\nDownstream port is OFF\n\n");
			}
		} else {
			if ( ykushxs.get_port_status(NULL) == 0x11 ) {
				printf("\n\nDownstream port is ON\n\n");
			} else {
				printf("\n\nDownstream port is OFF\n\n");
			}
		}   
		break;
        case YKUSHXS_DATALINES_OFF:
		if ( bySerialFlag )
			return ykushxs.datalines(argv[3], 0);
		else
			return ykushxs.datalines(NULL, 0);
                break;
        case YKUSHXS_DATALINES_ON:
		if ( bySerialFlag )
			return ykushxs.datalines(argv[3], 1);
		else
			return ykushxs.datalines(NULL, 1);
                break;
	default:
		ykushxs.ykushxs_help(argv[0]); 
		return -1;
		break;

	}

}


int YkushXs::display_version_firmware(void) {
        hid_report_out[0] = 0x30;
        sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);
        //print response
        if (( hid_report_in[0] != 0x30))
        {
                std::cout << "Firmware version-1.0.0\n";
                return 0;
        }
        std::cout << "Firmware version-" << (int) hid_report_in[1] << "." << (int) hid_report_in[2] << "." << (int) hid_report_in[3] << std::endl;
        return 0;
}

int YkushXs::datalines(char *serial, int on_flag) {

        if (on_flag)
	        hid_report_out[0] = 0x41;   //datalines On
        else
	        hid_report_out[0] = 0x40;   //dataline Off
	
	int res = sendHidReport(serial, hid_report_out, hid_report_in, 64);

	if ( res < 0 )
		return res;

	if ( hid_report_in[0] == 0x01 )
		return 0;
	else
		return -1;
}

int YkushXs::port_up(char *serial) {

	hid_report_out[0] = 0x11;   //port up
	
	int res = sendHidReport(serial, hid_report_out, hid_report_in, 64);

	if ( res < 0 )
		return res;

	if ( hid_report_in[0] == 0x01 )
		return 0;
	else
		return -1;
}


int YkushXs::port_down(char *serial)
{
	hid_report_out[0] = 0x01;   //port down

	int res = sendHidReport(serial, hid_report_out, hid_report_in, 64);

	if ( res < 0 )
		return res;

	if ( hid_report_in[0] == 0x01 )
		return 0;
	else
		return -1;
}


int YkushXs::get_port_status(char *serial)
{
	int status;

	hid_report_out[0] = 0x21;   //get status

	int res = sendHidReport(serial, hid_report_out, hid_report_in, 64);

	if ( res < 0 )
		return res;

	if ( hid_report_in[0] == 0x01 )
		return hid_report_in[1];
	else
		return -1;
}


int ykushxs_list_attached()
{
	YkushXs ykushxs;

	printf("\n\nAttached YKUSH XS Boards:\n");        
	if ( ykushxs.listConnected() == 0 ) {
		printf("\nNo YKUSH XS boards found.");
	}
	printf("\n\n");
	return 0;
}



void YkushXs::ykushxs_help(char * execName)
{
	Help help( execName );

	help.print_version();
	help.print_usage();
	help.print_ykushxs();
}






