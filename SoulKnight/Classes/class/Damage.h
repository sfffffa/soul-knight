#ifndef _DAMAGE_
#define _DAMAGE_

#include "cocos2d.h"
#include <memory>
#include <string>
#include <new>

USING_NS_CC;

class Damage :public Sprite {
public:

	virtual void setDamage(int damage) { *_damage = damage; }
	virtual void setCritOrNot(bool crit) { _crit = crit; }

	virtual int getDamage()const { return *_damage; }
	virtual bool getCritOrNot()const { return _crit; }

	virtual Damage *clone(bool crit)const = 0;
	//destructor
	virtual ~Damage() = default;
protected:

	bool init(int damage, bool crit);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int damage, bool crit);

	bool initWithSpriteFrameName(const std::string &spriteFrameName,
		int damage, bool crit);

	std::shared_ptr<int> _damage;//ÉËº¦Öµ
	bool _crit;//ÊÇ·ñ±©»÷

private:

	bool initMember(int damage, bool crit);
};

#endif // _DAMAGE_
