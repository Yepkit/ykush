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


#include <hidapi.h>





//-------------------------------------------------------------------------
// DEFINES
//-------------------------------------------------------------------------

#define USB_CMD_NON_BLOCKING	1	//1 -> Set the USB device handle to be non-blocking
					//0 -> Set the USB device handle to be blocking






/**********************************************************
 *
 *                  CLASSES
 *
 **********************************************************/
class UsbDevice {

    public:

        UsbDevice(unsigned int vendor_id, unsigned int product_id);

        int sendHidReport(char *serial, unsigned char *msg, unsigned char *resp_msg, int report_size);

        int listConnected();    //List connected devices


    private:

        unsigned short vid;       
        unsigned short pid; 

        hid_device *handle;

    protected:

        unsigned char hid_report_out[65];
        unsigned char hid_report_in[65];

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
