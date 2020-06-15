#include "RangedWeapon.h"

inline bool RangedWeapon::initMember(Damage *bullet) {
	if (!bullet) {
		return false;
	}

	_bullet = bullet;
	return true;
}

bool RangedWeapon::init(
	int MPconsume, float aspd, float critRate, float critMultiple, Damage *bullet) {
	if (!Weapon::init(MPconsume, aspd, critRate, critMultiple) || !initMember(bullet)) {
		return false;
	}
	return true;
}

RangedWeapon *RangedWeapon::create(
	int MPconsume, float aspd, float critRate, float critMultiple, Damage *bullet) {
	auto temp = new(std::nothrow) RangedWeapon();

	if (temp && temp->init(MPconsume, aspd, critRate, critMultiple, bullet)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool RangedWeapon::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int MPconsume, float aspd, float critRate, float critMultiple, Damage *bullet) {
	if (!Weapon::initWithSpriteFrame(spriteFrame, MPconsume, aspd, critRate, critMultiple) || !initMember(bullet)) {
		return false;
	}
	return true;
}

RangedWeapon *RangedWeapon::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int MPconsume, float aspd, float critRate, float critMultiple, Damage *bullet) {
	auto temp = new(std::nothrow) RangedWeapon();

	if (temp && temp->initWithSpriteFrame(spriteFrame, MPconsume, aspd, critRate, critMultiple, bullet)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool RangedWeapon::initWithSpriteFrameName(const std::string& spriteFrameName,
	int MPconsume, float aspd, float critRate, float critMultiple, Damage *bullet) {
	if (!Weapon::initWithSpriteFrameName(spriteFrameName, MPconsume, aspd, critRate, critMultiple) || !initMember(bullet)) {
		return false;
	}
	return true;
}

RangedWeapon *RangedWeapon::createWithSpriteFrameName(const std::string& spriteFrameName,
	int MPconsume, float aspd, float critRate, float critMultiple, Damage *bullet) {
	auto temp = new(std::nothrow) RangedWeapon();

	if (temp && temp->initWithSpriteFrameName(spriteFrameName, MPconsume, aspd, critRate, critMultiple, bullet)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

RangedWeapon *RangedWeapon::clone()const {
	RangedWeapon *temp = new(std::nothrow) RangedWeapon();

	if (temp && temp->initWithSpriteFrame(
		this->getSpriteFrame(), _MPconsume->getValue(), _aspd->getValue(),
		_critRate->getValue(), _critMultiple->getValue(),
		_bullet->clone())) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}