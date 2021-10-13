// SPDX-License-Identifier: Apache-2.0
/*
 *
 */
#include <ykush3.h>
#include <stdio.h>
#include <ykush_help.h>
#include <command_parser.h>
#include <iostream>
#include <string>
#include <string2val.h>
#include <cstring>

/*
 * Defines the actions to be taken based on the command line
 * parameters and options.
 */
int ykush3_action_parser(struct command_line *cmdl)
{
	Ykush3 ykush3;
	struct command_option *cur_opt;
	struct command_parameter *cur_param;
	char *serial;
	bool routed = false;

	std::string opt_name;
	std::string param_name;
	cur_opt = cmdl->options;
	while (cur_opt) {
		opt_name = cur_opt->name;

		if (opt_name.compare("-s") == 0) {
			ykush3.set_usb_serial(cur_opt->parameters->value);
		} else if (opt_name.compare("-u") == 0) {
			return ykush3.port_up(cur_opt->parameters->value[0]);
		} else if (opt_name.compare("-d") == 0) {
			return ykush3.port_down(cur_opt->parameters->value[0]);
		} else if (opt_name.compare("-g") == 0) {
                        return ykush3.get_port_status(cur_opt->parameters->value[0]);
		} else if (opt_name.compare("-on") == 0) {
          		return ykush3.port_up('4'); 
		} else if (opt_name.compare("-off") == 0) {
          		return ykush3.port_down('4'); 
		} else if (opt_name.compare("-l") == 0) {
          		return ykush3_list_attached(); 
		} else if (opt_name.compare("-c") == 0) {
              		return ykush3.config_port (cur_opt->parameters->value[0],
					cur_opt->parameters->next->value[0]);
		} else if (opt_name.compare("--reset") == 0) {
          		return ykush3.reset(); 
		} else if (opt_name.compare("-r") == 0) {
          		return ykush3.read_io(cur_opt->parameters->value[0]); 
		} else if (opt_name.compare("-w") == 0) {
          		return ykush3.write_io(cur_opt->parameters->value[0],
					cur_opt->parameters->next->value[0]); 
		} else if (opt_name.compare("--gpio") == 0) {
          		param_name = cur_opt->parameters->value;
			if (param_name.compare("enable") == 0)
				return ykush3.gpio_ctrl_enable();
			else if (param_name.compare("disable") == 0)
				return ykush3.gpio_ctrl_disable();
		} else if (opt_name.compare("--boot") == 0) {
			return ykush3.enter_bootloader();
		} else if (opt_name.compare("--bootloader-version") == 0) {
			return ykush3.display_version_bootloader();
		} else if (opt_name.compare("--firmware-version") == 0) {
			return ykush3.display_version_firmware();
		} else if (opt_name.compare("--i2c-slave") == 0) {
			param_name = cur_opt->parameters->value;
			if (param_name.compare("enable") == 0)
				return ykush3.i2c_enable_disable_control(true);
			else if (param_name.compare("disable") == 0)
				return ykush3.i2c_enable_disable_control(false);	
		} else if (opt_name.compare("--i2c-master") == 0) {
			param_name = cur_opt->parameters->value;
			if (param_name.compare("enable") == 0)
				return ykush3.i2c_enable_disable_gateway(true);
			else if (param_name.compare("disable") == 0)
				return ykush3.i2c_enable_disable_gateway(false);	
		} else if (opt_name.compare("--i2c-set-address") == 0) {
			if (cur_opt->parameters)
				return ykush3.i2c_set_address(
						cur_opt->parameters->value);
		} else if (opt_name.compare("--i2c-write") == 0) {
			return ykush3.i2c_write_buffer(cur_opt);
		} else if (opt_name.compare("--i2c-read") == 0) {
			return ykush3.i2c_read_buffer(cur_opt);
		}

		cur_opt = cur_opt->next;
	}
	if (cmdl)
		ykush3.print_help(cmdl->app_name);
	return 1;
}


int Ykush3::port_up(char port)
{  
	switch (port) {
	case '1':
		hid_report_out[0] = 0x11;
		break;
	case '2':     
		hid_report_out[0] = 0x12;
		break;
	case '3':     
		hid_report_out[0] = 0x13;
		break;
	case 'a':     
		hid_report_out[0] = 0x1a;
		break;
	case '4':     
		hid_report_out[0] = 0x14;
		break;
	default:
		return -1;
		break;
	}
    
    	return sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);
}

int Ykush3::port_down(char port)
{
    	switch (port) {
        case '1':
            	hid_report_out[0] = 0x01;
            	break;
        case '2':     
            	hid_report_out[0] = 0x02;
            	break;
        case '3':     
            	hid_report_out[0] = 0x03;
            	break;
        case 'a':     
            	hid_report_out[0] = 0x0a;
            	break;
        case '4':
            	hid_report_out[0] = 0x04;
            	break;
        default:
            	return 0;
            	break;
    	}
  
    	return sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);
}



