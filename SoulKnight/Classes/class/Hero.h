#ifndef _HERO_
#define _HERO_

#include "Character.h"
#include <functional>

class Hero :public Character {
public:
	static std::shared_ptr<Hero> create(
		int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
		std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		std::shared_ptr<Weapon> offhandWeapon = CloseInWeapon::create(),
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	static std::shared_ptr<Hero> createWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
		std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		std::shared_ptr<Weapon> offhandWeapon = CloseInWeapon::create(),
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	static std::shared_ptr<Hero> createWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
		std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		std::shared_ptr<Weapon> offhandWeapon = CloseInWeapon::create(),
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	//you should set shieldMAX before setting shield
	virtual void setShield(int shield) { _shield->setValue(shield); }
	virtual void setShieldMax(int shieldMax) { _shield->setValueMax(shieldMax); }
	virtual void setOffhandWeapon(std::shared_ptr<Weapon> offhandWeapon) { _offhandWeapon = offhandWeapon; }
	virtual void setCD(float cd) { _coolDown->setValue(cd); }
	virtual void setSkill(std::function<void(void)> skill) { _skill = skill; }

	virtual int getShield()const { return _shield->getValue(); }
	virtual int getShieldMax()const { return _shield->getValueMax(); }
	virtual float getCD()const { return _coolDown->getValue(); }

	virtual std::shared_ptr<LimitedAttribute<int>> getShieldInstance()const { return _shield; }
	virtual std::shared_ptr<Weapon> getOffhandWeaponInstance()const { return _offhandWeapon; }
	virtual std::shared_ptr<Attribute<float>> getCDInstance()const { return _coolDown; }

	virtual Hero *clone()const;

	//use move(Vec2(0,0)) to stop
	virtual void move(Vec2 dir);

	virtual void shoot();

	virtual void beShot(int damage);

	virtual void die();
protected:
	//destructor
	virtual ~Hero() = default;

	bool init(
		int HPMax, int shieldMax, int MPMax, float speed,
		std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
		std::function<void(void)> skill, float cd);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax, int shieldMax, int MPMax, float speed,
		std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
		std::function<void(void)> skill, float cd);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax, int shieldMax, int MPMax, float speed,
		std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
		std::function<void(void)> skill, float cd);

	std::shared_ptr<LimitedAttribute<int>> _shield;//盾
	std::shared_ptr<Weapon> _offhandWeapon;//副武器
	bool _weaponStatus = 0;//显示正在使用主武器还是副武器
	bool resurrection = 1;//可复活
	std::shared_ptr<Attribute<float>> _coolDown;//技能cd
	std::function<void(void)> _skill;//技能

private:
	bool initMember(int shieldMax, std::shared_ptr<Weapon> offhandWeapon,
		float coolDown, std::function<void(void)> skill);
};

#endif // _HERO_
