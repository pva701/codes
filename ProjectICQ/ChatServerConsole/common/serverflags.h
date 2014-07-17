#ifndef SERVERFLAGS_H
#define SERVERFLAGS_H

namespace ServerFlags {
    enum ConnectionStatus {Success_Auth, CE_Database, AE_Wrong_Login, AE_Wrong_Password, Time_Out};
    enum NotificationType {UserOffline, UserOnline};
}
#endif 
