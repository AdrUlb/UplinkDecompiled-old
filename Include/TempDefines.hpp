#pragma once

#if defined(__x86_64__)
#include <TempDefines_x64.hpp>
#elif defined(__i386__)
#include <TempDefines_x86.hpp>
#else
#error Platform not supported.
#endif

static inline void DeleteBTreeData(BTree<UplinkObject*>* tree)
{
	DeleteBTreeDataImpl(tree);
}

static inline void DeleteBTreeData(BTree<Option*>* tree)
{
	DeleteBTreeDataImpl((BTree<UplinkObject*>*)tree);
}
