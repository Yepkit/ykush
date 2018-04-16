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

#include "stdafx.h"
#include "ykush3.h"
#include <stdio.h>
#include <ykush_help.h>



enum ykushAction
{
    PORT_UP,
    PORT_DOWN,
    PORT_STATUS,
    LIST_BOARDS,
    GET_STATUS,
    EXT_CTRL_ON,
    EXT_CTRL_OFF,
    READ_IO,
    WRITE_IO,
    CONFIG,
    RESET,
    HELP
};




/*********************************************************
 *  Function: ykushxs_cmd_parser
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
void ykush3_cmd_parser(int argc, char** argv)
{
    char bySerialFlag = 0;
    enum ykushAction action = HELP;
    Ykush3 *ykush = new Ykush3();
    char port;
    char value;
    char status_response = 0;
    Help *help = new Help("../doc/ykush3_help.txt");



    if((argv[2][0]=='-') && (argv[2][1]=='s'))
    {
        if(argc < 6) {
            help->print();
            return;
        }
        bySerialFlag = 1;
        if(argv[4][0]=='-' && argv[4][1]=='u') {
            action = PORT_UP;
            port = argv[5][0];
        } else if(argv[4][0]=='-' && argv[4][1]=='d') {
            action = PORT_DOWN;
            port = argv[5][0];
        } else if(argv[4][0]=='-' && argv[4][1]=='l') {
            action = LIST_BOARDS;
        } else if(argv[4][0]=='-' && argv[4][1]=='g') {
            action = GET_STATUS;
            port = argv[5][0];
        } else if(argv[4][0]=='-' && argv[4][1]=='o') {
            if(argv[4][2]=='n') {
                action = EXT_CTRL_ON;
            } else if(argv[4][2]=='f' && argv[4][3]=='f') {
                action = EXT_CTRL_OFF;
            }
        } else if(argv[4][0]=='-' && argv[4][1]=='w') {
            if(argc < 7) {
                help->print();
                return;
            }
            action = WRITE_IO;
            port = argv[5][0];
            value = argv[6][0]; 
        } else if(argv[4][0]=='-' && argv[4][1]=='r') { 
            action = READ_IO;
            port = argv[5][0];
        } else if(argv[4][0]=='-' && argv[4][1]=='c') { 
            action = CONFIG;
            port = argv[5][0];
            value = argv[6][0];
        } else if(argv[4][0]=='-' && argv[4][1]=='r' && argv[4][2]=='e') { 
            action = RESET;
        } else {
            help->print();
            return;
        }

    } 
    else if((argv[2][0]=='-') && (argv[2][1]=='u'))
    {
        if(argc < 4) {
            help->print();
            return;
        }

        action = PORT_UP;
        port = argv[3][0];
    }
    else if((argv[2][0]=='-') && (argv[2][1]=='d'))
    {
        if(argc < 4){
            help->print();
            return;
        }
        action = PORT_DOWN;
        port = argv[3][0];
    }
    else if((argv[2][0]=='-') && (argv[2][1]=='l'))
    {
        action = LIST_BOARDS;

    }
    else if((argv[2][0]=='-') && (argv[2][1]=='g'))
    {
        if(argc < 4)
        {
            help->print();
            return;
        }
        action = GET_STATUS;
        port = argv[3][0];
    }
    else if((argv[2][0]=='-') && (argv[2][1]=='o'))
    {
        if(argc < 3)
        {
            help->print();
            return;
        }
        if(argv[2][2]=='n') {
            action = EXT_CTRL_ON;
        } else if(argv[2][2]=='f' && argv[2][3]=='f') {
            action = EXT_CTRL_OFF;
        }
    }
    else if((argv[2][0]=='-') && (argv[2][1]=='w'))
    {
        if(argc < 5)
        {
            help->print();
            return;
        }
        action = WRITE_IO;
        port = argv[3][0];
        value = argv[4][0];
    }
    else if((argv[2][0]=='-') && (argv[2][1]=='c'))
    {
        if(argc < 5)
        {
            help->print();
            return;
        }
        action = CONFIG;
        port = argv[3][0];
        value = argv[4][0];
    }
    else if((argv[2][0]=='-') && (argv[2][1]=='r') && (argv[2][2]=='e'))
    {
        action = RESET;
    }
    else
    {
        help->print();
        return;
    }




    switch (action)
    {
        case PORT_UP:
            if(bySerialFlag)
            {
               ykush->port_up(argv[3], port); 
            }
            else
            {
                ykush->port_up(NULL, port); 
            }
            break;

        case PORT_DOWN:
            if(bySerialFlag)
            {
                ykush->port_down(argv[3], port); 
            }
            else
            {
                ykush->port_down(NULL, port); 
            }
            break;

        case LIST_BOARDS:
            ykush3_list_attached(); 
            break;

        case GET_STATUS:
            if(bySerialFlag)
            {
                status_response = ykush->get_port_status(argv[3], port);
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

        case EXT_CTRL_ON:
            if(bySerialFlag)
            {
               ykush->port_up(argv[3], '4'); 
            }
            else
            {
                ykush->port_up(NULL, '4'); 
            }
            break;

        case EXT_CTRL_OFF:
            if(bySerialFlag)
            {
                ykush->port_down(argv[3], '4'); 
            }
            else
            {
                ykush->port_down(NULL, '4'); 
            }
            break;


        case WRITE_IO:
            if(bySerialFlag)
            {
                ykush->write_io(argv[3], port, value); 
            }
            else
            {
                ykush->write_io(NULL, port, value); 
            }
            break;


        case READ_IO:
            if(bySerialFlag)
            {
                printf("\n%d\n", ykush->read_io(argv[3], port)); 
            }
            else
            {
                printf("\n%d\n", ykush->read_io(NULL, port)); 
            }
            break;

        case CONFIG:
            if(bySerialFlag)
            {
                ykush->config_port(argv[3], port, value); 
            }
            else
            {
                ykush->config_port(NULL, port, value); 
            }
            break;

        case RESET:
            if(bySerialFlag)
            {
                ykush->reset(argv[3]); 
            }
            else
            {
                ykush->reset(NULL); 
            }
            break;

  

        default:
            help->print();
            break;

    }

}





/********************************************************
 ********************************************************
 *
 *  Ykush Class Implementation
 *
 ********************************************************
 ********************************************************/




