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

#ifndef DUMMYEVENTLOG_H_
#define DUMMYVENTLOG_H_
#include "Event.h"

namespace ngh {

class DummyEventLog : public ngh::EventLog
{

private:

public:
	DummyEventLog() {
	}

	virtual ~DummyEventLog(){
	}

	void init() {}
	void descrive() {}
	void end() {}
	void start() {}
	void stop() {}

	void event(ngh::EventType event, ngh::EventAction action) {
	    int a = (int)event;
	    a++;
		switch(action) {
		case ngh::EventAction::START:
		    break;
		case ngh::EventAction::STOP:
			break;
		case ngh::EventAction::UNDEF:
			break;
		}
	}
};

}
#endif /* DUMMYEVENTLOG_H_ */
