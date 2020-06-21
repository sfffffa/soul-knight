#ifndef __WILD_MAP_H__
#define __WILD_MAP_H__

#include "GlobalVariable.h"

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
	//Vector<std::shared_ptr<Weapon>> weaponVec;
	Vec2 _initiativeHeroOffset;
	Vec2 _initiativeMapOffset;
	//子弹资源管理器
	int _bulletIndex;
	std::map<int, std::shared_ptr<Bullet>> _bulletManagement;
	//道具资源管理器
	int _itemIndex;
	std::map<int, std::shared_ptr<Item>> _itemManagement;
	//小怪资源管理器（房间）
	std::vector<char> _roomStatus;
	int _room2Index;
	std::map<int, std::shared_ptr<Monster>> _room2;
	int _room3Index;
	std::map<int, std::shared_ptr<Monster>> _room3;
	int _room4Index;
	std::map<int, std::shared_ptr<Boss>> _room4;

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

	void update(float delta);
};

#endif // __WILD_MAP_H__