#pragma once

#include <cstddef>
#include <cstdio>

enum class UplinkObjectId
{
	Unassigned = 0,
	VLocation = 1,
	Message = 3,
	Person = 4,
	Agent = 5,
	Player = 6,
	Option = 8,
	VLocationSpecial = 9,
	LanComputer = 15,
	LanComputerSystem = 16,
	LanComputerLink = 17,
	Computer = 20,
	Data = 21,
	AccessLog = 23,
	Record = 25,
	BankComputer = 27,
	BankAccount = 28,
	SecuritySystem = 29,
	GenericScreen = 30,
	MessageScreen = 31,
	PasswordScreen = 32,
	MenuScreen = 33,
	MenuScreenOption = 34,
	DialogScreen = 35,
	DialogScreenWidget = 36,
	UserIDScreen = 37,
	LogScreen = 38,
	BBSScreen = 39,
	LinksScreen = 40,
	SharesListScreen = 41,
	CypherScreen = 42,
	HighSecurityScreen = 43,
	DisconnectedScree = 44,
	Company = 50,
	CompanyUplink = 52,
	Mission = 56,
	Sale = 57,
	SaleVersion = 58,
	News = 59,
	NotificationEvent = 70,
	ArrestEvent = 71,
	ShotByFedsEvent = 72,
	SeizeGatewayEvent = 73,
	InstallHardwareEvent = 74,
	WarningEvent = 75,
	AttemptMissionEvent = 76,
	RunPlotSceneEvent = 77,
	ChangeGatewayEvent = 78,
	BankRobberyEvent = 79
};

class UplinkObject
{
public:
	virtual ~UplinkObject();
	virtual bool Load(FILE* file);
	virtual void Save(FILE* file);
	virtual void Print();
	virtual void Update();
	virtual const char* GetID();
	virtual UplinkObjectId GetOBJECTID();

	void LoadID(FILE* file);
	void LoadID_END(FILE* file);
	void SaveID(FILE* file);
	void SaveID_END(FILE* file);
};
