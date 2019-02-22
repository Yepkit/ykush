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

#include "stdafx.h"
#include "ykush3.h"
#include <stdio.h>
#include <ykush_help.h>
#include <command_parser.h>
#include <iostream>
#include <string>
#include <string2val.h>
#include <cstring>




/*********************************************************
 *  Function: ykushxs_cmd_parser
 *
 *  Description:
 *
 *  Does the parsing of the user command and call the
 *  action to be undertaken.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************/
void ykush3_cmd_parser(int argc, char** argv)
{
	char bySerialFlag = 0;
	enum ykushAction action = HELP;
	Ykush3 *ykush = new Ykush3();
	char port;
	char value;
	char status_response = 0;
	Help *help = new Help("../doc/ykush3_help.txt");
	bool action_taken = false;

	CommandLine *cmd_handler = new CommandLine();
	
	if ( cmd_handler->parse(argc, argv) ) {
		std::cout << "Error parsing command." << std::endl;
		ykush->print_help("ykush3");
		return;
	}

	YkushCommand  user_command = cmd_handler->get_command();

	//verify if board is YKUSH3
	std::string board_str ("ykush3");
	if ( board_str.compare(user_command.board) != 0 ) {
		//board is not YKUSH3
		ykush->print_help("ykush3");
		return;
	}

	//iterate through command options
	std::string option_str;
	for (int i = 0; i < user_command.n_options; i++ ) {
		option_str = user_command.option[i].name;
		
		//Option: --i2c-usb-serial
		if ( option_str.compare("--i2c-usb-serial") ) {	
			if ( user_command.option[i].n_parameters == 1 ) {
				ykush->set_usb_serial(user_command.option[i].parameter[0]);
			}
		}

		//Option: --i2c-enable-control
		if ( option_str.compare("--i2c-enable-control") == 0 ) {	
			ykush->i2c_enable_disable_control(true);
			action_taken = true;
		} else if ( option_str.compare("--i2c-disable-control") == 0 ) {
			ykush->i2c_enable_disable_control(false);
			action_taken = true;
		}

		//Option: --i2c-enable-gateway
		if ( option_str.compare("--i2c-enable-gateway") == 0 ) {	
			ykush->i2c_enable_disable_control(true);
			action_taken = true;
		} else if ( option_str.compare("--i2c-disable-gateway") == 0 ) {
			ykush->i2c_enable_disable_control(false);
			action_taken = true;
		}

		//Option: --i2c-set-address <address>
		if ( option_str.compare("--i2c-set-address") == 0 ) {
			//get address parameter
			if ( user_command.option[i].n_parameters == 1 ) {
				if ( ykush->i2c_set_address(user_command.option[i].parameter[0]) ) {
					std::cout << "Error on sending I2C address" << std::endl; 
				}
			} else {
				ykush->print_help("ykush3");
			}
			action_taken = true;
		}

		//Option: --i2c-write <dev_address> <num_bytes> <byte1>...
		if ( option_str.compare("--i2c-write") == 0 ) {
			//get data bytes
			if ( user_command.option[i].n_parameters > 3 ) {
				if ( ykush->i2c_write(user_command.option[i].parameter[0], user_command.option[i].parameter[1], &user_command.option[i].parameter[2]) ) {
					std::cout << "Error on I2C writing" << std::endl;
					//...
					//ToDo: Have the handling of all error codes reported by the board firmware.
					//... 
				}
			} else {
				std::cout << "I2C write command requires at least one byte of data to be sent." << std::endl;
				ykush->print_help("ykush3");
			}
			action_taken = true;
		}

		//Option: --i2c-read <dev_address> <num_bytes>
		if ( option_str.compare("--i2c-read") == 0 ) {
			//get data bytes
			if ( user_command.option[i].n_parameters > 2 ) {
				unsigned char data_buffer[64];
				int bytes_read;
				if ( ykush->i2c_read(user_command.option[i].parameter[0], user_command.option[i].parameter[1], data_buffer, &bytes_read) ) {
					std::cout << "Error on I2C reading" << std::endl;
					//...
					//ToDo: Have the handling of all error codes reported by the board firmware.
					//... 
				}
			} else {
				std::cout << "I2C write command requires at least one byte of data to be sent." << std::endl;
				ykush->print_help("ykush3");
			}
			action_taken = true;
		}

		//Option: --version-bootloader
		if ( option_str.compare("--version-bootloader") == 0 ) {
			ykush->display_version_bootloader();
		}

		//Option: --version-firmware
		if ( option_str.compare("--version-firmware") == 0 ) {
			ykush->display_version_firmware();
		}

	}
	if ( action_taken )
		return;

	//...
	//----- LEGACY PARSING SCHEME... ToDo: Update to use of class CommandLine...
	//...
	if ( ( argv[2][0] == '-' ) && ( argv[2][1] == 's' ) ) {	//BY SERIAL
		
		if ( argc < 5 ) {
			help->print();
			return;
		}
		bySerialFlag = 1;
		
		if ( argv[4][0] == '-' && argv[4][1] == 'u' ) {
			
			action = PORT_UP;
			port = argv[5][0];
			
		} else if ( argv[4][0] == '-' && argv[4][1] == 'd' ) {
			
			action = PORT_DOWN;
			port = argv[5][0];
			
		} else if ( argv[4][0] == '-' && argv[4][1] == 'l' ) {
			
			action = LIST_BOARDS;
			
		} else if ( argv[4][0] == '-' && argv[4][1] == 'g' ) {
			
			action = GET_STATUS;
			port = argv[5][0];
			
		} else if ( argv[4][0] == '-' && argv[4][1] == 'o' ) {
			
			if ( argv[4][2]=='n' )
				action = EXT_CTRL_ON;
			else if ( argv[4][2] == 'f' && argv[4][3] == 'f' )
				action = EXT_CTRL_OFF;
			
		} else if ( argv[4][0] == '-' && argv[4][1] == 'w' ) {
			
			if ( argc < 7 ) {
				help->print();
				return;
			}
			action = WRITE_IO;
			port = argv[5][0];
			value = argv[6][0]; 
			
		} else if ( argv[4][0] == '-' && argv[4][1] == 'r' ) { 
			
			action = READ_IO;
			port = argv[5][0];
			
		} else if ( argv[4][0] == '-' && argv[4][1] == 'c' ) { 
			action = CONFIG;
			port = argv[5][0];
			value = argv[6][0];
		} else if ( argv[4][0] == '-' && argv[4][1] == '-' && argv[4][2] == 'r' && argv[4][3] == 'e' ) { 
			action = RESET;
		} else if ( argv[4][0] == '-' && argv[4][1] == '-' && argv[4][2] == 'g' && argv[4][3] == 'p' && argv[4][4] == 'i' ) {
			if ( argv[5][0] == 'e' && argv[5][1] == 'n' )
				action = GPIO_EN;
                        else if (argv[5][0] == 'd' && argv[5][1] == 'i' && argv[5][2] == 's')
				action = GPIO_DIS;
			else
				help->print();
		} else if ( argv[4][0] == '-' && argv[4][1] == '-' && argv[4][2] == 'b' && argv[4][3] == 'o' && argv[4][4] == 'o' ) { 
			action = ENTER_BOOTLOADER;		
		} else {
			help->print();
			return;
		}
	} else if ( ( argv[2][0] == '-' ) && ( argv[2][1] == 'u' ) ) {
		if ( argc < 3 ) {
			help->print();
			return;
		}
		action = PORT_UP;
		port = argv[3][0];
	} else if ( ( argv[2][0] == '-')  && ( argv[2][1] == 'd' ) ) {
		if ( argc < 4 ) {
			help->print();
			return;
		}
		action = PORT_DOWN;
		port = argv[3][0];
	} else if ( ( argv[2][0] == '-' ) && ( argv[2][1] == 'l' ) ) {
		action = LIST_BOARDS;
	} else if ( ( argv[2][0] == '-' ) && ( argv[2][1] == 'g' ) ) {
		if ( argc < 4 ) {
			help->print();
			return;
		}
		action = GET_STATUS;
		port = argv[3][0];
	} else if ( ( argv[2][0] == '-' ) && ( argv[2][1] == 'o' ) ) {
		if ( argc < 3 ) {
			help->print();
			return;
		}
		if ( argv[2][2] == 'n' )
			action = EXT_CTRL_ON;
		else if ( argv[2][2] == 'f' && argv[2][3] == 'f' ) 
			action = EXT_CTRL_OFF;
	} else if ( ( argv[2][0] == '-' ) && ( argv [2][1] == 'w' ) ) {
		if ( argc < 5 ) {
			help->print();
			return;
		}
		action = WRITE_IO;
		port = argv[3][0];
		value = argv[4][0];
	} else if ( argv[2][0] == '-' && argv[2][1] == 'r' ) { 
		action = READ_IO;
		port = argv[3][0];
	} else if ( ( argv[2][0] == '-' ) && ( argv[2][1] == 'c' ) ) {
		if ( argc < 5 ) {
			help->print();
			return;
		}
		action = CONFIG;
		port = argv[3][0];
		value = argv[4][0];
	} else if ( ( argv[2][0] == '-' ) && ( argv[2][1] == '-' ) && ( argv[2][2] == 'r' ) && ( argv[2][3] == 'e' ) ) {
		action = RESET;
	} else if ( argv[2][0] == '-' && argv[2][1] == '-' && argv[2][2] == 'b' && argv[2][3] == 'o' && argv[2][4] == 'o' ) { 
			action = ENTER_BOOTLOADER;
	} else if ( argv[2][0] == '-' && argv[2][1] == '-' && argv[2][2] == 'g' && argv[2][3] == 'p' && argv[2][4] == 'i' ) {
			if ( argv[3][0] == 'e' && argv[3][1] == 'n' )
				action = GPIO_EN;
                        else if (argv[3][0] == 'd' && argv[3][1] == 'i' && argv[3][2] == 's')
				action = GPIO_DIS;
			else
				help->print();
		
	} else {
		help->print();
		return;
	}


	switch ( action ) {
	case PORT_UP:
		if ( bySerialFlag ) 
			ykush->port_up(argv[3], port); 
		else
			ykush->port_up(NULL, port); 
		break;
	case PORT_DOWN:
		if ( bySerialFlag )
			ykush->port_down(argv[3], port); 
		else
			ykush->port_down(NULL, port); 
		break;
	case LIST_BOARDS:
		ykush3_list_attached(); 
		break;
	case GET_STATUS:
		if ( bySerialFlag ) {
			status_response = ykush->get_port_status(argv[3], port);
			if ( status_response >> 4 )
				 printf("\n\nDownstream port %d is ON\n\n", status_response & 0x0F );
			else 
				printf("\n\nDownstream port %d is OFF\n\n", status_response & 0x0F);
		} else {
			status_response = ykush->get_port_status(NULL, port);
			if ( status_response >> 4 )
			     printf("\n\nDownstream port %d is ON\n\n", status_response & 0x0F);
			else 
			     printf("\n\nDownstream port %d is OFF\n\n", status_response & 0x0F);
		}   
		break;
	case EXT_CTRL_ON:
		if ( bySerialFlag )
			ykush->port_up(argv[3], '4'); 
		else
			ykush->port_up(NULL, '4'); 
		break;
	case EXT_CTRL_OFF:
		if ( bySerialFlag )
			ykush->port_down(argv[3], '4'); 
		else
			ykush->port_down(NULL, '4'); 
		break;
	case WRITE_IO:
		if ( bySerialFlag )
			ykush->write_io(argv[3], port, value); 
		else
			ykush->write_io(NULL, port, value); 
		break;
	case READ_IO:
		if ( bySerialFlag )
			printf("\n%d\n", ykush->read_io(argv[3], port)); 
		else
			printf("\n%d\n", ykush->read_io(NULL, port)); 
		break;
	case CONFIG:
		if ( bySerialFlag )
		    ykush->config_port(argv[3], port, value); 
		else
		    ykush->config_port(NULL, port, value); 
		break;

	case RESET:
		if ( bySerialFlag )
			ykush->reset(argv[3]); 
		else
			ykush->reset(NULL); 
		break;
	case GPIO_EN:
		if ( bySerialFlag )
			ykush->gpio_ctrl_enable(argv[3]); 
		else
			ykush->gpio_ctrl_enable(NULL); 
		break;
	case GPIO_DIS:
		if ( bySerialFlag )
			ykush->gpio_ctrl_disable(argv[3]); 
		else
			ykush->gpio_ctrl_disable(NULL); 
		break;
	case ENTER_BOOTLOADER:
		if ( bySerialFlag )
			ykush->enter_bootloader(argv[3]); 
		else
			ykush->enter_bootloader(NULL);
		break;
	default:
		help->print();
		break;
	}

}





