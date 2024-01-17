#include <Util.hpp>

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

	size = newSize;
}

template <typename T> int DArray<T>::PutData(T& value)
{
	for (auto i = 0; i < size; i++)
	{
		if (!dataValid[i])
		{
			data[i] = value;
			dataValid[i] = true;
			return i;
		}
	}

	const auto oldSize = size;
	SetSize(oldSize + growSize);
	data[oldSize] = value;
	dataValid[oldSize] = true;
	return oldSize;
}