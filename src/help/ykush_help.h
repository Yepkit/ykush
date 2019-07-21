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



#ifndef _YKUSHCMD_HELP_H_
#define _YKUSHCMD_HELP_H_


class Help
{
	public:

		Help(char *exec_name);

		void print_usage(void);
		void print_board_names(void);
		void print_common_options(void);
		void print_ykush(void);
		void print_ykush2(void);
		void print_ykush3(void);
		void print_ykushxs(void);
		void print_all(void);

	private:

		char *app_name;
};






#endif
