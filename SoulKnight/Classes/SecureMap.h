#ifndef _SECURE_MAP_H_
#define _SECURE_MAP_H_

#include "cocos2d.h"
#include "class/Item.h"
#include "class/ElementalBullet.h"
#include "class/CloseInWeapon.h"
#include "class/RangedWeapon.h"
#include "class/Boss.h"
#include "class/Hero.h"
#include <string>

#define HERO 0x01
#define ENEMY 0x02
#define MY_BULLET 0x04
#define ENEMY_BULLET 0x08
#define NPC 0x10

class SecureMap : public cocos2d::Scene
{
public:

	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(SecureMap);

private:

	TMXTiledMap* _tiledmap;
	std::shared_ptr<Hero> _hero;

	//∏≥”ËNPCÕº∆¨º∞PhysicsBody
	Sprite *initNPC(const std::string& spriteFrameName);

	//∏≥”Ëhero PhysicsBody
	void initHero();

	//º¸≈Ãº‡Ã˝
	bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif // _SECURE_MAP_H_#pragma once
