// Program entry point
#include "commandParser.h"


unsigned int PRODUCT_ID = 0;
char *app_exc_name;

int main(int argc, char* argv[])
{
	app_exc_name = argv[0];
    	commandParser(argc, argv);
    	return 0;
}

