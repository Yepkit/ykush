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
#include "ykush_help.h"
#include <stdio.h>




Help::Help(const char *input_file_name)
{
    file_name = input_file_name;
}


void Help::print(void)
{
    FILE *fp;
    char line[150];

    //open text file
#ifndef LINUX
	fopen_s(&fp, file_name, "r");
#else
    fp = fopen(file_name, "r");
#endif

    if(fp==NULL)
    {
        printf("\n\nHelp file not found.\n\n");
        return;
    }

    //print line by line
    while(!feof(fp))
    {
        if(fgets(line, 150, fp)!=NULL)
        {
            printf("%s",line);
        }
    }

    printf("\n\n");
    fclose(fp);
    return;

}




