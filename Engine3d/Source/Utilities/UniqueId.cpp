#include "Pch.h"
#include "UniqueId.h"

uint32_t UniqueId::buffer = 0;

uint32_t UniqueId::GetId()
{
	buffer++;
	return buffer;
}
