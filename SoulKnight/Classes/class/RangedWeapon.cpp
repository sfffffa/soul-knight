#include "RangedWeapon.h"

inline bool RangedWeapon::initMember(std::shared_ptr<Damage> bullet) {
	if (!bullet) {
		return false;
	}

	_bullet = bullet;
	return true;
}

bool RangedWeapon::init(
	int MPconsume, float aspd, float critRate, float critMultiple, std::shared_ptr<Damage> bullet) {
	if (!Weapon::init(MPconsume, aspd, critRate, critMultiple) || !initMember(bullet)) {
		return false;
	}
	return true;
}

std::shared_ptr<RangedWeapon> RangedWeapon::create(
	int MPconsume, float aspd, float critRate, float critMultiple, std::shared_ptr<Damage> bullet) {
	RangedWeapon *temp = new(std::nothrow) RangedWeapon();

	if (temp && temp->init(MPconsume, aspd, critRate, critMultiple, bullet)) {
		return std::shared_ptr<RangedWeapon>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<RangedWeapon>(nullptr);
	}
}

bool RangedWeapon::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int MPconsume, float aspd, float critRate, float critMultiple, std::shared_ptr<Damage> bullet) {
	if (!Weapon::initWithSpriteFrame(spriteFrame, MPconsume, aspd, critRate, critMultiple) || !initMember(bullet)) {
		return false;
	}
	return true;
}

std::shared_ptr<RangedWeapon> RangedWeapon::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int MPconsume, float aspd, float critRate, float critMultiple, std::shared_ptr<Damage> bullet) {
	RangedWeapon *temp = new(std::nothrow) RangedWeapon();

	if (temp && temp->initWithSpriteFrame(spriteFrame, MPconsume, aspd, critRate, critMultiple, bullet)) {
		return std::shared_ptr<RangedWeapon>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<RangedWeapon>(nullptr);
	}
}

bool RangedWeapon::initWithSpriteFrameName(const std::string& spriteFrameName,
	int MPconsume, float aspd, float critRate, float critMultiple, std::shared_ptr<Damage> bullet) {
	if (!Weapon::initWithSpriteFrameName(spriteFrameName, MPconsume, aspd, critRate, critMultiple) || !initMember(bullet)) {
		return false;
	}
	return true;
}

std::shared_ptr<RangedWeapon> RangedWeapon::createWithSpriteFrameName(const std::string& spriteFrameName,
	int MPconsume, float aspd, float critRate, float critMultiple, std::shared_ptr<Damage> bullet) {
	RangedWeapon *temp = new(std::nothrow) RangedWeapon();

	if (temp && temp->initWithSpriteFrameName(spriteFrameName, MPconsume, aspd, critRate, critMultiple, bullet)) {
		return std::shared_ptr<RangedWeapon>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<RangedWeapon>(nullptr);
	}
}

RangedWeapon *RangedWeapon::clone()const {
	RangedWeapon *temp = new(std::nothrow) RangedWeapon();

	if (temp && temp->initWithSpriteFrame(
		this->getSpriteFrame(), _MPconsume->getValue(), _aspd->getValue(),
		_critRate->getValue(), _critMultiple->getValue(),
		std::shared_ptr<Damage>(_bullet->clone()))) {
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}