#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>
#include <ctime>

typedef void ClientConnection;
typedef void NetworkScreen;

enum class Status
{
	None,
	Client,
	Server
};

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
	Status status;

public:
	Network();
	virtual ~Network() override;
	virtual void Print() override;
	virtual void Update() override;
	virtual const char* GetID() override;

	inline NetworkServer* GetServer()
	{
		UplinkAssert(status == Status::Server);
		return &server;
	}

	inline NetworkClient* GetClient()
	{
		UplinkAssert(status == Status::Client);
		return &client;
	}
};
