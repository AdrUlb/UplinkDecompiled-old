#pragma once

#include <UplinkObject.hpp>

class Options : UplinkObject
{
	virtual ~Options() override;
	virtual bool Load() override;
	virtual void Save() override;
	virtual void Print() override;
	virtual const char* GetID() override;
};
