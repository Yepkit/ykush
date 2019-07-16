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


//-------------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------------
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "stdafx.h"
#include "yk_usb_device.h"

using namespace std;

int UsbDevice::listConnected() 
{
	int i=0;

	struct hid_device_info *devs, *cur_dev;
		
	devs = hid_enumerate(vid, pid);
	if (devs == NULL) 
	{
		if (handle == NULL)
		{
		// No HID devices found
		return 0;
		}
	}

	cur_dev = devs;
	while (cur_dev) 
	{
		printf("\n%ls\n", cur_dev->serial_number);
		cur_dev = cur_dev->next;
		i++;
	}
	
	hid_free_enumeration(devs);

	return i;
}


UsbDevice::UsbDevice(unsigned int vendor_id, unsigned int product_id) {
	pid = product_id;
	vid = vendor_id;
}




/**
 * Sends HID report with the data provided in the input buffer "msg". 
 *
 * \param [in] serial Pointer to a char array containing the serial number.
 * \param [in] msg Pointer to buffer containing the HID report message to be sent to USB device.
 * \param [out] resp_msg Pointer to buffer to which the HID report message replied by the device.
 * 
 * \retval 0 No error.
 * \retval 1 Unable to open USB device.
 * \retval 2 Unable to write HID report to USB device.
 * \retval 3 Unable to read HID report from USB device.
 *
 * Outputs:
 *
 *  The function returns the following value.
 *      0   -   No error
 *      -1  -   Error
 *
 *  In the case of error, a message is printed into the standard
 *  output.
 *
 *
 * Precedences: 
 *
 *  Requires that VENDOR_ID and PRODUCT_ID constants are defined.
 *
 *
 *****************************************************************/
int UsbDevice::sendHidReport(char *serial, unsigned char *msg, unsigned char *resp_msg, int report_size) {
	
    const size_t newsize = 100;
	wchar_t cserial[newsize];
    int res;	

    
    if (serial) {
#ifndef LINUX
		// Convert to a wchar_t*
		size_t origsize = strlen(serial) + 1;
		size_t convertedChars = 0;

		mbstowcs_s(&convertedChars, cserial, origsize, serial, _TRUNCATE);
#else
		mbstowcs(cserial, serial, newsize);
#endif
	}

    
	// Open the USB device 
	handle = hid_open(vid, pid, serial ? cserial : NULL);
	
	if (handle == NULL) {
		//printf("\n\nERROR: Unable to open USB device\n");
		return 1;
	}

	// Set the hid_read() function to be blocking (wait for response from the device).
	hid_set_nonblocking(handle, USB_CMD_NON_BLOCKING);

	
	//send HID report
	res = hid_write(handle, msg, report_size);

	if (res < 0) {
		printf("\n\nERROR: Unable to write HID report to USB device\n");
		hid_close(handle);
		return 2;
		}

	//read HID report
	res = hid_read(handle, resp_msg, report_size);

	if (res < 0) {
		printf("\n\nERROR: Unable to read HID report from USB device\n");
		hid_close(handle);
		return 3;
		}

	hid_close(handle);
	return 0;    //OK 
}