/********************************************************
 ********************************************************
 *
 *  Ykush Class Implementation
 *
 ********************************************************
 ********************************************************/




/*********************************************************
 * Method: port_up
 *
 * Description:
 *
 * Commands the YKUSH XS board to turn ON the downstream
 * port.
 *
 *
 *
 *
 *********************************************************/
int Ykush3::port_up(char *serial, char port)
{  
    //Create command msg
    hid_report_out[0] = 0;


    switch(port)
    {
        case '1':
            hid_report_out[1] = 0x11;
            break;

        case '2':     
            hid_report_out[1] = 0x12;
            break;
        
        case '3':     
            hid_report_out[1] = 0x13;
            break;

        case 'a':     
            hid_report_out[1] = 0x1a;
            break;

        case '4':     
            hid_report_out[1] = 0x14;
            break;

        default:
            return 0;
            break;

    }
    
    
    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 65);

}


/*********************************************************
 * Method: port_down
 *
 * Description:
 *
 * Commands the YKUSH XS board to turn OFF the downstream
 * port.
 *
 *
 *
 *
 *********************************************************/
int Ykush3::port_down(char *serial, char port)
{

    
    //Create command msg
    hid_report_out[0] = 0;      //Windows stuff

    switch(port)
    {
        case '1':
            hid_report_out[1] = 0x01;
            break;

        case '2':     
            hid_report_out[1] = 0x02;
            break;
        
        case '3':     
            hid_report_out[1] = 0x03;
            break;

        case 'a':     
            hid_report_out[1] = 0x0a;
            break;
        
        case '4':
            hid_report_out[1] = 0x04;
            break;

        default:
            return 0;
            break;

    }
  
    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 65);
   

}




