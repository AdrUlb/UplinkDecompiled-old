#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>
#include <ctime>

typedef void ClientConnection;
typedef void NetworkScreen;

class NetworkServer : UplinkObject
{
	int socket;
	time_t lastListen;
	bool listening;
	DArray<ClientConnection*> connections;

public:
	NetworkServer();
	virtual ~NetworkServer() override;
	virtual void Print() override;
	virtual void Update() override;
	virtual const char* GetID() override;
};

class NetworkClient : UplinkObject
{
	int socket;
	int type;
	int screenId;
	NetworkScreen* screen;

public:
	NetworkClient();
	virtual ~NetworkClient() override;
	virtual void Print() override;
	virtual void Update() override;
	virtual const char* GetID() override;
};

class Network : UplinkObject
{
	NetworkServer server;
	NetworkClient client;
	int status;

public:
	Network();
	virtual ~Network() override;
	virtual void Print() override;
	virtual void Update() override;
	virtual const char* GetID() override;
};
