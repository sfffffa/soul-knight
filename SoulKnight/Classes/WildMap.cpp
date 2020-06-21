#include "WildMap.h"
#include "SecureMap.h"
#include <random>
#include <ctime>

USING_NS_CC;
using namespace std;

extern std::shared_ptr<Hero> globalHero;
extern int globalCoin;
extern std::vector<std::shared_ptr<Monster>> globalMonsterRepository;
extern std::vector<std::shared_ptr<Weapon>> globalWeaponRepository;
extern std::vector<std::shared_ptr<Boss>> globalBossRepository;
extern int bulletIndex;
extern std::map<int, std::shared_ptr<Bullet>> bulletManagement;
extern int itemIndex;
extern std::map<int, std::shared_ptr<Item>> itemManagement;

extern std::vector<char> roomStatus;
extern int room2Index;
extern std::map<int, std::shared_ptr<Monster>> room2;
extern int room3Index;
extern std::map<int, std::shared_ptr<Monster>> room3;
extern int room4Index;
extern std::map<int, std::shared_ptr<Monster>> room4;
extern int room5Index;
extern std::map<int, std::shared_ptr<Boss>> room5;

std::default_random_engine engine(static_cast<unsigned>(time(0)));

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
	//怪物图集初始化
	auto weapon1 = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[0]->clone());
	auto monster1 = Monster::createWithSpriteFrameName(
		"monster1right.png", 15, 100, 400.0f, weapon1, 0.5, 0.1, 0.2);
	monster1->setCharacterName("monster1");
	monster1->setScale(0.3f);
	monster1->addChild(weapon1.get(), 1);

	auto weapon2 = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[2]->clone());
	auto monster2 = Monster::createWithSpriteFrameName(
		"monster2right.png", 20, 50, 300.0f, weapon2, 0.3, 0.2, 0.1);
	monster2->setCharacterName("monster2");
	monster2->setScale(0.3f);
	monster2->addChild(weapon2.get(), 1);

	auto weapon3 = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[1]->clone());
	auto boss = Boss::createWithSpriteFrameName(
		"boss1right.png", 200, 1000, 800.0f, globalWeaponRepository[1], 0.0f, 0.0f, 0.3f);
	boss->setCharacterName("boss1");
	boss->setScale(0.5, 0.5);
	boss->addChild(weapon3.get(), 1);

	globalMonsterRepository.push_back(monster1);
	globalMonsterRepository.push_back(monster2);
	globalBossRepository.push_back(boss);
	//globalMonsterRepository.push_back(boss1);

	//把所有monstermap元素放到位置上去
	addMonsterInRoom();

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
	itemIndex = 0;
	bulletIndex = 0;
	room2Index = 0;
	room3Index = 0;
	room4Index = 0;
	room5Index = 0;
	roomStatus = { 0,0,0,0,0,0 };

	_bulletZOrder = 0;
	_monsterZorder = 10000;
}

void WildMap::releaseMember() {
	bulletManagement.clear();
	itemManagement.clear();
	room2.clear();
	room3.clear();
	room4.clear();
	room5.clear();
	roomStatus.clear();
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
	heroContact->setPhysicsBody(physicsBody);
	heroContact->setVisible(false);

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

	monster->setPhysicsBody(monsterPhyBody);

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
	monsterContact->setPhysicsBody(physicsBody);
	monsterContact->setVisible(false);

	monster->addChild(monsterContact, -1);

	if (roomNum == 2) {
		monster->setTag(++room2Index);
		room2[monster->getTag()] = monster;
	}
	else if (roomNum == 3) {
		monster->setTag(++room3Index);
		room3[monster->getTag()] = monster;
	}
	else if (roomNum == 4) {
		monster->setTag(++room3Index);
		room4[monster->getTag()] = monster;
	}
	/*else if (roomNum == 5) {
		monster->setTag(++room3Index);
		room5[monster->getTag()] = monster;
	}*/
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

	bullet->setPhysicsBody(physicsBody);
	bullet->setTag(++bulletIndex);
	bulletManagement[bullet->getTag()] = bullet;
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

	bullet->setPhysicsBody(physicsBody);
	bullet->setTag(++bulletIndex);
	bulletManagement[bullet->getTag()] = bullet;
}

