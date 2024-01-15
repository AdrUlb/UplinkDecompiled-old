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
	NetworkServer__Print(this);
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
	NetworkClient__Print(this);
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
	Network__Print(this);
}

void Network::Update()
{
	Network__Update(this);
}

const char* Network::GetID()
{
	return "NETWORK";
}
