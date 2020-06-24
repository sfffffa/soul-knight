#include "SecureMap.h"
#include "AudioEngine.h"
#include "SettingScene.h"
#include "WildMap.h"

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
	// 1.2 成员初始化
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
	if (AudioEngine::getPlayingAudioCount())
	{
		AudioEngine::resume(0);
	}
	else
	{
		auto backgroundAudioID = AudioEngine::play2d("bgm2.mp3", true);
	}

	/////////////////////////////
	// 2. 背景初始化（不是地图）（类似于skyworld）				hth
	//
	auto background = DrawNode::create();
	background->drawSolidRect(origin, destination, cocos2d::Color4F(195 / 255.0f, 176 / 255.0f, 145 / 255.0f, 1.0f));
	this->addChild(background, -10);

	/////////////////////////////
	// 3. NPC初始化（addNPC函数）								hth 、 cyf
	//
	auto hunter = Sprite::createWithSpriteFrameName("hunter.png");
	initNPC(hunter);
	hunter->getPhysicsBody()->setTag(NPC_ASPD);
	auto oldMan = Sprite::createWithSpriteFrameName("oldman.png");
	initNPC(oldMan);
	oldMan->getPhysicsBody()->setTag(NPC_SPEED);

	/////////////////////////////
	//  4. hero 初始化										cyf
	//
	//依靠前一场景传参,此次初始化仅设置位置及physicsBody
	globalHero->setScale(0.3f);
	initHero();

	/////////////////////////////
	// 5. 地图初始化											hth 、 cyf
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

	globalHero->setPosition(Vec2(bornX, bornY));
	hunter->setPosition(Vec2(hunterX, hunterY));
	oldMan->setPosition(Vec2(oldmanX, oldmanY));

	_tiledmap->addChild(hunter, 11);
	_tiledmap->addChild(oldMan, 20);
	_tiledmap->addChild(globalHero.get(), 30);

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
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(SecureMap::onContactBegin, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(SecureMap::onContactSeparate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, _tiledmap);

	/////////////////////////////
	// 8. 属性面板初始化（Hero的血，蓝，盾以及金币，魔法币这一类）			hth
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
	// 9. 菜单初始化											hth
	//
	auto pause = MenuItemLabel::create(
		Label::createWithTTF("pause", "fonts/Marker Felt.ttf", 48), CC_CALLBACK_1(SecureMap::pausemenu, this));
	auto menu = Menu::createWithItem(pause);
	menu->setPosition(
		Vec2(origin.x + visibleSize.width - pause->getContentSize().width / 2 - 20,
			origin.y + visibleSize.height - pause->getContentSize().height / 2));
	this->addChild(menu, 300);

	/////////////////////////////
	// 10 鼠标的监听										hth
	//

	schedule(CC_SCHEDULE_SELECTOR(SecureMap::updateShield), 3.0f);
	schedule(CC_SCHEDULE_SELECTOR(SecureMap::updateHeroStatus));

	return true;
}

void SecureMap::initMember() {
	globalHero->setSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName(globalHero->getName() + "right.png"));
	globalHero->setToward(false);
	auto weaponRight = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getWeaponInstance()->getName() + "right.png");
	globalHero->getWeaponInstance()->setSpriteFrame(weaponRight);
	globalHero->getWeaponInstance()->setPosition(
		Vec2(globalHero->getContentSize().width, globalHero->getContentSize().height / 2));

	interactStatus.door = 0;
	interactStatus.hunter = 0;
	interactStatus.oldMan = 0;
}

void SecureMap::releaseMember() {
	//切换场景前的处理
	//1.删除场景内Hero的显示
	globalHero->removeFromParentAndCleanup(false);
	//2.bulletMangement 清空
	globalBulletManagement.clear();
	globalBulletIndex = 0;
	//3.itemMangement 清空
	globalItemManagement.clear();
	globalItemIndex = 0;
}

