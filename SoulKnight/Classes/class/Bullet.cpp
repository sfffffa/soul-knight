#include "Bullet.h"

inline bool Bullet::initMember(float speed) {
	auto spe = std::make_shared<float>(speed);
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
	auto temp = std::make_shared<Bullet>();

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
	auto temp = std::make_shared<Bullet>();

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
	auto temp = std::make_shared<Bullet>();

	if (temp && temp->initWithSpriteFrameName(spriteFrameName, damage, speed, crit)) {
		return temp;
	}
	else {
		return std::shared_ptr<Bullet>(nullptr);
	}
}

Bullet *Bullet::clone(bool crit)const {
	Bullet *temp = new(std::nothrow) Bullet();

	if (temp) {
		temp->setSpriteFrame(this->getSpriteFrame());
		temp->_damage = _damage;
		temp->_speed = _speed;
		temp->_crit = crit;
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}