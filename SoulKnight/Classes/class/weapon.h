#ifndef _WEAPON_
#define _WEAPON_

#include "Attribute.h"
#include "cocos2d.h"
#include <new>
#include <string>

USING_NS_CC;

class Weapon :public cocos2d::Sprite {
public:

	virtual void setMPCosume(int mpConsume) { _MPconsume->setValue(mpConsume); }
	virtual void setAspd(float aspd) { _aspd->setValue(aspd); }
	virtual void setCritRate(float critRate) { _critRate->setValue(critRate); }
	virtual void setCritMultiple(float critMultiple) { _critMultiple->setValue(critMultiple); }

	virtual int getMPCosume()const { return _MPconsume->getValue(); }
	virtual float getAspd()const { return _aspd->getValue(); }
	virtual float getCritRate()const { return _critRate->getValue(); }
	virtual float getCritMultiple()const { return _critMultiple->getValue(); }

	virtual Weapon *clone()const = 0;

	virtual void use() = 0;
protected:
	//destructor
	virtual ~Weapon() = default;

	bool init(
		int MPconsume, float aspd, float critRate, float critMultiple);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume, float aspd, float critRate, float critMultiple);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int MPconsume, float aspd, float critRate, float critMultiple);

	std::shared_ptr<Attribute<int>> _MPconsume;//ºÄÀ¶
	std::shared_ptr<Attribute<float>> _aspd;//¹¥ËÙ
	std::shared_ptr<Attribute<float>> _critRate;//±©»÷ÂÊ
	std::shared_ptr<Attribute<float>> _critMultiple;//±©»÷±¶Êý

private:

	bool initMember(
		int MPconsume, float aspd, float critRate, float critMultiple);
};

#endif //_WEAPON_
