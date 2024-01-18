#pragma once

template <typename T> BTree<T>::BTree() : left(nullptr), right(nullptr), key(nullptr), Value(0) {}
template <typename T> BTree<T>::BTree(const char* key, const T& value) : left(nullptr), right(nullptr), Value(value)
{
	this->key = new char[strlen(key) + 1];
	strcpy(this->key, key);
}

template <typename T> BTree<T>* BTree<T>::LookupTree(const char* key)
{
	auto node = this;

	while (node->key)
	{
		auto cmp = strcmp(key, node->key);

		// Next node is left
		if (cmp < 0)
		{
			if (!node->left)
				return nullptr;

			node = node->left;
			continue;
		}

		// Next node is right
		if (cmp > 0)
		{
			if (!node->right)
				return nullptr;

			node = node->right;
			continue;
		}

		// This node matches the key
		return node;
	}

	return nullptr;
}

template <typename T> void BTree<T>::PutData(const char* key, T& value)
{
	auto node = this;

	while (true)
	{
		// If this node does not have a key, store the value in this node
		if (!node->key)
		{
			node->key = new char[strlen(key) + 1];
			strcpy(node->key, key);
			Value = value;
			return;
		}

		auto cmp = strcmp(key, node->key);

		// Next node is left
		if (cmp < 0)
		{
			// Left node is free, use it to store the value
			if (!node->left)
			{
				node->left = new BTree<T>(key, value);
				return;
			}
			// Left node is not free, continue the search down its path
			node = node->left;
			continue;
		}

		// Next node is right
		if (cmp > 0)
		{
			// Right node is free, use it to store the value
			if (!node->right)
			{
				node->right = new BTree<T>(key, value);
				return;
			}

			// Right node is not free, continue the search down its path
			node = node->right;
			continue;
		}

		// A node with this key already existed, do nothing? (the decompiled code would append the node left?)
		break;
	}
}

template <typename T> DArray<T>* BTree<T>::ConvertToDArray()
{
	auto array = new DArray<T>();
	RecursiveConvertToDArray(array, this);
	return array;
}

template <typename T> DArray<char*>* BTree<T>::ConvertIndexToDArray()
{
	auto array = new DArray<char*>();
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
