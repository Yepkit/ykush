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

#include "yk_usb_device.h"

#ifdef _LIBUSB_
#include <usbhid.h>
#else
#include <hidapi.h>
#include <string.h>
#endif

#include <iostream>
#include <string>
#include <stdlib.h>

#ifdef _LIBUSB_
// Uses libusb directly
int UsbDevice::listConnected() 
{
	UsbHid *usbhid = new UsbHid();
	struct hid_device_info *devs, *cur_dev;
	int i = 1;

	devs = usbhid->enumerate(vid, pid);
	if (devs == NULL)
		return 0;

	cur_dev = devs;
	while (cur_dev) {
                std::cout << i << ". Board found with serial number: " << cur_dev->serial_number_ascii << "\n";
		cur_dev = cur_dev->next;
		i++;
	}

        usbhid->free_enumeration(devs);

	return i;
}
#else
// Uses hidapi
int UsbDevice::listConnected() 
{
	int i=0;

	struct hid_device_info *devs, *cur_dev;
		
	devs = hid_enumerate(vid, pid);
	if (devs == NULL) {
		std::cout << "No devices found.\n";
		return 0;
	}

	cur_dev = devs;
	while (cur_dev) {
		std::cout << i << ". Board found with serial number: " << cur_dev->serial_number << "\n"; 
		cur_dev = cur_dev->next;
		i++;
	}
	
	hid_free_enumeration(devs);

	return i;
}
#endif

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
 * \retval -1 Unable to open USB device.
 * \retval -2 Unable to write HID report to USB device.
 * \retval -3 Unable to read HID report from USB device.
 *
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
#ifdef _LIBUSB_
int UsbDevice::sendHidReport(char *serial, unsigned char *msg, unsigned char *resp_msg, int report_size) 
{
	UsbHid *usbhid = new UsbHid();
	int res;	

	res = usbhid->open(vid, pid, serial);
        if (res < 0) {
		std::cout << "Unable to open device\n";
		return -1;
        }

	res = usbhid->write(msg, report_size);
	if (res < 0) {
		std::cout << "Unable to write to device\n";
		return -2;
        }

	res = usbhid->read(resp_msg, report_size);
	if (res < 0) {
		std::cout << "Unable to read from device\n";
		return -3;
        }

	usbhid->close();

	return 0;
}
#else
int UsbDevice::sendHidReport(char *serial, unsigned char *msg, unsigned char *resp_msg, int report_size) 
{	
	const size_t newsize = 100;
	wchar_t cserial[newsize];
	int res, i;
	unsigned char out_buf[65];
	unsigned char in_buf[65];

	if (report_size <= 64) {
		out_buf[0] = 0;
		for (i = 0; i < report_size; i++) {
			out_buf[i+1] = msg[i]; 
		}
	} else {
		std::cout << "Invalid report size\n";
		return -1;
	}
	
	if (serial) {
			// Convert to a wchar_t*
			size_t origsize = strlen(serial) + 1;
			size_t convertedChars = 0;

			mbstowcs_s(&convertedChars, cserial, origsize, serial, _TRUNCATE);

		}

	// Open the USB device 
	handle = hid_open(vid, pid, serial ? cserial : NULL);
	
	if (handle == NULL) {
		//printf("\n\nERROR: Unable to open USB device\n");
		return -1;
	}

	// Set the hid_read() function to be blocking (wait for response from the device).
	hid_set_nonblocking(handle, USB_CMD_NON_BLOCKING);

	//send HID report
	res = hid_write(handle, out_buf, report_size++);
	if (res < 0) {
		std::cout << "Unable to write HID report to USB device\n";
		return -1;
	}

	//read HID report
	res = hid_read(handle, in_buf, report_size++);
	if (res < 0) {
		std::cout << "Unable to read HID report from USB device\n";
		return -1;
	}
	for (i = 0; i < report_size; i++) {
		resp_msg[i] = in_buf[i];
	}

	return 0; 
}
#endif









