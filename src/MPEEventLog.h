/*
 * MPEEventLog.h
 *
 *  Created on: Apr 7, 2015
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

#ifndef MPEEVENTLOG_H_
#define MPEEVENTLOG_H_
#ifdef  MPE
#include "Event.h"
#include <mpe.h>

namespace ngh {

//class EventMethod;

//class MPEEventLog : public ngh::EventMethod
class MPEEventLog : public ngh::EventLog
{

private:
	struct Event {
		int s_event;
		int e_event;
	};
	Event* eventlist;

public:
	MPEEventLog() {
	}

	virtual ~MPEEventLog(){
	    free(eventlist);
	}

	void init() {
		eventlist = new Event[(int)ngh::EventType::EventTypeSize];
		//eventlist = new Event[6];
		MPE_Init_log();

		// MASTER MPE
		for ( int r = (int) ngh::EventType::EventTypeFirst;
				r <= (int) ngh::EventType::EventTypeLast;
				++r )
		{
			eventlist[r].s_event = MPE_Log_get_event_number();
			eventlist[r].e_event = MPE_Log_get_event_number();
		}

		// Set description for all events in eventlist
		//descrive();

/*
		eventlist[0].s_event = MPE_Log_get_event_number();
		eventlist[0].e_event = MPE_Log_get_event_number();
*/

		/*
	    	s_init = MPE_Log_get_event_number();
	    	e_init = MPE_Log_get_event_number();
	    	s_regw = MPE_Log_get_event_number();
	    	e_regw = MPE_Log_get_event_number();

	    	if(ngh::ROLE::MASTER == role)
	    	{
	    	   	MPE_Describe_state(s_init, e_init, "Test", "red");
	    	   	MPE_Describe_state(s_regw, e_regw, "W::Register", "blue");
	    	   	MPE_Describe_state(s_regw, e_regw, "W::End", "blue");
	    	}
		 */
	}

	void descrive() {
		// MASTER MPE
		for ( int r = (int) ngh::EventType::EventTypeFirst;
				r <= (int) ngh::EventType::EventTypeLast;
				++r )
		{
			MPE_Describe_state(
					eventlist[r].s_event, eventlist[r].e_event,
					ngh::EventTypeNames[r].c_str(),
					ngh::EventTypeColors[r].c_str()
			);
		}
		/*

		MPE_Describe_state(
				eventlist[0].s_event, eventlist[0].e_event,
				ngh::EventTypeNames[0].c_str(),
				ngh::EventTypeColors[0].c_str()
				);
		MPE_Describe_state(
						eventlist[0].s_event, eventlist[0].e_event,
						"Test",
						"red"
						);
		*/
	}

	void end() {
		MPE_Finish_log( "pghcaller" );
		delete eventlist;
	}

	void start() {
		init();
		// Set description for all events in eventlist
		descrive();
	    MPE_Start_log();
	}

	void stop() {
		MPE_Stop_log();
		end();
	}

	void event(ngh::EventType event, ngh::EventAction action) {
		switch(action)
		{
		case ngh::EventAction::START:
			MPE_Log_event( eventlist[(int)event].s_event, 0,
					ngh::EventActionNames[(int)action].c_str() );
			//MPE_Log_event( eventlist[0].s_event, 0, "START");
			break;
		case ngh::EventAction::STOP:
			MPE_Log_event( eventlist[(int)event].e_event, 0,
								ngh::EventActionNames[(int)action].c_str() );
			//MPE_Log_event( eventlist[0].e_event, 0, "STOP");
			break;
		default:	// UNDEF actions here
			//ERR_("Event type not defined");
			MPE_Log_event( eventlist[0].s_event, 0, "UNDEF");
			MPE_Log_event( eventlist[0].e_event, 0, "UNDEF");
		}

	}
};


}
#endif
#endif /* MPEEVENTLOG_H_ */
