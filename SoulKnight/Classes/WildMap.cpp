#include "WildMap.h"
#include "SecureMap.h"

USING_NS_CC;
using namespace std;

extern std::shared_ptr<Hero> globalHero;
extern int globalCoin;

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
	auto gun1left = RangedWeapon::createWithSpriteFrameName("gun1left.png");
	auto gun1right = RangedWeapon::createWithSpriteFrameName("gun1right.png");
	auto gun2left = RangedWeapon::createWithSpriteFrameName("gun2left.png");
	auto gun2right = RangedWeapon::createWithSpriteFrameName("gun2right.png");
	auto gun3left = RangedWeapon::createWithSpriteFrameName("gun3left.png");
	auto gun3right = RangedWeapon::createWithSpriteFrameName("gun3right.png");
	auto swordleft = CloseInWeapon::createWithSpriteFrameName("swordleft.png");
	auto swordright = CloseInWeapon::createWithSpriteFrameName("swordright.png");
	auto blade1left = CloseInWeapon::createWithSpriteFrameName("blade1left.png");
	auto blade1right = CloseInWeapon::createWithSpriteFrameName("blade1right.png");
	auto meteorhammerleft = CloseInWeapon::createWithSpriteFrameName("meteorhammerleft.png");
	auto meteorhammerright = CloseInWeapon::createWithSpriteFrameName("meteorhammerright.png");
	auto wandleft = RangedWeapon::createWithSpriteFrameName("wandleft.png");
	auto wandright = RangedWeapon::createWithSpriteFrameName("wandright.png");

	auto bulletleft = Bullet::createWithSpriteFrameName("bulletleft.png");
	auto bulletright = Bullet::createWithSpriteFrameName("bulletright.png");
	auto elementalbulletleft = Bullet::createWithSpriteFrameName("bluefireleft.png");
	auto elementalbulletright = Bullet::createWithSpriteFrameName("bluefireright.png");
	/*weaponVec.push_back(gun1left);
	weaponVec.push_back(gun1right);
	weaponVec.push_back(gun2left);
	weaponVec.push_back(gun2right);
	weaponVec.push_back(gun3left);
	weaponVec.push_back(gun3right);
	weaponVec.push_back(swordleft);
	weaponVec.push_back(swordright);
	weaponVec.push_back(blade1left);
	weaponVec.push_back(blade1right);
	weaponVec.push_back(wandleft);
	weaponVec.push_back(wandright);
	weaponVec.push_back(meteorhammerleft);
	weaponVec.push_back(meteorhammerright);*/

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
		SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1right.png"));
	globalHero->setToward(false);

	_interactStatus.box = 0;
	_interactStatus.conductor = 0;
	_itemIndex = 0;
	_bulletIndex = 0;
	_room2Index = 0;
	_room3Index = 0;
	_room4Index = 0;
	_roomStatus = { 0,0,0,0,0 };

	_bulletZOrder = 0;
	_monsterZorder = 10000;
}

void WildMap::releaseMember() {
	_bulletManagement.clear();
	_itemManagement.clear();
	_room2.clear();
	_room3.clear();
	_room4.clear();
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
}

bool WildMap::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	static auto heroLeft = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1left.png");
	static auto heroRight = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1right.png");

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

void WildMap::update(float delta) {
	//相机跟随
	_tiledmap->setPosition(_initiativeHeroOffset + _initiativeMapOffset - globalHero->getPosition());

	//位置判断
}