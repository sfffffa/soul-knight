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
	std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
	std::function<void(void)> skill, float cd) {
	if (!Character::init(HPMax, MPMax, speed, weapon) ||
		!initMember(shieldMax, offhandWeapon, cd, skill)) {
		return false;
	}
	return true;
}

std::shared_ptr<Hero> Hero::create(
	int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
	std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
	std::shared_ptr<Weapon> offhandWeapon = CloseInWeapon::create(),
	std::function<void(void)> skill = [] {}, float cd = 0.0f) {
	Hero *temp = new(std::nothrow) Hero();
	if (temp &&
		temp->init(HPMax, shieldMax, MPMax, speed, weapon, offhandWeapon, skill, cd)) {
		return std::shared_ptr<Hero>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
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
	int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
	std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
	std::shared_ptr<Weapon> offhandWeapon = CloseInWeapon::create(),
	std::function<void(void)> skill = [] {}, float cd = 0.0f) {
	Hero *temp = new(std::nothrow) Hero();
	if (temp &&
		temp->initWithSpriteFrame(spriteFrame, HPMax, shieldMax, MPMax, speed, weapon, offhandWeapon, skill, cd)) {
		return std::shared_ptr<Hero>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
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
	int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
	std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
	std::shared_ptr<Weapon> offhandWeapon = CloseInWeapon::create(),
	std::function<void(void)> skill = [] {}, float cd = 0.0f) {
	Hero *temp = new(std::nothrow) Hero();
	if (temp &&
		temp->initWithSpriteFrameName(spriteFrameName, HPMax, shieldMax, MPMax, speed, weapon, offhandWeapon, skill, cd)) {
		return std::shared_ptr<Hero>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<Hero>(nullptr);
	}
}