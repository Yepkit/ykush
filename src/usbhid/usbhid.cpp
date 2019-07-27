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
#include <libusb.h>
#include <cstdlib>
#include <string.h>
#include <string>
#include <iostream>


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
                                        cur_dev->serial_number_ascii = get_usb_string_ascii(handle, desc.iSerialNumber);

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


void UsbHid::free_enumeration(struct hid_device_info *devs)
{
        struct hid_device_info *d = devs;
	while (d) {
		struct hid_device_info *next = d->next;
		//free(d->path);
		free(d->serial_number_ascii);
		free(d->manufacturer_string_ascii);
		free(d->product_string_ascii);
		free(d);
		d = next;
        }
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


 char *UsbHid::get_usb_string_ascii(libusb_device_handle *dev, uint8_t idx)
 {
        unsigned char buf[512];
	int len;
        wchar_t *str = NULL;

        // Get the string from libusb.
	//len = libusb_get_string_descriptor(dev, idx, 0x0409, (unsigned char*)buf, sizeof(buf));
        len = libusb_get_string_descriptor_ascii(dev, idx, (unsigned char*)buf, sizeof(buf));	
	if (len < 0)
                return NULL;
        return (char *)strdup((const char *)buf);
 }


int UsbHid::open(unsigned int vendor_id, unsigned int product_id, char *serial)
 {
        libusb_device **devs;
	libusb_device *dev;
	libusb_device_handle *handle;
	ssize_t num_devs;
        int i = 0;

        std::string str1, str2;

        if (serial != NULL)
                str1 = serial;

        if(init() < 0)
                return -1;
        num_devs = libusb_get_device_list(usb_context, &devs);
	if (num_devs < 0)
                return -1;
        //std::cout << "num_devs = " << num_devs << "\n";
        while ((dev = devs[i++]) != NULL) {
                struct libusb_device_descriptor desc;

                int res = libusb_get_device_descriptor(dev, &desc);
                if ((desc.idVendor == vendor_id) && (desc.idProduct == product_id)) {

                        res = libusb_open(dev, &open_device.handle);
                        //std::cout << "res = libusb_open = " << res << "\n";
                        if (res >= 0) {
                                if (serial == NULL) {   // Without serial number
                                        struct libusb_device_descriptor desc;
                                        struct libusb_config_descriptor *conf_desc = NULL;
                                        int f,j,k;

                                        libusb_get_device_descriptor(dev, &desc);
                                        if (libusb_get_active_config_descriptor(dev, &conf_desc) < 0) {
                                                std::cout << "cannot get active descriptor\n";
                                                libusb_free_device_list(devs, 1);
                                                libusb_close(open_device.handle);
                                                return -1; 
                                        }
                                        //std::cout << "conf_desc->bNumInterfaces = " << (unsigned int)conf_desc->bNumInterfaces << "\n";
                                        for (j = 0; j < conf_desc->bNumInterfaces; j++) {
                                                const struct libusb_interface *intf = &conf_desc->interface[j];
                                                for (k = 0; k < intf->num_altsetting; k++) {
                                                        const struct libusb_interface_descriptor *intf_desc;
                                                        intf_desc = &intf->altsetting[k];
                                                        if (intf_desc->bInterfaceClass == LIBUSB_CLASS_HID) {
                                                                //std::cout << "intf_desc->bInterfaceNumber = " << (unsigned int)intf_desc->bInterfaceNumber << "\n";
                                                                open_device.interface = intf_desc->bInterfaceNumber;
                                                                //std::cout << "intf_desc->bNumEndpoints = " << (unsigned int)intf_desc->bNumEndpoints << "\n";
                                                                // Find the INPUT and OUTPUT endpoints. An OUTPUT endpoint is not required. 
                                                                for (f = 0; f < intf_desc->bNumEndpoints; f++) {
                                                                        //std::cout << "f = " << f << "\n";
                                                                        const struct libusb_endpoint_descriptor *ep = &intf_desc->endpoint[f];

                                                                        /* Determine the type and direction of this
                                                                        endpoint. */
                                                                        int is_interrupt =
                                                                                (ep->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK)
                                                                        == LIBUSB_TRANSFER_TYPE_INTERRUPT;
                                                                        int is_output =
                                                                                (ep->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)
                                                                        == LIBUSB_ENDPOINT_OUT;
                                                                        int is_input =
                                                                                (ep->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)
                                                                        == LIBUSB_ENDPOINT_IN;

                                                                        /* Decide whether to use it for input or output. */
                                                                        if (open_device.input_endpoint == 0 &&
                                                                        is_interrupt && is_input) {
                                                                                /* Use this endpoint for INPUT */
                                                                                open_device.input_endpoint = ep->bEndpointAddress;
                                                                                open_device.input_ep_max_packet_size = ep->wMaxPacketSize;
                                                                        }
                                                                        if (open_device.output_endpoint == 0 &&
                                                                        is_interrupt && is_output) {
                                                                                /* Use this endpoint for OUTPUT */
                                                                                open_device.output_endpoint = ep->bEndpointAddress;
                                                                        }
                                                                }
                                                        }
                                                }
                                        }

 

                                        res = libusb_claim_interface(open_device.handle, open_device.interface);
                                        if (res < 0) {
                                                //try detach kernel driver
                                                int detached = 0;
                                                res = libusb_kernel_driver_active(open_device.handle, open_device.interface);
                                                if (res == 1) {
                                                        res = libusb_detach_kernel_driver(open_device.handle, open_device.interface);
                                                        if (res < 0)
                                                                std::cout << "Couldn't detach kernel driver, even though a kernel driver was attached.\n";
                                                        else
                                                                detached = 1;
                                                }
                                                res = libusb_claim_interface(open_device.handle, open_device.interface);
                                                
                                                if (res < 0) {
                                                        std::cout << "cannot claim interface\n";
                                                        libusb_free_device_list(devs, 1);
                                                        libusb_free_config_descriptor(conf_desc);
                                                        libusb_close(open_device.handle);
                                                        return -1;
                                                }      
                                        }
                                        //std::cout << "debug claim interface: " << res << "\n";

                                        /* Store off the string descriptor indexes */
                                        open_device.manufacturer_index = desc.iManufacturer;
                                        open_device.product_index      = desc.iProduct;
                                        open_device.serial_index       = desc.iSerialNumber;

                                        libusb_free_device_list(devs, 1);
                                        libusb_free_config_descriptor(conf_desc);
                                        return 0;       //Success
                                } else if (desc.iSerialNumber > 0) {    // With serial number
                                        str2 = get_usb_string_ascii(open_device.handle, desc.iSerialNumber);
                                        if ( str1.compare(str2) == 0 ) {
                                                //std::cout << "Serial found\n";
                                                struct libusb_device_descriptor desc;
                                                struct libusb_config_descriptor *conf_desc = NULL;
                                                int f,j,k;

                                                libusb_get_device_descriptor(dev, &desc);
                                                if (libusb_get_active_config_descriptor(dev, &conf_desc) < 0) {
                                                        std::cout << "cannot get active descriptor\n";
                                                        libusb_free_device_list(devs, 1);
							libusb_close(open_device.handle);
							return -1; 
                                                }
                                                //std::cout << "conf_desc->bNumInterfaces = " << (unsigned int)conf_desc->bNumInterfaces << "\n";
                                                for (j = 0; j < conf_desc->bNumInterfaces; j++) {
                                                        const struct libusb_interface *intf = &conf_desc->interface[j];
                                                        for (k = 0; k < intf->num_altsetting; k++) {
                                                                const struct libusb_interface_descriptor *intf_desc;
                                                                intf_desc = &intf->altsetting[k];
                                                                if (intf_desc->bInterfaceClass == LIBUSB_CLASS_HID) {
                                                                        //std::cout << "intf_desc->bInterfaceNumber = " << (unsigned int)intf_desc->bInterfaceNumber << "\n";
                                                                        open_device.interface = intf_desc->bInterfaceNumber;
                                                                        //std::cout << "intf_desc->bNumEndpoints = " << (unsigned int)intf_desc->bNumEndpoints << "\n";
                                                                        // Find the INPUT and OUTPUT endpoints. An OUTPUT endpoint is not required. 
                                                                        for (f = 0; f < intf_desc->bNumEndpoints; f++) {
                                                                                //std::cout << "f = " << f << "\n";
                                                                                const struct libusb_endpoint_descriptor *ep = &intf_desc->endpoint[f];

                                                                                /* Determine the type and direction of this
                                                                                endpoint. */
                                                                                int is_interrupt =
                                                                                        (ep->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK)
                                                                                == LIBUSB_TRANSFER_TYPE_INTERRUPT;
                                                                                int is_output =
                                                                                        (ep->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)
                                                                                == LIBUSB_ENDPOINT_OUT;
                                                                                int is_input =
                                                                                        (ep->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)
                                                                                == LIBUSB_ENDPOINT_IN;

                                                                                /* Decide whether to use it for input or output. */
                                                                                if (open_device.input_endpoint == 0 &&
                                                                                is_interrupt && is_input) {
                                                                                        /* Use this endpoint for INPUT */
                                                                                        open_device.input_endpoint = ep->bEndpointAddress;
                                                                                        open_device.input_ep_max_packet_size = ep->wMaxPacketSize;
                                                                                }
                                                                                if (open_device.output_endpoint == 0 &&
                                                                                is_interrupt && is_output) {
                                                                                        /* Use this endpoint for OUTPUT */
                                                                                        open_device.output_endpoint = ep->bEndpointAddress;
                                                                                }
                                                                        }
                                                                }
                                                        }
                                                }

                                                /* 
                                                int detached = 0;
                                                res = libusb_kernel_driver_active(open_device.handle, open_device.interface);
                                                if (res == 1) {
                                                        res = libusb_detach_kernel_driver(open_device.handle, open_device.interface);
                                                        if (res < 0)
                                                                std::cout << "Couldn't detach kernel driver, even though a kernel driver was attached.\n";
                                                        else
                                                                detached = 1;
                                                }
                                                */

                                                res = libusb_claim_interface(open_device.handle, open_device.interface);
						if (res < 0) {
							//try detach kernel driver
                                                        int detached = 0;
                                                        res = libusb_kernel_driver_active(open_device.handle, open_device.interface);
                                                        if (res == 1) {
                                                                res = libusb_detach_kernel_driver(open_device.handle, open_device.interface);
                                                                if (res < 0)
                                                                        std::cout << "Couldn't detach kernel driver, even though a kernel driver was attached.\n";
                                                                else
                                                                        detached = 1;
                                                        }
                                                        res = libusb_claim_interface(open_device.handle, open_device.interface);
                                                        
                                                        if (res < 0) {
                                                                std::cout << "cannot claim interface\n";
                                                                libusb_free_device_list(devs, 1);
                                                                libusb_free_config_descriptor(conf_desc);
                                                                libusb_close(open_device.handle);
                                                                return -1;
                                                        }
                                                }
                                                //std::cout << "debug claim interface: " << res << "\n";

                                                /* Store off the string descriptor indexes */
						open_device.manufacturer_index = desc.iManufacturer;
						open_device.product_index      = desc.iProduct;
						open_device.serial_index       = desc.iSerialNumber;
                                                
                                                libusb_free_device_list(devs, 1);
                                                libusb_free_config_descriptor(conf_desc);
                                                return 0;       //Success
                                        }
                                }
                        }
                        libusb_close(open_device.handle);
                } 
        }

        libusb_free_device_list(devs, 1);

        return -1;
 }


void UsbHid::close() {
        libusb_close(open_device.handle);
}


int UsbHid::write(unsigned char *data, size_t length)
{
        int res;

        /* Use the interrupt out endpoint */
        int actual_length;
        res = libusb_interrupt_transfer(open_device.handle,
                open_device.output_endpoint,
                (unsigned char*)data,
                length,
                &actual_length, 1000);
        //std::cout << "Interrupt transfer used\n";
        if (res < 0)
                return -1;


        return actual_length;
}

int UsbHid::read(unsigned char *data, int length)
{
        int res;
	int report_number = data[0];
	int skipped_report_id = 0;

        int actual_length;
        res = libusb_interrupt_transfer(open_device.handle,
                open_device.input_endpoint,
                data,
                length,
                &actual_length, 5000);
        
        //std::cout << "read res = " << res << "\n";

        if (res < 0)
                return -1;

	return actual_length;
}
