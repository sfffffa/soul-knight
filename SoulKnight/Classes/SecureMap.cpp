#include "SecureMap.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* SecureMap::createScene()
{
	return SecureMap::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SecureMapScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SecureMap::init()
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

	/////////////////////////////
	// 2. 背景初始化（不是地图）（类似于skyworld）				hth
	//
	auto background = DrawNode::create();
	background->drawSolidRect(origin, destination, cocos2d::Color4F(195 / 255.0f, 176 / 255.0f, 145 / 255.0f, 1.0f));
	this->addChild(background, -10);

	/////////////////////////////
	// 3. NPC初始化（addNPC函数）								hth、cyf
	//
	auto hunter = initNPC("hunter.png");
	auto oldMan = initNPC("oldman.png");

	/////////////////////////////
	//  4. hero 初始化										cyf
	//
	//依靠前一场景传参,此次初始化仅设置位置及physicsBody
	_hero = Hero::createWithSpriteFrameName("hero1right.png");
	_hero->setSpeed(500.0f);
	_hero->setScale(0.3f, 0.3f);
	initHero();

	/////////////////////////////
	// 5. 地图初始化											hth、cyf
	//
	_tiledmap = TMXTiledMap::create("safemap.tmx");
	_tiledmap->setAnchorPoint(Vec2::ZERO);
	_tiledmap->setPosition(Vec2(origin.x, origin.y + 70));
	this->addChild(_tiledmap, -1);

	/////////////////////////////
	// 5.1 地图瓦片初始化											hth、cyf
	//
	initLayer();

	/////////////////////////////
	// 5.2 地图中NPC，Hero初始化											hth、cyf
	//
	TMXObjectGroup* objectGroup = _tiledmap->getObjectGroup("object");
	auto heroBornPlace = objectGroup->getObject("born");
	auto hunterBornPlace = objectGroup->getObject("hunter");
	auto oldmanBornPlace = objectGroup->getObject("oldman");

	float bornX = heroBornPlace["x"].asFloat();
	float bornY = heroBornPlace["y"].asFloat();
	float hunterX = hunterBornPlace["x"].asFloat();
	float hunterY = hunterBornPlace["y"].asFloat();
	float oldmanX = oldmanBornPlace["x"].asFloat();
	float oldmanY = oldmanBornPlace["y"].asFloat();

	_hero->setPosition(Vec2(bornX, bornY));
	hunter->setPosition(Vec2(hunterX, hunterY));
	oldMan->setPosition(Vec2(oldmanX, oldmanY));

	_tiledmap->addChild(hunter, 100);
	_tiledmap->addChild(oldMan, 100);
	_tiledmap->addChild(_hero.get(), 1000);

	/////////////////////
	// 6 键盘监听														cyf
	//（移动 WASD）（攻击 J）（交互：NPC与Hero对话 space）（技能键 K）（换武器 L）
	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(SecureMap::onKeyPressed, this);
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(SecureMap::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, _tiledmap);
	/////////////////////
	// 7 碰撞检测（NPC与Hero对话）（子弹与NPC）（子弹与墙体）	cyf
	//

	/////////////////////////////
	// 8. 属性面板初始化（Hero的血，蓝，盾以及金币，魔法币这一类）			hth
	//

	auto bloodBar = ui::Slider::create();
	bloodBar->setEnabled(false);
	bloodBar->loadBarTexture("emptybar.png");
	bloodBar->loadProgressBarTexture("fullblood.png");
	bloodBar->setPercent(100);
	bloodBar->setPosition(Vec2(origin.x + bloodBar->getContentSize().width / 2, origin.y + visibleSize.height - bloodBar->getContentSize().height / 2));
	this->addChild(bloodBar, 200);

	auto blueBar = ui::Slider::create();
	blueBar->setEnabled(false);
	blueBar->loadBarTexture("emptybar.png");
	blueBar->loadProgressBarTexture("fullblue.png");
	blueBar->setPercent(100);
	blueBar->setPosition(Vec2(origin.x + bloodBar->getContentSize().width / 2, origin.y + visibleSize.height - bloodBar->getContentSize().height - blueBar->getContentSize().height / 2 - 10));
	this->addChild(blueBar, 200);

	auto shieldBar = ui::Slider::create();
	shieldBar->setEnabled(false);
	shieldBar->loadBarTexture("emptybar.png");
	shieldBar->loadProgressBarTexture("fullshield.png");
	shieldBar->setPercent(100);
	shieldBar->setPosition(Vec2(origin.x + bloodBar->getContentSize().width / 2, origin.y + visibleSize.height - bloodBar->getContentSize().height - blueBar->getContentSize().height - shieldBar->getContentSize().height / 2 - 20));
	this->addChild(shieldBar, 200);

	/*auto bloodBar = ui::LoadingBar::create("fullblood.png");
	bloodBar->setDirection(ui::LoadingBar::Direction::RIGHT);
	bloodBar->setPercent(1.0f);
	//bloodBar->setPosition(Vec2(1024, 768));
	bloodBar->setPosition(Vec2(origin.x + bloodBar->getContentSize().width / 2, origin.y + visibleSize.height - bloodBar->getContentSize().height / 2));
	this->addChild(bloodBar, 100);*/

	/////////////////////////////
	// 9. 菜单初始化											hth
	//

	/////////////////////////////
	// 10 鼠标的监听										hth
	//

	return true;
}

