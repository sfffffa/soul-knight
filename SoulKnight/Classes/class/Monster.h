#ifndef _MONSTER_
#define _MONSTER_

#include "Character.h"
#include <new>

class Monster :public Character {
public:
	static std::shared_ptr<Monster> create(
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		std::shared_ptr<Weapon> weapon = std::shared_ptr<Weapon>(nullptr),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f);

	static std::shared_ptr<Monster> createWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f);

	static std::shared_ptr<Monster> createWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f);

	virtual void setCoinChance(float coinChance) { *_coinChance = coinChance; }
	virtual void setHealthPotChance(float healthPotChance) { *_healthPotChance = healthPotChance; }
	virtual void setMagicPotChance(float magicPotChance) { *_magicPotChance = magicPotChance; }

	virtual float getCoinChance()const { return *_coinChance; }
	virtual float getHealthPotChance()const { return *_healthPotChance; }
	virtual float getMagicPotChance()const { return *_magicPotChance; }

	bool shoot()override;

	bool beShot(int damage)override;

	Monster *clone()const override final;

	/*void move(Vec2 dir)override;

	void shoot()override;

	void beShot(int damage)override;

	void die()override;*/
	//destructor
	virtual ~Monster() = default;
protected:

	bool init(
		int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance, float magicPotChance);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance, float magicPotChance);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax, int MPMax, float speed, std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance, float magicPotChance);

	std::shared_ptr<float> _coinChance;//½ð±Ò±¬ÂÊ
	std::shared_ptr<float> _healthPotChance;//ÑªÆ¿±¬ÂÊ
	std::shared_ptr<float> _magicPotChance;//À¶Æ¿±¬ÂÊ

private:

	bool initMember(float coinChance, float healthPotChance, float magicPotChance);
};

#endif // _MONSTER_
