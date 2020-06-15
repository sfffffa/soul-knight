#ifndef _RANGED_WEAPON_
#define _RANGED_WEAPON_

#include "weapon.h"
#include "ElementalBullet.h"
#include <new>

class RangedWeapon :public Weapon {
public:
	static RangedWeapon *create(
		int MPconsume = 0, float aspd = 0, float critRate = 0.0f, float critMultiple = 0.0f,
		Damage *bullet = Bullet::create());

	static RangedWeapon *createWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume = 0, float aspd = 0, float critRate = 0.0f, float critMultiple = 0.0f,
		Damage *bullet = Bullet::create());

	static RangedWeapon *createWithSpriteFrameName(const std::string& spriteFrameName,
		int MPconsume = 0, float aspd = 0, float critRate = 0.0f, float critMultiple = 0.0f,
		Damage *bullet = Bullet::create());

	virtual void setBullet(Damage *bullet) { _bullet = bullet; }

	virtual Damage *getBulletInstance()const { return _bullet; }

	RangedWeapon *clone()const override;

	/*void use()override;*/

protected:
	//destructor
	virtual ~RangedWeapon() = default;

	bool init(
		int MPconsume, float aspd, float critRate, float critMultiple, Damage *bullet);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume, float aspd, float critRate, float critMultiple, Damage *bullet);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int MPconsume, float aspd, float critRate, float critMultiple, Damage *bullet);

	Damage *_bullet;//×Óµ¯

private:

	bool initMember(Damage *bullet);
};

#endif // _RANGED_WEAPON_
