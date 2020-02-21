#include "Executive.h"
Executive::Executive()
{
	//both should be empty at startup
	m_menuStack = nullptr;

	m_loadedYear = getCurrentYear();

	m_militaryTime = false;

	m_timeArr = new char*[18];
	for(int i = 0; i < 18; i++)
	{
		m_timeArr[i] = new char[3];
	}
	clearTimeArr();
}

Executive::~Executive()
{
	if(m_menuStack != nullptr)
	{
		delete m_menuStack;
	}
	if(m_timeArr != nullptr)
	{
		for(int i = 0; i < 18; i++)
		{
			delete[] m_timeArr[i];
		}
		delete[] m_timeArr;
	}
}

void Executive::run(bool test)
{
	if(load())//executes load
	{
		m_menuStack = new Stack<Menu*>;

		Menu* newMenu = new MainMenu;
		m_menuStack->push(newMenu);

	}

	if(test)
	{
		loadTimeArr(std::string("000000000011111111110000000000111111111100000000000000"));
		handleAttendTimeMenu();
		//handleAttendTimeMenu();
		//Menu* testMenu = new TimeMenu;
		//m_menuStack->push(testMenu);
	}

	while(!m_menuStack->isEmpty())//loops as long as there are menus
	{
		std::string currentMenu = (m_menuStack->peek())->getName();

		if(currentMenu == "MainMenu")
		{
			handleMainMenu();
		}
		else if(currentMenu == "MonthMenu")
		{
			handleMonthMenu();
		}
		else if(currentMenu == "NewEventMenu")
		{
			handleNewEventMenu();
		}
		else if(currentMenu == "ViewEventMenu")
		{
			handleViewEventMenu();
		}
		else if(currentMenu == "TimeMenu")
		{
			handleTimeMenu();
		}
		else if(currentMenu == "SettingsMenu")
		{
			handleSettingsMenu();
		}
	}
	save();
}

bool Executive::load()
{
	//read from file then return true or return false if file cannot be openned

	//int m_eventsLoaded = 1;//should be read from file
	//m_calendar = new Calendar(m_eventsLoaded);

	return true;//placeholder return
}

void Executive::save()
{
	//save to file
	//possible feature: encrypt the save file to prevent it from being read or altered
	//			ex: convert chars to bits (8 bits) and then use XOR encryption
	//				   x XOR key = y, where x, y, and key are 8bit strings
	//				=> y XOR key = x


	std::cout << "save complete\n";//placeholder return
}

void Executive::handleMainMenu()
{
	MainMenu temp;
	temp.print(m_loadedYear);//print

	std::string validInputs[16] = {"q","1","2","3","4","5","6","7","8","9","10","11","12","b","n","s"};
	std::string monthArr[12] = {"1","2","3","4","5","6","7","8","9","10","11","12"};
	std::string input = getStrFromSet(16, validInputs);//ask user for input

	if(containsStr(input, 12, monthArr))
	{
		m_loadedMonth = stoi(input);
		Menu* newMenu = new MonthMenu();
		m_menuStack->push(newMenu);

	}
	else if(input == "q")
	{
		handleBack();
	}
	else if(input == "b")// move back 1 year
	{
		m_loadedYear = m_loadedYear - 1;
	}
	else if(input == "n")// move forward 1 year
	{
		m_loadedYear = m_loadedYear + 1;//currently no upper limit for year
	}
	else if(input == "s")
	{
		Menu* newMenu = new SettingsMenu();
		m_menuStack->push(newMenu);
	}

}

void Executive::handleMonthMenu()
{
	//create temp
	MonthMenu temp;
	//read number of events in a month from that file
	int range = EventsInMonth(m_loadedMonth);
	temp.setTotalEvents(range);
	temp.print(m_loadedMonth, m_loadedYear);
	int input = getIntRangeFromUser(0,range+1);
	if(input == 0)
	{
		handleBack();
	}
	else if(input == range+1)
	{
		Menu* temp = new NewEventMenu(m_loadedMonth);
		m_menuStack->push(temp);
	}
	/*else if(input>0 && input<range+1)
	{
		m_eventId = temp.returnID(input-1);
	}
	*/
	//ask user to chose event or make event
	//if make event, then create event menu and push
}
void Executive::PrintEventsInMonth()
{

}

