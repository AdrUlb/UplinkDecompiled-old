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
	BTree<T>* piVar2;

	auto current = this;

	while (current->key)
	{
		auto cmp = strcmp(key, current->key);

		// Next node is left
		if (cmp < 0)
		{
			if (!current->left)
				return nullptr;

			current = current->left;
			continue;
		}

		// Next node is right
		if (cmp > 0)
		{
			if (!current->right)
				return nullptr;

			current = current->right;
			continue;
		}

		// This node matches the key
		return current;
	}
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
