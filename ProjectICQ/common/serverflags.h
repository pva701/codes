#ifndef SERVERFLAGS_H
#define SERVERFLAGS_H

class ServerFlags {
public:
	enum ConnectionStatus {Success_Auth, CE_Database, AE_Wrong_Login, AE_Wrong_Password};
};
#endif 