/*********************************************************
 * Method: get_port_status
 *
 * Description:
 *
 * Commands the YKUSH XS to respond with the port status.
 *
 *  1 -> Port is ON/UP
 *  0 -> PORT is OFF/DOWN
 *
 *
 *
 *
 *********************************************************/
int Ykush3::get_port_status(char *serial, char port)
{
    int status;

    //Create command msg
    hid_report_out[0] = 0;      

    switch(port)
    {
        case '1':
            hid_report_out[1] = 0x21;
            break;

        case '2':     
            hid_report_out[1] = 0x22;
            break;
        
        case '3':     
            hid_report_out[1] = 0x23;
            break;

        case '4':     
            hid_report_out[1] = 0x24;
            break;

        default:
            return 0;
            break;

    }
  
    //send HID report to board 
    sendHidReport(serial, hid_report_out, hid_report_in, 65);

    //handle board response HID report
    status = hid_report_in[1];

    return status;
}






/****************************************************
 *
 *
 *
 *
 ****************************************************/ 
int Ykush3::write_io(char *serial, char port, char value)
{
    //Create command msg
    hid_report_out[0] = 0;

    
    hid_report_out[1] = 0x31;
    if(value=='0'){
        hid_report_out[3] = 0; 
    } else {
        hid_report_out[3] = 1; 
    }


    switch(port)
    {
        case '1':
            hid_report_out[2] = 0x01;
            break;

        case '2':     
            hid_report_out[2] = 0x02;
            break;
        
        case '3':     
            hid_report_out[2] = 0x03;
            break;
 
        default:
            return 0;
            break;

    }
    
    
    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 65);

}








