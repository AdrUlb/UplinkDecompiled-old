#include <TempDefines.hpp>
#include <UplinkObject.hpp>

UplinkObject::~UplinkObject() {}
bool UplinkObject::Load()
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