/*********************************************************
 * Method: port_up
 *
 * Description:
 *
 * Commands the YKUSH XS board to turn ON the downstream
 * port.
 *
 *
 *
 *
 *********************************************************/
int Ykush3::port_up(char *serial, char port)
{  
    //Create command msg
    hid_report_out[0] = 0;


    switch(port)
    {
        case '1':
            hid_report_out[1] = 0x11;
            break;

        case '2':     
            hid_report_out[1] = 0x12;
            break;
        
        case '3':     
            hid_report_out[1] = 0x13;
            break;

        case 'a':     
            hid_report_out[1] = 0x1a;
            break;

        case '4':     
            hid_report_out[1] = 0x14;
            break;

        default:
            return 0;
            break;

    }
    
    
    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 65);

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
int Ykush3::port_down(char *serial, char port)
{

    
    //Create command msg
    hid_report_out[0] = 0;      //Windows stuff

    switch(port)
    {
        case '1':
            hid_report_out[1] = 0x01;
            break;

        case '2':     
            hid_report_out[1] = 0x02;
            break;
        
        case '3':     
            hid_report_out[1] = 0x03;
            break;

        case 'a':     
            hid_report_out[1] = 0x0a;
            break;
        
        case '4':
            hid_report_out[1] = 0x04;
            break;

        default:
            return 0;
            break;

    }
  
    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 65);
   

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
int Ykush3::get_port_status(char *serial, char port)
{
    int status;

    //Create command msg
    hid_report_out[0] = 0;      

    switch(port)
    {
        case '1':
            hid_report_out[1] = 0x21;
            break;

        case '2':     
            hid_report_out[1] = 0x22;
            break;
        
        case '3':     
            hid_report_out[1] = 0x23;
            break;

        case '4':     
            hid_report_out[1] = 0x24;
            break;

        default:
            return 0;
            break;

    }
  
    //send HID report to board 
    sendHidReport(serial, hid_report_out, hid_report_in, 65);

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
int Ykush3::write_io(char *serial, char port, char value)
{
    //Create command msg
    hid_report_out[0] = 0;

    
    hid_report_out[1] = 0x31;
    if(value=='0'){
        hid_report_out[3] = 0; 
    } else {
        hid_report_out[3] = 1; 
    }


    switch(port)
    {
        case '1':
            hid_report_out[2] = 0x01;
            break;

        case '2':     
            hid_report_out[2] = 0x02;
            break;
        
        case '3':     
            hid_report_out[2] = 0x03;
            break;
 
        default:
            return 0;
            break;

    }
    
    
    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 65);

}








/****************************************************
 *
 *
 *
 *
 ****************************************************/ 
int Ykush3::read_io(char *serial, char port)
{
    //Create command msg
    hid_report_out[0] = 0;

    
    hid_report_out[1] = 0x30;

    switch(port)
    {
        case '1':
            hid_report_out[2] = 0x01;
            break;

        case '2':     
            hid_report_out[2] = 0x02;
            break;
        
        case '3':     
            hid_report_out[2] = 0x03;
            break;
 
        default:
            return 0;
            break;

    }
    
    
    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 65);
    
    //process response
    return hid_report_in[4];

}




/****************************************************
 *
 *
 *
 *
 ****************************************************/ 
int Ykush3::config_port(char *serial, char port, char value)
{
	//Create command msg
	hid_report_out[0] = 0;


	hid_report_out[1] = 0x41;
	if ( value=='0' )
		hid_report_out[3] = 0; 
	else if ( value=='0' )
		hid_report_out[3] = 1; 
	else
		hid_report_out[3] = 2;	//Persist mode
	


	switch ( port ) {
	case '1':
		hid_report_out[2] = 0x01;
		break;
	case '2':     
		hid_report_out[2] = 0x02;
		break;
	case '3':     
		hid_report_out[2] = 0x03;
		break;
	case 'e':     
		hid_report_out[2] = 0x04;
		break;
	default:
		return 0;
		break;
	}

	//send HID report to board
	return sendHidReport(serial, hid_report_out, hid_report_in, 65);

}




/****************************************************
 *
 *
 *
 *
 ****************************************************/ 
void Ykush3::reset(char *serial)
{
    //Create command msg
    hid_report_out[0] = 0;

    hid_report_out[1] = 0x55;        
    
    //send HID report to board
    sendHidReport(serial, hid_report_out, hid_report_in, 65);

}








/****************************************************
 *
 *
 *
 *
 ****************************************************/ 
void ykush3_list_attached()
{
    Ykush3 *ykush = new Ykush3();

    printf("\n\nAttached YKUSH Boards:\n");        
    if(ykush->listConnected()==0)
    {
        printf("\n\nNo YKUSH boards found.");
    } 
    
    printf("\n\n");

}









