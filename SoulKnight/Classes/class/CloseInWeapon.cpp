#include "CloseInWeapon.h"

inline bool CloseInWeapon::initMember(int damage, float rang) {
	auto ranTemp = std::make_shared<float>(rang);
	if (!ranTemp) {
		return false;
	}
	else {
		_range = ranTemp;
	}

	auto damTemp = std::make_shared<int>(damage);
	if (!damTemp) {
		return false;
	}
	else {
		_damage = damTemp;
	}

	return true;
}

bool CloseInWeapon::init(
	int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple) {
	if (!Weapon::init(MPconsume, aspd, critRate, critMultiple) || !initMember(damage, rang)) {
		return false;
	}

	return true;
}

std::shared_ptr<CloseInWeapon> CloseInWeapon::create(
	int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple) {
	auto temp = std::make_shared<CloseInWeapon>();

	if (temp && temp->init(MPconsume, damage, rang, aspd, critRate, critMultiple)) {
		return temp;
	}
	else {
		return std::shared_ptr<CloseInWeapon>(nullptr);
	}
}

bool CloseInWeapon::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple) {
	if (!Weapon::initWithSpriteFrame(spriteFrame, MPconsume, aspd, critRate, critMultiple)
		|| !initMember(damage, rang)) {
		return false;
	}

	return true;
}

std::shared_ptr<CloseInWeapon> CloseInWeapon::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple) {
	auto temp = std::make_shared<CloseInWeapon>();

	if (temp && temp->initWithSpriteFrame(spriteFrame, MPconsume, damage, rang, aspd, critRate, critMultiple)) {
		return temp;
	}
	else {
		return std::shared_ptr<CloseInWeapon>(nullptr);
	}
}

bool CloseInWeapon::initWithSpriteFrameName(const std::string &spriteFrameName,
	int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple) {
	if (!Weapon::initWithSpriteFrameName(spriteFrameName, MPconsume, aspd, critRate, critMultiple)
		|| !initMember(damage, rang)) {
		return false;
	}

	return true;
}

std::shared_ptr<CloseInWeapon> CloseInWeapon::createWithSpriteFrameName(const std::string &spriteFrameName,
	int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple) {
	auto temp = std::make_shared<CloseInWeapon>();

	if (temp && temp->initWithSpriteFrameName(spriteFrameName, MPconsume, damage, rang, aspd, critRate, critMultiple)) {
		return temp;
	}
	else {
		return std::shared_ptr<CloseInWeapon>(nullptr);
	}
}

CloseInWeapon *CloseInWeapon::clone()const {
	CloseInWeapon *temp = new(std::nothrow) CloseInWeapon();

	if (temp) {
		temp->setSpriteFrame(this->getSpriteFrame());
		temp->_MPconsume = _MPconsume;
		temp->_damage = _damage;
		temp->_range = _range;
		temp->_aspd = _aspd;
		temp->_critRate = _critRate;
		temp->_critMultiple = _critMultiple;
		temp->_weaponName = _weaponName;
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}