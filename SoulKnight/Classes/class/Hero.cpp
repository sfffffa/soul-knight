#include "Hero.h"

inline bool Hero::initMember(
	int shieldMax, std::shared_ptr<Weapon> offhandWeapon,
	float coolDown, std::function<void(void)> skill) {
	if (!offhandWeapon) {
		return false;
	}
	else {
		_offhandWeapon = offhandWeapon;
	}

	_shieldMAX = shieldMax;
	_shield = shieldMax;
	_coolDown = coolDown;
	_skill = skill;
	return true;
}

bool Hero::init(
	int HPMax, int shieldMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
	std::function<void(void)> skill, float cd) {
	if (!Character::init(HPMax, MPMax, speed, weapon) ||
		!initMember(shieldMax, offhandWeapon, cd, skill)) {
		return false;
	}
	return true;
}

std::shared_ptr<Hero> Hero::create(
	int HPMax, int shieldMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
	std::function<void(void)> skill, float cd) {
	auto temp = std::make_shared<Hero>();
	if (temp &&
		temp->init(HPMax, shieldMax, MPMax, speed, weapon, offhandWeapon, skill, cd)) {
		return temp;
	}
	else {
		return std::shared_ptr<Hero>(nullptr);
	}
}

bool Hero::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int shieldMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
	std::function<void(void)> skill, float cd) {
	if (!Character::initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon) ||
		!initMember(shieldMax, offhandWeapon, cd, skill)) {
		return false;
	}
	return true;
}

std::shared_ptr<Hero> Hero::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int shieldMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
	std::function<void(void)> skill, float cd) {
	auto temp = std::make_shared<Hero>();
	if (temp &&
		temp->initWithSpriteFrame(spriteFrame, HPMax, shieldMax, MPMax, speed, weapon, offhandWeapon, skill, cd)) {
		return temp;
	}
	else {
		return std::shared_ptr<Hero>(nullptr);
	}
}

bool Hero::initWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int shieldMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
	std::function<void(void)> skill, float cd) {
	if (!Character::initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon) ||
		!initMember(shieldMax, offhandWeapon, cd, skill)) {
		return false;
	}
	return true;
}

std::shared_ptr<Hero> Hero::createWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int shieldMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
	std::function<void(void)> skill, float cd) {
	auto temp = std::make_shared<Hero>();
	if (temp &&
		temp->initWithSpriteFrameName(spriteFrameName, HPMax, shieldMax, MPMax, speed, weapon, offhandWeapon, skill, cd)) {
		return temp;
	}
	else {
		return std::shared_ptr<Hero>(nullptr);
	}
}

Hero *Hero::clone()const {
	return nullptr;
}

bool Hero::shoot() {
	if (_MP < ((_weaponStatus) ? _offhandWeapon->getMPCosume() : _weapon->getMPCosume())) {
		return false;
	}
	_MP -= ((_weaponStatus) ? _offhandWeapon->getMPCosume() : _weapon->getMPCosume());
}

bool Hero::beShot(int damage) {
	_shield -= damage;
	if (_shield < 0) {
		_HP += _shield;
		_shield = 0;
		if (_HP <= 0) {
			_HP = 0;
			return true;
		}
	}
	return false;
}

bool Hero::changeWeapon() {
	if (!((_weaponStatus) ? _weapon->getAspd() : _offhandWeapon->getAspd())) {
		return false;
	}
	_weaponStatus ^= 01;
	return true;
}