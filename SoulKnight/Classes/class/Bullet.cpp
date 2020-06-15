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

std::shared_ptr<Bullet> Bullet::create(
	int damage, float speed, bool crit) {
	auto temp = std::shared_ptr<Bullet>();

	if (temp && temp->init(damage, speed, crit)) {
		return temp;
	}
	else {
		return std::shared_ptr<Bullet>(nullptr);
	}
}

bool Bullet::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int damage, float speed, bool crit) {
	if (!Damage::initWithSpriteFrame(spriteFrame, damage, crit) || !initMember(speed)) {
		return false;
	}

	return true;
}

std::shared_ptr<Bullet> Bullet::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int damage, float speed, bool crit) {
	auto temp = std::shared_ptr<Bullet>();

	if (temp && temp->initWithSpriteFrame(spriteFrame, damage, speed, crit)) {
		return temp;
	}
	else {
		return std::shared_ptr<Bullet>(nullptr);
	}
}

bool Bullet::initWithSpriteFrameName(const std::string &spriteFrameName,
	int damage, float speed, bool crit) {
	if (!Damage::initWithSpriteFrameName(spriteFrameName, damage, crit) || !initMember(speed)) {
		return false;
	}

	return true;
}

std::shared_ptr<Bullet> Bullet::createWithSpriteFrameName(const std::string &spriteFrameName,
	int damage, float speed, bool crit) {
	auto temp = std::shared_ptr<Bullet>();

	if (temp && temp->initWithSpriteFrameName(spriteFrameName, damage, speed, crit)) {
		return temp;
	}
	else {
		return std::shared_ptr<Bullet>(nullptr);
	}
}

Bullet *Bullet::clone()const {
	Bullet *temp = new(std::nothrow) Bullet();

	if (temp && temp->initWithSpriteFrame(this->getSpriteFrame(),
		_damage->getValue(), _speed->getValue(), _crit)) {
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}