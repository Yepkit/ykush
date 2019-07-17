#include <usbhid.h>
#include <iostream>

int main(int argc, char* argv[])
{
        UsbHid *usbhid = new UsbHid();
        unsigned int pid = 0xF2F7;
        unsigned int vid = 0x04D8;
        libusb_device_handle *handle;
        int i = 0;
    	
        struct hid_device_info *devs, *cur_dev;
		
	devs = usbhid->enumerate(vid, pid);
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
                std::cout << cur_dev->serial_number_ascii;
		//printf("\n%ls\n", cur_dev->serial_number);
		cur_dev = cur_dev->next;
		i++;
	}

    	return 0;
}
