#ifndef __WILD_MAP_H__
#define __WILD_MAP_H__

#include "cocos2d.h"
#include "class/LimitedAttribute.h"
#include "class/AttributeChanger.h"
#include "class/Item.h"
#include "class/ElementalBullet.h"
#include "class/CloseInWeapon.h"
#include "class/RangedWeapon.h"
#include "class/Boss.h"
#include "class/Hero.h"

#define HERO 0x01
#define ENEMY 0x02
#define MY_BULLET 0x04
#define ENEMY_BULLET 0x08
#define NPC 0x10

class WildMap : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(WildMap);

private:
};

#endif // __WILD_MAP_H__