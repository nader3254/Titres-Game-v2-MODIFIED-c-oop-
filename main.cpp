/* Command line version of Tetris
** Author: nader hany
*/


#include <Windows.h>
#include <iostream>


using namespace std;



#include "bootscreen.h"
#include "engine.h"


int main(int argc, char* argv[])
{

    BootStartUP*  boot = new BootStartUP();
	//WorkArea *    work = new WorkArea("initialize graphics area");
	engine *      game = new engine();

	return 0;
}
