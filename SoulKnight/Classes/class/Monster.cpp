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
	int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	if (!Character::init(HPMax, MPMax, speed, weapon) ||
		!initMember(coinChance, healthPotChance, magicPotChance)) {
		return false;
	}

	return true;
}

std::shared_ptr<Monster> Monster::create(
	int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	auto temp = std::shared_ptr<Monster>();
	if (temp &&
		temp->init(HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance)) {
		return temp;
	}
	else {
		return std::shared_ptr<Monster>(nullptr);
	}
}

bool Monster::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	if (!Character::initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon) ||
		!initMember(coinChance, healthPotChance, magicPotChance)) {
		return false;
	}

	return true;
}

std::shared_ptr<Monster> Monster::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	auto temp = std::shared_ptr<Monster>();
	if (temp &&
		temp->initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance)) {
		return temp;
	}
	else {
		return std::shared_ptr<Monster>(nullptr);
	}
}

bool Monster::initWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	if (!Character::initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon) ||
		!initMember(coinChance, healthPotChance, magicPotChance)) {
		return false;
	}

	return true;
}

std::shared_ptr<Monster> Monster::createWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	auto temp = std::shared_ptr<Monster>();
	if (temp &&
		temp->initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance)) {
		return temp;
	}
	else {
		return std::shared_ptr<Monster>(nullptr);
	}
}

Monster *Monster::clone()const {
	Monster *temp = new(std::nothrow) Monster();
	if (temp &&
		temp->initWithSpriteFrame(this->getSpriteFrame(),
			_HP->getValueMax(), _MP->getValueMax(), _speed->getValue(),
			std::shared_ptr<Weapon>(_weapon->clone()),
			_coinChance->getValue(), _healthPotChance->getValue(), _magicPotChance->getValue())) {
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}