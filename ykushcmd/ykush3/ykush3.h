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



class Ykush3UsbDevice : public UsbDevice
{
	public:

		Ykush3()
		: UsbDevice(0x04D8, 0xF11B)
		{     
		};

	char send_command(unsigned char *out_msg, unsigned char *in_msg);
}


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
	void print_help();

};


/**
 * Class that parses the command line and creates the message to be sent by USB
 */
class Ykush3Command : CommandParser
{
	public:
		/**
		 * parses YKUSH3 command line arguments.
		 * 
		 * This method will receive as input the command line arguments
		 * and will set the properties that classify the command to be executed.
		 * 
		 */
		void parse_command(int argc, char **argv);

		

	protected:
		char i2c_control_config(Ykush3Action action);
		char i2c_set_address(char *address);

	private:
		bool flag_with_serial;
		char *usb_serial_number;
		struct I2cCommand {

			enum I2CAction {
				I2C_CONTROL_ENABLE,
				I2C_CONTROL_DISABLE,
				I2C_GATEWAY_ENABLE,
				I2C_GATEWAY_DISABLE,
				I2C_SET_ADRRESS,
				I2C_WRITE,
				I2C_READ	
			}

			I2cAction action;

		}
		I2cCommand i2c_command;

		
};



//---------------------------------
//FUNCTIONS
//---------------------------------

void ykush3_cmd_parser(int argc, char** argv);


void ykush3_list_attached(void); 



#endif


