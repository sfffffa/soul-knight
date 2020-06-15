#include "Boss.h"

inline bool Boss::initMember(float weaponChance) {
	auto temp = Attribute<float>::createWithValue(weaponChance);
	if (!temp) {
		return false;
	}
	else {
		_weaponChance = temp;
	}
	return true;
}

bool Boss::init(
	int HPMax, int MPMax,
	float speed, Weapon *weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance) {
	if (!Monster::init(HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance) ||
		!initMember(weapomChance)) {
		return false;
	}
	return true;
}

Boss *Boss::create(
	int HPMax, int MPMax,
	float speed, Weapon *weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance) {
	auto temp = new(std::nothrow) Boss();
	if (temp &&
		temp->init(HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance, weapomChance)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool Boss::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int MPMax, float speed,
	Weapon *weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance) {
	if (!Monster::initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance) ||
		!initMember(weapomChance)) {
		return false;
	}
	return true;
}

Boss *Boss::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int MPMax, float speed,
	Weapon *weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance) {
	auto temp = new(std::nothrow) Boss();
	if (temp &&
		temp->initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance, weapomChance)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool Boss::initWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int MPMax, float speed,
	Weapon *weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance) {
	if (!Monster::initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance) ||
		!initMember(weapomChance)) {
		return false;
	}
	return true;
}

Boss *Boss::createWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int MPMax, float speed,
	Weapon *weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance) {
	auto temp = new(std::nothrow) Boss();
	if (temp &&
		temp->initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance, weapomChance)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}