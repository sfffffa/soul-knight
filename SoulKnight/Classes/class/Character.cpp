#include "Character.h"

inline bool Character::initMember(
	int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon) {
	if (!weapon) {
		return false;
	}
	else {
		_weapon = weapon;
	}

	auto HPTemp = std::make_shared<int>(HPMax);
	if (!HPTemp) {
		return false;
	}
	else {
		_HPMAX = HPTemp;
	}

	auto MPTemp = std::make_shared<int>(MPMax);
	if (!MPTemp) {
		return false;
	}
	else {
		_MPMAX = MPTemp;
	}

	auto speedTemp = std::make_shared<float>(speed);
	if (!speedTemp) {
		return false;
	}
	else {
		_speed = speedTemp;
	}

	_HP = HPMax;
	_MP = MPMax;

	return true;
}

bool Character::init(
	int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon) {
	if (!Sprite::init() ||
		!initMember(HPMax, MPMax, speed, weapon)) {
		return false;
	}

	return true;
}

bool Character::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon) {
	if (!Sprite::initWithSpriteFrame(spriteFrame) ||
		!initMember(HPMax, MPMax, speed, weapon)) {
		return false;
	}

	return true;
}

bool Character::initWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon) {
	if (!Sprite::initWithSpriteFrameName(spriteFrameName) ||
		!initMember(HPMax, MPMax, speed, weapon)) {
		return false;
	}

	return true;
}