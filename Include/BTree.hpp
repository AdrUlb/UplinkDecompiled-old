#pragma once

#include <DArray.hpp>
#include <Options.hpp>
#include <cstring>

template <typename T> class BTree
{
	BTree<T>* left;
	BTree<T>* right;
	char* key;
	T value;

public:
	BTree();
	BTree(const char* key, const T* const value);
	DArray<T>* ConvertToDArray();
	void RecursiveConvertToDArray(DArray<T>* arr, BTree<T>* tree);

	inline BTree<T>* Left()
	{
		return left;
	}

	inline BTree<T>* Right()
	{
		return right;
	}
};

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
