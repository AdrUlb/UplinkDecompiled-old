#pragma once

#include <BTree.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>

typedef void OptionChange;

struct ColourOption
{
	char unknown[0xC];
};

class Option : UplinkObject
{
public:
	char Name[0x40];
	char Tooltip[0x80];
	bool YesOrNo;
	bool Visible;
	int Value;
	Option();
	virtual ~Option() override;
	virtual bool Load(FILE* file) override;
	virtual void Save(FILE* file) override;
	virtual void Print() override;
	virtual const char* GetID() override;
	virtual UplinkObjectId GetOBJECTID() override;

	inline void SetValue(int value)
	{
		Value = value;
	}
};

class Options : UplinkObject
{
	BTree<Option*> options;
	LList<OptionChange*> optionChanges;
	char themeName[0x80];
	char themeAuthor[0x80];
	char themeTitle[0x80];
	char themeDescription[0x400];
	BTree<ColourOption*> colourOptions;

public:
	Options();
	void CreateDefaultOptions();
	Option* GetOption(const char* name);
	int GetOptionValue(const char* name);
	void SetOptionValue(const char* name, int value);
	bool IsOptionEqualTo(const char* name, int value);

	virtual ~Options() override;
	virtual bool Load(FILE* file) override;

private:
	virtual void Save(FILE* file) override;
	virtual void Print() override;
	virtual const char* GetID() override;
};