void Executive::handleNewEventMenu()
{
	NewEventMenu temp = NewEventMenu(m_loadedMonth);
	temp.print(m_loadedMonth,m_loadedYear);
	int x = 0;
	std::string creatorName;
	std::string EventName;
	int day;
	std::ofstream events;
	std::ofstream attendees;
	std::string FileName =	nameOfMonth(m_loadedMonth);
events.open("./data/" + FileName + ".txt", std::fstream::app);
	std::cout<<"Enter name of event creator: ";
	std::cin.ignore();
	std::getline(std::cin, creatorName);
	std::cout<<"Enter name of the event: ";
	//std::cin.ignore(); // ignores \n that cin >> str has lefted (if user pressed enter key)
	std::getline(std::cin, EventName);
	do{
	x++;
	day = 0;
	if(x==1)
		{
			std::cout<<"Enter day of event: ";
			std::cin>>day;
		}
		else if(x > 1)
		{
			std::cout<<"Enter a VALID day for the event: ";
			std::cin>>day;
		}
	}while(!isValidDate(m_loadedMonth,day,m_loadedYear));
	std::cout<<"Enter time of your event: "<<std::endl;
	TimeMenu* object = new TimeMenu();
	m_menuStack->push(object);
	handleTimeMenu();
	std::string array = ConvertArray();
	int id = generateID();
	events<<"Event: "<<id<<std::endl<<" "<<EventName<<std::endl<<" "<<m_loadedMonth<<'\t'<<day<<'\t'<<m_loadedYear<<std::endl<<" "<<creatorName<<" "<<array<<std::endl;
attendees.open("./data/Attendees.txt",std::fstream::app);
	attendees<<id<<" "<<creatorName<<std::endl;
	attendees<<array<<std::endl;
attendees.close();
events.close();
		std::cout << "[0] Back" << std::endl;
		int input = getIntRangeFromUser(0,0);
		if(input == 0)
		{
			handleBack();
		}
	//ask user for each event parameter
	//output to file
}
std::string Executive::ConvertArray()
{
	std::string time = "                                                      ";
	int x = 0;
	for(int i = 0; i < 18; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(m_timeArr[i][j] == 'n' || m_timeArr[i][j]=='_')
			{
				time[x] = '0';
			}
			else
			{
				time[x] = '1';
			}
			x++;
		}
	}
	return time;
}
int Executive::EventsInMonth(int month)
{
	std::ifstream fin;
	std::string line;
 	fin.open("./data/" + nameOfMonth(month)+".txt");
	int count = 0;
	while(!fin.eof())
	{
		std::getline(fin,line);
		if(line.substr(0,5)=="Event")
		{
			count++;
		}
	}
	fin.close();
	return count;

}
void Executive::handleSettingsMenu()
{
	int input;
	SettingsMenu temp = SettingsMenu();
	//allow the user to repeatedly change the time setting:
	do
	{
		temp.print(m_militaryTime);
		input  = getIntRangeFromUser(0,1);
		if(input == 1)
		{
			m_militaryTime = !m_militaryTime;
		}
	}while(input != 0);
	//go back one menu
	handleBack();
}
void Executive::handleViewEventMenu()
{
	std::ifstream fin;
    std::string temp;
    std::string creatorName;
    std::string eventName;
    int eventId;
    int month;
    int day;
    int year;
    int userChoice;
    int i;


    std::string fileName= "./data.txt" + nameOfMonth(m_loadedMonth) + ".txt"; //open file month
    while(fin>>temp)
    {
				i = 0;
        if(temp == std::to_string(m_eventId))
        	{
				fin>>eventId;
				std::getline(fin,eventName);
				std::getline(fin,creatorName);
				fin>>month;
				fin>>day;
				fin>>year;

				std::cout << "\t ===== " << eventName << " =====" << std::endl;
				//USE LIBRARY TO ALSO GET DAY OF THE WEEK??

				std::cout<<"Date: "<< nameOfMonth(m_loadedMonth) <<", "<< dayOfWeek(m_loadedMonth,day,m_loadedYear) << " "<< m_loadedYear << "\n" ;
				std::cout<<"Event Creator: "<<creatorName<<"\n";
			/* std::cout<<"Attendes: "<<"\n";
			//READ ATTENDEES
			    std::ifstream attendees;
			    std::string attendeeName;
			    int id;
			    //needs an attendee times variable

			    attendees.open("./data/Attendees.txt");
			    while(attendees>>id)
			    {
			      if(id == eventID)
			      {
			        i++;
				std::getline(attendees,attendeeName);
				//Line missing: get attendee's time variable

				std::cout<<attendeeName<<std::endl;
				//Line missing: print attending time
			      }


			      //gets attendee's name line and attendee's time line and skips them
			      else
			      {
				std::getline(attendees,attendeeName);
				//Line missing: std::getline(attendees,attendeeTime);
			      }

			    }

			    //If there are no attendees
			    if ( i = 0 )
			    {
			    	std::cout<<"No attendees.\n";
			    }

			    attendees.close();
			*/
        	}
    	}

		std::cout << "[1] Attend" << std::endl;
		std::cout << "[0] Back" << std::endl;
		userChoice=getIntRangeFromUser(0,1);
		if(userChoice==0)
		{
			handleBack();
		}

}

