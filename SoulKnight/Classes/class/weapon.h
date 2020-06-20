#ifndef _WEAPON_
#define _WEAPON_

#include "cocos2d.h"
#include "Bullet.h"
#include <new>
#include <memory>
#include <string>

USING_NS_CC;

class Weapon :public cocos2d::Sprite {
public:

	virtual void setMPCosume(int mpConsume) { *_MPconsume = mpConsume; }
	virtual void setAspd(float aspd) { *_aspd = aspd; }
	virtual void setCritRate(float critRate) { *_critRate = critRate; }
	virtual void setCritMultiple(float critMultiple) { *_critMultiple = critMultiple; }

	virtual int getMPCosume()const { return *_MPconsume; }
	virtual float getAspd()const { return *_aspd; }
	virtual float getCritRate()const { return *_critRate; }
	virtual float getCritMultiple()const { return *_critMultiple; }
	virtual std::shared_ptr<Bullet> getBulletInstance()const = 0;

	virtual Weapon *clone()const = 0;

	//virtual void use() = 0;

	//destructor
	virtual ~Weapon() = default;

protected:

	bool init(
		int MPconsume, float aspd, float critRate, float critMultiple);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int MPconsume, float aspd, float critRate, float critMultiple);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int MPconsume, float aspd, float critRate, float critMultiple);

	std::shared_ptr<int> _MPconsume;//ºÄÀ¶
	std::shared_ptr<float> _aspd;//¹¥ËÙ
	std::shared_ptr<float> _critRate;//±©»÷ÂÊ
	std::shared_ptr<float> _critMultiple;//±©»÷±¶Êý

private:

	bool initMember(
		int MPconsume, float aspd, float critRate, float critMultiple);
};

#endif //_WEAPON_
