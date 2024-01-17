#pragma once

#include <DArray.hpp>
#include <Options.hpp>
#include <cstring>

template <typename T> class BTree
{
	BTree<T>* left;
	BTree<T>* right;
	char* key;

public:
	T Value;

	BTree();
	BTree(const char* key, const T* const value);
	BTree<T>* LookupTree(const char* key);
	DArray<T>* ConvertToDArray();
	DArray<const char*>* ConvertIndexToDArray();

	inline BTree<T>* Left()
	{
		return left;
	}

	inline BTree<T>* Right()
	{
		return right;
	}

private:
	static void RecursiveConvertToDArray(DArray<T>* array, BTree<T>* tree);
	static void RecursiveConvertIndexToDArray(DArray<char*>* array, BTree<T>* tree);
};

#include <BTree.tpp>
