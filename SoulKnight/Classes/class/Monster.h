#ifndef _MONSTER_
#define _MONSTER_

#include "Character.h"

class Monster :public Character {
public:
	static std::shared_ptr<Monster> create(
		int HP = 0, int HPMax = 0, int shield = 0, int shieldMax = 0, int MP = 0, int MPMax = 0,
		float speed = 0.0f, std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f);

	static std::shared_ptr<Monster> createWithSpriteFrame(SpriteFrame *spriteFrame,
		int HP = 0, int HPMax = 0, int shield = 0, int shieldMax = 0, int MP = 0, int MPMax = 0,
		float speed = 0.0f, std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f);

	static std::shared_ptr<Monster> createWithSpriteFrameName(const std::string& spriteFrameName,
		int HP = 0, int HPMax = 0, int shield = 0, int shieldMax = 0, int MP = 0, int MPMax = 0,
		float speed = 0.0f, std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f);

	virtual void setCoinChance(float coinChance) { _coinChance->setValue(coinChance); }
	virtual void setHealthPotChance(float healthPotChance) { _healthPotChance->setValue(healthPotChance); }
	virtual void setMagicPotChance(float magicPotChance) { _magicPotChance->setValue(magicPotChance); }

	virtual float getCoinChance()const { return _coinChance->getValue(); }
	virtual float getHealthPotChance()const { return _healthPotChance->getValue(); }
	virtual float getMagicPotChance()const { return _magicPotChance->getValue(); }

	Monster *clone()const override final;

	void move(Vec2 dir)override;

	void shoot()override;

	void beShot(int damage)override;

	void die()override;

protected:
	//destructor
	virtual ~Monster() = default;

	bool init(
		int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
		float speed, std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance, float magicPotChance);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
		float speed, std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance, float magicPotChance);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
		float speed, std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance, float magicPotChance);

	std::shared_ptr<Attribute<float>> _coinChance;//½ð±Ò±¬ÂÊ
	std::shared_ptr<Attribute<float>> _healthPotChance;//ÑªÆ¿±¬ÂÊ
	std::shared_ptr<Attribute<float>> _magicPotChance;//À¶Æ¿±¬ÂÊ

private:

	bool initMember(float coinChance, float healthPotChance, float magicPotChance);
};

#endif // _MONSTER_
