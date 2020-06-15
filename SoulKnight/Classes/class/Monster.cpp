#include "Monster.h"

inline bool Monster::initMember(
	float coinChance, float healthPotChance, float magicPotChance) {
	auto coin = Attribute<float>::createWithValue(coinChance);
	if (!coin) {
		return false;
	}
	else {
		_coinChance = coin;
	}

	auto health = Attribute<float>::createWithValue(healthPotChance);
	if (!health) {
		return false;
	}
	else {
		_healthPotChance = health;
	}

	auto magic = Attribute<float>::createWithValue(magicPotChance);
	if (!magic) {
		return false;
	}
	else {
		_magicPotChance = magic;
	}

	return true;
}

bool Monster::init(
	int HPMax, int MPMax, float speed, Weapon *weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	if (!Character::init(HPMax, MPMax, speed, weapon) ||
		!initMember(coinChance, healthPotChance, magicPotChance)) {
		return false;
	}

	return true;
}

Monster *Monster::create(
	int HPMax, int MPMax, float speed, Weapon *weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	auto temp = new(std::nothrow) Monster();
	if (temp &&
		temp->init(HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool Monster::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int MPMax, float speed, Weapon *weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	if (!Character::initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon) ||
		!initMember(coinChance, healthPotChance, magicPotChance)) {
		return false;
	}

	return true;
}

Monster *Monster::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int MPMax, float speed, Weapon *weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	auto temp = new(std::nothrow) Monster();
	if (temp &&
		temp->initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool Monster::initWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int MPMax, float speed, Weapon *weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	if (!Character::initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon) ||
		!initMember(coinChance, healthPotChance, magicPotChance)) {
		return false;
	}

	return true;
}

Monster *Monster::createWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int MPMax, float speed, Weapon *weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	auto temp = new(std::nothrow) Monster();
	if (temp &&
		temp->initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance)) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

Monster *Monster::clone()const {
	Monster *temp = new(std::nothrow) Monster();
	if (temp &&
		temp->initWithSpriteFrame(this->getSpriteFrame(),
			_HP->getValueMax(), _MP->getValueMax(), _speed->getValue(),
			_weapon->clone(),
			_coinChance->getValue(), _healthPotChance->getValue(), _magicPotChance->getValue())) {
		temp->autorelease();
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}