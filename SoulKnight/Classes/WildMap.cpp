#include "WildMap.h"

USING_NS_CC;
using namespace std;

extern std::shared_ptr<Hero> globalHero;

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

	/////////////////////
	// 1.2成员初始化
	//
	_hero = globalHero;
	_hero->setSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1right.png"));

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
	// 2. 背景初始化（不是地图）（类似于skyworld）				hth
	//
	auto background = DrawNode::create();
	background->drawSolidRect(origin, destination, cocos2d::Color4F(195 / 255.0f, 176 / 255.0f, 145 / 255.0f, 1.0f));
	this->addChild(background, -10);
	/////////////////////////////
	// 3. 地图初始化											hth、cyf
	//
	_tiledmap = TMXTiledMap::create("map1.tmx");
	_tiledmap->setAnchorPoint(Vec2(0, 1));
	_tiledmap->setPosition(Vec2(origin.x + 320, origin.y + visibleSize.height - 70));
	this->addChild(_tiledmap, -1);
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

	/////////////////////
	// 5.3 碰撞检测
	//（进入地图块）（开始自由移动、攻击）（我方子弹与敌人）（敌方子弹与我方）（双方子弹与墙体）（出口）
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
	// 8 camera 初始化										cyf
	//

	return true;
}

void WildMap::initBullet(std::shared_ptr<Damage> bullet) {
	auto physicsBody = PhysicsBody::createBox(
		bullet->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setTag(MY_BULLET);
	physicsBody->setCategoryBitmask(MY_BULLET);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(DOOR | NPC | WALL);

	bullet->addComponent(physicsBody);
}

void WildMap::initWall(Sprite *wall) {
	auto physicsBody = PhysicsBody::createBox(
		wall->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(WALL);
	physicsBody->setCategoryBitmask(WALL);
	physicsBody->setCollisionBitmask(HERO);
	physicsBody->setContactTestBitmask(MY_BULLET);

	wall->addComponent(physicsBody);
}

void WildMap::initLayer() {
	//wall
	auto layer2 = _tiledmap->getLayer("layer2");
}