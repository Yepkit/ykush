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
#include "usbhid.h"
#include "libusb.h"
#include <cstdlib>
#include <string.h>


UsbHid::UsbHid()
{
        usb_context = NULL;
}


struct hid_device_info *UsbHid::enumerate(unsigned int vendor_id, unsigned int product_id) 
{
        libusb_device **devs;
	libusb_device *dev;
	libusb_device_handle *handle;
	ssize_t num_devs;
        int i = 0;

        struct hid_device_info *root = NULL;            // Return object. The first device found.
        struct hid_device_info *cur_dev = NULL;

        if(init() < 0)
                return NULL;

        num_devs = libusb_get_device_list(usb_context, &devs);
	if (num_devs < 0)
                return NULL;

        while ((dev = devs[i++]) != NULL) {
                struct libusb_device_descriptor desc;

                int res = libusb_get_device_descriptor(dev, &desc);
		unsigned int dev_vid = desc.idVendor;
                unsigned int dev_pid = desc.idProduct;
                if ((desc.idVendor == vendor_id) && (desc.idProduct == product_id)) {
                        struct hid_device_info *tmp;

                        // VID/PID match. Create the record.
                        tmp = (struct hid_device_info*) std::calloc(1, sizeof(struct hid_device_info));
                        if (cur_dev)
                                cur_dev->next = tmp;
                        else
                                root = tmp;
                        cur_dev = tmp;

                        // Fill out the record.
                        cur_dev->next = NULL;

                        res = libusb_open(dev, &handle);
                        if (res >= 0) {
                                /* Serial Number */
                                if (desc.iSerialNumber > 0)
                                        cur_dev->serial_number_ascii =
                                                get_usb_string_ascii(handle, desc.iSerialNumber);

                                /* Manufacturer and Product strings */
                                if (desc.iManufacturer > 0)
                                        cur_dev->manufacturer_string_ascii = get_usb_string_ascii(handle, desc.iManufacturer);
                                if (desc.iProduct > 0)
                                        cur_dev->product_string_ascii = get_usb_string_ascii(handle, desc.iProduct);

                        }
                        libusb_close(handle);
                } 
        }

        libusb_free_device_list(devs, 1);
        return root;
}


int UsbHid::init()
{
        if (!usb_context) {
		// Init Libusb
		if (libusb_init(&usb_context))
			return -1;
	}

        return 0;
}


 unsigned char *UsbHid::get_usb_string_ascii(libusb_device_handle *dev, uint8_t idx)
 {
        unsigned char buf[512];
	int len;
        wchar_t *str = NULL;

        // Get the string from libusb.
	//len = libusb_get_string_descriptor(dev, idx, 0x0409, (unsigned char*)buf, sizeof(buf));
        len = libusb_get_string_descriptor_ascii(dev, idx, (unsigned char*)buf, sizeof(buf));	
	if (len < 0)
                return NULL;
        return (unsigned char *)strdup((const char *)buf);
 }