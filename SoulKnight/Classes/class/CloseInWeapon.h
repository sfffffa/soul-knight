#ifndef _CLOSE_IN_WEAPON_
#define _CLOSE_IN_WEAPON_

#include "weapon.h"

class CloseInWeapon :public Weapon {
public:

	static std::shared_ptr<CloseInWeapon> create(
		int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f,
		float critRate = 0.0f, float critMultiple = 0.0f);

	static std::shared_ptr<CloseInWeapon> createWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f,
		float critRate = 0.0f, float critMultiple = 0.0f);

	static std::shared_ptr<CloseInWeapon> createWithSpriteFrameName(const std::string &spriteFrameName,
		int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f,
		float critRate = 0.0f, float critMultiple = 0.0f);

	virtual void setDamage(int damage) { *_damage = damage; }
	virtual void setRange(float rang) { *_range = rang; }

	virtual int getDamage()const { return *_damage; }
	virtual float getRange()const { return *_range; }
	std::shared_ptr<Damage> getBulletInstance()const override { return std::shared_ptr<Damage>(nullptr); }

	CloseInWeapon *clone()const override;

	//destructor
	virtual ~CloseInWeapon() = default;

protected:

	bool init(
		int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple);

	bool initWithSpriteFrameName(const std::string &spriteFrameName,
		int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple);

	std::shared_ptr<int> _damage;//¹¥»÷ÉËº¦
	std::shared_ptr<float> _range;//¹¥»÷·¶Î§

private:

	bool initMember(int damage, float rang);
};

#endif // _CLOSE_IN_WEAPON_
