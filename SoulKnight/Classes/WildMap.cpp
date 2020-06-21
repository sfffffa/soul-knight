#include "WildMap.h"
#include "SecureMap.h"

USING_NS_CC;
using namespace std;

extern std::shared_ptr<Hero> globalHero;
extern int globalCoin;
extern std::vector<std::shared_ptr<Monster>> globalMonsterRepository;
extern std::vector<std::shared_ptr<Weapon>> globalWeaponRepository;

Scene* WildMap::createScene()
{
	return WildMap::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in WildMapScene.cpp\n");
}

// on "init" you need to initialize your instance
bool WildMap::init()
{
	//////////////////////////////
	// 1. 预备初始化
	//
	/////////////////////
	// 1.1 父类初始化
	//
	if (!Scene::init())
	{
		return false;
	}
	if (!Scene::initWithPhysics()) {
		return false;
	}

	/////////////////////									cyf
	// 1.2成员初始化
	//

	initMember();

	/////////////////////
	// 1.2 基础信息提取
	//
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 destination(origin.x + visibleSize.width, origin.y + visibleSize.height);

	/////////////////////
	// 1.3 资源加载											hth
	//
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("new.plist");

	/////////////////////
	// 1.3.1 武器库初始化（相应子弹初始化）					hth
	//想法大概是在一个vector<std::shared_ptr<Weapon>>里提前初始化好所有武器，以供小怪爆武器时直接复制Weapon

    

	/////////////////////////////
	// 2. 背景初始化（不是地图）（类似于skyworld）				cyf
	//
	auto background = DrawNode::create();
	background->drawSolidRect(origin, destination, cocos2d::Color4F(195 / 255.0f, 176 / 255.0f, 145 / 255.0f, 1.0f));
	this->addChild(background, -10);

	/////////////////////////////
	// 3. Hero初始化											cyf
	//
	initHero();

	/////////////////////////////
	// 3. 地图初始化											hth、cyf
	//
	_tiledmap = TMXTiledMap::create("map1.tmx");
	_tiledmap->setAnchorPoint(Vec2(0, 1));
	_tiledmap->setPosition(Vec2(origin.x + 320, origin.y + visibleSize.height - 70));
	this->addChild(_tiledmap, -1);

	initLayer();

	TMXObjectGroup* objectGroup = _tiledmap->getObjectGroup("room1");
	auto heroBornPlace = objectGroup->getObject("heroborn");
	float bornX = heroBornPlace["x"].asFloat();
	float bornY = heroBornPlace["y"].asFloat();
	globalHero->setPosition(Vec2(bornX, bornY));
	_tiledmap->addChild(globalHero.get(), _monsterZorder);

	_initiativeMapOffset = _tiledmap->getPosition();
	_initiativeHeroOffset = globalHero->getPosition();
	//_tiledmap->setViewpointCenter(_player->getPosition());
	/////////////////////////////
	// 4. 小怪（及Boss）初始化								xyc
	//……
	auto monster1 = Monster::createWithSpriteFrameName("monster1right.png");
	auto monster2 = Monster::createWithSpriteFrameName("monster2right.png");
	auto boss1 = Monster::createWithSpriteFrameName("boss1right.png");
	globalMonsterRepository.push_back(monster1);
	globalMonsterRepository.push_back(monster2);
	globalMonsterRepository.push_back(boss1);

	TMXObjectGroup* room2 = _tiledmap->getObjectGroup("room2");
	TMXObjectGroup* room3 = _tiledmap->getObjectGroup("room3");
	TMXObjectGroup* room4 = _tiledmap->getObjectGroup("room4");
	TMXObjectGroup* room5 = _tiledmap->getObjectGroup("room5");
	//room2
	auto monsterborn2_1 = room2->getObject("monsterborn1");
	auto monsterborn2_2 = room2->getObject("monsterborn2");
	auto monsterborn2_3 = room2->getObject("monsterborn3");
	auto monsterborn2_4 = room2->getObject("monsterborn4");
	float born2_X1 = monsterborn2_1["x"].asFloat();
	float born2_Y1 = monsterborn2_1["y"].asFloat();
	float born2_X2 = monsterborn2_2["x"].asFloat();
	float born2_Y2 = monsterborn2_2["y"].asFloat();
	float born2_X3 = monsterborn2_3["x"].asFloat();
	float born2_Y3 = monsterborn2_3["y"].asFloat();
	float born2_X4 = monsterborn2_4["x"].asFloat();
	float born2_Y4 = monsterborn2_4["y"].asFloat();

	auto monster2_1 = globalMonsterRepository[0]->clone();
	auto monster2_2 = globalMonsterRepository[0]->clone();
	auto monster2_3 = globalMonsterRepository[0]->clone();
	auto monster2_4 = globalMonsterRepository[0]->clone();

	monster2_1->setPosition(Vec2(born2_X1, born2_Y1));
	monster2_2->setPosition(Vec2(born2_X2, born2_Y2));
	monster2_3->setPosition(Vec2(born2_X3, born2_Y3));
	monster2_4->setPosition(Vec2(born2_X4, born2_Y4));
	monster2_1->setScale(0.3, 0.3);
	monster2_2->setScale(0.3, 0.3);
	monster2_3->setScale(0.3, 0.3);
	monster2_4->setScale(0.3, 0.3);
	monsterMap2.insert(map<int, std::shared_ptr<Monster>>::value_type(1, monster2_1));
	monsterMap2.insert(map<int, std::shared_ptr<Monster>>::value_type(2, monster2_2));
	monsterMap2.insert(map<int, std::shared_ptr<Monster>>::value_type(3, monster2_3));
	monsterMap2.insert(map<int, std::shared_ptr<Monster>>::value_type(4, monster2_4));

	//room3
	auto monsterborn3_1 = room3->getObject("monsterborn1");
	auto monsterborn3_2 = room3->getObject("monsterborn2");
	auto monsterborn3_3 = room3->getObject("monsterborn3");
	auto monsterborn3_4 = room3->getObject("monsterborn4");
	float born3_X1 = monsterborn3_1["x"].asFloat();
	float born3_Y1 = monsterborn3_1["y"].asFloat();
	float born3_X2 = monsterborn3_2["x"].asFloat();
	float born3_Y2 = monsterborn3_2["y"].asFloat();
	float born3_X3 = monsterborn3_3["x"].asFloat();
	float born3_Y3 = monsterborn3_3["y"].asFloat();
	float born3_X4 = monsterborn3_4["x"].asFloat();
	float born3_Y4 = monsterborn3_4["y"].asFloat();

	auto monster3_1 = globalMonsterRepository[1]->clone();
	auto monster3_2 = globalMonsterRepository[1]->clone();
	auto monster3_3 = globalMonsterRepository[1]->clone();
	auto monster3_4 = globalMonsterRepository[1]->clone();

	monster3_1->setPosition(Vec2(born3_X1, born3_Y1));
	monster3_2->setPosition(Vec2(born3_X2, born3_Y2));
	monster3_3->setPosition(Vec2(born3_X3, born3_Y3));
	monster3_4->setPosition(Vec2(born3_X4, born3_Y4));
	monster3_1->setScale(0.3, 0.3);
	monster3_2->setScale(0.3, 0.3);
	monster3_3->setScale(0.3, 0.3);
	monster3_4->setScale(0.3, 0.3);
	monsterMap3.insert(map<int, std::shared_ptr<Monster>>::value_type(1, monster3_1));
	monsterMap3.insert(map<int, std::shared_ptr<Monster>>::value_type(2, monster3_2));
	monsterMap3.insert(map<int, std::shared_ptr<Monster>>::value_type(3, monster3_3));
	monsterMap3.insert(map<int, std::shared_ptr<Monster>>::value_type(4, monster3_4));
	
	//room4
	auto monsterborn4_1 = room4->getObject("monsterborn1");
	auto monsterborn4_2 = room4->getObject("monsterborn2");
	auto monsterborn4_3 = room4->getObject("monsterborn3");
	auto monsterborn4_4 = room4->getObject("monsterborn4");
	float born4_X1 = monsterborn4_1["x"].asFloat();
	float born4_Y1 = monsterborn4_1["y"].asFloat();
	float born4_X2 = monsterborn4_2["x"].asFloat();
	float born4_Y2 = monsterborn4_2["y"].asFloat();
	float born4_X3 = monsterborn4_3["x"].asFloat();
	float born4_Y3 = monsterborn4_3["y"].asFloat();
	float born4_X4 = monsterborn4_4["x"].asFloat();
	float born4_Y4 = monsterborn4_4["y"].asFloat();

	auto monster4_1 = globalMonsterRepository[0]->clone();
	auto monster4_2 = globalMonsterRepository[0]->clone();
	auto monster4_3 = globalMonsterRepository[0]->clone();
	auto monster4_4 = globalMonsterRepository[0]->clone();

	monster4_1->setPosition(Vec2(born4_X1, born4_Y1));
	monster4_2->setPosition(Vec2(born4_X2, born4_Y2));
	monster4_3->setPosition(Vec2(born4_X3, born4_Y3));
	monster4_4->setPosition(Vec2(born4_X4, born4_Y4));
	monster4_1->setScale(0.3, 0.3);
	monster4_2->setScale(0.3, 0.3);
	monster4_3->setScale(0.3, 0.3);
	monster4_4->setScale(0.3, 0.3);
	monsterMap4.insert(map<int, std::shared_ptr<Monster>>::value_type(1, monster4_1));
	monsterMap4.insert(map<int, std::shared_ptr<Monster>>::value_type(2, monster4_2));
	monsterMap4.insert(map<int, std::shared_ptr<Monster>>::value_type(3, monster4_3));
	monsterMap4.insert(map<int, std::shared_ptr<Monster>>::value_type(4, monster4_4));
	//room5
	auto boss = globalMonsterRepository[2]->clone();
	auto bossborn = room5->getObject("bossborn");
	float bossX = bossborn["x"].asFloat();
	float bossY = bossborn["y"].asFloat();
	boss->setPosition(Vec2(bossX, bossY));
	boss->setScale(0.5, 0.5);
	//把所有monstermap元素放到位置上去
	initMonsters();


	/////////////////////////////
	// 5. Hero 初始化										cyf
	//

	/////////////////////
	// 5.1 Hero 与 Weapon 组装								xyc
	//

	/////////////////////
	// 5.2 键盘监听（NPC与Hero对话）							cyf
	//
	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(WildMap::onKeyPressed, this);
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(WildMap::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, _tiledmap);
	/////////////////////
	// 5.2.1 键盘监听（移动）（WASD）							cyf
	//

	/////////////////////
	// 5.2.2 键盘监听（攻击）（J）							cyf
	//

	/////////////////////////////
	// 5.2.3 键盘监听（交互键）（space）						cyf
	//

	/////////////////////
	// 5.2.3 键盘监听（技能）（K）							cyf
	//

	/////////////////////
	// 5.2.4 键盘监听（换武器）（L）							xyc、cyf
	//

	/////////////////////									cyf
	// 5.3 碰撞检测
	//（进入地图块）（开始自由移动、攻击）（我方子弹与敌人）（敌方子弹与我方）（双方子弹与墙体）（出口）
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(WildMap::onContactBegin, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(WildMap::onContactSeparate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, _tiledmap);
	/////////////////////
	// 5.3.1 碰撞检测（进入地图块）							cyf
	//
	/////////////////////
	// 5.3.1.1 小怪激活（开始自由移动、攻击）					xyc
	//……


	/////////////////////
	// 5.3.2 碰撞检测（我方子弹与敌人）（敌方子弹与我方）（双方子弹与墙体）		cyf
	//
	/////////////////////
	// 5.3.3 碰撞检测（出口）									cyf
	//

	/////////////////////////////
	// 6. 属性面板初始化（Hero的血，蓝，盾以及金币，魔法币这一类）				hth
	//

	/////////////////////////////
	// 7. 菜单初始化											hth
	//

	/////////////////////////////
	// 7.1 鼠标的监听										hth
	//

	/////////////////////////////
	// 8 camera 跟随									cyf
	//
	scheduleUpdate();

	return true;
}

void WildMap::initMember() {
	globalHero->setSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName(globalHero->getHeroName() + "right.png"));
	globalHero->setToward(false);

	_interactStatus.box = 0;
	_interactStatus.conductor = 0;
	_itemIndex = 0;
	_bulletIndex = 0;
	_room2Index = 0;
	_room3Index = 0;
	_room4Index = 0;
	_room5Index = 0;
	_roomStatus = { 0,0,0,0,0,0 };

	_bulletZOrder = 0;
	_monsterZorder = 10000;
}

