//this project
#include"AppService.h"

//solution libs
#include"../sl/config.h"
#include"../Debug/dbgonly.hpp"

//third libs


//system
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
int main(int argc, char *argv[])
{
	AppService a;//need be in stack?a create method other than construct
	a.run(argc > 1 ? AbstractServiceApp::RT_CONSOLE : AbstractServiceApp::RT_SERVICE);
	return 0;
}