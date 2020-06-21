#ifndef _SECURE_MAP_H_
#define _SECURE_MAP_H_

#include "GlobalVariable.h"
#include <map>

class SecureMap : public cocos2d::Scene
{
public:

	static cocos2d::Scene* createScene();

	virtual bool init();

	void pausemenu(cocos2d::Ref* pSender);//pause

	// implement the "static create()" method manually
	CREATE_FUNC(SecureMap);

private:

	TMXTiledMap* _tiledmap;
	struct {
		unsigned int door : 1;
		unsigned int oldMan : 1;
		unsigned int hunter : 1;
		unsigned int : 1;
	} interactStatus;

	//赋予NPC图片及PhysicsBody
	Sprite *initNPC(const std::string& spriteFrameName);

	//赋予hero PhysicsBody
	void initHero();

	//武器初始化
	void initBullet(std::shared_ptr<Bullet> bullet);

	//场景物件初始化
	void initWall(Sprite *wall);
	void initDoor(Sprite *door);
	void initLayer();

	//键盘监听
	bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	//碰撞检测
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	bool onContactSeparate(cocos2d::PhysicsContact& contact);

	//交互
	void interact();
	//攻击
	void shoot();
	//起换武器,实操,不同于hero.h的changeweapon
	void changeWeaponActive();
	//技能
	void skill();
};

#endif // _SECURE_MAP_H_#pragma once
