#pragma once

template <typename T> class BTree
{
	BTree<T>* Left;
	BTree<T>* Right;
	char* Key;
	T Value;
};
