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

#include <ykush3.h>
#include <stdio.h>
#include <ykush_help.h>
#include <command_parser.h>
#include <iostream>
#include <string>
#include <string2val.h>
#include <cstring>


extern char *app_exc_name;


int 
ykush3_cmd_parser (int argc, char** argv)
  {
    char bySerialFlag = 0;
    enum ykush3Action action = YKUSH3_HELP;
    Ykush3 ykush3;
    char port;
    char value;
    char status_response = 0;
    bool action_taken = false;
    
    if (argc < 3)
      {
        ykush3.print_help ();
        return -1;
      }

    std::string str = argv[2];

    if (str.compare ("-s") == 0) 
      {	
        //BY SERIAL
        if (argc < 6) 
          {
            ykush3.print_help ();
            return -1;
          }
        // set serialç in object
        ykush3.set_usb_serial(argv[3]);

        str = argv[4];
        bySerialFlag = 1;
        if (str.compare ("-u") == 0 ) 
          {
            action = YKUSH3_PORT_UP;
            port = argv[5][0];
          }
        else if (str.compare ("-d") == 0 ) 
          {  
            action = YKUSH3_PORT_DOWN;
            port = argv[5][0];
          }
        else if (str.compare ("-g") == 0 )
          {
            action = YKUSH3_GET_STATUS;
            port = argv[5][0];
          }
        else if (str.compare ("-on") == 0 )
          {
            action = YKUSH3_EXT_CTRL_ON;
          }
        else if (str.compare ("-off") == 0 )
          {
            action = YKUSH3_EXT_CTRL_OFF;
          } 
        else if (str.compare ("-w") == 0 )
          {
            if (argc < 7) 
              {
                ykush3.print_help ();
                return -1;
              }
            action = YKUSH3_WRITE_IO;
            port = argv[5][0];
            value = argv[6][0]; 
          } 
        else if (str.compare ("-r") == 0) 
          { 
            action = YKUSH3_READ_IO;
            port = argv[5][0];
          } 
        else if (str.compare ("-c") == 0)
          { 
            action = YKUSH3_CONFIG;
            port = argv[5][0];
            value = argv[6][0];
          } 
        else if (str.compare("--reset") == 0) 
          { 
            action = YKUSH3_RESET;
          }
        else if (str.compare ("--gpio") == 0) 
          {
            std::string str2 = argv[5];
            if (str2.compare ("enable") == 0) 
              {
                action = YKUSH3_GPIO_EN;
              }
            else if (str2.compare ("disable") == 0 ) 
              {
                action = YKUSH3_GPIO_DIS;
              } 
            else 
              {
                ykush3.print_help ();
                return -1;
              }
          } 
        else if (str.compare ("--boot") == 0) 
          { 
            action = YKUSH3_ENTER_BOOTLOADER;		
          }
        else if (str.compare ("--firmware-version") == 0)
          {
            action = YKUSH3_FIRMWARE_VERSION;
          }
        else if (str.compare ("--bootloader-version") == 0)
          {
            action = YKUSH3_BOOTLOADER_VERSION;
          }
        else
          {
            ykush3.print_help ();
            return -1;
          }
      }
    // Without serial prefix
    else if (str.compare ("-u") == 0) 
      {
        if (argc < 3)
          {
            ykush3.print_help ();
            return -1;
          }
        action = YKUSH3_PORT_UP;
        port = argv[3][0];
      }
    else if (str.compare ("-d") == 0)
      {
        if (argc < 4) 
          {
            ykush3.print_help ();
            return -1;
          }
        action = YKUSH3_PORT_DOWN;
        port = argv[3][0];
      }
    else if (str.compare ("-l") == 0)
      {
        action = YKUSH3_LIST_BOARDS;
      } 
    else if (str.compare ("-g") == 0)
      {
        if (argc < 4)
          {
            ykush3.print_help ();
            return -1;
          }
        action = YKUSH3_GET_STATUS;
        port = argv[3][0];
      }
    else if (str.compare ("-on") == 0)
      {
        action = YKUSH3_EXT_CTRL_ON;
      }
    else if (str.compare ("-off") == 0)
      {
        action = YKUSH3_EXT_CTRL_OFF;
      } 
    else if (str.compare ("-w") == 0)
      {
        if (argc < 5)
          {
            ykush3.print_help ();
            return -1;
          }
        action = YKUSH3_WRITE_IO;
        port = argv[3][0];
        value = argv[4][0];
      }
    else if (str.compare ("-r") == 0)
      { 
        action = YKUSH3_READ_IO;
        port = argv[3][0];
      } 
    else if (str.compare ("-c") == 0)
      {
        if (argc < 5)
          {
            ykush3.print_help ();
            return -1;
          }
        action = YKUSH3_CONFIG;
        port = argv[3][0];
        value = argv[4][0];
      }
    else if (str.compare ("--reset") == 0)
      {
        action = YKUSH3_RESET;
      }
    else if (str.compare ("--boot") == 0)
      { 
          action = YKUSH3_ENTER_BOOTLOADER;
      }
    else if (str.compare ("--gpio") == 0)
      {
          std::string str2 = argv[3];
          if (str2.compare ("enable") == 0)
            {
              action = YKUSH3_GPIO_EN;
            }
          else if (str2.compare ("disable") == 0)
            {
              action = YKUSH3_GPIO_DIS;
            }
          else
            {
              ykush3.print_help ();
              return -1;
            }
      }
    else if (str.compare ("--firmware-version") == 0)
      {
        action = YKUSH3_FIRMWARE_VERSION;
      }
    else if (str.compare ("--bootloader-version") == 0)
      {
        action = YKUSH3_BOOTLOADER_VERSION;
      }
    else
      {
        ykush3.print_help ();
        return -1;
      }

    switch (action)
      {
      case YKUSH3_PORT_UP:
        if (bySerialFlag) 
          return ykush3.port_up (argv[3], port); 
        else
          return ykush3.port_up (NULL, port); 
        break;

      case YKUSH3_PORT_DOWN:
        if (bySerialFlag)
          return ykush3.port_down (argv[3], port); 
        else
          return ykush3.port_down (NULL, port); 
        break;

      case YKUSH3_LIST_BOARDS:
        ykush3_list_attached (); 
        break;

      case YKUSH3_GET_STATUS:
        if (bySerialFlag)
          {
            status_response = ykush3.get_port_status (argv[3], port);
            if (status_response < 0)
              return status_response;
            if (status_response >> 4)
              printf ("\n\nDownstream port %d is ON\n\n", status_response & 0x0F);
            else 
              printf ("\n\nDownstream port %d is OFF\n\n", status_response & 0x0F);
            return 0;
          } 
        else
          {
            status_response = ykush3.get_port_status (NULL, port);
            if (status_response < 0)
              return status_response;
            if (status_response >> 4)
                 printf ("\n\nDownstream port %d is ON\n\n", status_response & 0x0F);
            else 
                 printf ("\n\nDownstream port %d is OFF\n\n", status_response & 0x0F);
            return 0;
          }   
        break;

      case YKUSH3_EXT_CTRL_ON:
        if (bySerialFlag)
          return ykush3.port_up (argv[3], '4'); 
        else
          return ykush3.port_up (NULL, '4'); 
        break;

      case YKUSH3_EXT_CTRL_OFF:
        if (bySerialFlag)
          return ykush3.port_down (argv[3], '4'); 
        else
          return ykush3.port_down (NULL, '4'); 
        break;

      case YKUSH3_WRITE_IO:
        if (bySerialFlag)
          return ykush3.write_io (argv[3], port, value); 
        else
          return ykush3.write_io (NULL, port, value); 
        return 0;
        break;

      case YKUSH3_READ_IO:
        if (bySerialFlag)
          printf ("\n%d\n", ykush3.read_io(argv[3], port)); 
        else
          printf ("\n%d\n", ykush3.read_io(NULL, port)); 
        return 0;
        break;

      case YKUSH3_CONFIG:
        if (bySerialFlag)
              return ykush3.config_port (argv[3], port, value); 
        else
              return ykush3.config_port (NULL, port, value); 
        break;

      case YKUSH3_RESET:
        if (bySerialFlag)
          return ykush3.reset (argv[3]); 
        else
          return ykush3.reset (NULL); 
        break;

      case YKUSH3_GPIO_EN:
        if (bySerialFlag)
          return ykush3.gpio_ctrl_enable (argv[3]); 
        else
          return ykush3.gpio_ctrl_enable (NULL); 
        break;

      case YKUSH3_GPIO_DIS:
        if (bySerialFlag)
          return ykush3.gpio_ctrl_disable (argv[3]); 
        else
          return ykush3.gpio_ctrl_disable (NULL); 
        break;

      case YKUSH3_ENTER_BOOTLOADER:
        if (bySerialFlag)
          return ykush3.enter_bootloader (argv[3]); 
        else
          return ykush3.enter_bootloader (NULL);
        break;

      case YKUSH3_FIRMWARE_VERSION:
        return ykush3.display_version_firmware ();
        break;
      
      case YKUSH3_BOOTLOADER_VERSION:
        return ykush3.display_version_bootloader ();
        break;

      default:
        ykush3.print_help ();
        return -1;
        break;

      }

    return 0;
  }


