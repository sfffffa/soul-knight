#ifndef _CLOSE_IN_WEAPON_
#define _CLOSE_IN_WEAPON_

#include "weapon.h"

class CloseInWeapon :public Weapon {
public:

	static CloseInWeapon *create(
		int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f,
		float critRate = 0.0f, float critMultiple = 0.0f);

	static CloseInWeapon *createWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f,
		float critRate = 0.0f, float critMultiple = 0.0f);

	static CloseInWeapon *createWithSpriteFrameName(const std::string &spriteFrameName,
		int MPconsume = 0, int damage = 0, float rang = 0.0f, float aspd = 0.0f,
		float critRate = 0.0f, float critMultiple = 0.0f);

	virtual void setDamage(int damage) { _damage->setValue(damage); }
	virtual void setRange(float rang) { _range->setValue(rang); }

	virtual int getDamage()const { return _damage->getValue(); }
	virtual float getRange()const { return _range->getValue(); }

	virtual Attribute<int> *getDamageInstance()const { return _damage; }
	virtual Attribute<float> *getRangeInstance()const { return _range; }

	CloseInWeapon *clone()const override;

	/*void use()override;*/

protected:
	//destructor
	virtual ~CloseInWeapon() = default;

	bool init(
		int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple);

	bool initWithSpriteFrameName(const std::string &spriteFrameName,
		int MPconsume, int damage, float rang, float aspd, float critRate, float critMultiple);

	Attribute<int> *_damage;//¹¥»÷ÉËº¦
	Attribute<float> *_range;//¹¥»÷·¶Î§

private:

	bool initMember(int damage, float rang);
};

#endif // _CLOSE_IN_WEAPON_
