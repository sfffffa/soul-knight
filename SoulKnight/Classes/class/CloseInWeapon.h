#ifndef _CLOSE_IN_WEAPON_
#define _CLOSE_IN_WEAPON_

#include "weapon.h"

class CloseInWeapon :public Weapon {
public:

	static std::shared_ptr<CloseInWeapon> create(
		int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f, float critRate = 0.0f, float critMultiple = 0.0f);

	static std::shared_ptr<CloseInWeapon> createWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f, float critRate = 0.0f, float critMultiple = 0.0f);

	static std::shared_ptr<CloseInWeapon> createWithSpriteFrameName(const std::string &spriteFrameName,
		int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f, float critRate = 0.0f, float critMultiple = 0.0f);

	virtual void setDamage(int damage) { _damage->setValue(damage); }
	virtual void setRange(float rang) { _range->setValue(rang); }

	virtual int getDamage()const { return _damage->getValue(); }
	virtual float getRange()const { return _range->getValue(); }

	virtual std::shared_ptr<Attribute<int>> getDamageInstance()const { return _damage; }
	virtual std::shared_ptr<Attribute<float>> getRangeInstance()const { return _range; }

	CloseInWeapon *clone()const override;

	void use()override;

protected:
	//destructor
	virtual ~CloseInWeapon() = default;

	bool init(
		int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple);

	bool initWithSpriteFrameName(const std::string &spriteFrameName,
		int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple);

	std::shared_ptr<Attribute<int>> _damage;//¹¥»÷ÉËº¦
	std::shared_ptr<Attribute<float>> _range;//¹¥»÷·¶Î§

private:

	bool initMember(int damage, float rang);
};

#endif // _CLOSE_IN_WEAPON_
