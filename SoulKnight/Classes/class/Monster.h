#ifndef _MONSTER_
#define _MONSTER_

#include "Character.h"
#include <new>

class Monster :public Character {
public:
	static Monster *create(
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		Weapon *weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f);

	static Monster *createWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		Weapon *weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f);

	static Monster *createWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		Weapon *weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f);

	virtual void setCoinChance(float coinChance) { _coinChance->setValue(coinChance); }
	virtual void setHealthPotChance(float healthPotChance) { _healthPotChance->setValue(healthPotChance); }
	virtual void setMagicPotChance(float magicPotChance) { _magicPotChance->setValue(magicPotChance); }

	virtual float getCoinChance()const { return _coinChance->getValue(); }
	virtual float getHealthPotChance()const { return _healthPotChance->getValue(); }
	virtual float getMagicPotChance()const { return _magicPotChance->getValue(); }

	virtual Attribute<float> *getCoinChanceInstance()const { return _coinChance; }
	virtual Attribute<float> *getHealthPotChanceInstance()const { return _healthPotChance; }
	virtual Attribute<float> *getMagicPotChanceInatance()const { return _magicPotChance; }

	Monster *clone()const override final;

	/*void move(Vec2 dir)override;

	void shoot()override;

	void beShot(int damage)override;

	void die()override;*/

protected:
	//destructor
	virtual ~Monster() = default;

	bool init(
		int HPMax, int MPMax, float speed, Weapon *weapon,
		float coinChance, float healthPotChance, float magicPotChance);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax, int MPMax, float speed, Weapon *weapon,
		float coinChance, float healthPotChance, float magicPotChance);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax, int MPMax, float speed, Weapon *weapon,
		float coinChance, float healthPotChance, float magicPotChance);

	Attribute<float> *_coinChance;//½ð±Ò±¬ÂÊ
	Attribute<float> *_healthPotChance;//ÑªÆ¿±¬ÂÊ
	Attribute<float> *_magicPotChance;//À¶Æ¿±¬ÂÊ

private:

	bool initMember(float coinChance, float healthPotChance, float magicPotChance);
};

#endif // _MONSTER_
