#ifndef _ITEM_
#define _ITEM_

#include "Attribute.h"
#include "cocos2d.h"

using namespace cocos2d;

class Item :public Sprite {
public:
	enum Type { HEALTH, MAGIC, COIN };

	static Item *create(Type type = HEALTH, int value = 0);

	static Item *createWithSpriteFrame(SpriteFrame *spriteFrame,
		Type type = HEALTH, int value = 0);

	static Item *createWithSpriteFrameName(const std::string& spriteFrameName,
		Type type = HEALTH, int value = 0);

	virtual void setType(Type type) { _type = type; }
	virtual void setValue(int value) { _value->setValue(value); }

	virtual Type getType()const { return _type; }
	virtual int getValue()const { return _value->getValue(); }

	virtual Attribute<int> *getValueInstance()const { return _value; }

	//clone 共享同一 value
	virtual Item *clone()const;
protected:
	//destructor
	virtual ~Item() = default;

	bool init(Type type, int value);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame, Type type, int value);

	bool initWithSpriteFrameName(const std::string& spriteFrameName, Type type, int value);

	Type _type;//作用对象
	Attribute<int> *_value;//恢复值

private:
	bool initMember(Type type, int value);
};

#endif // _ITEM_
