#ifndef _RANGED_WEAPON_
#define _RANGED_WEAPON_

#include "weapon.h"
#include "ElementalBullet.h"
#include <new>

class RangedWeapon :public Weapon {
public:
	static std::shared_ptr<RangedWeapon> create(
		int MPconsume = 0, float aspd = 0, float critRate = 0.0f, float critMultiple = 0.0f,
		std::shared_ptr<Damage> bullet = Bullet::create());

	static std::shared_ptr<RangedWeapon> createWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume = 0, float aspd = 0, float critRate = 0.0f, float critMultiple = 0.0f,
		std::shared_ptr<Damage> bullet = Bullet::create());

	static std::shared_ptr<RangedWeapon> createWithSpriteFrameName(const std::string& spriteFrameName,
		int MPconsume = 0, float aspd = 0, float critRate = 0.0f, float critMultiple = 0.0f,
		std::shared_ptr<Damage> bullet = Bullet::create());

	virtual void setBullet(std::shared_ptr<Damage> bullet) { _bullet = bullet; }

	virtual std::shared_ptr<Damage> getBulletInstance()const { return _bullet; }

	RangedWeapon *clone()const override;

	//destructor
	virtual ~RangedWeapon() = default;

protected:

	bool init(
		int MPconsume, float aspd, float critRate, float critMultiple, std::shared_ptr<Damage> bullet);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume, float aspd, float critRate, float critMultiple, std::shared_ptr<Damage> bullet);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int MPconsume, float aspd, float critRate, float critMultiple, std::shared_ptr<Damage> bullet);

	std::shared_ptr<Damage> _bullet;//×Óµ¯

private:

	bool initMember(std::shared_ptr<Damage> bullet);
};

#endif // _RANGED_WEAPON_
