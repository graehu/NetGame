#ifndef GAME_H
#define GAME_H

//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>

#include <SDL/SDL.h>

#include "header/net.h"


class game
{
	public:
		game();
		~game();

		bool init(void);
		bool run(void);

		//state changes.

	protected:

	private:

	bool mRunning;
	network* mNet;


};
#endif//GAME_H
