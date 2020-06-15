#ifndef _HERO_
#define _HERO_

#include "Character.h"
#include <functional>
#include <new>

class Hero :public Character {
public:
	static Hero *create(
		int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
		Weapon *weapon = CloseInWeapon::create(),
		Weapon *offhandWeapon = CloseInWeapon::create(),
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	static Hero *createWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
		Weapon *weapon = CloseInWeapon::create(),
		Weapon *offhandWeapon = CloseInWeapon::create(),
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	static Hero *createWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
		Weapon *weapon = CloseInWeapon::create(),
		Weapon *offhandWeapon = CloseInWeapon::create(),
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	//you should set shieldMAX before setting shield
	virtual void setShield(int shield) { _shield->setValue(shield); }
	virtual void setShieldMax(int shieldMax) { _shield->setValueMax(shieldMax); }
	virtual void setOffhandWeapon(Weapon *offhandWeapon) { _offhandWeapon = offhandWeapon; }
	virtual void setCD(float cd) { _coolDown->setValue(cd); }
	virtual void setSkill(std::function<void(void)> skill) { _skill = skill; }

	virtual int getShield()const { return _shield->getValue(); }
	virtual int getShieldMax()const { return _shield->getValueMax(); }
	virtual float getCD()const { return _coolDown->getValue(); }

	virtual LimitedAttribute<int> *getShieldInstance()const { return _shield; }
	virtual Weapon *getOffhandWeaponInstance()const { return _offhandWeapon; }
	virtual Attribute<float> *getCDInstance()const { return _coolDown; }

	virtual Hero *clone()const override;

	//use move(Vec2(0,0)) to stop
	/*virtual void move(Vec2 dir) override;

	virtual void shoot() override;

	virtual void beShot(int damage) override;

	virtual void die() override;*/
protected:
	//destructor
	virtual ~Hero() = default;

	bool init(
		int HPMax, int shieldMax, int MPMax, float speed,
		Weapon *weapon, Weapon *offhandWeapon,
		std::function<void(void)> skill, float cd);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax, int shieldMax, int MPMax, float speed,
		Weapon *weapon, Weapon *offhandWeapon,
		std::function<void(void)> skill, float cd);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax, int shieldMax, int MPMax, float speed,
		Weapon *weapon, Weapon *offhandWeapon,
		std::function<void(void)> skill, float cd);

	LimitedAttribute<int> *_shield;//盾
	Weapon *_offhandWeapon;//副武器
	bool _weaponStatus = 0;//显示正在使用主武器还是副武器
	bool resurrection = 1;//可复活
	Attribute<float> *_coolDown;//技能cd
	std::function<void(void)> _skill;//技能

private:
	bool initMember(int shieldMax, Weapon *offhandWeapon,
		float coolDown, std::function<void(void)> skill);
};

#endif // _HERO_
