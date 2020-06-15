#include "CloseInWeapon.h"

inline bool CloseInWeapon::initMember(int damage, float rang) {
	auto ranTemp = Attribute<float>::createWithValue(rang);
	if (!ranTemp) {
		return false;
	}
	else {
		_range = ranTemp;
	}

	auto damTemp = Attribute<int>::createWithValue(damage);
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

CloseInWeapon *CloseInWeapon::create(
	int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple) {
	auto temp = new(std::nothrow) CloseInWeapon();

	if (temp && temp->init(MPconsume, damage, rang, aspd, critRate, critMultiple)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
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

CloseInWeapon *CloseInWeapon::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple) {
	auto temp = new(std::nothrow) CloseInWeapon();

	if (temp && temp->initWithSpriteFrame(spriteFrame, MPconsume, damage, rang, aspd, critRate, critMultiple)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
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

CloseInWeapon *CloseInWeapon::createWithSpriteFrameName(const std::string &spriteFrameName,
	int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple) {
	auto temp = new(std::nothrow) CloseInWeapon();

	if (temp && temp->initWithSpriteFrameName(spriteFrameName, MPconsume, damage, rang, aspd, critRate, critMultiple)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

CloseInWeapon *CloseInWeapon::clone()const {
	CloseInWeapon *temp = new(std::nothrow) CloseInWeapon();

	if (temp && temp->initWithSpriteFrame(this->getSpriteFrame(),
		_MPconsume->getValue(), _damage->getValue(), _range->getValue(), _aspd->getValue(), _critRate->getValue(), _critMultiple->getValue())) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

/*void CloseInWeapon::use() {
	//create
}*/