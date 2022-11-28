#pragma once
#include <stdint.h>
#include <typeinfo>

using unit_id = uint16_t;
using asset_flag = int64_t;

enum class Mino_Type
{
	I,O,T,L,J,S,Z
};

struct Player_Region
{
	int x, y;
	int size;
	int8_t grid_w, grid_h;
	static inline int GetTypeFlag()
	{
		return 1 << 0;
	}
};
struct Block
{
	int8_t x, y;
	static inline int GetTypeFlag()
	{
		return 1 << 2;
	}
};
struct Mino 
{
	Block block[4];
	Mino_Type type;
	static inline int GetTypeFlag()
	{
		return 1 << 3;
	}
};
class Unit
{
	unit_id id;
	asset_flag components;
public:
	Unit(unit_id uid):id(uid){}
	template<class T>
	T* GetComponent();
	inline unit_id GetId()
	{
		return id;
	}
};

template<class T>
class Some_Block
{
	size_t type_id;
	int block_size;
	int some_size;
	T* block;
public:
	Some_Block() :type_id(typeid(T).hash_code()), block_size(50), some_size(0)
	{
		block = new T[block_size];
	}
	inline T* Get(int index)
	{
		return index < some_size ? &block[index] : nullptr;
	}
	inline void Del(int index)
	{
		if (index < some_size) 
		{
			block[index] = block[some_size - 1];
			some_size--;
		}
	}

	inline void Add()
	{
		if (some_size !< block_size)
			Extension();
		some_size++;
	}

	inline void Add(T t)
	{
		if (some_size !< block_size)
			Extension();
		block[some_size] = t;
		some_size++;
	}

	bool Has(T t)
	{
		for (int i = 0; i < some_size; i++)
		{
			if (block[i] == t) return true;
		}
		return false;
	}

	bool Has(T* t)
	{
		for (int i = 0; i < some_size; i++)
		{
			if (&block[i] == t) return true;
		}
		return false;
	}

	int IndexOf(T t)
	{
		for (int i = 0; i < some_size; i++)
		{
			if (block[i] == t) return i;
		}
		return -1;
	}

	int IndexOf(T* t)
	{
		for (int i = 0; i < some_size; i++)
		{
			if (&block[i] == t) return i;
		}
		return -1;
	}

	void Extension()
	{
		auto temp = new T[block_size + 50];
		for (int i = 0; i < some_size; i++)
			temp[i] = block[i];
		delete[] block;
		block = temp;
		block_size += 50;
	}
};

template<class T>
struct Chain
{
	T block;
	Chain* next;

	inline bool HasNext() const
	{
		return next != nullptr;
	}Archtype
};

struct Compoent_Block
{
	size_t type;
	Some_Block<void> some_block;
};

class Archtype
{
	asset_flag type_flag = 0;
	Chain<Compoent_Block>* components;
	Some_Block<unit_id> unit_id_block;
public:
	bool HasUnit(unit_id id);
	template<class T>
	T* GetComponents(unit_id id)
	{
		size_t type = typeid(T).hash_code();
		int i;
		if ((i = unit_id_block.IndexOf(id)) >= 0) 
		{
			auto p = components;
			while (p != nullptr)
			{
				if (p->block.type == type)
					return (T*)p->block.some_block.Get(i);
				p = p->next;
			}
		}
		return nullptr_t
	}
	void AddUnit(Unit& unit);
	void DelUnit(Unit& unit);
};

class Some_House
{
	Chain<Archtype>* archtypes;
public:
	Some_House();

	template<class T>
	T* GetComponent(unit_id id)
	{
		auto p = archtypes;
		while (p != nullptr)
		{
			if (p->block.HasUnit(id))
				return p->block.GetComponents();
			p = p->next;
		}
		return nullptr;
	}
	bool HasUnit(unit_id id);
};
