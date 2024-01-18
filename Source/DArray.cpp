#include <DArray.hpp>

void PrintDArray(DArray<UplinkObject*>* array)
{
	UplinkAssert(array);

	for (auto i = 0; i < array->Size(); i++)
	{
		printf("Index = %d\n", i);

		if (!array->ValidIndex(i))
		{
			puts("Not a Valid Index");
			continue;
		}

		const auto item = array->GetData(i);

		if (!item)
		{
			puts("NULL");
			continue;
		}

		item->Print();
	}
}
