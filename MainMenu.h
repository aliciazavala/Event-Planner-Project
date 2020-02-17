#ifndef MAINMENU_H
#define MAINMENU_H

#include <string>

#include "Menu.h"
#include "project1_lib.h"

class MainMenu : public Menu
{
	public:
	MainMenu();
	~MainMenu();

	std::string getName() const;


	void print() const;
	//implement a settings menu
	
	private:
	//void clearScreen() const;
	
};

#endif