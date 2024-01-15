#include <Network.hpp>
#include <TempDefines.hpp>
#include <tcp4u.h>

NetworkServer::NetworkServer() : socket(-1), listening(false) {}

NetworkServer::~NetworkServer()
{
	DeleteDArrayDataD((DArray<UplinkObject*>*)&connections);
}

void NetworkServer::Print()
{
	printf("NetworkServer : listensocket:%d, lastlisten:%d\n", socket, lastListen);
	PrintDArray((DArray<UplinkObject*>*)&connections);
}

void NetworkServer::Update()
{
	NetworkServer__Update(this);
}

const char* NetworkServer::GetID()
{
	return "SERVER";
}

NetworkClient::NetworkClient()
{
	socket = -1;
	type = 0;
	screenId = 0;
	screen = nullptr;
}

NetworkClient::~NetworkClient()
{
	if (screen)
		delete screen;
}

void NetworkClient::Print()
{
	printf("NetworkClient : SOCKET:%d\n", socket);
	printf("\tcurrent screen:%d\n", screenId);
}

void NetworkClient::Update()
{
	NetworkClient__Update(this);
}

const char* NetworkClient::GetID()
{
	return "CLIENT";
}

Network::Network()
{
	status = 0;
	if (Tcp4uInit() != TCP4U_SUCCESS)
		puts("Network error : failed to initialise TCP");
}

Network::~Network()
{
	Tcp4uCleanup();
}

void Network::Print()
{
	puts("============== N E T W O R K ===============================");
	printf("Status:%d\n", status);
	switch (status)
	{
		case 1:
			GetClient()->Print();
			break;
		case 2:
			GetServer()->Print();
			break;
	}
	puts("============== E N D  O F  N E T W O R K ===================");
}

void Network::Update()
{
	Network__Update(this);
}

const char* Network::GetID()
{
	return "NETWORK";
}
