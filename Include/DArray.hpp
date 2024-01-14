#pragma once

#include <Util.hpp>
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

template <typename T> DArray<T>::DArray() : growSize(1), size(0), data(nullptr), dataValid(nullptr) {}
template <typename T> DArray<T>::~DArray()
{
	Empty();
}

template <typename T> void DArray<T>::Empty()
{
	if (data)
		delete[] data;

	if (dataValid)
		delete[] dataValid;

	data = nullptr;
	dataValid = nullptr;
	size = 0;
}

template <typename T> T DArray<T>::GetData(int index)
{
	UplinkAssert(index < size && index >= 0);

	if (dataValid[index] == false)
		std::cout << "DArray::GetData called, referenced unused data.  (Index = " << index << ")\n";

	return data[index];
}
