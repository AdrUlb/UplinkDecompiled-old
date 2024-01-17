#pragma once

template <typename T> BTree<T>::BTree() : left(nullptr), right(nullptr), key(nullptr), Value(0) {}
template <typename T> BTree<T>::BTree(const char* key, const T* const value) : left(nullptr), right(nullptr)
{
	this->key = new char[strlen(key) + 1];
	strcpy(this->key, key);
	value = *value;
}

template <typename T> BTree<T>* BTree<T>::LookupTree(const char* key)
{
	BTree<T>* lastBranch;
	auto currentBranch = this;
	int cmp;

	do
	{
		do
		{
			// The current branch does not have a key
			if (!currentBranch->key)
				return nullptr;

			cmp = strcmp(key, currentBranch->key);

			// The current branch matches the key
			if (cmp == 0)
				return currentBranch;

			// There is no left path
			if (!currentBranch->left)
				break;

			lastBranch = currentBranch;
			currentBranch = currentBranch->left;
		} while (cmp < 0); // Loop while the next branch is to the left

		if (!lastBranch->right)
			return nullptr;

		currentBranch = lastBranch->right;

	} while (cmp > 0); // Loop while the next branch is to the right

	// The next branch was neither left nor right but also not the last branch, value not found
	return nullptr;
}

template <typename T> DArray<T>* BTree<T>::ConvertToDArray()
{
	auto array = new DArray<T>();
	RecursiveConvertToDArray(array, this);
	return array;
}

template <typename T> DArray<const char*>* BTree<T>::ConvertIndexToDArray()
{
	auto array = new DArray<const char*>();
	RecursiveConvertIndexToDArray(array, this);
	return array;
}

template <typename T> void BTree<T>::RecursiveConvertToDArray(DArray<T>* array, BTree<T>* tree)
{
	UplinkAssert(array);

	while (tree)
	{
		if (tree->key)
			array->PutData(tree->Value);

		RecursiveConvertToDArray(array, tree->Left());

		tree = tree->Right();
	}
}

template <typename T> void BTree<T>::RecursiveConvertIndexToDArray(DArray<char*>* array, BTree<T>* tree)
{
	UplinkAssert(array);

	while (tree)
	{
		if (tree->key)
			array->PutData(tree->key);

		RecursiveConvertIndexToDArray(array, tree->Left());

		tree = tree->Right();
	}
}
