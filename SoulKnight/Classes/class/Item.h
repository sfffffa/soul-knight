#ifndef _ITEM_
#define _ITEM_

#include "Attribute.h"
#include "cocos2d.h"

USING_NS_CC;

class Item :public Sprite {
public:
	static std::shared_ptr<Item> create(int value = 0);

	static std::shared_ptr<Item> createWithSpriteFrame(SpriteFrame *spriteFrame, int value = 0);

	static std::shared_ptr<Item> createWithSpriteFrameName(const std::string& spriteFrameName, int value = 0);

	virtual void setValue(int value) { _value->setValue(value); }

	virtual int getValue()const { return _value->getValue(); }

	virtual std::shared_ptr<Attribute<int>> getValueInstance()const { return _value; }

	//clone 共享同一 value
	virtual Item *clone()const;
protected:
	//destructor
	virtual ~Item() = default;

	bool init(int value);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame, int value);

	bool initWithSpriteFrameName(const std::string& spriteFrameName, int value);

	std::shared_ptr<Attribute<int>> _value;//恢复值

private:
	bool initMember(int value);
};

#endif // _ITEM_
