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
#ifndef _USBHID_H_
#define _USBHID_H_

#include "libusb.h"


/** hidapi info structure */
struct hid_device_info {
        /** Platform-specific device path */
        char *path;
        /** Device Vendor ID */
        unsigned short vendor_id;
        /** Device Product ID */
        unsigned short product_id;
        /** Serial Number */
        wchar_t *serial_number;
        unsigned char *serial_number_ascii;
        /** Device Release Number in binary-coded decimal,
                 also known as Device Version Number */
        unsigned short release_number;
        /** Manufacturer String */
        wchar_t *manufacturer_string;
        unsigned char *manufacturer_string_ascii;
        /** Product string */
        wchar_t *product_string;
        /** Usage Page for this Device/Interface
                 (Windows/Mac only). */
        unsigned char *product_string_ascii;
        unsigned short usage_page;
        /** Usage for this Device/Interface
                 (Windows/Mac only).*/
        unsigned short usage;
        /** The USB interface which this logical device
                 represents.
                * Valid on both Linux implementations in all cases.
                * Valid on the Windows implementation only if the device
                        contains more than one interface.
                * Valid on the Mac implementation if and only if the device
                        is a USB HID device. */
        int interface_number;

        /** Pointer to the next device */
        struct hid_device_info *next;
};


class UsbHid {
        public:
                /**
                 * \brief UsbHid class constructor.
                 */
                UsbHid();

                /**
                 * \brief Enumerates the USB devices with the provided VID and PID.
                 * 
                 * \return List of USB devices currently attached to the system with the provided VID and PID. 
                 */
                struct hid_device_info *enumerate(unsigned int vendor_id, unsigned int product_id);
        
        private:
                libusb_context *usb_context;

                int init();

                wchar_t *get_usb_string(libusb_device_handle *dev, uint8_t idx);

                unsigned char *get_usb_string_ascii(libusb_device_handle *dev, uint8_t idx);
                


};


#endif


