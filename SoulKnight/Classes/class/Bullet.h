#ifndef _BULLET_
#define _BULLET_

#include "Damage.h"
#include <new>

class Bullet :public Damage {
public:
	static Bullet *create(
		int damage = 0, float speed = 0.0f, bool crit = false);

	static Bullet *createWithSpriteFrame(SpriteFrame *spriteFrame,
		int damage = 0, float speed = 0.0f, bool crit = false);

	static Bullet *createWithSpriteFrameName(const std::string &spriteFrameName,
		int damage = 0, float speed = 0.0f, bool crit = false);

	virtual void setSpeed(float speed) { _speed->setValue(speed); }

	virtual float getSpeed()const { return _speed->getValue(); }

	virtual Attribute<float> *getSpeedInstance()const { return _speed; }

	Bullet *clone()const override;

protected:
	//destructor
	virtual ~Bullet() = default;

	bool init(int damage, float speed, bool crit);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int damage, float speed, bool crit);

	bool initWithSpriteFrameName(const std::string &spriteFrameName,
		int damage, float speed, bool crit);

	Attribute<float> *_speed;//ËÙ¶È

private:

	bool initMember(float speed);
};

#endif // _BULLET_