int Ykush3::get_port_status(char port)
{
    	int status;      

	switch (port) {
        case '1':
            	hid_report_out[0] = 0x21;
            	break;
        case '2':     
            	hid_report_out[0] = 0x22;
            	break;
        case '3':     
            	hid_report_out[0] = 0x23;
            	break;
        case '4':     
            	hid_report_out[0] = 0x24;
            	break;
        default:
            	return -1;
		break;
    	}
  
    	sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);
    	
	status = hid_report_in[1];
        switch (status) {
        case 0x01:
                std::cout << "Port 1 OFF\n";
                break;
        case 0x02:
                std::cout << "Port 2 OFF\n";
                break;
        case 0x03:
                std::cout << "Port 3 OFF\n";
                break;
        case 0x11:
                std::cout << "Port 1 ON\n";
                break;
        case 0x12:
                std::cout << "Port 2 ON\n";
                break;
        case 0x13:
                std::cout << "Port 3 ON\n";
                break;
        default:
                return -1;
        }

    	return 0;
}



int Ykush3::write_io(char port, char value)
{

    	hid_report_out[0] = 0x31;
    	if (value == '0')
        	hid_report_out[2] = 0; 
	else
        	hid_report_out[2] = 1; 

    	switch (port) {
        case '1':
            	hid_report_out[1] = 0x01;
            	break;
        case '2':     
           	hid_report_out[1] = 0x02;
            	break;
        case '3':     
            	hid_report_out[1] = 0x03;
            	break;
        default:
            	return 1;
		break;
    	}
    
    	return sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);
}



int Ykush3::read_io(char port)
{
    	hid_report_out[0] = 0x30;

    	switch (port) {
        case '1':
            	hid_report_out[1] = 0x01;
            	break;
        case '2':     
            	hid_report_out[1] = 0x02;
            	break;
        case '3':     
            	hid_report_out[1] = 0x03;
            	break;
        default:
		return 1;
            	break;
    	}
    
    	sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);
	return hid_report_in[3];
}

 
int Ykush3::config_port(char port, char value)
{
	hid_report_out[0] = 0x41;
	if (value == '0')
		hid_report_out[2] = 0; 
	else if (value == '1')
		hid_report_out[2] = 1; 
	else
		hid_report_out[2] = 2;	//Persist mode
	


	switch (port) {
	case '1':
		hid_report_out[1] = 0x01;
		break;
	case '2':     
		hid_report_out[1] = 0x02;
		break;
	case '3':     
		hid_report_out[1] = 0x03;
		break;
	case 'e':     
		hid_report_out[1] = 0x04;
		break;
	default:
		return 1;
		break;
	}

	return sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);
}


int Ykush3::reset(void)
{
	hid_report_out[0] = 0x55;        
	
	sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);
	return 0;
}



int Ykush3::gpio_ctrl_enable(void)
{
	hid_report_out[0] = 0x32;
	hid_report_out[1] = 0x01;
    
	sendHidReport (usb_serial, hid_report_out, hid_report_in, 64);
	return 0;
}



int Ykush3::gpio_ctrl_disable(void)
{
	hid_report_out[0] = 0x32;
	hid_report_out[1] = 0x00;
    
	sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);
	return 0;
}



int 
Ykush3::enter_bootloader (void)
  {
    hid_report_out[0] = 0x42;
    
    //send HID report to board
    sendHidReport (usb_serial, hid_report_out, hid_report_in, 64);
    return 0;
  }

int Ykush3::i2c_enable_disable_control(bool enable_flag) 
{
	hid_report_out[0] = 0x51;
	hid_report_out[1] = 0x01;
	
	if ( enable_flag )
		hid_report_out[2] = 0x01;
	else 
		hid_report_out[2] = 0x00;

	sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);

	if ((hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x51)) {
		//command executed with success
		return 0;
	} 

	return 1;
}

int Ykush3::i2c_enable_disable_gateway(bool enable_flag)
{
	hid_report_out[0] = 0x51;
	hid_report_out[1] = 0x02;

	if ( enable_flag )
		hid_report_out[2] = 0x01;
	else 
		hid_report_out[2] = 0x00;

	sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);

	if ( (hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x51) ) {
		//command executed with success
		return 0;
	} 

	return 1;
}

int Ykush3::i2c_set_address(char *i2c_address)
{
	std::cout << "i2c_set_address com address: " << i2c_address << std::endl;

	hid_report_out[0] = 0x51;
	hid_report_out[1] = 0x03;

	hex2bin(i2c_address + 2, &hid_report_out[2], 2);
	sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);

	if ((hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x51)) {
		//command executed with success
		return 0;
	}

	return 1;
}


