#include "Item.h"

inline bool Item::initMember(Type type, int value) {
	auto temp = Attribute<int>::createWithValue(value);
	if (!temp) {
		return false;
	}
	else {
		_value = temp;
	}

	_type = type;
	return true;
}

bool Item::init(Type type, int value) {
	if (!Sprite::init() || !initMember(type, value)) {
		return false;
	}
	return true;
}

Item *Item::create(Type type, int value) {
	Item *temp = new(std::nothrow) Item();
	if (temp && temp->init(type, value)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool Item::initWithSpriteFrame(SpriteFrame *spriteFrame, Type type, int value) {
	if (!Sprite::initWithSpriteFrame(spriteFrame) || !initMember(type, value)) {
		return false;
	}
	return true;
}

Item *Item::createWithSpriteFrame(SpriteFrame *spriteFrame, Type type, int value) {
	Item *temp = new(std::nothrow) Item();
	if (temp && temp->initWithSpriteFrame(spriteFrame, type, value)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool Item::initWithSpriteFrameName(const std::string& spriteFrameName, Type type, int value) {
	if (!Sprite::initWithSpriteFrameName(spriteFrameName) || !initMember(type, value)) {
		return false;
	}
	return true;
}

Item *Item::createWithSpriteFrameName(const std::string& spriteFrameName, Type type, int value) {
	Item *temp = new(std::nothrow) Item();
	if (temp && temp->initWithSpriteFrameName(spriteFrameName, type, value)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

//所有复制的道具共享属性
Item *Item::clone()const {
	Item *temp = new(std::nothrow) Item();
	if (temp) {
		temp->setSpriteFrame(this->getSpriteFrame());
		temp->_type = _type;
		temp->_value = _value;
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}