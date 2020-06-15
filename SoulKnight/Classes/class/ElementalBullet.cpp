#include "ElementalBullet.h"

inline bool ElementalBullet::initMember(Element element) {
	auto ele = Attribute<Element>::createWithValue(element);
	if (!ele) {
		return false;
	}
	else {
		_element = ele;
	}

	return true;
}

bool ElementalBullet::init(
	int damage, float speed, bool crit, Element element) {
	if (!Bullet::init(damage, speed, crit) || !initMember(element)) {
		return false;
	}

	return true;
}

ElementalBullet *ElementalBullet::create(
	int damage, float speed, bool crit, Element element) {
	auto temp = new(std::nothrow) ElementalBullet();

	if (temp && temp->init(damage, speed, crit, element)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool ElementalBullet::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int damage, float speed, bool crit, Element element) {
	if (!Bullet::initWithSpriteFrame(spriteFrame, damage, speed, crit) || !initMember(element)) {
		return false;
	}

	return true;
}

ElementalBullet *ElementalBullet::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int damage, float speed, bool crit, Element element) {
	auto temp = new(std::nothrow) ElementalBullet();

	if (temp && temp->initWithSpriteFrame(spriteFrame, damage, speed, crit, element)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool ElementalBullet::initWithSpriteFrameName(const std::string &spriteFrameName,
	int damage, float speed, bool crit, Element element) {
	if (!Bullet::initWithSpriteFrameName(spriteFrameName, damage, speed, crit) || !initMember(element)) {
		return false;
	}

	return true;
}

ElementalBullet *ElementalBullet::createWithSpriteFrameName(const std::string &spriteFrameName,
	int damage, float speed, bool crit, Element element) {
	auto temp = new(std::nothrow) ElementalBullet();

	if (temp && temp->initWithSpriteFrameName(spriteFrameName, damage, speed, crit, element)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

ElementalBullet *ElementalBullet::clone()const {
	ElementalBullet *temp = new(std::nothrow) ElementalBullet();

	if (temp && temp->initWithSpriteFrame(
		this->getSpriteFrame(), _damage->getValue(), _speed->getValue(), _crit, _element->getValue())) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}