#pragma once

#include <cstddef>
#include <cstdio>

class UplinkObject
{
public:
	virtual ~UplinkObject();
	virtual bool Load(FILE* file);
	virtual void Save(FILE* file);
	virtual void Print();
	virtual void Update();
	virtual const char* GetID();
	virtual size_t GetOBJECTID();

	void LoadID(FILE* file);
	void LoadID_END(FILE* file);
	void SaveID(FILE* file);
	void SaveID_END(FILE* file);
};
