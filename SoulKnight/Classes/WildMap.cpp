#include "WildMap.h"
#include "SecureMap.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <utility>

USING_NS_CC;
using namespace std;

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
	// 3. Hero 初始化											cyf
	//

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
	initGlobalMonsterRepository();

	initGlobalBossRepository();

	//把所有monstermap元素放到位置上去
	addMonsterInRooms();

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
	healthBar = ui::Slider::create();
	healthBar->setEnabled(false);
	healthBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	healthBar->setScale(1.2f, 0.8f);
	healthBar->loadBarTexture("rawmaterials/emptybar.png");
	healthBar->loadProgressBarTexture("rawmaterials/fullblood.png");
	healthBar->setPercent(100);
	healthBar->setPosition(
		Vec2(healthBar->getContentSize().width*0.12f, healthBar->getContentSize().height*3.0f));

	magicBar = ui::Slider::create();
	magicBar->setEnabled(false);
	magicBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	magicBar->setScale(1.2f, 0.8f);
	magicBar->loadBarTexture("rawmaterials/emptybar.png");
	magicBar->loadProgressBarTexture("rawmaterials/fullblue.png");
	magicBar->setPercent(100);
	magicBar->setPosition(
		Vec2(healthBar->getContentSize().width*0.12f, healthBar->getContentSize().height*1.0f));

	shieldBar = ui::Slider::create();
	shieldBar->setScale(1.2f, 0.8f);
	shieldBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	shieldBar->setEnabled(false);
	shieldBar->loadBarTexture("rawmaterials/emptybar.png");
	shieldBar->loadProgressBarTexture("rawmaterials/fullshield.png");
	shieldBar->setPercent(100);
	shieldBar->setPosition(
		Vec2(healthBar->getContentSize().width*0.12f, healthBar->getContentSize().height*2.0f));

	auto panel = DrawNode::create();
	panel->drawSolidRect(Vec2::ZERO,
		Vec2(healthBar->getContentSize().width*1.44f,
			healthBar->getContentSize().height*4.0f),
		cocos2d::Color4F(237 / 255.0f, 189 / 255.0f, 101 / 255.0f, 1.0f));
	panel->setOpacity(220);
	panel->setAnchorPoint(Vec2(0.0f, 1.0f));
	panel->setPosition(Vec2(origin.x + 10, visibleSize.height + 62));
	this->addChild(panel, 200);

	panel->addChild(shieldBar, 10);
	panel->addChild(healthBar, 10);
	panel->addChild(magicBar, 10);

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
	schedule(CC_SCHEDULE_SELECTOR(WildMap::updateShield), 3.0f);
	schedule(CC_SCHEDULE_SELECTOR(WildMap::aiInTwoSec), 2.0f);
	schedule(CC_SCHEDULE_SELECTOR(WildMap::aiInThreeSec), 3.0f);

	return true;
}

void WildMap::initMember() {
	globalHero->setSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName(globalHero->getName() + "right.png"));
	globalHero->setToward(false);
	auto weaponRight = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getWeaponInstance()->getName() + "right.png");
	globalHero->getWeaponInstance()->setSpriteFrame(weaponRight);
	globalHero->getWeaponInstance()->setPosition(
		Vec2(globalHero->getContentSize().width, globalHero->getContentSize().height / 2));

	for (int i = 2; i < 6; ++i) {
		roomStatus[i] = 0;
		globalRoomsIndex[i] = 0;
	}

	_interactStatus.box = 0;
	_interactStatus.conductor = 0;

	_conductorHasAdded = 0;

	_bulletZOrder = 0;
	_monsterZorder = 1000;

	roomStatus.clear();
}

