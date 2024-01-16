#pragma once

template <typename T> BTree<T>::BTree() : left(nullptr), right(nullptr), key(nullptr), value(0) {}
template <typename T> BTree<T>::BTree(const char* key, const T* const value) : left(nullptr), right(nullptr)
{
	this->key = new char[strlen(key) + 1];
	strcpy(this->key, key);
	value = *value;
}

template <typename T> DArray<T>* BTree<T>::ConvertToDArray()
{
	auto arr = new DArray<T>();
	RecursiveConvertToDArray(arr, left);
	return arr;
}

template <typename T> void BTree<T>::RecursiveConvertToDArray(DArray<T>* array, BTree<T>* tree)
{
	UplinkAssert(array);

	while (tree)
	{
		if (tree->key)
			array->PutData(&tree->value);

		RecursiveConvertToDArray(array, tree->Left());

		tree = tree->Right();
	}
}