int Ykush3::i2c_write(char *i2c_address_ASCII,
		int num_bytes,
		char **data_to_write_ASCII)
{
	hid_report_out[0] = 0x52;
	hid_report_out[1] = 0x01;

	//convert i2c_address_ASCII to binary
	hex2bin(i2c_address_ASCII + 2, &hid_report_out[2], 2);

	//convert num_bytes_ASCII to binary
        /*
	int size = strlen(num_bytes_ASCII);
	if (size <= 0)
		return 1;
        dec2bin(num_bytes_ASCII, &hid_report_out[3], size);
        */
	if (num_bytes > 60) 
		hid_report_out[3] = 60;
        else
                hid_report_out[3] = num_bytes;

	//convert data_to_write_ASCII to binary
	for (int i = 0; i < hid_report_out[3]; i++) {
		hex2bin(data_to_write_ASCII[i]+2, &hid_report_out[i + 4], 2);
	}

        for (int i=0; i<64; i++)
                std::cout << std::hex << (int)hid_report_out[i] << " ";

	sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);

	if ((hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x52)) {
		//command executed with success
		return 0;
	}
	return 0;
}


int Ykush3::i2c_read(char *i2c_address_ASCII,
		char *num_bytes_ASCII,
		unsigned char *data_buffer,
		int *bytes_read)
{
	hid_report_out[0] = 0x52;
	hid_report_out[1] = 0x02;

	//convert i2c_address_ASCII to binary
	hex2bin(i2c_address_ASCII + 2, &hid_report_out[2], 2);

	//convert num_bytes_ASCII to binary
	int size = strlen(num_bytes_ASCII);
	if ( size <= 0 )
		return 1;
	dec2bin(num_bytes_ASCII, &hid_report_out[3], size);

	sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);

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


int 
Ykush3::display_version_bootloader (void)
  {
    int major, minor, patch;
    hid_report_out[0] = 0x61;
    hid_report_out[1] = 0x01;
    if (sendHidReport(usb_serial, hid_report_out, hid_report_in, 64) != 0)
      {
        std::cout << "Unable to get bootloader version \n";
        return 1; 
      }
    //print response
    if ((hid_report_in[0] != 0x01) && (hid_report_in[0] != 0x61)) 
      {
        std::cout << "Bootloader version-0.10.0\n";
        return 0; 
      }

    std::cout << "Bootloader version-"
	    << (int) hid_report_in[2]
	    << "."
	    << (int) hid_report_in[3]
	    << "."
	    << (int) hid_report_in[4]
	    << std::endl;
    return 0;
  }
		

int
Ykush3::display_version_firmware (void)
  {
    hid_report_out[0] = 0x61;
    hid_report_out[1] = 0x02;
    sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);
    //print response
    if (( hid_report_in[0] != 0x01) && (hid_report_in[0] != 0x61))
      {
        std::cout << "Firmware version-1.0.0\n";
        return 0; 
      }
    std::cout << "Firmware version-" << (int) hid_report_in[2] << "." << (int) hid_report_in[3] << "." << (int) hid_report_in[4] << std::endl;
    return 0;
  }



int Ykush3::set_usb_serial(char *serial) 
{
	usb_serial = serial;
	return 0;
}


 
int ykush3_list_attached()
{
	Ykush3 ykush3;
	printf("\n\nAttached YKUSH Boards:\n");        
	if( ykush3.listConnected() == 0 )
		printf("\n\nNo YKUSH boards found.");
	printf("\n\n");
	return 0;
}


void Ykush3::print_help(char *app_name) 
{
	Help help(app_name);

	help.print_version();
	help.print_usage();
	help.print_ykush3();
}



int Ykush3::i2c_write_buffer(struct command_option *cur_opt)
{
	if (cur_opt) {
		command_parameter *param = cur_opt->parameters->next;
                param = param->next;
		char *buffer[60];
		char num_bytes = 0;
		while (param) {
                        buffer[num_bytes] = param->value;
			num_bytes++;
			param = param->next;
		}
		return i2c_write(cur_opt->parameters->value, num_bytes, buffer);
	}
	return 1;
}

int Ykush3::i2c_read_buffer(struct command_option *cur_opt)
{
	if (cur_opt) {
                char *address = cur_opt->parameters->value;
		command_parameter *param = cur_opt->parameters->next;
                param = param->next;
                char *num_bytes = param->value;
		unsigned char buffer[60];
	        int bytes_read = 0;
                i2c_read(address, num_bytes, buffer, &bytes_read);
                for (int i=0; i<bytes_read; i++)
                        std::cout << buffer[i] << std::endl;
	}
	return 1;
}