/****************************************************
 *
 *
 *
 *
 ****************************************************/ 
int Ykush3::read_io(char *serial, char port)
{
    //Create command msg
    hid_report_out[0] = 0;

    
    hid_report_out[1] = 0x30;

    switch(port)
    {
        case '1':
            hid_report_out[2] = 0x01;
            break;

        case '2':     
            hid_report_out[2] = 0x02;
            break;
        
        case '3':     
            hid_report_out[2] = 0x03;
            break;
 
        default:
            return 0;
            break;

    }
    
    
    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 65);
    
    //process response
    return hid_report_in[4];

}




/****************************************************
 *
 *
 *
 *
 ****************************************************/ 
int Ykush3::config_port(char *serial, char port, char value)
{
	//Create command msg
	hid_report_out[0] = 0;


	hid_report_out[1] = 0x41;
	if ( value == '0' )
		hid_report_out[3] = 0; 
	else if ( value == '1' )
		hid_report_out[3] = 1; 
	else
		hid_report_out[3] = 2;	//Persist mode
	


	switch ( port ) {
	case '1':
		hid_report_out[2] = 0x01;
		break;
	case '2':     
		hid_report_out[2] = 0x02;
		break;
	case '3':     
		hid_report_out[2] = 0x03;
		break;
	case 'e':     
		hid_report_out[2] = 0x04;
		break;
	default:
		return 0;
		break;
	}

	//send HID report to board
	return sendHidReport(serial, hid_report_out, hid_report_in, 65);

}




