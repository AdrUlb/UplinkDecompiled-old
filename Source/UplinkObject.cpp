#include <TempDefines.hpp>
#include <UplinkObject.hpp>

UplinkObject::~UplinkObject() {}

bool UplinkObject::Load(FILE* file)
{
	(void)file;
	return true;
}

void UplinkObject::Save(FILE* file)
{
	(void)file;
}

void UplinkObject::Print() {}
void UplinkObject::Update() {}

const char* UplinkObject::GetID()
{
	return "UOBJ";
}

UplinkObjectId UplinkObject::GetOBJECTID()
{
	return UplinkObjectId::Unassigned;
}

void UplinkObject::LoadID(FILE* file)
{
	(void)file;
}

void UplinkObject::LoadID_END(FILE* file)
{
	(void)file;
}

void UplinkObject::SaveID(FILE* file)
{
	(void)file;
}

void UplinkObject::SaveID_END(FILE* file)
{
	(void)file;
}
