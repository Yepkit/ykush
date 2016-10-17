/****************************************************************************
 FileName:      usbcom.cpp
 Dependencies:  See INCLUDES section
 Compiler:      Visual Studio Community 2015
 Company:       Yepkit, Lda.

 Software License Agreement:

 Copyright (c) 2015 Yepkit Lda

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*****************************************************************************
 File Description:

    Change History:
        Rev     Date            Description
        ----    -----------     -----------------------------------------
        1.0     2015-09-11      First Release


 ****************************************************************************
 *  Summary:
 *
 *  Functions that communicate with the YKUSH board and/or provide
 *  status information.
 *
*****************************************************************************/



//-------------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------------
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "stdafx.h"

#ifndef LINUX
	#include "windows\hidapi.h"
#else
	#include "linux/hidapi.h"
#endif

#include "usbcom.h"


using namespace std;


//-------------------------------------------------------------------------
// DEFINES
//-------------------------------------------------------------------------
#define VERSION "0.1.0"
#define VENDOR_ID 0x04D8
#define PRODUCT_ID 0xF2F7
#define OLD_PRODUCT_ID 0x0042




/**************************************************************************
 * listDevices() - List YKUSH Devices Serial Number
 *-------------------------------------------------------------------------
 *
 * This function prints the iSerial of the
 * attached YKUSH devices.
 *
 **************************************************************************/
int listDevices() {

    	// Enumerate and print the YKUSH devices on the system
        
        hid_device *handle;
    	struct hid_device_info *devs, *cur_dev;

    	devs = hid_enumerate(0x0, 0x0);
    	if (devs == NULL) {
            //APAGAR
            // Limited Legaccy firmware support (does not work in all systems)
            //handle = hid_open(VENDOR_ID, OLD_PRODUCT_ID, NULL);
            //FIM APAGAR
            if (handle == NULL){
                // No HID devices found
                printf("\nNo HID USB devices connected to this Host\n");
            }

    	}

    	cur_dev = devs;
    	while (cur_dev) {
        	if ((cur_dev->vendor_id == VENDOR_ID) && (cur_dev->product_id == PRODUCT_ID)) {
            		printf("YKUSH device found with Serial Number: %ls", cur_dev->serial_number);
            		printf("\n");
            		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
            		printf("  Product:      %ls\n", cur_dev->product_string);
            		printf("\n");
        	} else if ((cur_dev->vendor_id == VENDOR_ID) && (cur_dev->product_id == OLD_PRODUCT_ID)) {
                    printf("YKUSH device found with Serial Number: %ls", cur_dev->serial_number);
            		printf("\n");
            		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
            		printf("  Product:      %ls\n", cur_dev->product_string);
                    printf("  NOTE: This is an old version of the firmware. If you have access to a PIC programmer contact Yepkit to get the latest version of the firmware.");
            		printf("\n");
            }

        	cur_dev = cur_dev->next;
    	}
    	hid_free_enumeration(devs);
    
	return 0;
}



/*******************************************************************************
 * commandsBySerial(char* iSerial, char *cmd, char* resp, int num) - Sends commands to YKUSH board
 * -----------------------------------------------------------------------------
 *
 * Description:
 *
 * 	All commands in the cmd array are send to the YKUSH board and the
 * 	response bytes are stored in the matching resp entry. If more than
 * 	one YKUSH board is connected to the host it will send the command
 * 	just to the first one that appears in the enumerated list.
 *
 * 	If there are more than one YKUSH board connected to the same host
 * 	the commandBySerial function should be used instead.
 *
 * Inputs:
 *
 *	iSerial	-	Serial number of the YKUSH board to issue the command.
 *	cmds	-	Command bytes to be sent to YKUSH.
 *	resp	-	Array for storing response bytes
 *	num	-	Number of entries in cmds and resp
 *
 * Outputs:
 *
 * 	The function will return the response byte or zero.
 *
 ********************************************************************************/
