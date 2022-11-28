// RurDog.cpp: 定义应用程序的入口点。
//
#include "RurDog.h"


int main()
{
	return 0;
}

inline bool Archtype::HasUnit(unit_id id)
{
	return unit_id_block.Has(id);
}

void Archtype::AddUnit(Unit& unit)
{
	unit_id_block.Add(unit.GetId());
	auto p = components;
	while (p != nullptr)
	{
		p->block.some_block.Add();
		p = p->next;
	}
}

void Archtype::DelUnit(Unit& unit)
{
	int index = unit_id_block.IndexOf(index);
	if (index >= 0)
	{
		unit_id_block.Del(index);
		auto p = components;
		while (p != nullptr)
		{
			p->block.some_block.Del(index);
			p = p->next;
		}
	}
}

bool Some_House::HasUnit(unit_id id)
{
	auto p = archtypes;
	while (p != nullptr)
	{
		if (p->block.HasUnit(id))
			return true;
		p = p->next;
	}
	return false;
}
