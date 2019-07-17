#include <usbhid.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
        UsbHid *usbhid = new UsbHid();
        unsigned int pid = 0xF2F7;
        unsigned int vid = 0x04D8;
        libusb_device_handle *handle;
        int i = 0;
        std::string str2 = "YK21493";
        unsigned char hid_report_out[65];
        unsigned char hid_report_in[65];
        char serialInput[] = "YK21493";
        int res;
    	
        struct hid_device_info *devs, *cur_dev;
		
        std::cout << "Get attach devices serial numbers test...\n";
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
                std::cout << cur_dev->serial_number_ascii << "\n";

                std::string str1;
                str1 = cur_dev->serial_number_ascii;
                if ( str1.compare(str2) == 0 ) {
                        std::cout << "MATCH\n";
                }
		//printf("\n%ls\n", cur_dev->serial_number);
		cur_dev = cur_dev->next;
		i++;
	}

        usbhid->free_enumeration(devs);



        std::cout << "Send command test...\n";
        //abrir device
        res = usbhid->open(vid, pid, serialInput);
        if (res < 0) {
                 std::cout << "Unable to open device";
                 return 0;
        }

        hid_report_out[0] = 0x01;       //port 1 off
        std::cout << "usbhid->write : " << usbhid->write(hid_report_out, 64) << "\n";
        std::cout << "usbhid->read : " << usbhid->read(hid_report_in, 64) << "\n";
        usbhid->close();
        for (i=0; i < 64; i++)
                std::cout << " " << std::hex << (int)hid_report_in[i];
        std::cout << std::endl;

    	return 0;
}