void Executive::handleTimeMenu()
{
	TimeMenu temp;

	clearTimeArr();
	if(!temp.run(m_timeArr, m_militaryTime))
	{
		clearTimeArr();
	}
	handleBack();
}

void Executive::handleAttendTimeMenu()
{
	TimeMenu temp;
	if(!temp.run(m_timeArr, m_militaryTime))
	{
		clearTimeArr();
	}
	handleBack();
}

void Executive::handleBack()
{
	m_menuStack->pop();
}

void Executive::clearTimeArr()
{
	for(int i = 0; i < 18; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			m_timeArr[i][j] = '_';
		}
	}
}

void Executive::loadTimeArr(std::string timeString)
{
	int index = 0;
	for(int i = 0; i < 18; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(timeString.at(index) == '0')
			{
				m_timeArr[i][j] = 'X';
			}
			index++;
		}
	}
}

/**
std::string stringToTime(std::string timeStr)
{
	timeStr = timeStr + "0";
	int i1 = -1;
	int i2 = 0;
	std::string finalString = "";
	bool first = true;
	for(int i = 0; i < timeStr; i++)
	{
		if(timeStr.at(i) == 1)
		{
			i1 = i;
			for(int j = i; j <= timeStr; i++)
			{
				if(i = 0)
				{
					i2 = j-1;
					finalString = finalString + indexToTime(i1) + " - " indexToTime(i2);
					if(!first)
					{
						std::cout << ", ";
					}
					first = false;
				}
			}
		}
	}
}

std::string indexToTime(int index)
{

	switch (index)
	{
		case 0: return ("05:00",);
		case 1: return ("05:20");
		case 2: return ("05:40");
		case 3: return ("06:00");
		case 4: return ("06:20");
		case 5: return ("06:40");
		case 6: return ("07:00");
		case 7: return ("07:20");
		case 8: return ("07:40");
		case 9: return ("08:00");
		case 10: return ("08:20");
		case 11: return ("08:40");
		case 12: return ("09:00");
		case 13: return ("09:20");
		case 14: return ("09:40");
		case 15: return ("10:00");
		case 16: return ("10:20");
		case 17: return ("10:40");
		case 18: return ("11:00");
		case 19: return ("11:20");
		case 21: return ("11:40");
		case 20: return ("13:00");
		case 22: return ("13:20");
		case 23: return ("13:40");
		case 24: return ("14:00");
		case 25: return ("14:20");
		case 26: return ("14:40");
		case 28: return ("03:00");
		case 27: return ("03:20");
		case 29: return ("03:40");
		case 30: return ("04:00");
		case 31: return ("04:20");
		case 32: return ("04:40");
		case 33: return ("05:00");
		case 34: return ("05:20");
		case 35: return ("05:40");
		case 36: return ("06:00");
		case 37: return ("06:20");
		case 38: return ("06:40");
		case 39: return ("07:00");
		case 40: return ("07:20");
		case 41: return ("07:40");
		case 42: return ("08:00");
		case 43: return ("08:20");
		case 44: return ("08:40");
		case 45: return ("09:00");
		case 46: return ("09:20");
		case 47: return ("09:40");
		case 48: return ("10:00");
		case 49: return ("10:20");
		case 50: return ("10:40");
		case 51: return ("11:00");
		case 52: return ("11:20");
		case 53: return ("11:40");
		default: throw(std::runtime_error("invalid index"))
	}
}
*/