void WildMap::releaseMember() {
	globalBulletIndex = 0;
	std::for_each(globalBulletManagement.begin(), globalBulletManagement.end(),
		[](std::pair<int, std::shared_ptr<Bullet>> bullet) {
		bullet.second->removeFromParentAndCleanup(true);
	});
	globalBulletManagement.clear();

	globalItemIndex = 0;
	std::for_each(globalItemManagement.begin(), globalItemManagement.end(),
		[](std::pair<int, std::shared_ptr<Item>> ite) {
		ite.second->removeFromParentAndCleanup(true);
	});
	globalItemManagement.clear();

	globalRoomsIndex.clear();
	std::for_each(globalRooms.begin(), globalRooms.end(),
		[](std::pair<int, std::map<int, std::shared_ptr<Monster>>> room) {
		std::for_each(room.second.begin(), room.second.end(),
			[](std::pair<int, std::shared_ptr<Monster>> monster) {
			monster.second->removeFromParentAndCleanup(true);
		});
	});
	globalRooms.clear();
	globalHero->removeFromParentAndCleanup(false);

	globalHero->setHP(globalHero->getHPMax());
	globalHero->setShield(globalHero->getShieldMax());
	globalHero->setMP(globalHero->getMPMax());
	globalHero->getPhysicsBody()->setContactTestBitmask(BOX | CONDUCTOR | ITEM | NPC);
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
	if (_conductorHasAdded) {
		return;
	}
	auto conduction = Sprite::createWithSpriteFrameName("conductioncircle.png");
	initConductor(conduction);
	conduction->setScale(1.8f, 4.5f);
	conduction->setAnchorPoint(Vec2(0.5f, 0.3f));
	conduction->setOpacity(210);
	TMXObjectGroup* room5 = _tiledmap->getObjectGroup("room5");
	auto conX = room5->getObject("conductordoor")["x"].asFloat();
	auto conY = room5->getObject("conductordoor")["y"].asFloat();
	conduction->setPosition(Vec2(conX, conY));
	_tiledmap->addChild(conduction, 12);
	_conductorHasAdded = true;
}

void WildMap::addMonsterInRoom(int roomNum) {
	TMXObjectGroup* roomm = _tiledmap->getObjectGroup(
		static_cast<string>("room") + static_cast<char>(roomNum + '0'));
	std::uniform_int_distribution<int> roomRan(0, 10);
	auto roommNum = roomRan(engine);
	for (int i = 0; i <= roommNum; ++i) {
		auto monsterSource = globalMonsterRepository[0];

		auto monster = static_cast<std::shared_ptr<Monster>>(monsterSource->clone());
		initEnemy(monster, roomNum);

		//生成坐标
		auto bornLD = roomm->getObject("monsterborn3");
		auto bornRU = roomm->getObject("monsterborn2");
		std::uniform_real_distribution<float> bornX(bornLD["x"].asFloat(), bornRU["x"].asFloat());
		std::uniform_real_distribution<float> bornY(bornLD["y"].asFloat(), bornRU["y"].asFloat());

		monster->setPosition(Vec2(bornX(engine), bornY(engine)));
		monster->setScale(monsterSource->getScale());
		monster->setAnchorPoint(monsterSource->getAnchorPoint());
		monster->setName(monsterSource->getName());
		_tiledmap->addChild(monster.get(), ++_monsterZorder);
	}
	for (int i = 0; i < 10 - roommNum; ++i) {
		auto monsterSource = globalMonsterRepository[1];

		auto monster = static_cast<std::shared_ptr<Monster>>(monsterSource->clone());
		initEnemy(monster, roomNum);

		//生成坐标
		auto bornLD = roomm->getObject("monsterborn3");
		auto bornRU = roomm->getObject("monsterborn2");
		std::uniform_real_distribution<float> bornX(bornLD["x"].asFloat(), bornRU["x"].asFloat());
		std::uniform_real_distribution<float> bornY(bornLD["y"].asFloat(), bornRU["y"].asFloat());

		monster->setPosition(Vec2(bornX(engine), bornY(engine)));
		monster->setScale(monsterSource->getScale());
		monster->setAnchorPoint(monsterSource->getAnchorPoint());
		monster->setName(monsterSource->getName());
		_tiledmap->addChild(monster.get(), ++_monsterZorder);
	}
}

void WildMap::addMonsterInRooms() {
	//小怪房间
	for (int i = 2; i < 5; ++i) {
		addMonsterInRoom(i);
	}

	//room5
	/*TMXObjectGroup* roomm5 = _tiledmap->getObjectGroup("room5");
	auto bossBorn = roomm5->getObject("bossborn");
	initEnemy(globalBossRepository[0], 5);
	globalBossRepository[0]->setPosition(Vec2(bossBorn["x"].asFloat(), bossBorn["y"].asFloat()));
	_tiledmap->addChild(globalBossRepository[0].get(), ++_monsterZorder);*/
}

