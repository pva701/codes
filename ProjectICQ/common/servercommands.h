#ifndef SERVERCOMMANDS_H
#define SERVERCOMMANDS_H

#include <qnumeric.h>

namespace ServerCommands {
    const quint16 AUTH = 0;
    const quint16 LOAD_USERLIST = 1;
    const quint16 SEND_MESSAGE = 2;
    const quint16 TIME_SERVER = 3;
    const quint16 ADD_USER_BY_ID = 4;
    const quint16 ADD_USER_BY_LOGIN = 5;
    const quint16 CHANGE_STATUS_FRIEND = 6;
    const quint16 YOU_ADDED_IN_USERLIST = 7;
    const quint16 LOAD_HISTORY = 8;
    const quint16 REGISTER_USER = 9;
    const quint16 FIND_USER = 10;
    const quint16 NOTIFY_ON_OFF = 11;
    const quint16 LOAD_NOTIFYS = 12;
    const quint16 READ_MESSAGE_NOTIFY = 13;
    const quint16 ADD_USER_BY_DIALOG = 14;
}
#endif 
