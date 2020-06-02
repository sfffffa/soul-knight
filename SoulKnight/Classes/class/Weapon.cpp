#include "weapon.h"

inline bool Weapon::initMember(
	int MPconsume, float aspd, float critRate, float critMultiple) {
	auto mpcTemp = Attribute<int>::createWithValue(MPconsume);
	if (!mpcTemp) {
		return false;
	}
	else {
		_MPconsume = mpcTemp;
	}

	auto aspdTemp = Attribute<float>::createWithValue(aspd);
	if (!mpcTemp) {
		return false;
	}
	else {
		_aspd = aspdTemp;
	}

	auto crTemp = Attribute<float>::createWithValue(critRate);
	if (!crTemp) {
		return false;
	}
	else {
		_critRate = crTemp;
	}

	auto cmTemp = Attribute<float>::createWithValue(critMultiple);
	if (!cmTemp) {
		return false;
	}
	else {
		_critMultiple = cmTemp;
	}

	return true;
}

bool Weapon::init(
	int MPconsume, float aspd, float critRate, float critMultiple) {
	if (!Sprite::init() || !initMember(MPconsume, aspd, critRate, critMultiple)) {
		return false;
	}

	return true;
}

bool Weapon::initWithSpriteFrameName(const std::string& spriteFrameName,
	int MPconsume, float aspd, float critRate, float critMultiple) {
	if (!Sprite::initWithSpriteFrameName(spriteFrameName) || !initMember(MPconsume, aspd, critRate, critMultiple)) {
		return false;
	}

	return true;
}

bool Weapon::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int MPconsume, float aspd, float critRate, float critMultiple) {
	if (!Sprite::initWithSpriteFrame(spriteFrame) || !initMember(MPconsume, aspd, critRate, critMultiple)) {
		return false;
	}

	return true;
}