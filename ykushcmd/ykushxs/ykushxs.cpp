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
#include "ykushxs.h"
#include <stdio.h>
#include <ykush_help.h>


enum ykushxsAction
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
void ykushxs_cmd_parser(int argc, char** argv)
{
    char bySerialFlag = 0;
    enum ykushxsAction action = HELP;
    YkushXs *ykushxs = new YkushXs();
    Help *help = new Help("../doc/ykushxs_help.txt");

    if(argc < 3)
    {
        //ykushxs_help(argv[0]);
        help->print();
        return;
    }

    if((argv[2][0]=='-') && (argv[2][1]=='s'))
    {
        bySerialFlag = 1;
        if(argv[4][0]=='-' && argv[4][1]=='u') 
        {
            action = PORT_UP;
        } else if(argv[4][0]=='-' && argv[4][1]=='d') 
        {
            action = PORT_DOWN;
        } else if(argv[4][0]=='-' && argv[4][1]=='l') 
        {
            action = LIST_BOARDS;
        } else if(argv[4][0]=='-' && argv[4][1]=='g') 
        {
            action = GET_STATUS;
        } else 
        {
            help->print();
            return;
        }

    } 
    else if((argv[2][0]=='-') && (argv[2][1]=='u'))
    {
        action = PORT_UP;
    }
    else if((argv[2][0]=='-') && (argv[2][1]=='d'))
    {
        action = PORT_DOWN;
    }
    else if((argv[2][0]=='-') && (argv[2][1]=='l'))
    {
        action = LIST_BOARDS;
    }
    else if((argv[2][0]=='-') && (argv[2][1]=='g'))
    {
        action = GET_STATUS;
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
               ykushxs->port_up(argv[3]);
            }
            else
            {
                ykushxs->port_up(NULL);
            }
            break;

        case PORT_DOWN:
            if(bySerialFlag)
            {
                ykushxs->port_down(argv[3]);
            }
            else
            {
                ykushxs->port_down(NULL);
            }
            break;

        case LIST_BOARDS:
                ykushxs_list_attached(); 
            break;

        case GET_STATUS:
            if(bySerialFlag)
            {
                if(ykushxs->get_port_status(argv[3])==0x11)
                {
                    printf("\n\nDownstream port is ON\n\n");
                } 
                else 
                {
                    printf("\n\nDownstream port is OFF\n\n");
                }
            }
            else
            {
                if(ykushxs->get_port_status(NULL)==0x11)
                {
                    printf("\n\nDownstream port is ON\n\n");
                } 
                else 
                {
                    printf("\n\nDownstream port is OFF\n\n");
                }
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
 *  YkushXs Class Implementation
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
int YkushXs::port_up(char *serial)
{
    
    //Create command msg
    hid_report_out[0] = 0;      //Windows stuff
    hid_report_out[1] = 0x11;   //port up

    //send HID report to board
    sendHidReport(serial, hid_report_out, hid_report_in, 65);

    //handle board response HID report
    //TODO

    return 0;
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
int YkushXs::port_down(char *serial)
{
    
    //Create command msg
    hid_report_out[0] = 0;      //Windows stuff
    hid_report_out[1] = 0x01;   //port down

    //send HID report to board
    sendHidReport(serial, hid_report_out, hid_report_in, 65);

    //handle board response HID report
    //TODO

    return 0;
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
int YkushXs::get_port_status(char *serial)
{
    int status;

    //Create command msg
    hid_report_out[0] = 0;      //Windows stuff
    hid_report_out[1] = 0x21;   //get status

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
void ykushxs_list_attached()
{
    YkushXs *ykushxs = new YkushXs();
    char ** attached_serials;
    int i = 0;

    printf("\n\nAttached YKUSH XS Boards:\n");        
    if(ykushxs->listConnected()==0)
    {
        printf("\nNo YKUSH XS boards found.");
    }
    printf("\n\n");

}






/***************************************************
 * Function: ykushxs_help
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
void ykushxs_help(char * execName)
{
    printf("\n\n-------------------");
    printf("\n\tUsage:\n");
    printf("-------------------\n");
    printf("\n%s -d \t\tTurns DOWN the downstream port\n", execName);
    printf("\n%s -u \t\tTurns UP the downstream port\n", execName);
    printf("\n%s -g \t\tObtains the switching status of port\n", execName);
    printf("\n%s -l \t\tLists all currently attached YKUSH XS boards\n", execName);
    printf("\n%s -s serial_number -d \tTurns DOWN the downstream port for the board with the specified serial number\n", execName);
    printf("\n%s -s serial_number -u \tTurns UP the downstream port for the board with the specified serial number\n\n\n", execName);
    printf("\n%s -s serial_number -g \tObtains the switching status of port for the board with the specified serial number\n\n\n", execName);


}






