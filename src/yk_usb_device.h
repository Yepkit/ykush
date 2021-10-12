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
#ifndef _YK_USB_DEVICE_H_
#define _YK_USB_DEVICE_H_

#ifdef _LIBUSB_
#include <usbhid.h>
#else
#include <hidapi/hidapi.h>
#endif
#include <stdlib.h>

/**
 * \defgroup yepkit_usb_device_handler Yepkit USB device handling
 */

#ifndef _LIBUSB_
#define USB_CMD_NON_BLOCKING	1	//1 -> Set the USB device handle to be non-blocking
#define USB_CMD_BLOCKING	0	//0 -> Set the USB device handle to be blocking and wait for response
#endif

/**
 * \ingroup yepkit_usb_device_handler
 * 
 * \brief Handles the USB HID for a given PID and VID.
 */
class UsbDevice {

	public:

		UsbDevice(unsigned int vendor_id, unsigned int product_id);
		int sendHidReport(char *serial, unsigned char *msg, unsigned char *resp_msg, int report_size);

                /**
		 * Prints to standard output the list of connected devices with PID a VID provided in the class constructor.
		 * 
		 * \return Number of attached devices.
		 */
		int listConnected(void);    //List connected devices

	private:

		unsigned short vid;       
		unsigned short pid; 
#ifndef _LIBUSB_
		hid_device *handle;
#endif

	protected:

		unsigned char hid_report_out[64];
		unsigned char hid_report_in[64];
		char *usb_serial = NULL;

};

/*****************************************************************
 * Function: 
 *
 *  send_usb_msg
 *
 *
 * Description:
 *
 *  Sends HID report with the data provided in the input buffer
 *  "msg". 
 *
 *
 * Inputs:
 *
 *      serial      -   target usb device serial number string
 *
 *      msg         -   message (HID report) to be sent
 *
 *      resp_msg    -   response message (HID report) received
 *
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
int send_usb_msg(char *serial, char *msg, char *resp_msg); 
char commands(char *cmd, char *resp, int num);
char command(char cmd);
char commandsBySerial(char *iSerial, char *cmd, char *resp, int num);
char commandBySerial(char *iSerial, char cmd);
int listDevices();


#endif // USBCOM_H
