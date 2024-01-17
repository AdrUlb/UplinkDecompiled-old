#pragma once

#include <iostream>

template <typename T> class DArray
{
	int growSize;
	int size;
	T* data;
	bool* dataValid;

public:
	DArray();
	~DArray();
	void Empty();
	void SetSize(int newSize);
	int PutData(T& value);

	inline int Size()
	{
		return size;
	}

	inline bool ValidIndex(int index)
	{
		if (index >= size || index < 0)
			return false;

		return dataValid[index] != false;
	}

	T GetData(int index);
};

#include <DArray.tpp>
