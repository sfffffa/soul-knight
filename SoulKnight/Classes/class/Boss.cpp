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
	float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance) {
	if (!Monster::init(HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance) ||
		!initMember(weapomChance)) {
		return false;
	}
	return true;
}

std::shared_ptr<Boss> Boss::create(
	int HPMax = 0, int MPMax = 0, float speed = 0.0f,
	std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
	float coinChance = 0.0f, float healthPotChance = 0.0f,
	float magicPotChance = 0.0f, float weapomChance = 0.0f) {
	Boss *temp = new(std::nothrow) Boss();
	if (temp &&
		temp->init(HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance, weapomChance)) {
		return std::shared_ptr<Boss>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<Boss>(nullptr);
	}
}

bool Boss::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance) {
	if (!Monster::initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance) ||
		!initMember(weapomChance)) {
		return false;
	}
	return true;
}

std::shared_ptr<Boss> Boss::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax = 0, int MPMax = 0, float speed = 0.0f,
	std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
	float coinChance = 0.0f, float healthPotChance = 0.0f,
	float magicPotChance = 0.0f, float weapomChance = 0.0f) {
	Boss *temp = new(std::nothrow) Boss();
	if (temp &&
		temp->initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance, weapomChance)) {
		return std::shared_ptr<Boss>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<Boss>(nullptr);
	}
}

bool Boss::initWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance) {
	if (!Monster::initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance) ||
		!initMember(weapomChance)) {
		return false;
	}
	return true;
}

std::shared_ptr<Boss> Boss::createWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax = 0, int MPMax = 0, float speed = 0.0f,
	std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
	float coinChance = 0.0f, float healthPotChance = 0.0f,
	float magicPotChance = 0.0f, float weapomChance = 0.0f) {
	Boss *temp = new(std::nothrow) Boss();
	if (temp &&
		temp->initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance, weapomChance)) {
		return std::shared_ptr<Boss>(temp);
	}
	else {
		delete temp;
		temp = nullptr;
		return std::shared_ptr<Boss>(nullptr);
	}
}