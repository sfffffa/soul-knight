#ifndef __WILD_MAP_H__
#define __WILD_MAP_H__

#include "GlobalVariable.h"
#include <ctime>
#include <random>

class WildMap : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(WildMap);

private:
	TMXTiledMap* _tiledmap;
	struct {
		unsigned int conductor : 1;
		unsigned int box : 1;
		unsigned int : 1;
		unsigned int : 1;
	} _interactStatus;

	Vec2 _initiativeHeroOffset;
	Vec2 _initiativeMapOffset;

	//小怪资源管理器（房间）
	//std::vector<char> _roomStatus;//0表示未进入，1表示正在房间里，2表示已探索

	//ZOrder
	int _bulletZOrder;
	int _monsterZorder;

	int curRoomNum = 0;

	void initMember();
	void releaseMember();

	//场景物件初始化
	void initHero();
	void initEnemy(std::shared_ptr<Monster> monster, int roomNum);
	void initMyBullet(std::shared_ptr<Bullet> bullet);
	void initEnemyBullet(std::shared_ptr<Bullet> bullet);
	void initWall(Sprite *wall);
	void initDoor(Sprite *door);
	void initBox(Sprite *box);
	void initConductor(Sprite *conductor);
	void initItem(std::shared_ptr<Item> item);
	void initLayer();

	void monstersAi();

	void addMonsterInRoom();
	void addconductor();
	//void addItem(Item::Type type);
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
	//技能
	void skill();

	void positionMonitor();

	void update(float delta);
};

#endif // __WILD_MAP_H__