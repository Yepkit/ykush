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


#ifndef _YKUSH2_H_
#define _YKUSH2_H_


#include <yk_usb_device.h>





//------------------------------------
//CLASSES
//------------------------------------

class Ykush2 : public UsbDevice  
{
    public:

        Ykush2(unsigned int pid)
            : UsbDevice(0x04D8, pid)
        {     
        }

        int get_port_status(char *serial, char port);     //get downstream port status

        int port_up(char *serial, char port);

        int port_down(char *serial, char port);

    private:

        bool is_legacy;
    
};





//---------------------------------
//FUNCTIONS
//---------------------------------

void ykush2_cmd_parser(int argc, char** argv);


void ykush2_list_attached(void); 



#endif


