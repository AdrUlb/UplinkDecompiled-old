#include <TempDefines.hpp>
#include <UplinkObject.hpp>

UplinkObject::~UplinkObject() {}

bool UplinkObject::Load(FILE* file)
{
	return true;
}

void UplinkObject::Save() {}
void UplinkObject::Print() {}
void UplinkObject::Update() {}

const char* UplinkObject::GetID()
{
	return "UOBJ";
}

size_t UplinkObject::GetOBJECTID()
{
	return 0;
}

void UplinkObject::LoadID(FILE* file) {}
void UplinkObject::LoadID_END(FILE* file) {}