Sprite *SecureMap::initNPC(const std::string& spriteFrameName) {
	auto npc = cocos2d::Sprite::createWithSpriteFrameName(spriteFrameName);

	auto physicsBody = cocos2d::PhysicsBody::createBox(
		npc->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(NPC);
	physicsBody->setCategoryBitmask(0x04);
	physicsBody->setCollisionBitmask(0x02);
	physicsBody->setContactTestBitmask(0x0A);

	npc->addComponent(physicsBody);

	npc->setScale(2.5);
	return npc;
}

void SecureMap::initHero() {
	_hero->setAnchorPoint(Vec2(0.38, 0.1));

	auto physicsBody = cocos2d::PhysicsBody::createBox(
		Size(_hero->getContentSize().width, _hero->getContentSize().height / 5),
		PhysicsMaterial(0.0f, 0.0f, 0.0f), Vec2(0.0f, -0.4f*_hero->getContentSize().height));
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setTag(HERO);
	physicsBody->setCategoryBitmask(0x02);
	physicsBody->setCollisionBitmask(0x07);
	physicsBody->setContactTestBitmask(0x04);

	_hero->addComponent(physicsBody);
}

void SecureMap::initWall(Sprite *wall) {
	auto physicsBody = PhysicsBody::createBox(
		wall->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(WALL);
	physicsBody->setCategoryBitmask(0x01);
	physicsBody->setCollisionBitmask(0x02);
	physicsBody->setContactTestBitmask(0x08);

	wall->addComponent(physicsBody);
}

void SecureMap::initLayer() {
	//wall
	auto layer2 = _tiledmap->getLayer("layer2");
	for (int i = 3; i < 26; ++i) {
		initWall(layer2->getTileAt(Vec2(1, i)));
	}
	for (int i = 1; i < 62; ++i) {
		initWall(layer2->getTileAt(Vec2(i, 26)));
	}
	for (int i = 26; i > 3; --i) {
		initWall(layer2->getTileAt(Vec2(62, i)));
	}
	for (int i = 62; i > 1; --i) {
		initWall(layer2->getTileAt(Vec2(i, 3)));
	}

	//tree
	auto layer6 = _tiledmap->getLayer("layer6");
	for (int i = 4; i < 26; ++i) {
		initWall(layer6->getTileAt(Vec2(2, i)));
		initWall(layer6->getTileAt(Vec2(61, i)));
	}

	//bookshelf
	auto layer7 = _tiledmap->getLayer("layer7");
	for (int i = 42; i < 50; ++i) {
		initWall(layer7->getTileAt(Vec2(i, 4)));
	}
	for (int i = 15; i < 23; ++i) {
		initWall(layer7->getTileAt(Vec2(i, 4)));
	}
	auto layer8 = _tiledmap->getLayer("layer8");
	initWall(layer8->getTileAt(Vec2(13, 4)));
	initWall(layer8->getTileAt(Vec2(14, 4)));
	initWall(layer8->getTileAt(Vec2(50, 4)));
	initWall(layer8->getTileAt(Vec2(51, 4)));

	//seat
	auto layer9 = _tiledmap->getLayer("layer9");
	initWall(layer9->getTileAt(Vec2(39, 4)));
	initWall(layer9->getTileAt(Vec2(40, 4)));
	initWall(layer9->getTileAt(Vec2(41, 4)));
	initWall(layer9->getTileAt(Vec2(23, 4)));
	initWall(layer9->getTileAt(Vec2(24, 4)));
	initWall(layer9->getTileAt(Vec2(25, 4)));
}

bool SecureMap::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		_hero->getPhysicsBody()->setVelocity(Vec2(0, _hero->getSpeed()));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		_hero->getPhysicsBody()->setVelocity(Vec2(-_hero->getSpeed(), 0));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		_hero->getPhysicsBody()->setVelocity(Vec2(0, -_hero->getSpeed()));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		_hero->getPhysicsBody()->setVelocity(Vec2(_hero->getSpeed(), 0));
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

bool SecureMap::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	Vec2 velocity = _hero->getPhysicsBody()->getVelocity();
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		_hero->getPhysicsBody()->setVelocity(Vec2(velocity.x, 0));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		_hero->getPhysicsBody()->setVelocity(Vec2(0, velocity.y));
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