/*
 * Time.h
 *
 *  Created on: Jan 16, 2015
 *  Author: javier.navarro@caos.uab.es
 *  Copyright (c) 2015 Javier Navarro, GNU license.
 *
 *  Original work from:
 *  GHcaller
 *  Copyright (c) 2013 Bruno Nevado, GNU license.
 *
 *  This file is part of PGHcaller.
 *
 *  PGHcaller is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PGHcaller is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PGHcaller.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef TIME_H_
#define TIME_H_

#include <iostream>
#include <sstream>
#include <ctime>
#include "Event.h"


class Time
{
public:
	Time() {
		reset();
	}

	~Time(){
	};

	void start() {
		if(enabled)
		{
			Event e;
			e.timestamp = clock();
			e.event = EventType::START;
			events.push_back(e);
		}
	}

	void end() {
		if (enabled)
		{
			Event e;
			e.timestamp = clock();
			e.event = EventType::END;
			events.push_back(e);
			enabled = false;
		}
	}

	void event(EventType event) {
		if (enabled)
		{
			Event e;
			e.timestamp = clock();
			e.event = event;
			events.push_back(e);
		}
	}

	void event() {
			if (enabled)
			{
				Event e;
				e.event = EventType::UNDEF;
				e.timestamp = clock();
				events.push_back(e);
			}
	}

	double elapsedSeconds()	{
		return elapsed()/1000;
	}

	double elapsed() {
		clock_t now = clock();
		Event lastEvent = events.back();
		return( (now - lastEvent.timestamp)  / ( CLOCKS_PER_SEC / 1000 ) );
	}


	std::string total(EventType eventType) {
		std::stringstream ss;

		// start & end must be defined
		if(2 <= events.size())
		{
			std::vector<Event>::iterator it = events.begin();
			Event last_event, this_event;
			double ttime = 0;

			//start with endl to make output more parseable
			ss << std::endl << "# Event_Name Timestamp Elapsed (miliseconds)" << std::endl;

			last_event = *it; ++it;
			Event start = last_event;

			while( it != events.end() )
			{
				this_event = *it;

				// Check if this event has required type and store elapsed time
				if(eventType == this_event.event)
				{
					double time = getDiff(last_event, this_event);
					ttime += time;
				}

				// Get next elements
				last_event = *it; ++it;
			}
			ss << "# TOTAL " << EventTypeNames[static_cast<int>(eventType)]  << " " << ttime << std::endl ;
		}
		else
		{
			ss << "# No events registered" << std::endl;
		}
		return ss.str();
	}

	std::string info(EventType eventType) {
		std::stringstream ss;

		// start & end must be defined
		if(2 <= events.size())
		{
			std::vector<Event>::iterator it = events.begin();
			Event last_event, this_event;
			double ttime = 0;

			//start with endl to make output more parseable
			ss << std::endl << "# Event_Name Timestamp Elapsed (miliseconds)" << std::endl;

			last_event = *it; ++it;
			Event start = last_event;

			while( it != events.end() )
			{
				this_event = *it;

				// Check if this event has required type and store elapsed time
				if(eventType == this_event.event)
				{
					double time = getDiff(last_event, this_event);
					ttime += time;

					ss << "# " << EventTypeNames[static_cast<int>(this_event.event)];
					ss << " " << getTimeStamp(this_event) << " " << time << std::endl ;
				}

				// Get next elements
				last_event = *it; ++it;

			}
			ss << "# TOTAL " << EventTypeNames[static_cast<int>(eventType)]  << " " << ttime << std::endl ;
		}
		else
		{
			ss << "# No events registered" << std::endl;
		}
		return ss.str();
	}

	std::string info() {
		std::stringstream ss;

		// start & end must be defined
		if(2 <= events.size())
		{

			std::vector<Event>::iterator it = events.begin();
			Event last_event, this_event;

			//start with endl to make output more parseable
			ss << std::endl << "# Event_Name Timestamp Elapsed (miliseconds)" << std::endl;

			last_event = *it; ++it;
			Event start = last_event;

			ss << "# " << EventTypeNames[static_cast<int>(last_event.event)];
			ss << " " << getTimeStamp(last_event) << " 0" << std::endl ;

			while( it != events.end() )
			{
				this_event = *it;

				//ss << "# " << EventTypeNames[static_cast<int>(last_event.event)] << "->" << EventTypeNames[static_cast<int>(this_event.event)];
				ss << "# " << EventTypeNames[static_cast<int>(this_event.event)];
				ss << " " << getTimeStamp(this_event) << " " << getDiff(last_event, this_event) << std::endl ;
				// Get next elements
				last_event = *it; ++it;

			}
			ss << "# TOTAL" << " " << getDiff(start, last_event) << std::endl ;
		}
		else
		{
			ss << "# No events registered" << std::endl;
		}
		return ss.str();
	}



private:
	//clock_t global_start, global_end, init_task, end_task, event;
	struct Event {
		clock_t timestamp;
		EventType event;
	};

	std::vector<Event> events;
	bool enabled;

	double getTimeStamp(Event e){
		return (e.timestamp / ( CLOCKS_PER_SEC / 1000 ));
	}
	double getDiff(Event e1, Event e2){
			return ((e2.timestamp - e1.timestamp) / ( CLOCKS_PER_SEC / 1000 ));
		}


  void reset()
	{
		events.clear();
		enabled = true;
	}
};
#endif /* TIME_H_ */
