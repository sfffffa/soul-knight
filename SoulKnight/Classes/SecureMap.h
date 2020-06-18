#ifndef _SECURE_MAP_H_
#define _SECURE_MAP_H_

#include "GlobalVariable.h"

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

	//赋予NPC图片及PhysicsBody
	Sprite *initNPC(const std::string& spriteFrameName);

	//赋予hero PhysicsBody
	void initHero();

	//武器初始化
	void initBullet(std::shared_ptr<Damage> bullet);

	//场景物件初始化
	void initWall(Sprite *wall);
	void initDoor(Sprite *door);
	void initLayer();

	//键盘监听
	bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif // _SECURE_MAP_H_#pragma once
