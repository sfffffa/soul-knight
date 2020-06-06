#ifndef _DAMAGE_
#define _DAMAGE_

#include "cocos2d.h"
#include "Attribute.h"
#include <memory>
#include <string>

USING_NS_CC;

//ÉËº¦ÊµÌå
class Damage :public Sprite {
public:

	virtual void setDamage(int damage) { _damage->setValue(damage); }
	virtual void setCritOrNot(bool crit) { _crit = crit; }

	virtual int getDamage()const { return _damage->getValue(); }
	virtual bool getCritOrNot()const { return _crit; }

	virtual std::shared_ptr<Attribute<int>> getDamageInatance() { return _damage; }

	virtual Damage *clone()const = 0;

protected:
	//destructor
	virtual ~Damage() = default;

	bool init(int damage, bool crit);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int damage, bool crit);

	bool initWithSpriteFrameName(const std::string &spriteFrameName,
		int damage, bool crit);

	std::shared_ptr<Attribute<int>> _damage;//ÉËº¦Öµ
	bool _crit;//ÊÇ·ñ±©»÷

private:

	bool initMember(int damage, bool crit);
};

#endif // _DAMAGE_
