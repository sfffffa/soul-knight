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

std::shared_ptr<CloseInWeapon> CloseInWeapon::create(
	int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f, float critRate = 0.0f, float critMultiple = 0.0f) {
	CloseInWeapon *temp = new(std::nothrow) CloseInWeapon();

	if (temp && temp->init(MPconsume, damage, rang, aspd, critRate, critMultiple)) {
		return std::shared_ptr<CloseInWeapon>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<CloseInWeapon>(nullptr);
	}
}

bool CloseInWeapon::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple) {
	if (!Weapon::initWithSpriteFrame(spriteFrame, MPconsume, aspd, critRate, critMultiple) || !initMember(damage, rang)) {
		return false;
	}

	return true;
}

std::shared_ptr<CloseInWeapon> CloseInWeapon::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f, float critRate = 0.0f, float critMultiple = 0.0f) {
	CloseInWeapon *temp = new(std::nothrow) CloseInWeapon();

	if (temp && temp->initWithSpriteFrame(spriteFrame, MPconsume, damage, rang, aspd, critRate, critMultiple)) {
		return std::shared_ptr<CloseInWeapon>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<CloseInWeapon>(nullptr);
	}
}

bool CloseInWeapon::initWithSpriteFrameName(const std::string &spriteFrameName,
	int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple) {
	if (!Weapon::initWithSpriteFrameName(spriteFrameName, MPconsume, aspd, critRate, critMultiple) || !initMember(damage, rang)) {
		return false;
	}

	return true;
}

std::shared_ptr<CloseInWeapon> CloseInWeapon::createWithSpriteFrameName(const std::string &spriteFrameName,
	int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f, float critRate = 0.0f, float critMultiple = 0.0f) {
	CloseInWeapon *temp = new(std::nothrow) CloseInWeapon();

	if (temp && temp->initWithSpriteFrameName(spriteFrameName, MPconsume, damage, rang, aspd, critRate, critMultiple)) {
		return std::shared_ptr<CloseInWeapon>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<CloseInWeapon>(nullptr);
	}
}

CloseInWeapon *CloseInWeapon::clone()const {
	CloseInWeapon *temp = new(std::nothrow) CloseInWeapon();

	if (temp && temp->initWithSpriteFrame(this->getSpriteFrame(),
		_MPconsume->getValue(), _damage->getValue(), _range->getValue(), _aspd->getValue(), _critRate->getValue(), _critMultiple->getValue())) {
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

void CloseInWeapon::use() {
	//create
}