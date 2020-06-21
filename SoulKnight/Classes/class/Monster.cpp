#include "Monster.h"

inline bool Monster::initMember(
	float coinChance, float healthPotChance, float magicPotChance) {
	auto coin = std::make_shared<float>(coinChance);
	if (!coin) {
		return false;
	}
	else {
		_coinChance = coin;
	}

	auto health = std::make_shared<float>(healthPotChance);
	if (!health) {
		return false;
	}
	else {
		_healthPotChance = health;
	}

	auto magic = std::make_shared<float>(magicPotChance);
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
	auto temp = std::make_shared<Monster>();
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
	auto temp = std::make_shared<Monster>();
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
	auto temp = std::make_shared<Monster>();
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
	if (temp) {
		temp->setSpriteFrame(this->getSpriteFrame());
		temp->_HPMAX = _HPMAX;
		temp->_HP = *_HPMAX;
		temp->_MPMAX = _MPMAX;
		temp->_MP = *_MPMAX;
		temp->_speed = _speed;
		temp->_weapon = static_cast<std::shared_ptr<Weapon>>(_weapon->clone());
		temp->_coinChance = _coinChance;
		temp->_healthPotChance = _healthPotChance;
		temp->_magicPotChance = _magicPotChance;
		temp->_charaName = _charaName;
		temp->setScale(getScale());
		return temp;
	}
	else {
		delete temp;
		temp = nullptr;
		return nullptr;
	}
}

bool Monster::shoot() {
	if (_MP < _weapon->getMPCosume()) {
		return false;
	}
	_MP -= _weapon->getMPCosume();
	return true;
}

bool Monster::beShot(int damage) {
	if (_HP <= damage) {
		_HP = 0;
		return true;
	}
	_HP -= damage;
	return false;
}