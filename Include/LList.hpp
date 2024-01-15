#pragma once

template <typename T> class LListItem
{
	T value;

public:
	LListItem<T>* Next;
	LListItem<T>* Prev;

	LListItem();
};

template <typename T> class LList
{
	LListItem<T>* firstItem;
	LListItem<T>* lastItem;
	LListItem<T>* cachedItem;
	int cachedItemIndex;
	int length;

public:
	LList();
	~LList();
};

template <typename T> LListItem<T>::LListItem() : Next(nullptr), Prev(nullptr) {}

template <typename T> LList<T>::LList() : firstItem(nullptr), lastItem(nullptr), cachedItem(nullptr), cachedItemIndex(-1), length(0) {}

template <typename T> LList<T>::~LList()
{
	auto item = firstItem;

	while (item)
	{
		auto next = item->Next;
		delete item;
		item = next;
	}

	firstItem = nullptr;
	lastItem = nullptr;
	length = 0;
	cachedItem = nullptr;
	cachedItemIndex = -1;
}
