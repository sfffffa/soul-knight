#include "Damage.h"

inline bool Damage::initMember(int damage, bool crit) {
	auto dam = Attribute<int>::createWithValue(damage);
	if (!dam) {
		return false;
	}
	else {
		_damage = dam;
	}

	_crit = crit;
	return true;
}

bool Damage::init(int damage, bool crit) {
	if (!Sprite::init() || !initMember(damage, crit)) {
		return false;
	}

	return true;
}

bool Damage::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int damage, bool crit) {
	if (!Sprite::initWithSpriteFrame(spriteFrame) || !initMember(damage, crit)) {
		return false;
	}

	return true;
}

bool Damage::initWithSpriteFrameName(const std::string &spriteFrameName,
	int damage, bool crit) {
	if (!Sprite::initWithSpriteFrameName(spriteFrameName) || !initMember(damage, crit)) {
		return false;
	}

	return true;
}