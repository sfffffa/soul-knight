#include "ElementalBullet.h"

inline bool ElementalBullet::initMember(Element element) {
	auto ele = std::make_shared<Element>(element);
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

std::shared_ptr<ElementalBullet> ElementalBullet::create(
	int damage, float speed, bool crit, Element element) {
	auto temp = std::make_shared<ElementalBullet>();

	if (temp && temp->init(damage, speed, crit, element)) {
		return temp;
	}
	else {
		return std::shared_ptr<ElementalBullet>(nullptr);
	}
}

bool ElementalBullet::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int damage, float speed, bool crit, Element element) {
	if (!Bullet::initWithSpriteFrame(spriteFrame, damage, speed, crit) || !initMember(element)) {
		return false;
	}

	return true;
}

std::shared_ptr<ElementalBullet> ElementalBullet::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int damage, float speed, bool crit, Element element) {
	auto temp = std::make_shared<ElementalBullet>();

	if (temp && temp->initWithSpriteFrame(spriteFrame, damage, speed, crit, element)) {
		return temp;
	}
	else {
		return std::shared_ptr<ElementalBullet>(nullptr);
	}
}

bool ElementalBullet::initWithSpriteFrameName(const std::string &spriteFrameName,
	int damage, float speed, bool crit, Element element) {
	if (!Bullet::initWithSpriteFrameName(spriteFrameName, damage, speed, crit) || !initMember(element)) {
		return false;
	}

	return true;
}

std::shared_ptr<ElementalBullet> ElementalBullet::createWithSpriteFrameName(const std::string &spriteFrameName,
	int damage, float speed, bool crit, Element element) {
	auto temp = std::make_shared<ElementalBullet>();

	if (temp && temp->initWithSpriteFrameName(spriteFrameName, damage, speed, crit, element)) {
		return temp;
	}
	else {
		return std::shared_ptr<ElementalBullet>(nullptr);
	}
}

ElementalBullet *ElementalBullet::clone(bool crit)const {
	ElementalBullet *temp = new(std::nothrow) ElementalBullet();

	if (temp) {
		temp->setSpriteFrame(this->getSpriteFrame());
		temp->_damage = _damage;
		temp->_speed = _speed;
		temp->_crit = crit;
		temp->_element = _element;
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}