#include <BTree.hpp>
#include <TempDefines.hpp>

bool LoadBTree(BTree<char*>* tree, FILE* file)
{
	if (!tree)
	{
		printf("LoadBTree called with tree = nullptr at %s:%d\n", __FILE__, __LINE__);
		return false;
	}

	int nodeCount;
	if (!FileReadData(&nodeCount, 4, 1, file))
		return false;

	if (nodeCount > 0x40000)
	{
		printf("WARNING: LoadBTree, number of items appears to be wrong, size=%d\n", nodeCount);
		return false;
	}

	for (auto iVar2 = 0; nodeCount > iVar2; iVar2++)
	{
		char* key;
		if (!LoadDynamicString(key, file))
			return false;

		if (!key)
		{
			printf("WARNING: LoadBTree NULL id");
			return false;
		}

		char* value;
		if (!LoadDynamicString(value, file))
		{
			if (key)
				delete[] key;

			return false;
		}

		tree->PutData(key, value);

		if (key)
			delete[] key;
	}

	return true;
}

bool LoadBTree(BTree<UplinkObject*>* tree, FILE* file)
{
	if (!tree)
	{
		printf("LoadBTree called with tree = nullptr at %s:%d\n", __FILE__, __LINE__);
		return false;
	}

	int nodeCount;
	if (!FileReadData(&nodeCount, 4, 1, file))
		return false;

	if (nodeCount > 0x40000)
	{
		printf("WARNING: LoadBTree, number of items appears to be wrong, size=%d\n", nodeCount);
		return false;
	}

	for (auto i = 0; i < nodeCount; i++)
	{
		char* key = nullptr;
		if (!LoadDynamicString(key, file))
			return false;

		if (!key)
		{
			printf("WARNING: LoadBTree NULL id");
			return false;
		}

		int id;
		if (!FileReadData(&id, 4, 1, file))
			return false;

		auto value = CreateUplinkObject(id);
		if (!value)
		{
			delete[] key;
			return false;
		}

		if (!value->Load(file))
		{
			delete[] key;
			delete value;
			return false;
		}

		tree->PutData(key, value);

		if (key)
			delete[] key;
	}
	return true;
}

void SaveBTree(BTree<char*>* tree, FILE* file)
{
	UplinkAssert(tree);
	auto values = tree->ConvertToDArray();
	auto indices = tree->ConvertIndexToDArray();
	auto valueCount = values->Size();

	if (valueCount <= 0)
	{
		auto count = 0;
		fwrite(&count, sizeof(count), 1, file);

		delete values;
		delete indices;
		return;
	}

	auto validValueCount = 0;
	for (auto i = 0; i < valueCount; i++)
	{
		if (values->ValidIndex(i))
			validValueCount++;
	}

	if (validValueCount > 0x40000)
	{
		printf("WARNING: SaveBTree, number of items appears to be too big, size=%d, maxsize=%d\n", validValueCount, 0x40000);
		validValueCount = 0x40000;
	}

	fwrite(&validValueCount, sizeof(validValueCount), 1, file);

	validValueCount = 0;
	for (int i = 0; i < valueCount && validValueCount < 0x40000; i++)
	{
		if (!values->ValidIndex(i))
			continue;

		UplinkAssert(indices->ValidIndex(i));

		SaveDynamicString(indices->GetData(i), file);
		SaveDynamicString(values->GetData(i), file);

		validValueCount++;
	}

	delete values;
	delete indices;
}

void SaveBTree(BTree<UplinkObject*>* tree, FILE* file)
{
	UplinkAssert(tree);
	auto values = tree->ConvertToDArray();
	auto indices = tree->ConvertIndexToDArray();
	auto valueCount = values->Size();

	if (valueCount <= 0)
	{
		auto count = 0;
		fwrite(&count, sizeof(count), 1, file);

		delete values;
		delete indices;
		return;
	}

	auto validValueCount = 0;
	for (auto i = 0; i < valueCount; i++)
	{
		if (values->ValidIndex(i))
			validValueCount++;
	}

	if (validValueCount > 0x40000)
	{
		printf("WARNING: SaveBTree, number of items appears to be too big, size=%d, maxsize=%d\n", validValueCount, 0x40000);
		validValueCount = 0x40000;
	}

	fwrite(&validValueCount, sizeof(validValueCount), 1, file);

	validValueCount = 0;
	for (auto i = 0; i < valueCount && validValueCount < 0x40000; i++)
	{
		if (!values->ValidIndex(i))
			continue;

		UplinkAssert(indices->ValidIndex(i));

		SaveDynamicString(indices->GetData(i), file);

		const auto value = values->GetData(i);
		auto objectId = value->GetOBJECTID();

		UplinkAssert(objectId != UplinkObjectId::Unassigned);

		fwrite(&objectId, sizeof(objectId), 1, file);

		value->Save(file);

		validValueCount++;
	}

	delete values;
	delete indices;
}