bool WildMap::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	static auto heroLeft = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getName() + "left.png");
	static auto heroRight = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getName() + "right.png");

	auto weaponLeft = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getWeaponInstance()->getName() + "left.png");
	auto weaponRight = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getWeaponInstance()->getName() + "right.png");

	auto offHandWeaponLeft = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getOffhandWeaponInstance()->getName() + "left.png");
	auto offHandWeaponRight = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getOffhandWeaponInstance()->getName() + "right.png");

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
				Vec2(-globalHero->getContentSize().width *0.25f, globalHero->getContentSize().height / 2));
			globalHero->setToward(true);

			globalHero->getOffhandWeaponInstance()->setSpriteFrame(offHandWeaponLeft);
			globalHero->getOffhandWeaponInstance()->setPosition(
				Vec2(-globalHero->getContentSize().width *0.25f, globalHero->getContentSize().height / 2));

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
		if (bodyB->getTag()&ENEMY_CONTACT) {
			if (globalRooms[curRoomNum][bodyB->getNode()->getParent()->getTag()]->
				beShot(globalBulletManagement[bodyA->getNode()->getTag()]->getDamage())) {
				bodyB->getNode()->getParent()->removeFromParentAndCleanup(true);
				globalRooms[curRoomNum].erase(bodyB->getNode()->getParent()->getTag());
				bodyB->setTag(0);
				bodyB->removeFromWorld();
			}
		}
		bodyA->getNode()->removeFromParentAndCleanup(true);
		globalBulletManagement.erase(bodyA->getNode()->getTag());
		bodyA->setTag(0);
		bodyA->removeFromWorld();
		return false;
	}
	if (bodyB->getTag()&MY_BULLET) {
		if (bodyA->getTag()&ENEMY_CONTACT) {
			if (globalRooms[curRoomNum][bodyA->getNode()->getParent()->getTag()]->
				beShot(globalBulletManagement[bodyB->getNode()->getTag()]->getDamage())) {
				bodyA->getNode()->getParent()->removeFromParentAndCleanup(true);
				globalRooms[curRoomNum].erase(bodyA->getNode()->getParent()->getTag());
				bodyA->setTag(0);
				bodyA->removeFromWorld();
			}
		}
		bodyB->getNode()->removeFromParentAndCleanup(true);
		globalBulletManagement.erase(bodyB->getNode()->getTag());
		bodyB->setTag(0);
		bodyB->removeFromWorld();
		return false;
	}
	if (bodyA->getTag()&ENEMY_BULLET) {
		if (bodyB->getTag()&HERO_CONTACT) {
			if (globalHero->beShot(globalBulletManagement[bodyA->getNode()->getTag()]->getDamage())) {
				//加载死亡动画
				globalHero->getPhysicsBody()->setCollisionBitmask(0x00);
				//回城
				releaseMember();
				Director::getInstance()->replaceScene(TransitionFade::create(2.0f, SecureMap::createScene()));
				return false;
			}
		}
		bodyA->getNode()->removeFromParentAndCleanup(true);
		globalBulletManagement.erase(bodyA->getNode()->getTag());
		bodyA->setTag(0);
		bodyA->removeFromWorld();
		return false;
	}
	if (bodyB->getTag()&ENEMY_BULLET) {
		if (bodyA->getTag()&HERO_CONTACT) {
			if (globalHero->beShot(globalBulletManagement[bodyB->getNode()->getTag()]->getDamage())) {
				//加载死亡动画
				globalHero->getPhysicsBody()->setCollisionBitmask(0x00);
				//回城
				releaseMember();
				Director::getInstance()->replaceScene(TransitionFade::create(2.0f, SecureMap::createScene()));
				return false;
			}
		}
		bodyB->getNode()->removeFromParentAndCleanup(true);
		globalBulletManagement.erase(bodyB->getNode()->getTag());
		bodyB->setTag(0);
		bodyB->removeFromWorld();
		return true;
	}

	//渲染顺序
	if ((bodyA->getTag()&HERO_CONTACT && bodyB->getTag()&ENEMY_CONTACT) ||
		(bodyA->getTag()&ENEMY_CONTACT && bodyB->getTag()&HERO_CONTACT) ||
		(bodyA->getTag()&ENEMY_CONTACT && bodyB->getTag()&ENEMY_CONTACT)) {
		if ((bodyA->getNode()->getParent()->getPositionY() < bodyB->getNode()->getParent()->getPositionY() &&
			bodyA->getNode()->getParent()->getLocalZOrder() < bodyB->getNode()->getParent()->getLocalZOrder()) ||
			(bodyB->getNode()->getParent()->getPositionY() < bodyA->getNode()->getParent()->getPositionY() &&
				bodyB->getNode()->getParent()->getLocalZOrder() < bodyA->getNode()->getParent()->getLocalZOrder())) {
			auto tempZorder = bodyA->getNode()->getParent()->getLocalZOrder();
			bodyA->getNode()->getParent()->setLocalZOrder(bodyB->getNode()->getParent()->getLocalZOrder());
			bodyB->getNode()->getParent()->setLocalZOrder(tempZorder);
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
		auto curItem = globalItemManagement[bodyB->getNode()->getTag()];
		if (curItem->getType() == Item::Type::COIN) {
			globalCoin += curItem->getValue();
			bodyB->getNode()->removeFromParentAndCleanup(true);
			globalItemManagement.erase(curItem->getTag());
		}
		else {
			globalHero->getItem(curItem);
			bodyB->getNode()->removeFromParentAndCleanup(true);
			globalItemManagement.erase(curItem->getTag());
		}
		return true;
	}
	if (bodyA->getTag()&ITEM && bodyB->getTag()&HERO) {
		auto curItem = globalItemManagement[bodyA->getNode()->getTag()];
		if (curItem->getType() == Item::Type::COIN) {
			globalCoin += curItem->getValue();
			bodyA->getNode()->removeFromParentAndCleanup(true);
			globalItemManagement.erase(curItem->getTag());
		}
		else {
			globalHero->getItem(curItem);
			bodyA->getNode()->removeFromParentAndCleanup(true);
			globalItemManagement.erase(curItem->getTag());
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
	return false;
}

void WildMap::interact() {
	if (_interactStatus.conductor) {
		releaseMember();
		Director::getInstance()->replaceScene(TransitionFade::create(2.0f, SecureMap::createScene()));
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

	auto bulletSample = globalHero->getWeaponInstance()->getBulletInstance();
	auto bullet = static_cast<std::shared_ptr<Bullet>>(
		bulletSample->clone(false));
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
	bullet->setRotation(-shootDir.getAngle() * 180 / 3.14f);
	bullet->setAnchorPoint(bulletSample->getAnchorPoint());
	bullet->setScale(bulletSample->getScale());
	bullet->getPhysicsBody()->setVelocity(bullet->getSpeed()*shootDir);
	_tiledmap->addChild(bullet.get(), 10000);
}

void WildMap::aiShoot(std::shared_ptr<Monster> monster) {
	auto redBullet = Bullet::createWithSpriteFrameName("bulletright.png", 2, 300.0f);
	Vec2 shootDir = globalHero->getPosition() - monster->getPosition();
	shootDir.normalize();
	shootDir *= 300.0f;
	initEnemyBullet(redBullet);
	redBullet->setPosition(monster->getPosition());
	redBullet->setRotation(-shootDir.getAngle() * 180 / 3.14f);
	redBullet->getPhysicsBody()->setVelocity(shootDir);
	_tiledmap->addChild(redBullet.get(), 10000);
}

void WildMap::aiMove(std::shared_ptr<Monster> monster) {
	std::uniform_real_distribution<float> X(-1.0f, 1.0f);
	auto dir = Vec2(X(engine), X(engine));
	dir.normalize();
	monster->getPhysicsBody()->setVelocity(monster->getSpeed()*dir);
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
		if (globalRooms[2].empty() && roomStatus[2] == 1) {
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
		if (globalRooms[3].empty() && roomStatus[3] == 1) {
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
		if (globalRooms[4].empty() && roomStatus[4] == 1) {
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
		if (globalRooms[5].empty() && roomStatus[5] == 1) {
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

	//状态更新
	healthBar->setPercent(100.0f*globalHero->getHP() / globalHero->getHPMax());
	shieldBar->setPercent(100.0f*globalHero->getShield() / globalHero->getShieldMax());
	magicBar->setPercent(100.0f*globalHero->getMP() / globalHero->getMPMax());
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

void WildMap::updateShield(float delta) {
	globalHero->setShield(globalHero->getShield() + 1);
}

void WildMap::aiInTwoSec(float delta) {
	if (curRoomNum < 2) {
		return;
	}
	std::uniform_real_distribution<double> action(0.0, 1.0);
	for (int i = 0; i < 6; ++i) {
		auto actionNum = action(engine);
		if (globalRooms[curRoomNum].count(i)) {
			if (actionNum < 0.2) {
				globalRooms[curRoomNum][i]->getPhysicsBody()->setVelocity(Vec2::ZERO);
			}
			else if (actionNum < 0.68) {
				aiShoot(globalRooms[curRoomNum][i]);
			}
			else {
				aiMove(globalRooms[curRoomNum][i]);
			}
		}
	}
}

void WildMap::aiInThreeSec(float delta) {
	if (curRoomNum < 2) {
		return;
	}
	std::uniform_real_distribution<double> action(0.0, 1.0);
	for (int i = 6; i < 12; ++i) {
		auto actionNum = action(engine);
		if (globalRooms[curRoomNum].count(i)) {
			if (actionNum < 0.2) {
				globalRooms[curRoomNum][i]->getPhysicsBody()->setVelocity(Vec2::ZERO);
			}
			else if (actionNum < 0.68) {
				aiShoot(globalRooms[curRoomNum][i]);
			}
			else {
				aiMove(globalRooms[curRoomNum][i]);
			}
		}
	}
}