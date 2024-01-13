#pragma once

#include <cstddef>

class UplinkObject
{
public:
	virtual ~UplinkObject();
	virtual bool Load();
	virtual void Save();
	virtual void Print();
	virtual void Update();
	virtual const char* GetID();
	virtual size_t GetOBJECTID();
};
