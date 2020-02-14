#ifndef CALENDAR_H
#define CALENDAR_H

#include <string>

#include "project1_lib.h"
#include "Event.h"


class Calendar
{
	public:

	//construct array
	Calendar(int size);
	//getters/setters
	void setSize(int s);
	int getSize();
	void insertEvent(Event x);
	//methods for getting events in a month
	Event* getEvents(std::string month);
	//method to search for an event (later feature)
		//search from events
	//array to return events from the same month
	Event* m_Month;

	private:
	Event* m_eventList;
	int m_numevents;
	//data structure for events
};

#endif
