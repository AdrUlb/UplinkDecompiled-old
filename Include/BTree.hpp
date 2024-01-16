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

#include <BTree.tpp>
