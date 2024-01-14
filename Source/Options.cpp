#include <Options.hpp>
#include <TempDefines.hpp>

Options::Options()
{
	strncpy(themeName, "graphics", themeNameMax);
}

Options::~Options()
{
	DeleteBTreeData(&options);
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

bool Options::Load()
{
	return Options__Load(this);
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