void WildMap::initWall(Sprite *wall) {
	auto physicsBody = PhysicsBody::createBox(
		wall->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(WALL);
	physicsBody->setCategoryBitmask(WALL);
	physicsBody->setCollisionBitmask(HERO | ENEMY | ITEM);
	physicsBody->setContactTestBitmask(MY_BULLET | ENEMY_BULLET);

	wall->setPhysicsBody(physicsBody);
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
	door->setPhysicsBody(physicsBody);
}

void WildMap::initBox(Sprite *box) {
	auto physicsBody = PhysicsBody::createBox(
		box->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(BOX);
	physicsBody->setCategoryBitmask(BOX);
	physicsBody->setCollisionBitmask(HERO | ITEM);
	physicsBody->setContactTestBitmask(MY_BULLET | HERO);

	box->setPhysicsBody(physicsBody);
}

void WildMap::initConductor(Sprite *conductor) {
	auto physicsBody = PhysicsBody::createBox(
		conductor->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(CONDUCTOR);
	physicsBody->setCategoryBitmask(CONDUCTOR);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(HERO);

	conductor->setPhysicsBody(physicsBody);
}

void WildMap::initItem(std::shared_ptr<Item> item) {
	auto physicsBody = PhysicsBody::createBox(
		item->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(ITEM);
	physicsBody->setCategoryBitmask(ITEM);
	physicsBody->setCollisionBitmask(WALL | BOX);
	physicsBody->setContactTestBitmask(HERO);

	item->setPhysicsBody(physicsBody);
	item->setTag(++itemIndex);
	itemManagement[item->getTag()] = item;
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

void WildMap::addMonsterInRoom() {
	//room2
	TMXObjectGroup* roomm2 = _tiledmap->getObjectGroup("room2");
	std::uniform_int_distribution<int> room2Num(0, 10);
	auto roomm2Num = room2Num(engine);
	for (int i = 0; i <= roomm2Num; ++i) {
		auto monster = static_cast<std::shared_ptr<Monster>>(globalMonsterRepository[0]->clone());
		initEnemy(monster, 2);
		auto bornLD = roomm2->getObject("monsterborn3");
		auto bornRU = roomm2->getObject("monsterborn2");
		std::uniform_real_distribution<float> bornX(bornLD["x"].asFloat(), bornRU["x"].asFloat());
		std::uniform_real_distribution<float> bornY(bornLD["y"].asFloat(), bornRU["y"].asFloat());
		monster->setPosition(Vec2(bornX(engine), bornY(engine)));
		_tiledmap->addChild(monster.get(), ++_monsterZorder);
	}
	for (int i = 0; i < 10 - roomm2Num; ++i) {
		auto monster = static_cast<std::shared_ptr<Monster>>(globalMonsterRepository[1]->clone());
		initEnemy(monster, 2);
		auto bornLD = roomm2->getObject("monsterborn3");
		auto bornRU = roomm2->getObject("monsterborn2");
		std::uniform_real_distribution<float> bornX(bornLD["x"].asFloat(), bornRU["x"].asFloat());
		std::uniform_real_distribution<float> bornY(bornLD["y"].asFloat(), bornRU["y"].asFloat());
		monster->setPosition(Vec2(bornX(engine), bornY(engine)));
		_tiledmap->addChild(monster.get(), ++_monsterZorder);
	}

	//room3
	TMXObjectGroup* roomm3 = _tiledmap->getObjectGroup("room3");
	std::uniform_int_distribution<int> room3Num(0, 10);
	auto roomm3Num = room3Num(engine);
	for (int i = 0; i <= roomm3Num; ++i) {
		auto monster = static_cast<std::shared_ptr<Monster>>(globalMonsterRepository[0]->clone());
		initEnemy(monster, 3);
		auto bornLD = roomm3->getObject("monsterborn3");
		auto bornRU = roomm3->getObject("monsterborn2");
		std::uniform_real_distribution<float> bornX(bornLD["x"].asFloat(), bornRU["x"].asFloat());
		std::uniform_real_distribution<float> bornY(bornLD["y"].asFloat(), bornRU["y"].asFloat());
		monster->setPosition(Vec2(bornX(engine), bornY(engine)));
		_tiledmap->addChild(monster.get(), ++_monsterZorder);
	}
	for (int i = 0; i < 10 - roomm3Num; ++i) {
		auto monster = static_cast<std::shared_ptr<Monster>>(globalMonsterRepository[1]->clone());
		initEnemy(monster, 3);
		auto bornLD = roomm3->getObject("monsterborn3");
		auto bornRU = roomm3->getObject("monsterborn2");
		std::uniform_real_distribution<float> bornX(bornLD["x"].asFloat(), bornRU["x"].asFloat());
		std::uniform_real_distribution<float> bornY(bornLD["y"].asFloat(), bornRU["y"].asFloat());
		monster->setPosition(Vec2(bornX(engine), bornY(engine)));
		_tiledmap->addChild(monster.get(), ++_monsterZorder);
	}

	//room4
	TMXObjectGroup* roomm4 = _tiledmap->getObjectGroup("room4");
	std::uniform_int_distribution<int> room4Num(0, 10);
	auto roomm4Num = room4Num(engine);
	for (int i = 0; i <= roomm4Num; ++i) {
		auto monster = static_cast<std::shared_ptr<Monster>>(globalMonsterRepository[0]->clone());
		initEnemy(monster, 4);
		auto bornLD = roomm4->getObject("monsterborn3");
		auto bornRU = roomm4->getObject("monsterborn2");
		std::uniform_real_distribution<float> bornX(bornLD["x"].asFloat(), bornRU["x"].asFloat());
		std::uniform_real_distribution<float> bornY(bornLD["y"].asFloat(), bornRU["y"].asFloat());
		monster->setPosition(Vec2(bornX(engine), bornY(engine)));
		_tiledmap->addChild(monster.get(), ++_monsterZorder);
	}
	for (int i = 0; i < 10 - roomm4Num; ++i) {
		auto monster = static_cast<std::shared_ptr<Monster>>(globalMonsterRepository[1]->clone());
		initEnemy(monster, 4);
		auto bornLD = roomm4->getObject("monsterborn3");
		auto bornRU = roomm4->getObject("monsterborn2");
		std::uniform_real_distribution<float> bornX(bornLD["x"].asFloat(), bornRU["x"].asFloat());
		std::uniform_real_distribution<float> bornY(bornLD["y"].asFloat(), bornRU["y"].asFloat());
		monster->setPosition(Vec2(bornX(engine), bornY(engine)));
		_tiledmap->addChild(monster.get(), ++_monsterZorder);
	}

	//room5
	/*TMXObjectGroup* roomm5 = _tiledmap->getObjectGroup("room5");
	auto bossBorn = roomm5->getObject("bossborn");
	initEnemy(globalBossRepository[0], 5);
	globalBossRepository[0]->setPosition(Vec2(bossBorn["x"].asFloat(), bossBorn["y"].asFloat()));
	_tiledmap->addChild(globalBossRepository[0].get(), ++_monsterZorder);*/
}

bool WildMap::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	static auto heroLeft = SpriteFrameCache::getInstance()->getSpriteFrameByName(globalHero->getHeroName() + "left.png");
	static auto heroRight = SpriteFrameCache::getInstance()->getSpriteFrameByName(globalHero->getHeroName() + "right.png");

	auto weaponLeft = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getWeaponInstance()->getWeaponName() + "left.png");
	auto weaponRight = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getWeaponInstance()->getWeaponName() + "right.png");

	auto offHandWeaponLeft = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getOffhandWeaponInstance()->getWeaponName() + "left.png");
	auto offHandWeaponRight = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getOffhandWeaponInstance()->getWeaponName() + "right.png");


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

			globalHero->getWeaponInstance()->setSpriteFrame(weaponLeft);
			globalHero->getWeaponInstance()->setPosition(
				Vec2(-globalHero->getContentSize().width *0.25, globalHero->getContentSize().height / 2));
			globalHero->setToward(true);

			globalHero->getOffhandWeaponInstance()->setSpriteFrame(offHandWeaponLeft);
			globalHero->getOffhandWeaponInstance()->setPosition(
				Vec2(-globalHero->getContentSize().width *0.25, globalHero->getContentSize().height / 2));

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

			globalHero->getWeaponInstance()->setSpriteFrame(weaponRight);
			globalHero->getWeaponInstance()->setPosition(
				Vec2(globalHero->getContentSize().width, globalHero->getContentSize().height / 2));
			globalHero->setToward(false);

			globalHero->getOffhandWeaponInstance()->setSpriteFrame(offHandWeaponRight);
			globalHero->getOffhandWeaponInstance()->setPosition(
				Vec2(globalHero->getContentSize().width, globalHero->getContentSize().height / 2));

			globalHero->setToward(false);
		}
		globalHero->getPhysicsBody()->setVelocity(Vec2(globalHero->getSpeed(), 0));
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		shoot();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		changeWeaponActive();
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
			/*switch (curRoomNum) {
			case 2:
				if (room2[bodyB->getNode()->getTag()]->
					beShot(bulletManagement[bodyA->getNode()->getTag()]->getDamage())) {
					room2.erase(bodyB->getNode()->getTag());
					bodyB->getNode()->removeFromParentAndCleanup(true);
					bodyB->setTag(0);
				}break;
			case 3:
				if (room3[bodyB->getNode()->getTag()]->
					beShot(bulletManagement[bodyA->getNode()->getTag()]->getDamage())) {
					room3.erase(bodyB->getNode()->getTag());
					bodyB->getNode()->removeFromParentAndCleanup(true);
					bodyB->setTag(0);
				}break;
			case 4:
				if (room4[bodyB->getNode()->getTag()]->
					beShot(bulletManagement[bodyA->getNode()->getTag()]->getDamage())) {
					room4.erase(bodyB->getNode()->getTag());
					bodyB->getNode()->removeFromParentAndCleanup(true);
					bodyB->setTag(0);
				}break;
			}*/
		}
		bodyA->getNode()->removeFromParentAndCleanup(true);
		bulletManagement.erase(bodyA->getNode()->getTag());
		bodyA->setTag(0);
		bodyA->removeFromWorld();
		return true;
	}
	if (bodyB->getTag()&MY_BULLET) {
		if (bodyA->getTag()&ENEMYCONTACT) {
			/*switch (curRoomNum) {
			case 2:
				if (room2[bodyA->getNode()->getTag()]->
					beShot(bulletManagement[bodyB->getNode()->getTag()]->getDamage())) {
					room2.erase(bodyA->getNode()->getTag());
					bodyA->getNode()->removeFromParentAndCleanup(true);
					bodyA->setTag(0);
				}break;
			case 3:
				if (room3[bodyA->getNode()->getTag()]->
					beShot(bulletManagement[bodyB->getNode()->getTag()]->getDamage())) {
					room3.erase(bodyA->getNode()->getTag());
					bodyA->getNode()->removeFromParentAndCleanup(true);
					bodyA->setTag(0);
				}break;
			case 4:
				if (room4[bodyA->getNode()->getTag()]->
					beShot(bulletManagement[bodyB->getNode()->getTag()]->getDamage())) {
					room4.erase(bodyA->getNode()->getTag());
					bodyA->getNode()->removeFromParentAndCleanup(true);
					bodyA->setTag(0);
				}break;
			}*/
		}
		bodyB->getNode()->removeFromParentAndCleanup(true);
		bulletManagement.erase(bodyB->getNode()->getTag());
		bodyB->setTag(0);
		bodyB->removeFromWorld();
		return true;
	}
	if (bodyA->getTag()&ENEMY_BULLET) {
		if (bodyB->getTag()&HEROCONTACT) {
			if (globalHero->beShot(bulletManagement[bodyA->getNode()->getTag()]->getDamage())) {
				//加载死亡动画
				//回城
			}
		}
		bodyA->getNode()->removeFromParentAndCleanup(true);
		bulletManagement.erase(bodyA->getNode()->getTag());
		bodyA->setTag(0);
		bodyA->removeFromWorld();
		return true;
	}
	if (bodyB->getTag()&ENEMY_BULLET) {
		if (bodyA->getTag()&HEROCONTACT) {
			if (globalHero->beShot(bulletManagement[bodyB->getNode()->getTag()]->getDamage())) {
				//加载死亡动画
				//回城
			}
		}
		bodyB->getNode()->removeFromParentAndCleanup(true);
		bulletManagement.erase(bodyB->getNode()->getTag());
		bodyB->setTag(0);
		bodyB->removeFromWorld();
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
		auto curItem = itemManagement[bodyB->getNode()->getTag()];
		if (curItem->getType() == Item::Type::COIN) {
			globalCoin += curItem->getValue();
			bodyB->getNode()->removeFromParentAndCleanup(true);
			itemManagement.erase(curItem->getTag());
		}
		else {
			globalHero->getItem(curItem);
			bodyB->getNode()->removeFromParentAndCleanup(true);
			itemManagement.erase(curItem->getTag());
		}
		return true;
	}
	if (bodyA->getTag()&ITEM && bodyB->getTag()&HERO) {
		auto curItem = itemManagement[bodyA->getNode()->getTag()];
		if (curItem->getType() == Item::Type::COIN) {
			globalCoin += curItem->getValue();
			bodyA->getNode()->removeFromParentAndCleanup(true);
			itemManagement.erase(curItem->getTag());
		}
		else {
			globalHero->getItem(curItem);
			bodyA->getNode()->removeFromParentAndCleanup(true);
			itemManagement.erase(curItem->getTag());
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
			auto distance = shape.getBody()->getNode()->getPosition().distance(globalHero->getPosition());
			if (distance < distanceMIN) {
				distanceMIN = distance;
				shootDir = shape.getBody()->getNode()->getPosition() - globalHero->getPosition();
			}
		}
		return true;
	}, Rect::Rect(globalHero->getPhysicsBody()->getPosition().x - 500,
		globalHero->getPhysicsBody()->getPosition().y - 500, 1000, 1000), nullptr);

	auto bullet = static_cast<std::shared_ptr<Bullet>>(
		globalHero->getWeaponInstance()->getBulletInstance()->clone(false));
	initMyBullet(bullet);
	if (shootDir == Vec2::ZERO) {
		if (globalHero->isTowardLeft()) {
			shootDir = Vec2(-1, 0);
		}
		else {
			shootDir = Vec2(1, 0);
		}
	}
	else {
		shootDir.normalize();
	}
	bullet->setPosition(globalHero->getPosition());
	bullet->setRotation(shootDir.getAngle());
	bullet->getPhysicsBody()->setVelocity(bullet->getSpeed()*shootDir);
	_tiledmap->addChild(bullet.get(), 10000);
}

