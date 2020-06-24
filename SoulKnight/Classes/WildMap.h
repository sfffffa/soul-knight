#ifndef __WILD_MAP_H__
#define __WILD_MAP_H__

#include "GlobalVariable.h"
#include "ui/CocosGUI.h"

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

	cocos2d::ui::Slider *healthBar;
	cocos2d::ui::Slider *shieldBar;
	cocos2d::ui::Slider *magicBar;

	Vec2 _initiativeHeroOffset;
	Vec2 _initiativeMapOffset;

	//小怪资源管理器（房间）
	//std::vector<char> _roomStatus;//0表示未进入，1表示正在房间里，2表示已探索

	//ZOrder
	int _bulletZOrder;
	int _monsterZorder;

	bool _conductorHasAdded = 0;

	int curRoomNum = 0;

	void initMember();
	void releaseMember();

	//场景物件初始化
	void initLayer();

	//void monstersAi();

	void addMonsterInRoom(int roomNum);
	void addMonsterInRooms();
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

	//武器切换
	void changeWeaponActive();
	//技能
	void skill();

	void aiShoot(std::shared_ptr<Monster> monster);
	void aiMove(std::shared_ptr<Monster> monster);

	void positionMonitor();

	void update(float delta);
	void updateShield(float delta);
	void aiInTwoSec(float delta);
	void aiInThreeSec(float delta);
};

#endif // __WILD_MAP_H__