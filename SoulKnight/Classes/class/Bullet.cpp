#include "Bullet.h"

inline bool Bullet::initMember(float speed) {
	auto spe = Attribute<float>::createWithValue(speed);
	if (!spe) {
		return false;
	}
	else {
		_speed = spe;
	}

	return true;
}

bool Bullet::init(
	int damage, float speed, bool crit) {
	if (!Damage::init(damage, crit) || !initMember(speed)) {
		return false;
	}

	return true;
}

Bullet *Bullet::create(
	int damage, float speed, bool crit) {
	auto temp = new(std::nothrow) Bullet();

	if (temp && temp->init(damage, speed, crit)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool Bullet::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int damage, float speed, bool crit) {
	if (!Damage::initWithSpriteFrame(spriteFrame, damage, crit) || !initMember(speed)) {
		return false;
	}

	return true;
}

Bullet *Bullet::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int damage, float speed, bool crit) {
	auto temp = new(std::nothrow) Bullet();

	if (temp && temp->initWithSpriteFrame(spriteFrame, damage, speed, crit)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool Bullet::initWithSpriteFrameName(const std::string &spriteFrameName,
	int damage, float speed, bool crit) {
	if (!Damage::initWithSpriteFrameName(spriteFrameName, damage, crit) || !initMember(speed)) {
		return false;
	}

	return true;
}

Bullet *Bullet::createWithSpriteFrameName(const std::string &spriteFrameName,
	int damage, float speed, bool crit) {
	auto temp = new(std::nothrow) Bullet();

	if (temp && temp->initWithSpriteFrameName(spriteFrameName, damage, speed, crit)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

Bullet *Bullet::clone()const {
	Bullet *temp = new(std::nothrow) Bullet();

	if (temp && temp->initWithSpriteFrame(this->getSpriteFrame(),
		_damage->getValue(), _speed->getValue(), _crit)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}