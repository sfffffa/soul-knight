#include "Boss.h"

inline bool Boss::initMember(float weaponChance,
	float coolDown, std::function<void(void)> skill) {
	_weaponChance = weaponChance;
	_coolDown = coolDown;
	_skill = skill;
	return true;
}

bool Boss::init(
	int HPMax, int MPMax,
	float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance,
	std::function<void(void)> skill, float cd) {
	if (!Monster::init(HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance) ||
		!initMember(weapomChance, cd, skill)) {
		return false;
	}
	return true;
}

std::shared_ptr<Boss> Boss::create(
	int HPMax, int MPMax,
	float speed, std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance,
	std::function<void(void)> skill, float cd) {
	auto temp = std::make_shared<Boss>();
	if (temp &&
		temp->init(HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance, weapomChance, skill, cd)) {
		return temp;
	}
	else {
		return std::shared_ptr<Boss>(nullptr);
	}
}

bool Boss::initWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance,
	std::function<void(void)> skill, float cd) {
	if (!Monster::initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance) ||
		!initMember(weapomChance, cd, skill)) {
		return false;
	}
	return true;
}

std::shared_ptr<Boss> Boss::createWithSpriteFrame(SpriteFrame *spriteFrame,
	int HPMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance,
	std::function<void(void)> skill, float cd) {
	auto temp = std::make_shared<Boss>();
	if (temp &&
		temp->initWithSpriteFrame(spriteFrame, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance, weapomChance, skill, cd)) {
		return temp;
	}
	else {
		return std::shared_ptr<Boss>(nullptr);
	}
}

bool Boss::initWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance,
	std::function<void(void)> skill, float cd) {
	if (!Monster::initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance) ||
		!initMember(weapomChance, cd, skill)) {
		return false;
	}
	return true;
}

std::shared_ptr<Boss> Boss::createWithSpriteFrameName(const std::string& spriteFrameName,
	int HPMax, int MPMax, float speed,
	std::shared_ptr<Weapon> weapon,
	float coinChance, float healthPotChance,
	float magicPotChance, float weapomChance,
	std::function<void(void)> skill, float cd) {
	auto temp = std::make_shared<Boss>();
	if (temp &&
		temp->initWithSpriteFrameName(spriteFrameName, HPMax, MPMax, speed, weapon, coinChance, healthPotChance, magicPotChance, weapomChance, skill, cd)) {
		return temp;
	}
	else {
		return std::shared_ptr<Boss>(nullptr);
	}
}