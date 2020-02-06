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


#ifndef _YKUSH3_H_
#define _YKUSH3_H_


#include <yk_usb_device.h>
#include <command_parser.h>

enum ykush3Action {
	YKUSH3_PORT_UP,
	YKUSH3_PORT_DOWN,
	YKUSH3_PORT_STATUS,
	YKUSH3_LIST_BOARDS,
	YKUSH3_GET_STATUS,
	YKUSH3_EXT_CTRL_ON,
	YKUSH3_EXT_CTRL_OFF,
	YKUSH3_READ_IO,
	YKUSH3_WRITE_IO,
	YKUSH3_CONFIG,
	YKUSH3_RESET,
	YKUSH3_GPIO_EN,
	YKUSH3_GPIO_DIS,
	YKUSH3_ENTER_BOOTLOADER,
	//I2C
	YKUSH3_I2C_CONTROL_ENABLE,
	YKUSH3_YKUSH3_I2C_CONTROL_DISABLE,
	YKUSH3_I2C_GATEWAY_ENABLE,
	YKUSH3_I2C_GATEWAY_DISABLE,
	YKUSH3_I2C_SET_ADRRESS,
	YKUSH3_I2C_WRITE,
	YKUSH3_I2C_READ,

	YKUSH3_HELP
};


class Ykush3 : public UsbDevice  
{
    	public:

		Ykush3()
		: UsbDevice(0x04D8, 0xF11B)
		{     
		}

		//Downstream ports and 5V EXT port control 
		int get_port_status(char *serial, char port);     //get downstream port status

		int port_up(char *serial, char port);

		int port_down(char *serial, char port);


		//GPIO control
		int write_io(char *serial, char port, char value);

		int read_io(char *serial, char port);
		
		int gpio_ctrl_enable(char *serial);
		
		int gpio_ctrl_disable(char *serial);
		
		int enter_bootloader(char *serial);


		//Configurations control
		int config_port(char *serial, char port, char value);

		//Reset
		int reset(char *serial);
		
		//Help
		void print_help(void);

		//I2C
		int i2c_enable_disable_control(bool enable_flag);		//ToDo
		int i2c_enable_disable_gateway(bool enable_flag);		//ToDo
		int i2c_set_address(char *i2c_address);			//ToDo
		int i2c_write(char *i2c_address_ASCII, char *num_bytes_ASCII, char **data_to_write_ASCII);	//ToDo
		int i2c_read(char *i2c_address_ASCII, char *num_bytes_ASCII, unsigned char *data_buffer, int *bytes_read);	//ToDo

		//Versioning
		int display_version_bootloader(void);
		int display_version_firmware(void);

		int set_usb_serial(char *serial);
	
	private:

		char *usb_serial;
};





//---------------------------------
//FUNCTIONS
//---------------------------------

int ykush3_cmd_parser(int argc, char** argv);


int ykush3_list_attached(void); 



#endif


