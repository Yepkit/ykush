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

#ifndef _YKUSH_H_
#define _YKUSH_H_

#include <yk_usb_device.h>
#include <command_parser.h>

/**
 * \defgroup ykush YKUSH boards
 */

/**
 * \ingroup ykush
 * \brief Available options for YKUSH boards.
 */
enum ykushAction {
	YKUSH_PORT_UP,
	YKUSH_PORT_DOWN,
	YKUSH_PORT_STATUS,
	YKUSH_LIST_BOARDS,
	YKUSH_GET_STATUS,
	YKUSH_HELP
};

/**
 * \ingroup ykush
 * \brief Handles the YKUSH device.
 */
class Ykush : public UsbDevice  
{
	public:

		Ykush(unsigned int pid)
		: UsbDevice(0x04D8, pid)
		{     
			if( pid == 0x0042 ) {
				is_legacy = true;
			} else {
				is_legacy = false;
			}
			non_blocking_usb_comm = false;
		}

		int get_port_status(char *serial, char port);     //get downstream port status

		/**
		 * \brief Switch ON YKUSH downstream(s).
		 * 
		 */
		int port_up(char *serial, char port);

		int port_down(char *serial, char port);

		void print_help(char *exec_name);

	private:

		bool is_legacy;
		bool non_blocking_usb_comm;
    
};





//---------------------------------
//FUNCTIONS
//---------------------------------

int ykush_cmd_parser(int argc, char** argv);


void ykush_help(char * execName);


void ykush_up(char * iserial);


void ykush_down(char * iserial);


int ykush_list_attached(void); 



#endif


