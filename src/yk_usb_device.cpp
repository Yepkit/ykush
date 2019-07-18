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
#include <usbhid.h>
#include <iostream>
#include <string>


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