int Ykush3::port_up(char *serial, char port)
{
	switch(port) {
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
	}


    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 64);

}



int Ykush3::port_down(char *serial, char port)
{
    switch(port)
    {
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
    }

    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 64);


}



int Ykush3::get_port_status(char *serial, char port)
{
    int status;

    switch(port)
    {
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
            return 0;
    }

    //send HID report to board
    sendHidReport(serial, hid_report_out, hid_report_in, 64);

    //handle board response HID report
    status = hid_report_in[1];

    return status;
}



int Ykush3::write_io(char *serial, char port, char value)
{

    hid_report_out[0] = 0x31;
    if(value=='0'){
        hid_report_out[2] = 0;
    } else {
        hid_report_out[2] = 1;
    }


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

        default:
            return 0;
            break;

    }

    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 64);

}



int Ykush3::read_io(char *serial, char port)
{

    hid_report_out[0] = 0x30;

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

        default:
            return 0;
            break;

    }


    //send HID report to board
    return sendHidReport(serial, hid_report_out, hid_report_in, 64);

    //process response
    return hid_report_in[3];

}


int Ykush3::config_port(char *serial, char port, char value)
{
	hid_report_out[0] = 0x41;
	if ( value == '0' )
		hid_report_out[2] = 0;
	else if ( value == '1' )
		hid_report_out[2] = 1;
	else
		hid_report_out[2] = 2;	//Persist mode



	switch ( port ) {
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
		return 0;
		break;
	}

	//send HID report to board
	return sendHidReport(serial, hid_report_out, hid_report_in, 64);

}


