#include "SecureMap.h"

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
	spritecache->addSpriteFramesWithFile("spritesheet.plist");

	/////////////////////////////
	// 2. 背景初始化（不是地图）（类似于skyworld）				hth
	//
	auto background = DrawNode::create();
	background->drawSolidRect(origin, destination, cocos2d::Color4F(195 / 255.0f, 176 / 255.0f, 145 / 255.0f, 1.0f));
	this->addChild(background, -10);
	/////////////////////////////
	//  NPC初始化（addNPC函数）								hth、cyf
	//
	auto hunter = initNPC("hunternpc.png");
	auto oldMan = initNPC("oldmannpc.png");

	/////////////////////////////
	//  hero 拟初始化										cyf
	//
	//依靠前一场景传参

	/////////////////////////////
	// 3. 地图初始化											hth、cyf
	//
	_tiledmap = TMXTiledMap::create("safemap.tmx");
	_tiledmap->setAnchorPoint(Vec2::ZERO);
	_tiledmap->setPosition(Vec2(origin.x, origin.y + 70));
	this->addChild(_tiledmap);

	hunter->setPosition(Vec2(origin.x+512, origin.y + visibleSize.height - 320));
	oldMan->setPosition(Vec2(origin.y + visibleSize.width-512, origin.y + visibleSize.height -320));
	_tiledmap->addChild(hunter);
	_tiledmap->addChild(oldMan);

	/////////////////////////////
	// 5. Hero初始化（Hero初始化应在上一个选择英雄场景中完成）	hth、cyf
	//    此次初始化仅设置位置及physicsBody（见拟初始化）
	//

	/////////////////////
	// 5.1 键盘监听（NPC与Hero对话）							cyf
	//
	/////////////////////
	// 5.1.1 键盘监听（移动）（WASD）							cyf
	//

	/////////////////////
	// 5.1.2 键盘监听（攻击）（J）							cyf
	//

	/////////////////////////////
	// 5.1.3 键盘监听（交互键）（space）						cyf
	//

	/////////////////////
	// 5.1.3 键盘监听（技能键不可用的设置）					cyf
	//

	/////////////////////
	// 5.1.4 键盘监听（换武器键不可用的设置）					cyf
	//

	/////////////////////
	// 5.2 碰撞检测（NPC与Hero对话）（子弹与NPC）（子弹与墙体）	cyf
	//

	/////////////////////////////
	// 6. 属性面板初始化（Hero的血，蓝，盾以及金币，魔法币这一类）			hth
	//

	/////////////////////////////
	// 7. 菜单初始化											hth
	//

	/////////////////////////////
	// 7.1 鼠标的监听										hth
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
	physicsBody->setCollisionBitmask(0x01);
	physicsBody->setContactTestBitmask(0x0A);

	npc->addComponent(physicsBody);

	return npc;
}

std::shared_ptr<Hero> SecureMap::initHero(std::shared_ptr<Hero> hero) {
	auto physicsBody = cocos2d::PhysicsBody::createBox(
		hero->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(HERO);
	physicsBody->setCategoryBitmask(0x04);
	physicsBody->setCollisionBitmask(0x01);
	physicsBody->setContactTestBitmask(0x0A);

	hero->addComponent(physicsBody);

	return hero;
}