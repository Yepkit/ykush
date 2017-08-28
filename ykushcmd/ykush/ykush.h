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





//------------------------------------
//CLASSES
//------------------------------------

class Ykush : public UsbDevice  
{
    public:

        Ykush()
            : UsbDevice(0x04D8, 0xF2F7)
        {            
        }

        int get_port_status(char *serial, char port);     //get downstream port status

        int port_up(char *serial, char port);

        int port_down(char *serial, char port);



    
};





//---------------------------------
//FUNCTIONS
//---------------------------------

void ykush_cmd_parser(int argc, char** argv);


void ykush_help(char * execName);


void ykush_up(char * iserial);


void ykush_down(char * iserial);


void ykush_list_attached(void); 



#endif


