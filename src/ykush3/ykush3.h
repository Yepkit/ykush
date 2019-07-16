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

enum ykushAction {
	PORT_UP,
	PORT_DOWN,
	PORT_STATUS,
	LIST_BOARDS,
	GET_STATUS,
	EXT_CTRL_ON,
	EXT_CTRL_OFF,
	READ_IO,
	WRITE_IO,
	CONFIG,		//ToDo: Validar
	RESET,
	GPIO_EN,
	GPIO_DIS,
	ENTER_BOOTLOADER,
	//I2C
	I2C_CONTROL_ENABLE,
	I2C_CONTROL_DISABLE,
	I2C_GATEWAY_ENABLE,
	I2C_GATEWAY_DISABLE,
	I2C_SET_ADRRESS,
	I2C_WRITE,
	I2C_READ,

	HELP
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
		
		void gpio_ctrl_enable(char *serial);
		
		void gpio_ctrl_disable(char *serial);
		
		void enter_bootloader(char *serial);


		//Configurations control
		int config_port(char *serial, char port, char value);

		//Reset
		void reset(char *serial);
		
		//Help
		void print_help(const char *help_section);

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

void ykush3_cmd_parser(int argc, char** argv);


void ykush3_list_attached(void); 



#endif


