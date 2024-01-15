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
	void SetSize(int newSize);
	int PutData(T* inValue);

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

template <typename T> void DArray<T>::SetSize(int newSize)
{
	if (newSize == size)
		return;

	if (newSize == 0)
	{
		size = 0;

		if (data)
			delete[] data;

		if (dataValid)
			delete[] dataValid;

		data = nullptr;
		dataValid = nullptr;
		return;
	}

	const auto oldSize = size;
	size = newSize;

	const auto newData = new T[newSize];
	const auto newDataValid = new bool[newSize];

	for (auto i = 0; i < newSize && i < oldSize; i++)
	{
		newData[i] = data[i];
		newDataValid[i] = dataValid[i];
	}

	if (newSize > oldSize)
	{
		for (auto i = oldSize; i < newSize; i++)
			newDataValid[i] = false;
	}

	if (data)
		delete[] data;

	if (dataValid)
		delete[] dataValid;

	data = newData;
	dataValid = newDataValid;
	return;
}

template <typename T> int DArray<T>::PutData(T* inValue)
{
	if (size == 0)
	{
		SetSize(growSize);
		data[0] = *inValue;
		dataValid[0] = true;
		return 0;
	}

	int i;
	for (i = 0; dataValid[i] != false; i++)
	{
		if (i == size)
		{
			const auto oldSize = size;
			SetSize(oldSize + growSize);
			dataValid = dataValid;
			data[oldSize] = *inValue;
			dataValid[oldSize] = true;
			return oldSize;
		}
	}

	data[i] = *inValue;
	dataValid[i] = true;
	return i;
}