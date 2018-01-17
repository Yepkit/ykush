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
#include "ykush.h"
#include <stdio.h>
#include <ykush_help.h>



enum ykushAction
{
    PORT_UP,
    PORT_DOWN,
    PORT_STATUS,
    LIST_BOARDS,
    GET_STATUS,
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
void ykush_cmd_parser(int argc, char** argv)
{
    char bySerialFlag = 0;
    enum ykushAction action = HELP;
    Ykush *ykush = new Ykush(0xF2F7);
    Ykush *ykushLegacy = new Ykush(0x0042);
    char port;
    char status_response = 0;
    Help *help = new Help("../doc/general_help.txt");



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
            action = PORT_UP;
            port = argv[4][0];

        } else if(argv[3][0]=='-' && argv[3][1]=='d') 
        {
            action = PORT_DOWN;
            port = argv[4][0];
        } else if(argv[3][0]=='-' && argv[3][1]=='l') 
        {
            action = LIST_BOARDS;
        } else if(argv[3][0]=='-' && argv[3][1]=='g') 
        {
            action = GET_STATUS;
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

        action = PORT_UP;
        port = argv[2][0];
    }
    else if((argv[1][0]=='-') && (argv[1][1]=='d'))
    {
        if(argc < 3)
        {
            help->print();
            return;
        }
        action = PORT_DOWN;
        port = argv[2][0];
    }
    else if((argv[1][0]=='-') && (argv[1][1]=='l'))
    {
        action = LIST_BOARDS;

    }
    else if((argv[1][0]=='-') && (argv[1][1]=='g'))
    {
        if(argc < 3)
        {
            help->print();
            return;
        }
        action = GET_STATUS;
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
        case PORT_UP:
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

        case PORT_DOWN:
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

        case LIST_BOARDS:
            ykush_list_attached(); 
            break;

        case GET_STATUS:
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
int Ykush::port_up(char *serial, char port)
{  
    //Create command msg
    hid_report_out[0] = 0;      //Windows stuff


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
        return sendHidReport(serial, hid_report_out, hid_report_in, 65);
    }

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
        return sendHidReport(serial, hid_report_out, hid_report_in, 65);
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

    //Create command msg
    hid_report_out[0] = 0;      //Windows stuff

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
        sendHidReport(serial, hid_report_out, hid_report_in, 65);
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
void ykush_help(char * execName)
{

    printf("\n-------------------");
    printf("\n\tUsage for YKUSH boards:\n");
    printf("-------------------\n");
    printf("\n%s -d downstream_number \t\tTurns DOWN the downstream port with the number downstream_number\n", execName);
    printf("\n%s -u downstream_number \t\tTurns UP the downstream port with the number downstream_number\n", execName);
    printf("\n%s -g downstream_number \t\tObtains the switching status of port with the number downstream_number\n", execName);
    printf("\n%s -l \t\t\t\tLists all currently attached YKUSH boards\n", execName);
    printf("\n%s -s serial_number -d downstream_number \tTurns DOWN the downstream port with the number downstream_number for the board with the specified serial number\n", execName);
    printf("\n%s -s serial_number -u downstream_number \tTurns UP the downstream port with the number downstream_number for the board with the specified serial number\n\n\n", execName);
    printf("\n%s -s serial_number -g downstream_number \tObtains the switching status of port with the number downstream_number for the board with the specified serial number\n\n\n", execName);



}






