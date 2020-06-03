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
	int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
	float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	if (!Character::init(HP, HPMax, shield, shieldMax, MP, MPMax, speed, weapon) ||
		!initMember(coinChance, healthPotChance, magicPotChance)) {
		return false;
	}

	return true;
}

std::shared_ptr<Monster> Monster::create(
	int HP = 0, int HPMax = 0, int shield = 0, int shieldMax = 0, int MP = 0, int MPMax = 0,
	float speed = 0.0f, std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
	float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f) {
	Monster *temp = new(std::nothrow) Monster();
	if (temp &&
		temp->init(HP, HPMax, shield, shieldMax, MP, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance)) {
		return std::shared_ptr<Monster>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<Monster>(nullptr);
	}
}

bool Monster::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
	float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	if (!Character::initWithSpriteFrame(spriteFrame, HP, HPMax, shield, shieldMax, MP, MPMax, speed, weapon) ||
		!initMember(coinChance, healthPotChance, magicPotChance)) {
		return false;
	}

	return true;
}

std::shared_ptr<Monster> Monster::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int HP = 0, int HPMax = 0, int shield = 0, int shieldMax = 0, int MP = 0, int MPMax = 0,
	float speed = 0.0f, std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
	float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f) {
	Monster *temp = new(std::nothrow) Monster();
	if (temp &&
		temp->initWithSpriteFrame(spriteFrame, HP, HPMax, shield, shieldMax, MP, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance)) {
		return std::shared_ptr<Monster>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<Monster>(nullptr);
	}
}

bool Monster::initWithSpriteFrameName(const std::string& spriteFrameName,
	int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
	float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance, float magicPotChance) {
	if (!Character::initWithSpriteFrameName(spriteFrameName, HP, HPMax, shield, shieldMax, MP, MPMax, speed, weapon) ||
		!initMember(coinChance, healthPotChance, magicPotChance)) {
		return false;
	}

	return true;
}

std::shared_ptr<Monster> Monster::createWithSpriteFrameName(const std::string& spriteFrameName,
	int HP = 0, int HPMax = 0, int shield = 0, int shieldMax = 0, int MP = 0, int MPMax = 0,
	float speed = 0.0f, std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
	float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f) {
	Monster *temp = new(std::nothrow) Monster();
	if (temp &&
		temp->initWithSpriteFrameName(spriteFrameName, HP, HPMax, shield, shieldMax, MP, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance)) {
		return std::shared_ptr<Monster>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<Monster>(nullptr);
	}
}

Monster *Monster::clone()const {
	Monster *temp = new(std::nothrow) Monster();
	if (temp &&
		temp->initWithSpriteFrame(this->getSpriteFrame(),
			_HP->getValue(), _HP->getValueMax(), _shield->getValue(), _shield->getValueMax(),
			_MP->getValue(), _MP->getValueMax(), _speed->getValue(),
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