/****************************************************
 *
 *
 *
 *
 ****************************************************/ 
void Ykush3::reset(char *serial)
{
    //Create command msg
    hid_report_out[0] = 0;

    hid_report_out[1] = 0x55;        
    
    //send HID report to board
    sendHidReport(serial, hid_report_out, hid_report_in, 65);

}




/**
 * 
 */
void Ykush3::gpio_ctrl_enable(char *serial)
{
    //Create command msg
    hid_report_out[0] = 0;
    hid_report_out[1] = 0x32;
    hid_report_out[2] = 0x01;
    
    //send HID report to board
    sendHidReport(serial, hid_report_out, hid_report_in, 65);

}


/**
 * 
 */
void Ykush3::gpio_ctrl_disable(char *serial)
{
	//Create command msg
	hid_report_out[0] = 0;
	hid_report_out[1] = 0x32;
	hid_report_out[2] = 0x00;
	
	//send HID report to board
	sendHidReport(serial, hid_report_out, hid_report_in, 65);

}



/**
 * 
 */
void Ykush3::enter_bootloader(char *serial)
{
	//Create command msg
	hid_report_out[0] = 0;
	hid_report_out[1] = 0x42;
	
	//send HID report to board
	sendHidReport(serial, hid_report_out, hid_report_in, 65);

}


/**************************************************************
 * 
 * 			I2C Methods
 * 
 **************************************************************/

int Ykush3::i2c_enable_disable_control(bool enable_flag) 
{
	//std::cout << "i2c_enable_disable_control com flag: " << enable_flag << std::endl;

	//Create command msg
	hid_report_out[0] = 0;
	hid_report_out[1] = 0x51;
	hid_report_out[2] = 0x01;
	
	if ( enable_flag )
		hid_report_out[3] = 0x01;
	else 
		hid_report_out[3] = 0x00;

	sendHidReport(usb_serial, hid_report_out, hid_report_in, 65);

	if ( (hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x51) ) {
		//command executed with success
		return 0;
	} 

	return 1;
}


int Ykush3::i2c_enable_disable_gateway(bool enable_flag)
{
	//std::cout << "i2c_enable_disable_gateway com flag: " << enable_flag << std::endl;

	//Create command msg
	hid_report_out[0] = 0;
	hid_report_out[1] = 0x51;
	hid_report_out[2] = 0x02;

	if ( enable_flag )
		hid_report_out[3] = 0x01;
	else 
		hid_report_out[3] = 0x00;

	sendHidReport(usb_serial, hid_report_out, hid_report_in, 65);

	if ( (hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x51) ) {
		//command executed with success
		return 0;
	} 

	return 1;
}


int Ykush3::i2c_set_address(char *i2c_address)
{
	std::cout << "i2c_set_address com address: " << i2c_address << std::endl;
	//Create command msg
	hid_report_out[0] = 0;
	hid_report_out[1] = 0x51;
	hid_report_out[2] = 0x03;

	hex2bin(i2c_address + 2, &hid_report_out[3], 2);
	sendHidReport(usb_serial, hid_report_out, hid_report_in, 65);

	if ( (hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x51) ) {
		//command executed with success
		return 0;
	}

	return 1;
}


