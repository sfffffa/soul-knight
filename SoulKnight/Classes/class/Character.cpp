#include "Character.h"

inline bool Character::initMember(
	int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
	float speed, std::shared_ptr<Weapon> weapon) {
	if (!weapon) {
		return false;
	}
	else {
		_weapon = weapon;
	}

	auto HPTemp = LimitedAttribute<int>::createWithValueMax(HP, HPMax);
	if (!HPTemp) {
		return false;
	}
	else {
		_HP = HPTemp;
	}

	auto shieldTemp = LimitedAttribute<int>::createWithValueMax(shield, shieldMax);
	if (!shieldMax) {
		return false;
	}
	else {
		_shield = shieldTemp;
	}

	auto MPTemp = LimitedAttribute<int>::createWithValueMax(MP, MPMax);
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
	int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
	float speed, std::shared_ptr<Weapon> weapon) {
	if (!Sprite::init() ||
		!initMember(HP, HPMax, shield, shieldMax, MP, MPMax, speed, weapon)) {
		return false;
	}

	return true;
}

bool Character::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
	float speed, std::shared_ptr<Weapon> weapon) {
	if (!Sprite::initWithSpriteFrame(spriteFrame) ||
		!initMember(HP, HPMax, shield, shieldMax, MP, MPMax, speed, weapon)) {
		return false;
	}

	return true;
}

bool Character::initWithSpriteFrameName(const std::string& spriteFrameName,
	int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
	float speed, std::shared_ptr<Weapon> weapon) {
	if (!Sprite::initWithSpriteFrameName(spriteFrameName) ||
		!initMember(HP, HPMax, shield, shieldMax, MP, MPMax, speed, weapon)) {
		return false;
	}

	return true;
}