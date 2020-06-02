#ifndef _BULLET_
#define _BULLET_

#include "Damage.h"

class Bullet :public Damage {
public:
	static std::shared_ptr<Bullet> create(
		int damage = 0, float speed = 0.0f, bool crit = false);

	static std::shared_ptr<Bullet> createWithSpriteFrame(SpriteFrame *spriteFrame,
		int damage = 0, float speed = 0.0f, bool crit = false);

	static std::shared_ptr<Bullet> createWithSpriteFrameName(const std::string &spriteFrameName,
		int damage = 0, float speed = 0.0f, bool crit = false);

	virtual void setSpeed(float speed) { _speed->setValue(speed); }

	virtual float getSpeed()const { return _speed->getValue(); }

	Bullet *clone()const override;

protected:
	//destructor
	virtual ~Bullet() = default;

	bool init(int damage, float speed, bool crit);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int damage, float speed, bool crit);

	bool initWithSpriteFrameName(const std::string &spriteFrameName,
		int damage, float speed, bool crit);

	std::shared_ptr<Attribute<float>> _speed;//ËÙ¶È

private:

	bool initMember(float speed);
};

#endif // _BULLET_
