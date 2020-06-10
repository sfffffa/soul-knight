#ifndef _SECURE_MAP_H_
#define _SECURE_MAP_H_

#include "cocos2d.h"
#include "class/LimitedAttribute.h"
#include "class/AttributeChanger.h"
#include "class/Item.h"
#include "class/ElementalBullet.h"
#include "class/CloseInWeapon.h"
#include "class/RangedWeapon.h"
#include "class/Boss.h"
#include "class/Hero.h"
#include <string>

#define HERO 10
#define ENEMY 20
#define MY_BULLET 30
#define ENEMY_BULLET 40
#define NPC 50

class SecureMap : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(SecureMap);

private:

	void addNPC(const std::string& spriteFrameName, cocos2d::Vec2 pos);

	void addHero(std::shared_ptr<Hero> hero);
};

#endif // _SECURE_MAP_H_