int Ykush3::i2c_write(char *i2c_address_ASCII, char *num_bytes_ASCII, char **data_to_write_ASCII)
{
	hid_report_out[0] = 0;
	hid_report_out[1] = 0x52;
	hid_report_out[2] = 0x01;

	//convert i2c_address_ASCII to binary
	hex2bin(i2c_address_ASCII + 2, &hid_report_out[3], 2);

	//convert num_bytes_ASCII to binary
	int size = strlen(num_bytes_ASCII);
	if ( size <= 0 )
		return 1;
	dec2bin(num_bytes_ASCII, &hid_report_out[4], size);
	if ( hid_report_out[4] > 60 ) 
		return 2;

	//convert data_to_write_ASCII to binary
	for ( int i = 0; i < hid_report_out[4]; i++ ) {
		hex2bin(num_bytes_ASCII + 2, &hid_report_out[i + 5], 2);
	}

	sendHidReport(usb_serial, hid_report_out, hid_report_in, 65);

	if ( (hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x52) ) {
		//command executed with success
		return 0;
	}
	return 0;
}


int Ykush3::i2c_read(char *i2c_address_ASCII, char *num_bytes_ASCII, unsigned char *data_buffer, int *bytes_read)
{
	hid_report_out[0] = 0;
	hid_report_out[1] = 0x52;
	hid_report_out[2] = 0x02;

	//convert i2c_address_ASCII to binary
	hex2bin(i2c_address_ASCII + 2, &hid_report_out[3], 2);

	//convert num_bytes_ASCII to binary
	int size = strlen(num_bytes_ASCII);
	if ( size <= 0 )
		return 1;
	dec2bin(num_bytes_ASCII, &hid_report_out[4], size);

	sendHidReport(usb_serial, hid_report_out, hid_report_in, 65);

	//handle response message
	if ((hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x52)) {
		//get num_bytes
		if (hid_report_in[2] < 0)
			return 1;
		int i;
		for (i = 0; (i < hid_report_in[2]) && (i < 60); i++) {
			data_buffer[i] = hid_report_in[i + 4];
		}
		*bytes_read = i; 
	} else {
		return 1;
	}

	return 0;
}


int Ykush3::display_version_bootloader(void)
{
	hid_report_out[0] = 0;
	hid_report_out[1] = 0x61;
	hid_report_out[2] = 0x01;
	if ( sendHidReport(usb_serial, hid_report_out, hid_report_in, 65) != 0 ) {
		std::cout << "Unable to get bootloader version \n";
		return 1; 
	}
	//print response
	if ( ( hid_report_in[0] != 0x01 ) && ( hid_report_in[0] != 0x61 ) ) {
		std::cout << "Bootloader version-1.0.0\n";
		return 0; 
	}
	std::cout << "Bootloader version-" << hid_report_in[2] << "." << hid_report_in[3] << "." << hid_report_in[4];
	return 0;
}
		

int Ykush3::display_version_firmware(void)
{
	hid_report_out[0] = 0;
	hid_report_out[1] = 0x61;
	hid_report_out[2] = 0x02;
	sendHidReport(usb_serial, hid_report_out, hid_report_in, 65);
	//print response
	if ( ( hid_report_in[0] != 0x01 ) && ( hid_report_in[0] != 0x61 ) ) {
		std::cout << "Firmware version-1.0.0\n";
		std::cout << "There is a new version of the firmware available for download in yepkit.com.\n";
		std::cout << "Please consider updating the boot loader for unlocking the newest board features.\n"
		return 0; 
	}
	std::cout << "Bootloader version-" << hid_report_in[2] << "." << hid_report_in[3] << "." << hid_report_in[4];
	return 1;
}





int Ykush3::set_usb_serial(char *serial) {
	usb_serial = serial;
	return 0;
}


void Ykush3::print_help(const char *help_section) 
{
	std::cout << "ToDo: Help print YKUSH3" << std::endl;
}





/****************************************************
 *
 *
 *
 *
 ****************************************************/ 
void ykush3_list_attached()
{
    Ykush3 *ykush = new Ykush3();

    printf("\n\nAttached YKUSH Boards:\n");        
    if(ykush->listConnected()==0)
    {
        printf("\n\nNo YKUSH boards found.");
    } 
    
    printf("\n\n");

}









