#ifndef _BULLET_
#define _BULLET_

#include "Damage.h"
#include <new>

class Bullet :public Damage {
public:
	static std::shared_ptr<Bullet> create(
		int damage = 0, float speed = 0.0f, bool crit = false);

	static std::shared_ptr<Bullet> createWithSpriteFrame(SpriteFrame *spriteFrame,
		int damage = 0, float speed = 0.0f, bool crit = false);

	static std::shared_ptr<Bullet> createWithSpriteFrameName(const std::string &spriteFrameName,
		int damage = 0, float speed = 0.0f, bool crit = false);

	virtual void setSpeed(float speed) { *_speed = speed; }

	virtual float getSpeed()const { return *_speed; }

	Bullet *clone(bool crit)const override;

	//destructor
	virtual ~Bullet() = default;
protected:

	bool init(int damage, float speed, bool crit);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int damage, float speed, bool crit);

	bool initWithSpriteFrameName(const std::string &spriteFrameName,
		int damage, float speed, bool crit);

	std::shared_ptr<float> _speed;//ËÙ¶È

private:
	bool initMember(float speed);
};

#endif // _BULLET_
