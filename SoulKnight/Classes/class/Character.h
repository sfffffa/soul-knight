#ifndef _CHARACTER_
#define _CHARACTER_

#include "CloseInWeapon.h"
#include "RangedWeapon.h"
#include "cocos2d.h"
#include <memory>
#include <new>
#include <string>

USING_NS_CC;

class Character :public Sprite {
public:

	//you should set HPMAX before setting HP
	virtual void setHP(int HP) { _HP = (HP > *_HPMAX) ? *_HPMAX : HP; }
	virtual void setHPMax(int HPMax) { *_HPMAX = HPMax; }
	//you should set MPMAX before setting MP
	virtual void setMP(int MP) { _MP = (MP > *_MPMAX) ? *_MPMAX : MP; }
	virtual void setMPMax(int MPMax) { *_MPMAX = MPMax; }
	virtual void setSpeed(float speed) { *_speed = speed; }
	virtual void setWeapon(std::shared_ptr<Weapon> weapon) { _weapon = weapon; }

	virtual int getHP()const { return _HP; }
	virtual int getHPMax()const { return *_HPMAX; }
	virtual int getMP()const { return _MP; }
	virtual int getMPMax()const { return *_MPMAX; }
	virtual float getSpeed()const { return *_speed; }
	virtual std::shared_ptr<Weapon> getWeaponInstance()const { return _weapon; }

	virtual Character *clone()const = 0;

	//return当前蓝量是否够本次射击
	virtual bool shoot() = 0;

	//返回遭受伤害后是否死亡
	virtual bool beShot(int damage) = 0;
	/*//use move(Vec2(0,0)) to stop
	virtual void move(Vec2 dir) = 0;

	virtual void die() = 0;*/

protected:
	//destructor
	virtual ~Character() = default;

	bool init(
		int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon);

	int _HP;
	std::shared_ptr<int> _HPMAX;
	int _MP;
	std::shared_ptr<int> _MPMAX;
	std::shared_ptr<float> _speed;
	std::shared_ptr<Weapon> _weapon;
private:

	bool initMember(
		int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon);
};

#endif //_CHARACTER_
