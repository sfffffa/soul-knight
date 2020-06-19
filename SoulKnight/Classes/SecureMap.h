#ifndef _SECURE_MAP_H_
#define _SECURE_MAP_H_

#include "GlobalVariable.h"

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
	std::shared_ptr<Hero> _hero;
	struct {
		unsigned int door : 1;
		unsigned int oldMan : 1;
		unsigned int hunter : 1;
		unsigned int : 1;
	} interactStatus;

	//¸³ÓèNPCÍ¼Æ¬¼°PhysicsBody
	Sprite *initNPC(const std::string& spriteFrameName);

	//¸³Óèhero PhysicsBody
	void initHero();

	//ÎäÆ÷³õÊ¼»¯
	void initBullet(std::shared_ptr<Damage> bullet);

	//³¡¾°Îï¼þ³õÊ¼»¯
	void initWall(Sprite *wall);
	void initDoor(Sprite *door);
	void initLayer();

	//¼üÅÌ¼àÌý
	bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	//Åö×²¼ì²â
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	bool onContactSeparate(cocos2d::PhysicsContact& contact);

	//½»»¥
	void interact();
};

#endif // _SECURE_MAP_H_#pragma once
