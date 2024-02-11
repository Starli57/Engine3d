#pragma once
class UniqueId
{
public:
	static uint32_t GetId();

private:
	static uint32_t buffer;
};

