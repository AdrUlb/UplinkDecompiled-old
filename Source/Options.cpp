#include <Options.hpp>
#include <RedShirt.hpp>
#include <TempDefines.hpp>

Option::Option() : Name{0}, Tooltip{0}, YesOrNo(false), Visible(true), Value(0) {}

Option::~Option() {}

bool Option::Load(FILE* file)
{
	LoadID(file);

	if (!FileReadData(Name, sizeof(Name), 1, file))
	{
		Name[0] = 0;
		return false;
	}
	Name[sizeof(Name) - 1] = 0;

	if (!FileReadData(Tooltip, sizeof(Tooltip), 1, file))
	{
		Tooltip[0] = 0;
		return 0;
	}
	Tooltip[sizeof(Tooltip) - 1] = 0;

	if (!FileReadData(&YesOrNo, 1, 1, file))
		return false;

	if (!FileReadData(&Visible, 1, 1, file))
		return false;

	if (!FileReadData(&Value, 4, 1, file))
		return false;

	LoadID_END(file);
	return true;
}

void Option::Save(FILE* file)
{
	SaveID(file);
	fwrite(Name, sizeof(Name), 1, file);
	fwrite(Tooltip, sizeof(Tooltip), 1, file);
	fwrite(&YesOrNo, sizeof(YesOrNo), 1, file);
	fwrite(&Visible, sizeof(Visible), 1, file);
	fwrite(&Value, sizeof(Value), 1, file);
	SaveID_END(file);
}

void Option::Print()
{
	printf("Option : name=%s, value=%d\n\tYesOrNo=%d, Visible=%d\n", Name, Value, YesOrNo, Visible);
}

const char* Option::GetID()
{
	return "OPTION";
}

UplinkObjectId Option::GetOBJECTID()
{
	return UplinkObjectId::Option;
}

Options::Options()
{
	strncpy(themeName, "graphics", sizeof(themeName));
}

Options::~Options()
{
	DeleteBTreeData((BTree<UplinkObject*>*)&options);

	const auto colourOptionsArr = colourOptions.ConvertToDArray();

	for (auto index = 0; index < colourOptionsArr->Size(); index++)
	{
		if (!colourOptionsArr->ValidIndex(index))
			continue;

		const auto item = colourOptionsArr->GetData(index);
		
		if (item)
			delete item;
	}

	delete colourOptionsArr;
}

bool Options::Load(FILE* file)
{
	(void)file;
	FILE* optionsFile;
	char optionsFilePath[0x100];
	char themeName[sizeof(themeName) + 4];
	char saveVersion[0x20];

	UplinkSnprintf(optionsFilePath, sizeof(optionsFilePath), "%soptions", gApp->UsersPath);

	printf("Loading uplink options from %s...", optionsFilePath);

	const auto fileEncrypted = RedShirt::FileEncryptedNoVerify(optionsFilePath);

	if (fileEncrypted)
	{
		if (!RedShirt::FileEncrypted(optionsFilePath))
		{
			puts("failed");
			return false;
		}

		optionsFile = RedShirt::FileOpen(optionsFilePath, "rb");
	}
	else
		optionsFile = fopen(optionsFilePath, "rb");

	if (!optionsFile)
	{
		puts("failed");
		return false;
	}

	if (!FileReadData(saveVersion, 6, 1, optionsFile) || saveVersion[0] == 0 || strcmp(saveVersion, gMinSaveVersion) < 0 ||
		strcmp(saveVersion, saveVersion) > 0)
	{
		puts("\nERROR : Could not load options due to incompatible version format");
		if (fileEncrypted)
		{
			RedShirt::FileClose(optionsFilePath, optionsFile);
			return false;
		}
		fclose(optionsFile);
		return false;
	}

	puts("success");

	LoadID(optionsFile);
	if (!LoadBTree((BTree<UplinkObject*>*)&options, optionsFile))
	{
		DeleteBTreeData((BTree<UplinkObject*>*)&options);
		return false;
	}
	LoadID_END(optionsFile);

	// The size of themeNameLength in the file depends on the native integer size
	// This code will work with either both 32-bit and 64-bit integers in this field
	uint32_t themeNameLength = 0;
	if ((fgetc(optionsFile) == 't' && fread(&themeNameLength, sizeof(themeNameLength), 1, optionsFile) == 1) &&
		themeNameLength + 1 < sizeof(themeName))
	{
		if (fread(themeName, themeNameLength, 1, optionsFile) == 1)
		{
			auto fixedThemeName = themeName;

			if (themeNameLength > 0 && themeName[0] == 0)
			{
				fread(fixedThemeName + themeNameLength, 4, 1, optionsFile);
				fixedThemeName += 4;
			}

			fixedThemeName[themeNameLength] = 0;
			UplinkStrncpy(this->themeName, fixedThemeName, sizeof(themeName));
		}
	}

	if (fileEncrypted)
	{
		RedShirt::FileClose(optionsFilePath, optionsFile);
	}
	else
		fclose(optionsFile);

	return true;
}

void Options::Save(FILE* file)
{
	(void)file;
	MakeDirectory(gApp->UsersPath);

	char optionsFilePath[0x100];
	UplinkSnprintf(optionsFilePath, sizeof(optionsFilePath), "%soptions", gApp->UsersPath);

	printf("Saving uplink options to %s...", optionsFilePath);

	auto optionsFile = fopen(optionsFilePath, "wb");

	if (!optionsFile)
	{
		puts("failed");
		return;
	}

	puts("success");

	fwrite(gCurrentSaveVersion, 6, 1, optionsFile);

	SaveID(optionsFile);
	SaveBTree((BTree<UplinkObject*>*)&options, optionsFile);
	SaveID_END(optionsFile);

	fputc('t', optionsFile);
	const auto themeNameLength = strlen(themeName);
	fwrite(&themeNameLength, sizeof(themeNameLength), 1, optionsFile);
	fwrite(themeName, themeNameLength, 1, optionsFile);

	fclose(optionsFile);

	RedShirt::EncryptFile(optionsFilePath);
}

void Options::Print()
{
	Options__Print(this);
}

const char* Options::GetID()
{
	return "OPTIONS";
}

void Options::CreateDefaultOptions()
{
	Options__CreateDefaultOptions(this);
}

Option* Options::GetOption(const char* name)
{
	const auto branch = options.LookupTree(name);

	if (!branch)
		return nullptr;

	return branch->Value;
}

int Options::GetOptionValue(const char* name)
{
	const auto option = GetOption(name);

	if (!option)
	{
		char buffer[0x100];
		UplinkSnprintf(buffer, sizeof(buffer), "Option %s not found", name);
		UplinkAbort(buffer);
	}

	return option->Value;
}

void Options::SetOptionValue(const char* name, const int value)
{
	const auto optionTree = options.LookupTree(name);

	if (!optionTree)
	{
		printf("Tried to set unrecognised option: %s\n", name);
		return;
	}

	const auto option = optionTree->Value;

	UplinkAssert(option);
	option->SetValue(value);
}

bool Options::IsOptionEqualTo(const char* name, const int value)
{
	Option* option = GetOption(name);

	if (!option)
		return false;

	return option->Value == value;
}
