#include "Item.h"

inline bool Item::initMember(int value) {
	auto temp = Attribute<int>::createWithValue(value);
	if (!temp) {
		return false;
	}
	else {
		_value = temp;
	}
	return true;
}

bool Item::init(int value) {
	if (!Sprite::init() || !initMember(value)) {
		return false;
	}
	return true;
}

std::shared_ptr<Item> Item::create(int value = 0) {
	Item *temp = new(std::nothrow) Item();
	if (temp && temp->init(value)) {
		return std::shared_ptr<Item>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<Item>(nullptr);
	}
}

bool Item::initWithSpriteFrame(SpriteFrame *spriteFrame, int value) {
	if (!Sprite::initWithSpriteFrame(spriteFrame) || !initMember(value)) {
		return false;
	}
	return true;
}

std::shared_ptr<Item> Item::createWithSpriteFrame(SpriteFrame *spriteFrame, int value = 0) {
	Item *temp = new(std::nothrow) Item();
	if (temp && temp->initWithSpriteFrame(spriteFrame, value)) {
		return std::shared_ptr<Item>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<Item>(nullptr);
	}
}

bool Item::initWithSpriteFrameName(const std::string& spriteFrameName, int value) {
	if (!Sprite::initWithSpriteFrameName(spriteFrameName) || !initMember(value)) {
		return false;
	}
	return true;
}

std::shared_ptr<Item> Item::createWithSpriteFrameName(const std::string& spriteFrameName, int value = 0) {
	Item *temp = new(std::nothrow) Item();
	if (temp && temp->initWithSpriteFrameName(spriteFrameName, value)) {
		return std::shared_ptr<Item>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<Item>(nullptr);
	}
}

Item *Item::clone()const {
	Item *temp = new(std::nothrow) Item();
	if (temp) {
		temp->setSpriteFrame(this->getSpriteFrame());
		temp->_value = _value;
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}