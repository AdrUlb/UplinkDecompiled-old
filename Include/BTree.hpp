#pragma once

#include <DArray.hpp>
#include <cstring>

template <typename T> class BTree
{
	BTree<T>* left;
	BTree<T>* right;
	char* key;

public:
	T Value;

	BTree();
	BTree(const char* key, const T& value);
	BTree<T>* LookupTree(const char* key);
	void PutData(const char* key, const T& value);
	DArray<T>* ConvertToDArray();
	DArray<char*>* ConvertIndexToDArray();
	void Empty();

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

bool LoadBTree(BTree<char*>* tree, FILE* file);
bool LoadBTree(BTree<UplinkObject*>* tree, FILE* file);
void SaveBTree(BTree<char*>* tree, FILE* file);
void SaveBTree(BTree<UplinkObject*>* tree, FILE* file);

#include <BTree.tpp>
