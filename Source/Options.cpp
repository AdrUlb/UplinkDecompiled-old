#include <Options.hpp>
#include <TempDefines.hpp>

Options::Options()
{
	strncpy(themeName, "graphics", themeNameMax);
}

Options::~Options()
{
	DeleteBTreeData((BTree<UplinkObject*>*)&options);
	const auto colourOptionsArr = colourOptions.ConvertToDArray();

	for (auto index = 0; index < colourOptionsArr->Size(); index++)
	{
		if (colourOptionsArr->ValidIndex(index))
		{
			const auto item = colourOptionsArr->GetData(index);
			if (item)
				delete item;
		}
	}

	delete colourOptionsArr;
}

bool Options::Load(FILE* file)
{
	FILE* optionsFile;
	char optionsFilePath[0x100];
	char themeName[themeNameMax];
	char saveVersion[0x20];

	UplinkSnprintf(optionsFilePath, sizeof(optionsFilePath), "%soptions", gApp->UsersPath);

	printf("Loading uplink options from %s...", optionsFilePath);

	const auto fileEncrypted = RsFileEncryptedNoVerify(optionsFilePath);

	if (fileEncrypted)
	{
		if (!RsFileEncrypted(optionsFilePath))
		{
			puts("failed");
			return false;
		}

		optionsFile = RsFileOpen(optionsFilePath, "rb");
	}
	else
		optionsFile = fopen(optionsFilePath, "rb");

	if (!optionsFile)
	{
		puts("failed");
		return false;
	}

	if (!FileReadDataInt(__FILE__, __LINE__, saveVersion, 6, 1, optionsFile) || saveVersion[0] == 0 || strcmp(saveVersion, "SAV56") < 0 ||
		strcmp(saveVersion, "SAV62") > 0)
	{
		puts("\nERROR : Could not load options due to incompatible version format");
		if (fileEncrypted)
		{
			RsFileClose(optionsFilePath, optionsFile);
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

	// FIXME: loading a theme WILL fail if the size of size_t is different from the platform that saved it
	size_t themeNameLength = 0;
	if ((fgetc(optionsFile) == 't' && fread(&themeNameLength, sizeof(themeNameLength), 1, optionsFile) == 1) &&
		themeNameLength + 1 < themeNameMax)
	{
		if (fread(themeName, themeNameLength, 1, optionsFile) == 1)
		{
			themeName[themeNameLength] = 0;
			UplinkStrncpy(this->themeName, themeName, themeNameMax);
		}
	}

	if (fileEncrypted)
	{
		RsFileClose(optionsFilePath, optionsFile);
	}
	else
		fclose(optionsFile);

	return true;
}

void Options::Save()
{
	return Options__Save(this);
}

void Options::Print()
{
	return Options__Print(this);
}

const char* Options::GetID()
{
	return "OPTIONS";
}

void Options::CreateDefaultOptions()
{
	Options__CreateDefaultOptions(this);
}