int Ykush3::reset(char *serial)
{
	hid_report_out[0] = 0x55;

	//send HID report to board
	return sendHidReport(serial, hid_report_out, hid_report_in, 64);
}



int
Ykush3::gpio_ctrl_enable (char *serial)
  {
    hid_report_out[0] = 0x32;
    hid_report_out[1] = 0x01;
    
    //send HID report to board
    sendHidReport (serial, hid_report_out, hid_report_in, 64);
    return 0;
  }



int
Ykush3::gpio_ctrl_disable (char *serial)
  {
    hid_report_out[0] = 0x32;
    hid_report_out[1] = 0x00;
    
    //send HID report to board
    sendHidReport (serial, hid_report_out, hid_report_in, 64);
    return 0;
  }



int 
Ykush3::enter_bootloader (char *serial)
  {
    hid_report_out[0] = 0x42;
    
    //send HID report to board
    sendHidReport (serial, hid_report_out, hid_report_in, 64);
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

	if ( (hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x51) ) {
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

	if ( (hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x51) ) {
		//command executed with success
		return 0;
	}

	return 1;
}


int Ykush3::i2c_write(char *i2c_address_ASCII, char *num_bytes_ASCII, char **data_to_write_ASCII)
{
	hid_report_out[0] = 0x52;
	hid_report_out[1] = 0x01;

	//convert i2c_address_ASCII to binary
	hex2bin(i2c_address_ASCII + 2, &hid_report_out[2], 2);

	//convert num_bytes_ASCII to binary
	int size = strlen(num_bytes_ASCII);
	if ( size <= 0 )
		return 1;
	dec2bin(num_bytes_ASCII, &hid_report_out[3], size);
	if ( hid_report_out[3] > 60 )
		return 2;

	//convert data_to_write_ASCII to binary
	for ( int i = 0; i < hid_report_out[3]; i++ ) {
		hex2bin(num_bytes_ASCII + 2, &hid_report_out[i + 4], 2);
	}

	sendHidReport(usb_serial, hid_report_out, hid_report_in, 64);

	if ( (hid_report_in[0] == 0x01) && (hid_report_in[1] == 0x52) ) {
		//command executed with success
		return 0;
	}
	return 0;
}


int Ykush3::i2c_read(char *i2c_address_ASCII, char *num_bytes_ASCII, unsigned char *data_buffer, int *bytes_read)
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
    if (sendHidReport (usb_serial, hid_report_out, hid_report_in, 64) != 0 ) 
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

    std::cout << "Bootloader version-" << (int) hid_report_in[2] << "." << (int) hid_report_in[3] << "." << (int) hid_report_in[4] << std::endl;
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


void Ykush3::print_help(void)
{
	Help help(app_exc_name);

	help.print_version();
	help.print_usage();
	help.print_ykush3();
}






