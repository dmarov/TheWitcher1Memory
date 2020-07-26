#include <windows.h>
#include <iostream>
#include "Player.h"

int main(int argc, char **argv)
{
	while (true) {
		try {
			Player player = Player();

			while (true) {
				player.setVitality(10000);
				player.setEndurance(10000);
				Sleep(100);
			}

		} catch (std::exception e) {
			std::cout << e.what() << std::endl;
			Sleep(1000);
		}
	}
}
