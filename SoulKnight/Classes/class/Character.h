#ifndef _CHARACTER_
#define _CHARACTER_

#include "Attribute.h"
#include "LimitedAttribute.h"
#include "CloseInWeapon.h"
#include "RangedWeapon.h"
#include "cocos2d.h"
#include <memory>
#include <new>

USING_NS_CC;

class Character :public Sprite {
public:

	//you should set HPMAX before setting HP
	virtual void setHP(int HP) { _HP->setValue(HP); }
	virtual void setHPMax(int HPMax) { _HP->setValueMax(HPMax); }
	//you should set MPMAX before setting MP
	virtual void setMP(int MP) { _MP->setValue(MP); }
	virtual void setMPMax(int MPMax) { _MP->setValueMax(MPMax); }
	virtual void setSpeed(float speed) { _speed->setValue(speed); }
	virtual void setWeapon(std::shared_ptr<Weapon> weapon) { _weapon = weapon; }

	virtual int getHP()const { return _HP->getValue(); }
	virtual int getHPMax()const { return _HP->getValueMax(); }
	virtual int getMP()const { return _MP->getValue(); }
	virtual int getMPMax()const { return _MP->getValueMax(); }
	virtual float getSpeed()const { return _speed->getValue(); }

	virtual std::shared_ptr<LimitedAttribute<int>> getHPInstance()const { return _HP; }
	virtual std::shared_ptr<LimitedAttribute<int>> getMPInstance()const { return _MP; }
	virtual std::shared_ptr<Attribute<float>> getSpeedInstance()const { return _speed; }
	virtual std::shared_ptr<Weapon> getWeaponInstance()const { return _weapon; }

	virtual Character *clone()const = 0;

	//use move(Vec2(0,0)) to stop
	virtual void move(Vec2 dir) = 0;

	virtual void shoot() = 0;

	virtual void beShot(int damage) = 0;

	virtual void die() = 0;

protected:
	//destructor
	virtual ~Character() = default;

	bool init(
		int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon);

	std::shared_ptr<LimitedAttribute<int>> _HP;
	std::shared_ptr<LimitedAttribute<int>> _MP;
	std::shared_ptr<Attribute<float>> _speed;
	std::shared_ptr<Weapon> _weapon;
private:

	bool initMember(
		int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon);
};

#endif //_CHARACTER_
