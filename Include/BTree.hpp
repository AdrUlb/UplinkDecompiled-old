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
	void RecursiveConvertToDArray(DArray<T>* arr, BTree<T>* left);
};

template <typename T> BTree<T>::BTree() : left(nullptr), right(nullptr), key(nullptr), value(0) {}
template <typename T> BTree<T>::BTree(const char* key, const T* const value) : left(nullptr), right(nullptr)
{
	key = new char[strlen(key) + 1];
	strcpy(key, key);
	value = *value;
}

template <typename T> DArray<T>* BTree<T>::ConvertToDArray()
{
	auto arr = new DArray<T>();
	RecursiveConvertToDArray(arr, left);
	return arr;
}

template <typename T> void BTree<T>::RecursiveConvertToDArray(DArray<T>* arr, BTree<T>* left)
{

}