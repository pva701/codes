#ifndef SERVERFLAGS_H
#define SERVERFLAGS_H

namespace ServerFlags {
    enum ConnectionStatus {Success_Auth, CE_Database, AE_Wrong_Login, AE_Wrong_Password, Time_Out};
    enum StatusType {UserOffline, UserOnline};
    enum NotificationType {RequestAddToFriends, UnreadMessages};
    enum UserType {InUserlist, Friend, Discard};
}
#endif 