char commandsBySerial(char *iSerial, char *cmd, char *resp, int num)
{

	int res;
    unsigned char buf[65];
    unsigned char buf6[6];
    bool isLegacy = false;
    #define MAX_STR 255
    wchar_t wstr[MAX_STR];
    hid_device *handle;
    int i;
	const size_t newsize = 100;
	wchar_t serial[newsize];
	
     
	if (iSerial) {
		// Convert to a wchar_t*
		size_t origsize = strlen(iSerial) + 1;
		size_t convertedChars = 0;

#ifndef LINUX
		mbstowcs_s(&convertedChars, serial, origsize, iSerial, _TRUNCATE);
#else
		mbstowcs(serial, iSerial, newsize);
#endif
	}

     	// Open the YKUSH device 
        handle = hid_open(VENDOR_ID, PRODUCT_ID, iSerial ? serial : NULL);

    
        if (handle == NULL) {

            // Limited Legaccy firmware support (does not work in all systems)
            handle = hid_open(VENDOR_ID, OLD_PRODUCT_ID, NULL);

            if (handle == NULL){
                return 0;
            }

            isLegacy = true;

        }

     	// Set the hid_read() function to be blocking (wait for response from YKUSH).
    	hid_set_nonblocking(handle, 0);

	for (i = 0; i < num; i++) {
		// Send an Output report with the desired command
		if (isLegacy) {
			buf6[0] = 0; 	// First byte is report number
			buf6[1] = cmd[i];	// Command byte
			buf6[2] = cmd[i];	// Command confirm byte
			res = hid_write(handle, buf6, 6);
		} else {
			buf[0] = 0; 	// First byte is report number
			buf[1] = cmd[i];	// Command byte
			buf[2] = cmd[i];	// Command confirm byte
			res = hid_write(handle, buf, 65);
		}

		// Read response
		if (isLegacy) {
			res = hid_read(handle, buf, 6);
		} else {
			res = hid_read(handle, buf, 65);
		}

		if (res < 0) {
			printf("Unable to read YKUSH command response\n");
		}

		if (res >= 1) {
			resp[i] = buf[1];
		}
	}

    	return 0;
 }

/*******************************************************************************
 * commands(char *cmd, char* resp, int num) - Sends commands to YKUSH board
 * -----------------------------------------------------------------------------
 *
 * Description:
 *
 * 	All commands in the cmd array are send to the YKUSH board and the
 * 	response bytes are stored in the matching resp entry. If more than
 * 	one YKUSH board is connected to the host it will send the command
 * 	just to the first one that appears in the enumerated list.
 *
 * 	If there are more than one YKUSH board connected to the same host
 * 	the commandBySerial function should be used instead.
 *
 * Inputs:
 *
 *	cmds	-	Command bytes to be sent to YKUSH.
 *	resp	-	Array for storing response bytes
 *	num	-	Number of entries in cmds and resp
 *
 * Outputs:
 *
 * 	The function will return the response byte or zero.
 *
 ********************************************************************************/
char commands(char *cmd, char *resp, int num)
{
	return commandsBySerial(NULL, cmd, resp, num);
}

/*******************************************************************************
 * commandBySerial(char *iSerial, char cmd) - Sends the command to YKUSH board
 * -----------------------------------------------------------------------------
 *
 * Description:
 *
 * 	The command in the variable cmd is sent to YKUSH board. If more than
 * 	one YKUSH board is connected to the host it will send the command
 * 	just to the first one that appears in the enumerated list.
 *
 * 	If there are more than one YKUSH board connected to the same host
 * 	the commandBySerial function should be used instead.
 *
 * Inputs:
 *
 *	iSerial	-	Serial number of the YKUSH board to issue the command.
 *	cmd	-	Command byte to be sent to YKUSH.
 *
 * Outputs:
 *
 * 	The function will return the response byte or zero.
 *
 ********************************************************************************/
char commandBySerial(char *iSerial, char cmd)
{
	char resp;

	commandsBySerial(iSerial, &cmd, &resp, 1);

	return resp;
}

/*******************************************************************************
 * command(char cmd) - Sends the command to YKUSH board
 * -----------------------------------------------------------------------------
 *
 * Description:
 *
 * 	The command in the variable cmd is sent to YKUSH board. If more than
 * 	one YKUSH board is connected to the host it will send the command
 * 	just to the first one that appears in the enumerated list.
 *
 * 	If there are more than one YKUSH board connected to the same host
 * 	the commandBySerial function should be used instead.
 *
 * Inputs:
 *
 * 	cmd	-	Command byte to be sent to YKUSH.
 *
 * Outputs:
 *
 * 	The function will return the response byte.
 *
 ********************************************************************************/
char command(char cmd)
{
	return commandBySerial(NULL, cmd);
}