void WildMap::positionMonitor() {
	//位置判断
	auto curHeroPosition = globalHero->getPosition();
	TMXObjectGroup* roomm2 = _tiledmap->getObjectGroup("room2");
	TMXObjectGroup* roomm3 = _tiledmap->getObjectGroup("room3");
	TMXObjectGroup* roomm4 = _tiledmap->getObjectGroup("room4");
	TMXObjectGroup* roomm5 = _tiledmap->getObjectGroup("room5");
	auto room2DL = roomm2->getObject("monsterborn3");
	auto room3DL = roomm3->getObject("monsterborn3");
	auto room4DL = roomm4->getObject("monsterborn3");
	auto room5DL = roomm5->getObject("monsterborn3");
	auto room2UR = roomm2->getObject("monsterborn2");
	auto room3UR = roomm3->getObject("monsterborn2");
	auto room4UR = roomm4->getObject("monsterborn2");
	auto room5UR = roomm5->getObject("monsterborn2");
	if (roomStatus[2] != 2) {
		if (room2.empty() && roomStatus[2] == 1) {
			curRoomNum = 0;
			roomStatus[2] = 2;
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
		else if (roomStatus[2] == 0) {
			if ((globalHero->getPositionX() > room2DL["x"].asFloat() &&
				globalHero->getPositionY() > room2DL["y"].asFloat()) &&
				(globalHero->getPositionX() < room2UR["x"].asFloat() &&
					globalHero->getPositionY() < room2UR["y"].asFloat())) {
				curRoomNum = 2;
				roomStatus[2] = 1;
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
	if (roomStatus[3] != 2) {
		if (room3.empty() && roomStatus[3] == 1) {
			curRoomNum = 0;
			roomStatus[3] = 2;
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
		else if (roomStatus[3] == 0) {
			if ((globalHero->getPositionX() > room3DL["x"].asFloat() &&
				globalHero->getPositionY() > room3DL["y"].asFloat()) &&
				(globalHero->getPositionX() < room3UR["x"].asFloat() &&
					globalHero->getPositionY() < room3UR["y"].asFloat())) {
				curRoomNum = 3;
				roomStatus[3] = 1;
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
	if (roomStatus[4] != 2) {
		if (room4.empty() && roomStatus[4] == 1) {
			curRoomNum = 0;
			roomStatus[4] = 2;
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
		else if (roomStatus[4] == 0) {
			if ((globalHero->getPositionX() > room4DL["x"].asFloat() &&
				globalHero->getPositionY() > room4DL["y"].asFloat()) &&
				(globalHero->getPositionX() < room4UR["x"].asFloat() &&
					globalHero->getPositionY() < room4UR["y"].asFloat())) {
				curRoomNum = 4;
				roomStatus[4] = 1;
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
	if (roomStatus[5] != 2) {
		if (room5.empty() && roomStatus[5] == 1) {
			roomStatus[5] = 2;
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
		else if (roomStatus[5] == 0) {
			if ((globalHero->getPositionX() > room5DL["x"].asFloat() &&
				globalHero->getPositionY() > room5DL["y"].asFloat()) &&
				(globalHero->getPositionX() < room5UR["x"].asFloat() &&
					globalHero->getPositionY() < room5UR["y"].asFloat())) {
				roomStatus[5] = 1;
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

void WildMap::monstersAi()
{
	int currentRoom = 0;
	for (int i = 1; i < 6; i++)
	{
		if (roomStatus[i] == 1)
		{
			currentRoom = i;
			break;
		}
	}

	if (currentRoom == 0 || currentRoom == 1)
	{
		return;
	}

	if (currentRoom == 2)//room2
	{
		for (int i = 1; i <= 4; i++)
		{
			std::default_random_engine e(static_cast<unsigned>(time(0)));
			float Y = room2[i]->getPosition().y;
			float X = room2[i]->getPosition().x;
			float maxY = Y + 100;
			float minY = Y - 100;
			float maxX = X + 100;
			float minX = X - 100;
			int dirChosen = random(0, 1);
			if (dirChosen)
			{
				std::uniform_real_distribution<float> randomY(minY, maxY);
				room2[i]->setPosition(Vec2(room2[i]->getPosition().x, randomY(e)));
			}
			else
			{
				std::uniform_real_distribution<float> randomX(minX, maxX);
				room2[i]->setPosition(Vec2(randomX(e), room2[i]->getPosition().y));
			}
		}
	}
	if (currentRoom == 3)//room3
	{
		for (int i = 1; i <= 4; i++)
		{
			std::default_random_engine e(static_cast<unsigned>(time(0)));
			float Y = room3[i]->getPosition().y;
			float X = room3[i]->getPosition().x;
			float maxY = Y + 100;
			float minY = Y - 100;
			float maxX = X + 100;
			float minX = X - 100;
			int dirChosen = random(0, 1);
			if (dirChosen)
			{
				std::uniform_real_distribution<float> randomY(minY, maxY);
				room3[i]->setPosition(Vec2(room3[i]->getPosition().x, randomY(e)));
			}
			else
			{
				std::uniform_real_distribution<float> randomX(minX, maxX);
				room3[i]->setPosition(Vec2(randomX(e), room3[i]->getPosition().y));
			}
		}
	}
	if (currentRoom == 4)//room4
	{
		for (int i = 1; i <= 4; i++)
		{
			std::default_random_engine e(static_cast<unsigned>(time(0)));
			float Y = room4[i]->getPosition().y;
			float X = room4[i]->getPosition().x;
			float maxY = Y + 100;
			float minY = Y - 100;
			float maxX = X + 100;
			float minX = X - 100;
			int dirChosen = random(0, 1);
			if (dirChosen)
			{
				std::uniform_real_distribution<float> randomY(minY, maxY);
				room4[i]->setPosition(Vec2(room4[i]->getPosition().x, randomY(e)));
			}
			else
			{
				std::uniform_real_distribution<float> randomX(minX, maxX);
				room4[i]->setPosition(Vec2(randomX(e), room4[i]->getPosition().y));
			}
		}
	}
	if (currentRoom == 5)//room5
	{
		std::default_random_engine e(static_cast<unsigned>(time(0)));
		float Y = room5[1]->getPosition().y;
		float X = room5[1]->getPosition().x;
		float maxY = Y + 150;
		float minY = Y - 150;
		float maxX = X + 150;
		float minX = X - 150;
		int dirChosen = random(0, 1);
		if (dirChosen)
		{
			std::uniform_real_distribution<float> randomY(minY, maxY);
			room5[1]->setPosition(Vec2(room5[1]->getPosition().x, randomY(e)));
		}
		else
		{
			std::uniform_real_distribution<float> randomX(minX, maxX);
			room5[1]->setPosition(Vec2(randomX(e), room5[1]->getPosition().y));
		}
	}
}

void WildMap::changeWeaponActive()
{
	if (!globalHero->getWeaponStatus())
	{
		auto _weapon = globalHero->getWeaponInstance();
		auto _offHandWeapon = globalHero->getOffhandWeaponInstance();
		globalHero->removeChild(_weapon.get(), 1);
		globalHero->addChild(_offHandWeapon.get(), 2);
	}
	else if (globalHero->getWeaponStatus())
	{
		auto _weapon = globalHero->getWeaponInstance();
		auto _offHandWeapon = globalHero->getOffhandWeaponInstance();
		globalHero->removeChild(_offHandWeapon.get(), 2);
		globalHero->addChild(_weapon.get(), 1);
	}

	globalHero->changeWeapon();
}