void SecureMap::initLayer() {
	//wall
	auto layer2 = _tiledmap->getLayer("layer2");
	for (int i = 3; i < 28; ++i) {
		initWall(layer2->getTileAt(Vec2(1, i)));
	}
	for (int i = 2; i < 62; ++i) {
		initWall(layer2->getTileAt(Vec2(i, 26)));
		//layer2->getTileAt(Vec2(i, 26))->setGlobalZOrder(100);
		//layer2->getTileAt(Vec2(i, 27))->setGlobalZOrder(100);
		//layer2->getTileAt(Vec2(i, 26))//->getParent()->reorderChild(layer2->getTileAt(Vec2(i, 26)), 666);
	}
	for (int i = 28; i > 3; --i) {
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
	for (int i = 3; i < 12; ++i) {
		initWall(layer6->getTileAt(Vec2(i, 4)));
	}
	for (int i = 53; i < 61; ++i) {
		initWall(layer6->getTileAt(Vec2(i, 4)));
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

	//conductor
	auto layer4 = _tiledmap->getLayer("layer4");
	initConductor(layer4->getTileAt(Vec2(31, 3)));
	initConductor(layer4->getTileAt(Vec2(32, 3)));
	initConductor(layer4->getTileAt(Vec2(33, 3)));
}

bool SecureMap::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	//Hero 左右图片获取
	static auto heroLeft = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getName() + "left.png");
	static auto heroRight = SpriteFrameCache::getInstance()->getSpriteFrameByName(
		globalHero->getName() + "right.png");

	//Hero 手拿武器对应左右图片获取
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

bool SecureMap::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
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

bool SecureMap::onContactBegin(cocos2d::PhysicsContact& contact) {
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();
	//子弹和其他
	if (bodyA->getTag()&MY_BULLET) {
		bodyA->getNode()->removeFromParentAndCleanup(true);
		globalBulletManagement.erase(bodyA->getNode()->getTag());
		bodyA->setTag(0);
		bodyA->removeFromWorld();
		return false;
	}
	if (bodyB->getTag()&MY_BULLET) {
		bodyB->getNode()->removeFromParentAndCleanup(true);
		globalBulletManagement.erase(bodyB->getNode()->getTag());
		bodyB->setTag(0);
		bodyB->removeFromWorld();
		return false;
	}
	//Hero 和传送门
	if ((bodyA->getTag()&HERO && bodyB->getTag()&CONDUCTOR) ||
		(bodyA->getTag()&CONDUCTOR && bodyB->getTag()&HERO)) {
		interactStatus.door = 1;
		return false;
	}
	//Hero与Npc
	return true;
}

bool SecureMap::onContactSeparate(cocos2d::PhysicsContact& contact) {
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();
	//Hero 和门
	if ((bodyA->getTag()&HERO && bodyB->getTag()&CONDUCTOR) ||
		(bodyA->getTag()&CONDUCTOR && bodyB->getTag()&HERO)) {
		interactStatus.door = 0;
		return false;
	}
	//Hero和NPC
}

void SecureMap::interact() {
	if (interactStatus.door) {
		releaseMember();
		Director::getInstance()->replaceScene(TransitionFade::create(1.5f, WildMap::createScene()));
	}
	//NPC交互
}

void SecureMap::shoot() {
	if (!globalHero->shoot()) {
		return;
	}
	auto haveBullet = globalHero->getWeaponInstance()->getBulletInstance();
	if (!haveBullet) {//近战
	}
	else {//远程
		auto bullet = static_cast<std::shared_ptr<Bullet>>(haveBullet->clone(false));
		initMyBullet(bullet);
		if (globalHero->isTowardLeft()) {
			bullet->setSpriteFrame(
				SpriteFrameCache::getInstance()->getSpriteFrameByName("bulletleft.png"));
			bullet->getPhysicsBody()->setVelocity(Vec2(-bullet->getSpeed(), 0));
		}
		else {
			bullet->setSpriteFrame(
				SpriteFrameCache::getInstance()->getSpriteFrameByName("bulletright.png"));
			bullet->getPhysicsBody()->setVelocity(Vec2(bullet->getSpeed(), 0));
		}
		bullet->setPosition(globalHero->getPosition());
		bullet->setAnchorPoint(haveBullet->getAnchorPoint());
		bullet->setScale(haveBullet->getScale());
		_tiledmap->addChild(bullet.get(), 11);
	}
}

void SecureMap::changeWeaponActive()
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

void SecureMap::pausemenu(cocos2d::Ref* pSender)
{
	AudioEngine::pause(0);
	Director::getInstance()->pause();
	Scene* settingScene = Setting::createScene();
	Director::getInstance()->pushScene(settingScene);
}

void SecureMap::updateShield(float delta) {
	globalHero->setShield(globalHero->getShield() + 1);
}

void SecureMap::updateHeroStatus(float delta) {
	healthBar->setPercent(100.0f*globalHero->getHP() / globalHero->getHPMax());
	shieldBar->setPercent(100.0f*globalHero->getShield() / globalHero->getShieldMax());
	magicBar->setPercent(100.0f*globalHero->getMP() / globalHero->getMPMax());
}