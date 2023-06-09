#pragma once
#include "ItemInfo.h"

class ItemInfo;
class Item
{
public:
	Item();
	~Item();

	Item(const Item& _Other) = delete;
	Item(Item&& _Other) noexcept = delete;
	Item& operator=(const Item& _Other) = delete;
	Item& operator=(Item&& _Other) noexcept = delete;

	inline ItemInfo* GetInfo()
	{
		return &MyInfo_;
	}

	virtual void Use() {}

protected:

private:
	ItemInfo MyInfo_;
};

