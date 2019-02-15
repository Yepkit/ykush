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


#include "string2val.h"
#include <stdlib.h>
#include <stdio.h>



/*****************************************************************
 * Function:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Precedences:
 *
 *****************************************************************/
int char2int(char input)
{
	if(input >= '0' && input <= '9')
	return input - '0';
	if(input >= 'A' && input <= 'F')
	return input - 'A' + 10;
	if(input >= 'a' && input <= 'f')
	return input - 'a' + 10;
	//throw std::invalid_argument("Invalid input string");
	return 0;
}





/*****************************************************************
 * Function:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Precedences:
 *
 *****************************************************************/
// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
int hex2bin(char* src, unsigned char* output, int size)
{
	//unsigned char *out = (unsigned char*)malloc(size/2);
	unsigned char out;
	int i;

	for(i=0; i<size; i=i+2) {
		*(output++) = char2int(src[0])*16 + char2int(src[1]);
		src += 2; 
	}

	//printf("\nout = 0x%x\n", *out);

	return 0;
}


int dec2bin(char* src, unsigned char* output, int size)
{
	//unsigned char *out = (unsigned char*)malloc(size/2);
	unsigned char out;
	int base;
	for(int i = 0; i < size; i++) {
		base = 1;
		for (int j = 1; j < (size - i); j++) {
			base = base * 10;
		}
		*(output++) = char2int(src[0]) * base;
		src++; 
	}

	//printf("\nout = 0x%x\n", *out);

	return 0;
}