void WildMap::releaseMember() {
	_bulletManagement.clear();
	_itemManagement.clear();
	_room2.clear();
	_room3.clear();
	_room4.clear();
	_room5.clear();
	globalHero->removeFromParentAndCleanup(false);
}

void WildMap::initHero() {
	//hero本身physicsBody属性设置
	auto heroPhyBody = globalHero->getPhysicsBody();
	heroPhyBody->setCollisionBitmask(ENEMY | WALL | BOX);
	heroPhyBody->setContactTestBitmask(BOX | CONDUCTOR | ITEM);

	//跟随方块（用于渲染顺序及伤害判定）
	auto heroContact = DrawNode::create();
	heroContact->drawSolidRect(Vec2::ZERO, globalHero->getContentSize(),
		cocos2d::Color4F(195 / 255.0f, 176 / 255.0f, 145 / 255.0f, 1.0f));

	auto physicsBody = PhysicsBody::createBox(
		globalHero->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(HEROCONTACT);
	physicsBody->setCategoryBitmask(HEROCONTACT);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(ENEMY_BULLET | ENEMYCONTACT);
	heroContact->addComponent(physicsBody);

	globalHero->addChild(heroContact, -1);
}

void WildMap::initEnemy(std::shared_ptr<Monster> monster, int roomNum) {
	//init enemy
	monster->setAnchorPoint(Vec2(0.38, 0.1));

	auto monsterPhyBody = cocos2d::PhysicsBody::createBox(
		Size(monster->getContentSize().width, monster->getContentSize().height * 2 / 5),
		PhysicsMaterial(0.0f, 0.0f, 0.0f), Vec2(0.0f, -0.3f*monster->getContentSize().height));
	monsterPhyBody->setDynamic(true);
	monsterPhyBody->setGravityEnable(false);
	monsterPhyBody->setRotationEnable(false);
	monsterPhyBody->setTag(ENEMY);
	monsterPhyBody->setCategoryBitmask(ENEMY);
	monsterPhyBody->setCollisionBitmask(WALL | HERO | DOOR | ENEMY);
	monsterPhyBody->setContactTestBitmask(0x00);

	monster->addComponent(monsterPhyBody);

	//init enemy contact
	auto monsterContact = DrawNode::create();
	monsterContact->drawSolidRect(Vec2::ZERO, monster->getContentSize(),
		cocos2d::Color4F(195 / 255.0f, 176 / 255.0f, 145 / 255.0f, 1.0f));

	auto physicsBody = PhysicsBody::createBox(
		monster->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(ENEMYCONTACT);
	physicsBody->setCategoryBitmask(ENEMYCONTACT);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(MY_BULLET | HEROCONTACT | ENEMYCONTACT);
	monsterContact->addComponent(physicsBody);

	monster->addChild(monsterContact, -1);

	if (roomNum == 2) {
		monster->setTag(++_room2Index);
		_room2[monster->getTag()] = monster;
	}
	else if (roomNum == 3) {
		monster->setTag(++_room3Index);
		_room3[monster->getTag()] = monster;
	}
}

void WildMap::initMyBullet(std::shared_ptr<Bullet> bullet) {
	auto physicsBody = PhysicsBody::createBox(
		bullet->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setTag(MY_BULLET);
	physicsBody->setCategoryBitmask(MY_BULLET);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(DOOR | ENEMYCONTACT | WALL | BOX);

	bullet->addComponent(physicsBody);
	bullet->setTag(++_bulletIndex);
	_bulletManagement[bullet->getTag()] = bullet;
}

void WildMap::initEnemyBullet(std::shared_ptr<Bullet> bullet) {
	auto physicsBody = PhysicsBody::createBox(
		bullet->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setTag(ENEMY_BULLET);
	physicsBody->setCategoryBitmask(ENEMY_BULLET);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(DOOR | HEROCONTACT | WALL);

	bullet->addComponent(physicsBody);
	bullet->setTag(++_bulletIndex);
	_bulletManagement[bullet->getTag()] = bullet;
}

void WildMap::initWall(Sprite *wall) {
	auto physicsBody = PhysicsBody::createBox(
		wall->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(WALL);
	physicsBody->setCategoryBitmask(WALL);
	physicsBody->setCollisionBitmask(HERO | ENEMY | ITEM);
	physicsBody->setContactTestBitmask(MY_BULLET | ENEMY_BULLET);

	wall->addComponent(physicsBody);
}

void WildMap::initDoor(Sprite *door) {
	auto physicsBody = PhysicsBody::createBox(
		door->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(DOOR);
	physicsBody->setCategoryBitmask(DOOR);
	physicsBody->setCollisionBitmask(HERO | ENEMY);
	physicsBody->setContactTestBitmask(MY_BULLET | ENEMY_BULLET);

	door->setVisible(false);
	door->addComponent(physicsBody);
}

void WildMap::initBox(Sprite *box) {
	auto physicsBody = PhysicsBody::createBox(
		box->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(BOX);
	physicsBody->setCategoryBitmask(BOX);
	physicsBody->setCollisionBitmask(HERO | ITEM);
	physicsBody->setContactTestBitmask(MY_BULLET | HERO);

	box->addComponent(physicsBody);
}

void WildMap::initConductor(Sprite *conductor) {
	auto physicsBody = PhysicsBody::createBox(
		conductor->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(CONDUCTOR);
	physicsBody->setCategoryBitmask(CONDUCTOR);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(HERO);

	conductor->addComponent(physicsBody);
}

void WildMap::initItem(std::shared_ptr<Item> item) {
	auto physicsBody = PhysicsBody::createBox(
		item->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(ITEM);
	physicsBody->setCategoryBitmask(ITEM);
	physicsBody->setCollisionBitmask(WALL | BOX);
	physicsBody->setContactTestBitmask(HERO);

	item->addComponent(physicsBody);
	item->setTag(++_itemIndex);
	_itemManagement[item->getTag()] = item;
}

void WildMap::initLayer() {
	//wall
	auto layer2 = _tiledmap->getLayer("layer2");
	for (int i = 0; i < 44; ++i) {
		initWall(layer2->getTileAt(Vec2(i, 1)));
		initWall(layer2->getTileAt(Vec2(i, 110)));
	}
	for (int i = 0; i < 19; ++i) {
		initWall(layer2->getTileAt(Vec2(i, 32)));
		initWall(layer2->getTileAt(Vec2(i, 40)));
		initWall(layer2->getTileAt(Vec2(i, 71)));
		initWall(layer2->getTileAt(Vec2(i, 79)));
	}
	for (int i = 25; i < 44; ++i) {
		initWall(layer2->getTileAt(Vec2(i, 32)));
		initWall(layer2->getTileAt(Vec2(i, 40)));
		initWall(layer2->getTileAt(Vec2(i, 71)));
		initWall(layer2->getTileAt(Vec2(i, 79)));
	}
	for (int i = 53; i < 97; ++i) {
		initWall(layer2->getTileAt(Vec2(i, 40)));
		initWall(layer2->getTileAt(Vec2(i, 110)));
	}
	for (int i = 53; i < 72; ++i) {
		initWall(layer2->getTileAt(Vec2(i, 71)));
		initWall(layer2->getTileAt(Vec2(i, 79)));
	}
	for (int i = 78; i < 97; ++i) {
		initWall(layer2->getTileAt(Vec2(i, 71)));
		initWall(layer2->getTileAt(Vec2(i, 79)));
	}
	for (int i = 43; i < 54; ++i) {
		initWall(layer2->getTileAt(Vec2(i, 52)));
		initWall(layer2->getTileAt(Vec2(i, 59)));
		initWall(layer2->getTileAt(Vec2(i, 91)));
		initWall(layer2->getTileAt(Vec2(i, 98)));
	}
	for (int i = 2; i < 32; ++i) {
		initWall(layer2->getTileAt(Vec2(0, i)));
		initWall(layer2->getTileAt(Vec2(43, i)));
	}
	for (int i = 41; i < 71; ++i) {
		initWall(layer2->getTileAt(Vec2(0, i)));
		initWall(layer2->getTileAt(Vec2(96, i)));
	}
	for (int i = 80; i < 110; ++i) {
		initWall(layer2->getTileAt(Vec2(0, i)));
		initWall(layer2->getTileAt(Vec2(96, i)));
	}
	for (int i = 33; i < 40; ++i) {
		initWall(layer2->getTileAt(Vec2(18, i)));
		initWall(layer2->getTileAt(Vec2(25, i)));
	}
	for (int i = 72; i < 79; ++i) {
		initWall(layer2->getTileAt(Vec2(18, i)));
		initWall(layer2->getTileAt(Vec2(25, i)));
		initWall(layer2->getTileAt(Vec2(71, i)));
		initWall(layer2->getTileAt(Vec2(78, i)));
	}
	for (int i = 41; i < 52; ++i) {
		initWall(layer2->getTileAt(Vec2(43, i)));
		initWall(layer2->getTileAt(Vec2(53, i)));
	}
	for (int i = 60; i < 71; ++i) {
		initWall(layer2->getTileAt(Vec2(43, i)));
		initWall(layer2->getTileAt(Vec2(53, i)));
	}
	for (int i = 80; i < 91; ++i) {
		initWall(layer2->getTileAt(Vec2(43, i)));
		initWall(layer2->getTileAt(Vec2(53, i)));
	}
	for (int i = 99; i < 110; ++i) {
		initWall(layer2->getTileAt(Vec2(43, i)));
		initWall(layer2->getTileAt(Vec2(53, i)));
	}

	//door
	auto layer3 = _tiledmap->getLayer("layer3");
	for (int i = 19; i < 25; ++i) {
		initDoor(layer3->getTileAt(Vec2(i, 40)));
		initDoor(layer3->getTileAt(Vec2(i, 39)));
		initDoor(layer3->getTileAt(Vec2(i, 71)));
		initDoor(layer3->getTileAt(Vec2(i, 72)));
		initDoor(layer3->getTileAt(Vec2(i, 79)));
		initDoor(layer3->getTileAt(Vec2(i, 78)));
	}
	for (int i = 72; i < 78; ++i) {
		initDoor(layer3->getTileAt(Vec2(i, 71)));
		initDoor(layer3->getTileAt(Vec2(i, 72)));
		initDoor(layer3->getTileAt(Vec2(i, 79)));
		initDoor(layer3->getTileAt(Vec2(i, 78)));
	}
	for (int i = 52; i < 59; ++i) {
		initDoor(layer3->getTileAt(Vec2(43, i)));
		initDoor(layer3->getTileAt(Vec2(53, i)));
	}
	for (int i = 91; i < 98; ++i) {
		initDoor(layer3->getTileAt(Vec2(43, i)));
		initDoor(layer3->getTileAt(Vec2(53, i)));
	}
}

void WildMap::addconductor() {
	static bool hasAdded = false;
	if (hasAdded) {
		return;
	}
	auto conduction = Sprite::createWithSpriteFrameName("conductioncircle.png");
	initDoor(conduction);
	TMXObjectGroup* room5 = _tiledmap->getObjectGroup("room5");
	auto conX = room5->getObject("conductordoor")["x"].asFloat();
	auto conY = room5->getObject("conductordoor")["y"].asFloat();
	conduction->setPosition(Vec2(conX, conY));
	_tiledmap->addChild(conduction, 1);
	hasAdded = true;
}

bool WildMap::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	static auto heroLeft = SpriteFrameCache::getInstance()->getSpriteFrameByName(globalHero->getHeroName() + "left.png");
	static auto heroRight = SpriteFrameCache::getInstance()->getSpriteFrameByName(globalHero->getHeroName() + "right.png");

	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		globalHero->getPhysicsBody()->setVelocity(Vec2(0, globalHero->getSpeed()));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		if (!globalHero->isTowardLeft()) {
			globalHero->setSpriteFrame(heroLeft);
			globalHero->setToward(true);
		}
		globalHero->getPhysicsBody()->setVelocity(Vec2(-globalHero->getSpeed(), 0));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		globalHero->getPhysicsBody()->setVelocity(Vec2(0, -globalHero->getSpeed()));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		if (globalHero->isTowardLeft()) {
			globalHero->setSpriteFrame(heroRight);
			globalHero->setToward(false);
		}
		globalHero->getPhysicsBody()->setVelocity(Vec2(globalHero->getSpeed(), 0));
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		shoot();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_L:
		break;
	default:
		break;
	}
	return true;
}

bool WildMap::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	Vec2 velocity = globalHero->getPhysicsBody()->getVelocity();
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		interact();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		if (velocity.y > 0)
			globalHero->getPhysicsBody()->setVelocity(Vec2(velocity.x, 0));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		if (velocity.y < 0)
			globalHero->getPhysicsBody()->setVelocity(Vec2(velocity.x, 0));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		if (velocity.x < 0)
			globalHero->getPhysicsBody()->setVelocity(Vec2(0, velocity.y));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		if (velocity.x > 0)
			globalHero->getPhysicsBody()->setVelocity(Vec2(0, velocity.y));
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_L:
		break;
	default:
		break;
	}
	return true;
}

bool WildMap::onContactBegin(cocos2d::PhysicsContact& contact) {
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();

	//子弹
	if (bodyA->getTag()&MY_BULLET) {
		if (bodyB->getTag()&ENEMYCONTACT) {
			//if()
		}
		bodyA->getNode()->removeFromParentAndCleanup(true);
		_bulletManagement.erase(bodyA->getNode()->getTag());
		return true;
	}
	if (bodyB->getTag()&MY_BULLET) {
		if (bodyA->getTag()&ENEMYCONTACT) {
			//if ()
		}
		bodyB->getNode()->removeFromParentAndCleanup(true);
		_bulletManagement.erase(bodyB->getNode()->getTag());
		return true;
	}
	if (bodyA->getTag()&ENEMY_BULLET) {
		if (bodyB->getTag()&HEROCONTACT) {
			if (globalHero->beShot(_bulletManagement[bodyA->getNode()->getTag()]->getDamage())) {
				//加载死亡动画
				//回城
			}
		}
		bodyA->getNode()->removeFromParentAndCleanup(true);
		_bulletManagement.erase(bodyA->getNode()->getTag());
		return true;
	}
	if (bodyB->getTag()&ENEMY_BULLET) {
		if (bodyA->getTag()&HEROCONTACT) {
			if (globalHero->beShot(_bulletManagement[bodyB->getNode()->getTag()]->getDamage())) {
				//加载死亡动画
				//回城
			}
		}
		bodyB->getNode()->removeFromParentAndCleanup(true);
		_bulletManagement.erase(bodyB->getNode()->getTag());
		return true;
	}

	//渲染顺序
	if ((bodyA->getTag()&HEROCONTACT && bodyB->getTag()&ENEMYCONTACT) ||
		(bodyA->getTag()&ENEMYCONTACT && bodyB->getTag()&HEROCONTACT) ||
		(bodyA->getTag()&ENEMYCONTACT && bodyB->getTag()&ENEMYCONTACT)) {
		if ((bodyA->getNode()->getPositionY() < bodyB->getNode()->getPositionY() &&
			bodyA->getNode()->getLocalZOrder() < bodyB->getNode()->getLocalZOrder()) ||
			(bodyB->getNode()->getPositionY() < bodyA->getNode()->getPositionY() &&
				bodyB->getNode()->getLocalZOrder() < bodyA->getNode()->getLocalZOrder())) {
			auto tempZorder = bodyA->getNode()->getLocalZOrder();
			bodyA->getNode()->setLocalZOrder(bodyB->getNode()->getLocalZOrder());
			bodyB->getNode()->setLocalZOrder(tempZorder);
		}
		return true;
	}

	//交互
	if ((bodyA->getTag()&HERO && bodyB->getTag()&CONDUCTOR) ||
		(bodyA->getTag()&CONDUCTOR && bodyB->getTag()&HERO)) {
		_interactStatus.conductor = 1;
		return false;
	}
	if ((bodyA->getTag()&HERO && bodyB->getTag()&BOX) ||
		(bodyA->getTag()&BOX && bodyB->getTag()&HERO)) {
		_interactStatus.box = 1;
		return false;
	}
	if (bodyA->getTag()&HERO && bodyB->getTag()&ITEM) {
		auto curItem = _itemManagement[bodyB->getNode()->getTag()];
		if (curItem->getType() == Item::Type::COIN) {
			globalCoin += curItem->getValue();
			bodyB->getNode()->removeFromParentAndCleanup(true);
			_itemManagement.erase(curItem->getTag());
		}
		else {
			globalHero->getItem(curItem);
			bodyB->getNode()->removeFromParentAndCleanup(true);
			_itemManagement.erase(curItem->getTag());
		}
		return true;
	}
	if (bodyA->getTag()&ITEM && bodyB->getTag()&HERO) {
		auto curItem = _itemManagement[bodyA->getNode()->getTag()];
		if (curItem->getType() == Item::Type::COIN) {
			globalCoin += curItem->getValue();
			bodyA->getNode()->removeFromParentAndCleanup(true);
			_itemManagement.erase(curItem->getTag());
		}
		else {
			globalHero->getItem(curItem);
			bodyA->getNode()->removeFromParentAndCleanup(true);
			_itemManagement.erase(curItem->getTag());
		}
		return true;
	}

	return true;
}

bool WildMap::onContactSeparate(cocos2d::PhysicsContact& contact) {
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();

	if ((bodyA->getTag()&HERO && bodyB->getTag()&CONDUCTOR) ||
		(bodyA->getTag()&CONDUCTOR && bodyB->getTag()&HERO)) {
		_interactStatus.conductor = 0;
		return false;
	}
	if ((bodyA->getTag()&HERO && bodyB->getTag()&BOX) ||
		(bodyA->getTag()&BOX && bodyB->getTag()&HERO)) {
		_interactStatus.box = 0;
		return false;
	}
}

void WildMap::interact() {
	if (_interactStatus.conductor) {
		releaseMember();
		Director::getInstance()->pushScene(TransitionJumpZoom::create(2.0f, SecureMap::createScene()));
		return;
	}
	//NPC交互
}

void WildMap::shoot() {
	Vec2 shootDir = Vec2::ZERO;
	float distanceMIN = 100000.0f;
	if (!globalHero->shoot()) {
		return;
	}
	this->getPhysicsWorld()->queryRect(
		[&](PhysicsWorld& world, PhysicsShape& shape, void* userData)->bool {
		if (shape.getBody()->getTag()&ENEMY) {
			auto distance = shape.getCenter().distance(globalHero->getPosition());
			if (distance < distanceMIN) {
				distanceMIN = distance;
				shootDir = shape.getCenter() - globalHero->getPosition();
			}
		}
		return true;
	}, Rect::Rect(globalHero->getPositionX() - 1000, globalHero->getPositionY() - 1000, 2000, 2000), nullptr);

	auto bullet = static_cast<std::shared_ptr<Bullet>>(
		globalHero->getWeaponInstance()->getBulletInstance()->clone(false));
	initMyBullet(bullet);
	if (shootDir == Vec2::ZERO) {
		if (globalHero->isTowardLeft()) {
			shootDir = (-1, 0);
		}
		else {
			shootDir = (1, 0);
		}
	}
	else {
		shootDir.normalize();
	}
	bullet->setPosition(globalHero->getPosition());
	bullet->setRotation(shootDir.getAngle());
	bullet->getPhysicsBody()->setVelocity(bullet->getSpeed()*shootDir);
	_tiledmap->addChild(bullet.get(), 1);
}

void WildMap::positionMonitor() {
	//位置判断
	auto curHeroPosition = globalHero->getPosition();
	TMXObjectGroup* room2 = _tiledmap->getObjectGroup("room2");
	TMXObjectGroup* room3 = _tiledmap->getObjectGroup("room3");
	TMXObjectGroup* room4 = _tiledmap->getObjectGroup("room4");
	TMXObjectGroup* room5 = _tiledmap->getObjectGroup("room5");
	auto room2DL = room2->getObject("monsterborn3");
	auto room3DL = room3->getObject("monsterborn3");
	auto room4DL = room4->getObject("monsterborn3");
	auto room5DL = room5->getObject("monsterborn3");
	auto room2UR = room2->getObject("monsterborn2");
	auto room3UR = room3->getObject("monsterborn2");
	auto room4UR = room4->getObject("monsterborn2");
	auto room5UR = room5->getObject("monsterborn2");
	if (_roomStatus[2] != 2) {
		if (_room2.empty() && _roomStatus[2] == 1) {
			_roomStatus[2] = 2;
			//hero 碰撞设置
			globalHero->getPhysicsBody()->setCollisionBitmask(
				globalHero->getPhysicsBody()->getCollisionBitmask() ^ DOOR);
			//门移除
			auto layer3 = _tiledmap->getLayer("layer3");
			for (int i = 19; i < 25; ++i) {
				layer3->removeTileAt(Vec2(i, 39));
				layer3->removeTileAt(Vec2(i, 40));
				layer3->removeTileAt(Vec2(i, 71));
				layer3->removeTileAt(Vec2(i, 72));
			}
			for (int i = 52; i < 59; ++i) {
				layer3->removeTileAt(Vec2(43, i));
			}

			return;
		}
		else if (_roomStatus[2] == 0) {
			if ((globalHero->getPositionX() > room2DL["x"].asFloat() &&
				globalHero->getPositionY() > room2DL["y"].asFloat()) &&
				(globalHero->getPositionX() < room2UR["x"].asFloat() &&
					globalHero->getPositionY() < room2UR["y"].asFloat())) {
				_roomStatus[2] = 1;
				//door
				auto layer3 = _tiledmap->getLayer("layer3");
				for (int i = 19; i < 25; ++i) {
					layer3->getTileAt(Vec2(i, 39))->setVisible(true);
					layer3->getTileAt(Vec2(i, 40))->setVisible(true);
					layer3->getTileAt(Vec2(i, 71))->setVisible(true);
					layer3->getTileAt(Vec2(i, 72))->setVisible(true);
				}
				for (int i = 52; i < 59; ++i) {
					layer3->getTileAt(Vec2(43, i))->setVisible(true);
				}
				//碰撞掩码
				globalHero->getPhysicsBody()->setCollisionBitmask(
					globalHero->getPhysicsBody()->getCollisionBitmask() | DOOR);
				return;
			}
		}
	}
	if (_roomStatus[3] != 2) {
		if (_room3.empty() && _roomStatus[3] == 1) {
			_roomStatus[3] = 2;
			//door;
			//hero 碰撞设置
			globalHero->getPhysicsBody()->setCollisionBitmask(
				globalHero->getPhysicsBody()->getCollisionBitmask() ^ DOOR);
			//门移除
			auto layer3 = _tiledmap->getLayer("layer3");
			for (int i = 52; i < 59; ++i) {
				layer3->removeTileAt(Vec2(53, i));
			}
			for (int i = 72; i < 78; ++i) {
				layer3->removeTileAt(Vec2(i, 71));
				layer3->removeTileAt(Vec2(i, 72));
			}
			return;
		}
		else if (_roomStatus[3] == 0) {
			if ((globalHero->getPositionX() > room3DL["x"].asFloat() &&
				globalHero->getPositionY() > room3DL["y"].asFloat()) &&
				(globalHero->getPositionX() < room3UR["x"].asFloat() &&
					globalHero->getPositionY() < room3UR["y"].asFloat())) {
				_roomStatus[3] = 1;
				//door
				auto layer3 = _tiledmap->getLayer("layer3");
				for (int i = 52; i < 59; ++i) {
					layer3->getTileAt(Vec2(53, i))->setVisible(true);
				}
				for (int i = 72; i < 78; ++i) {
					layer3->getTileAt(Vec2(i, 71))->setVisible(true);
					layer3->getTileAt(Vec2(i, 72))->setVisible(true);
				}
				//碰撞掩码
				globalHero->getPhysicsBody()->setCollisionBitmask(
					globalHero->getPhysicsBody()->getCollisionBitmask() | DOOR);
				return;
			}
		}
	}
	if (_roomStatus[4] != 2) {
		if (_room4.empty() && _roomStatus[4] == 1) {
			_roomStatus[4] = 2;
			//door;
			//hero 碰撞设置
			globalHero->getPhysicsBody()->setCollisionBitmask(
				globalHero->getPhysicsBody()->getCollisionBitmask() ^ DOOR);
			//门移除
			auto layer3 = _tiledmap->getLayer("layer3");
			for (int i = 91; i < 98; ++i) {
				layer3->removeTileAt(Vec2(43, i));
			}
			for (int i = 19; i < 25; ++i) {
				layer3->removeTileAt(Vec2(i, 78));
				layer3->removeTileAt(Vec2(i, 79));
			}
			//加载conductor
			auto conduction = Sprite::createWithSpriteFrameName("conductioncircle.png");

			return;
		}
		else if (_roomStatus[4] == 0) {
			if ((globalHero->getPositionX() > room4DL["x"].asFloat() &&
				globalHero->getPositionY() > room4DL["y"].asFloat()) &&
				(globalHero->getPositionX() < room4UR["x"].asFloat() &&
					globalHero->getPositionY() < room4UR["y"].asFloat())) {
				_roomStatus[4] = 1;
				//door
				auto layer3 = _tiledmap->getLayer("layer3");
				for (int i = 91; i < 98; ++i) {
					layer3->getTileAt(Vec2(43, i))->setVisible(true);
				}
				for (int i = 19; i < 25; ++i) {
					layer3->getTileAt(Vec2(i, 78))->setVisible(true);
					layer3->getTileAt(Vec2(i, 79))->setVisible(true);
				}
				//碰撞掩码
				globalHero->getPhysicsBody()->setCollisionBitmask(
					globalHero->getPhysicsBody()->getCollisionBitmask() | DOOR);
				return;
			}
		}
	}
	if (_roomStatus[5] != 2) {
		if (_room5.empty() && _roomStatus[5] == 1) {
			_roomStatus[5] = 2;
			//door;
			//hero 碰撞设置
			globalHero->getPhysicsBody()->setCollisionBitmask(
				globalHero->getPhysicsBody()->getCollisionBitmask() ^ DOOR);
			//门移除
			auto layer3 = _tiledmap->getLayer("layer3");
			for (int i = 91; i < 98; ++i) {
				layer3->removeTileAt(Vec2(53, i));
			}
			for (int i = 72; i < 78; ++i) {
				layer3->removeTileAt(Vec2(i, 78));
				layer3->removeTileAt(Vec2(i, 79));
			}
			//conductor加载
			addconductor();
			return;
		}
		else if (_roomStatus[5] == 0) {
			if ((globalHero->getPositionX() > room5DL["x"].asFloat() &&
				globalHero->getPositionY() > room5DL["y"].asFloat()) &&
				(globalHero->getPositionX() < room5UR["x"].asFloat() &&
					globalHero->getPositionY() < room5UR["y"].asFloat())) {
				_roomStatus[5] = 1;
				//door
				auto layer3 = _tiledmap->getLayer("layer3");
				for (int i = 91; i < 98; ++i) {
					layer3->getTileAt(Vec2(53, i))->setVisible(true);
				}
				for (int i = 72; i < 78; ++i) {
					layer3->getTileAt(Vec2(i, 78))->setVisible(true);
					layer3->getTileAt(Vec2(i, 79))->setVisible(true);
				}
				//碰撞掩码
				globalHero->getPhysicsBody()->setCollisionBitmask(
					globalHero->getPhysicsBody()->getCollisionBitmask() | DOOR);
				return;
			}
		}
	}
}

void WildMap::update(float delta) {
	//相机跟随
	_tiledmap->setPosition(_initiativeHeroOffset + _initiativeMapOffset - globalHero->getPosition());

	//位置判断
	positionMonitor();
}

void WildMap::initMonsters()
{
	initEnemy(monsterMap2[1], 2);
	initEnemy(monsterMap2[2], 2);
	initEnemy(monsterMap2[3], 2);
	initEnemy(monsterMap2[4], 2);
	initEnemy(monsterMap3[1], 3);
	initEnemy(monsterMap3[2], 3);
	initEnemy(monsterMap3[3], 3);
	initEnemy(monsterMap3[4], 3);
	initEnemy(monsterMap4[1], 4);
	initEnemy(monsterMap4[2], 4);
	initEnemy(monsterMap4[3], 4);
	initEnemy(monsterMap4[4], 4);
	
	_tiledmap->addChild(monsterMap2[1].get(), 30);
	_tiledmap->addChild(monsterMap2[2].get(), 30);
	_tiledmap->addChild(monsterMap2[3].get(), 30);
	_tiledmap->addChild(monsterMap2[4].get(), 30);
	
	_tiledmap->addChild(monsterMap3[1].get(), 30);
	_tiledmap->addChild(monsterMap3[2].get(), 30);
	_tiledmap->addChild(monsterMap3[3].get(), 30);
	_tiledmap->addChild(monsterMap3[4].get(), 30);

	_tiledmap->addChild(monsterMap4[1].get(), 30);
	_tiledmap->addChild(monsterMap4[2].get(), 30);
	_tiledmap->addChild(monsterMap4[3].get(), 30);
	_tiledmap->addChild(monsterMap4[4].get(), 30);
}