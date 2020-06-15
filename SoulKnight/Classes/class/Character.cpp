#include "Character.h"

inline bool Character::initMember(
	int HPMax, int MPMax, float speed, Weapon *weapon) {
	if (!weapon) {
		return false;
	}
	else {
		_weapon = weapon;
	}

	auto HPTemp = LimitedAttribute<int>::createWithValue(HPMax);
	if (!HPTemp) {
		return false;
	}
	else {
		_HP = HPTemp;
	}

	auto MPTemp = LimitedAttribute<int>::createWithValue(MPMax);
	if (!MPTemp) {
		return false;
	}
	else {
		_MP = MPTemp;
	}

	auto speedTemp = Attribute<float>::createWithValue(speed);
	if (!speedTemp) {
		return false;
	}
	else {
		_speed = speedTemp;
	}

	return true;
}

bool Character::init(
	int HPMax, int MPMax, float speed, Weapon *weapon) {
	if (!Sprite::init() ||
		!initMember(HPMax, MPMax, speed, weapon)) {
		return false;
	}

	return true;
}

bool Character::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int MPMax, float speed, Weapon *weapon) {
	if (!Sprite::initWithSpriteFrame(spriteFrame) ||
		!initMember(HPMax, MPMax, speed, weapon)) {
		return false;
	}

	return true;
}

bool Character::initWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int MPMax, float speed, Weapon *weapon) {
	if (!Sprite::initWithSpriteFrameName(spriteFrameName) ||
		!initMember(HPMax, MPMax, speed, weapon)) {
		return false;
	}

	return true;
}