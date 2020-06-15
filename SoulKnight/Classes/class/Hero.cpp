#include "Hero.h"

inline bool Hero::initMember(
	int shieldMax, Weapon *offhandWeapon,
	float coolDown, std::function<void(void)> skill) {
	if (!offhandWeapon) {
		return false;
	}
	else {
		_offhandWeapon = offhandWeapon;
	}

	auto shi = LimitedAttribute<int>::createWithValue(shieldMax);
	if (!shi) {
		return false;
	}
	else {
		_shield = shi;
	}

	auto cd = Attribute<float>::createWithValue(coolDown);
	if (!cd) {
		return false;
	}
	else {
		_coolDown = cd;
	}

	_skill = skill;
	return true;
}

bool Hero::init(
	int HPMax, int shieldMax, int MPMax, float speed,
	Weapon *weapon, Weapon *offhandWeapon,
	std::function<void(void)> skill, float cd) {
	if (!Character::init(HPMax, MPMax, speed, weapon) ||
		!initMember(shieldMax, offhandWeapon, cd, skill)) {
		return false;
	}
	return true;
}

Hero *Hero::create(
	int HPMax, int shieldMax, int MPMax, float speed,
	Weapon *weapon, Weapon *offhandWeapon,
	std::function<void(void)> skill, float cd) {
	auto temp = new(std::nothrow) Hero();
	if (temp &&
		temp->init(HPMax, shieldMax, MPMax, speed, weapon, offhandWeapon, skill, cd)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool Hero::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int shieldMax, int MPMax, float speed,
	Weapon *weapon, Weapon *offhandWeapon,
	std::function<void(void)> skill, float cd) {
	if (!Character::initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon) ||
		!initMember(shieldMax, offhandWeapon, cd, skill)) {
		return false;
	}
	return true;
}

Hero *Hero::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int shieldMax, int MPMax, float speed,
	Weapon *weapon, Weapon *offhandWeapon,
	std::function<void(void)> skill, float cd) {
	auto temp = new(std::nothrow) Hero();
	if (temp &&
		temp->initWithSpriteFrame(spriteFrame, HPMax, shieldMax, MPMax, speed, weapon, offhandWeapon, skill, cd)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool Hero::initWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int shieldMax, int MPMax, float speed,
	Weapon *weapon, Weapon *offhandWeapon,
	std::function<void(void)> skill, float cd) {
	if (!Character::initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon) ||
		!initMember(shieldMax, offhandWeapon, cd, skill)) {
		return false;
	}
	return true;
}

Hero *Hero::createWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int shieldMax, int MPMax, float speed,
	Weapon *weapon, Weapon *offhandWeapon,
	std::function<void(void)> skill, float cd) {
	auto temp = new(std::nothrow) Hero();
	if (temp &&
		temp->initWithSpriteFrameName(spriteFrameName, HPMax, shieldMax, MPMax, speed, weapon, offhandWeapon, skill, cd)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

Hero *Hero::clone()const {
	return nullptr;
}