/*
 * Event.h
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

#ifndef SRC_EVENT_H_
#define SRC_EVENT_H_

#include "Logger.h"
#include "string"

namespace ngh {

enum class EventType {
    UNDEF = 0,
            START,
            END,
            INIT,
            EMPTYFASTA,
            INDEX,
            ASSIGNTASKS,
            BUILD,
            APPENDFASTA,
            WAIT,
            REGISTER,
            WORKER_WAIT,
            WORKER_READ,
            WORKER_PARSE,
            WORKER_PROCESS,
            WORKER_PROCESS_CHUNK,
            WORKER_WRITE,
            WORKER_END,
            WORKER_INDEX,
            WORKER_APPEND_CHUNK,
            EventTypeSize,
            EventTypeFirst = UNDEF,
            EventTypeLast = WORKER_APPEND_CHUNK
};

enum class EventAction {
    START = 0,
            STOP,
            UNDEF,
            EventActionSize,
            EventActionFirst = START,
            EventActionLast = STOP
};

const std::string EventActionNames[] = {
        "Start",
        "Stop"
};


const std::string EventTypeNames[] = {
        "UNDEFINED",
        "START",
        "END",
        "INIT_PROGRAM",
        "EMPTY_FASTA",
        "INDEX_INPUTFILE",
        "ASSIGN_TASKS",
        "BUILD",
        "APPEND_FASTA",
        "WAIT_WORKERS",
        "REGISTER_WORKERS",
        "WORKER_WAIT",
        "WORKER_READ",
        "WORKER_PARSE",
        "WORKER_PROCESS",
        "WORKER_PROCESS_CHUNK",
        "WORKER_WRITE",
        "WORKER_END",
        "WORKER_INDEX",
        "WORKER_APPEND_CHUNK"
};

const std::string EventTypeColors[] = {
        "pink",
        "blue",
        "green",
        "cyan",
        "yellow",
        "green",
        "red",
        "blue violet",
        "green",
        "red",
        "cyan",
        "black",  // WORKER WAIT
        "yellow",
        "pink",
        "blue",
        "green",
        "red3",
        "green",
        "red4",
        "grey"
};

class EventLog {
 protected:
    ngh::Logger *logger;

 public:
    EventLog() {
        logger = ngh::Logger::getInstance();
    }
    ~EventLog() {}

    virtual void init() = 0;
    virtual void descrive() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void event(ngh::EventType event, ngh::EventAction action) = 0;
};
}  // namespace ngh

#endif  // SRC_EVENT_H_

