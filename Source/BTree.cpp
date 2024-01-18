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