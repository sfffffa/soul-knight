#include "GlobalVariable.h"

USING_NS_CC;

int globalCoin;
int globalMagicStone;

std::shared_ptr<Hero> globalHero;//英雄
std::vector<std::shared_ptr<Weapon>> globalWeaponRepository;//武器库
std::vector<std::shared_ptr<Monster>> globalMonsterRepository;//小怪库
std::vector<std::shared_ptr<Boss>> globalBossRepository;//boss库

int globalBulletIndex;//子弹计数器
std::map<int, std::shared_ptr<Bullet>> globalBulletManagement;//子弹管理器

int globalItemIndex;//道具计数器
std::map<int, std::shared_ptr<Item>> globalItemManagement;//道具管理器

std::map<int, char> roomStatus;
std::map<int, int> globalRoomsIndex;
std::map<int, std::map<int, std::shared_ptr<Monster>>> globalRooms;

void initGlobalWeaponRepository() {
	static bool status = false;
	if (status) {
		return;
	}

	//init bullet
	auto redBullet = Bullet::createWithSpriteFrameName("bulletright.png", 2, 2000.0f);
	auto blueBullet = Bullet::createWithSpriteFrameName("bluefireright.png", 5, 1300.0f);
	blueBullet->setScale(0.75f);
	blueBullet->setAnchorPoint(Vec2(0.0f, 0.5f));

	//init ranged weapon
	auto gun1 = RangedWeapon::createWithSpriteFrameName(
		"gun1right.png", 2, 3.0f, 0.5f, 2.0f, redBullet);
	gun1->setName("gun1");
	//
	globalWeaponRepository.push_back(gun1);

	auto gun2 = RangedWeapon::createWithSpriteFrameName(
		"gun2right.png", 3, 1.0f, 1.0f, 2.0f, redBullet);
	gun2->setName("gun2");
	//
	globalWeaponRepository.push_back(gun2);

	auto gun3 = RangedWeapon::createWithSpriteFrameName(
		"gun3right.png", 2, 5.0f, 0.1f, 5.0f, redBullet);
	gun3->setName("gun3");
	//
	globalWeaponRepository.push_back(gun3);

	auto wand = RangedWeapon::createWithSpriteFrameName(
		"wandright.png", 2, 0.3f, 0.1f, 2.5f, blueBullet);
	wand->setName("wand");
	wand->setAnchorPoint(Vec2(0.35f, 0.5f));
	wand->setScale(3.5f);
	globalWeaponRepository.push_back(wand);

	//init close-in weapon!!
	auto sword = CloseInWeapon::createWithSpriteFrameName(
		"swordright.png", 0, 8, 500.0f, 0.6f, 0.3f, 3.0f);
	sword->setName("sword");
	auto blade1 = CloseInWeapon::createWithSpriteFrameName(
		"blade1right.png", 2, 10, 550.0f, 0.6f, 0.4f, 4.0f);
	blade1->setName("blade1");
	auto meteorhammer = CloseInWeapon::createWithSpriteFrameName(
		"meteorhammerright.png", 5, 25, 550.0f, 0.8f, 0.5f, 5.0f);
	meteorhammer->setName("meteorhammer");

	status = true;
}

void initGlobalMonsterRepository() {
	static bool status = false;
	if (status) {
		return;
	}

	auto weapon1 = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[0]->clone());
	auto monster1 = Monster::createWithSpriteFrameName(
		"monster1right.png", 20, 100, 400.0f, weapon1, 0.5f, 0.1f, 0.2f);
	monster1->setName("monster1");
	monster1->setScale(0.3f);
	globalMonsterRepository.push_back(monster1);

	auto weapon2 = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[0]->clone());
	auto monster2 = Monster::createWithSpriteFrameName(
		"monster2right.png", 15, 200, 300.0f, weapon2, 0.3f, 0.2f, 0.1f);
	monster2->setName("monster2");
	monster2->setScale(0.3f);
	globalMonsterRepository.push_back(monster2);

	status = true;
}

void initGlobalBossRepository() {
	static bool status = false;
	if (status) {
		return;
	}

	auto weapon3 = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[0]->clone());
	auto boss = Boss::createWithSpriteFrameName(
		"boss1right.png", 200, 1000, 800.0f, weapon3, 0.0f, 0.0f, 0.3f);
	boss->setName("boss1");
	boss->setScale(0.5f);
	globalBossRepository.push_back(boss);

	status = true;
}

void initHero() {
	static bool status = false;
	if (status) {
		return;
	}

	auto heroPhysicsBody = cocos2d::PhysicsBody::createBox(
		Size(globalHero->getContentSize().width, globalHero->getContentSize().height * 2 / 5),
		PhysicsMaterial(0.0f, 0.0f, 0.0f), Vec2(0.0f, -0.3f*globalHero->getContentSize().height));
	heroPhysicsBody->setDynamic(true);
	heroPhysicsBody->setGravityEnable(false);
	heroPhysicsBody->setRotationEnable(false);
	heroPhysicsBody->setTag(HERO);
	heroPhysicsBody->setCategoryBitmask(HERO);
	heroPhysicsBody->setCollisionBitmask(WALL | NPC | ENEMY | BOX);
	heroPhysicsBody->setContactTestBitmask(BOX | CONDUCTOR | ITEM | NPC);

	globalHero->addComponent(heroPhysicsBody);

	//跟随方块（用于渲染顺序及伤害判定）
	auto heroContact = DrawNode::create();
	heroContact->drawSolidRect(Vec2::ZERO, globalHero->getContentSize(),
		cocos2d::Color4F(195 / 255.0f, 176 / 255.0f, 145 / 255.0f, 1.0f));

	auto physicsBody = PhysicsBody::createBox(
		globalHero->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(HERO_CONTACT);
	physicsBody->setCategoryBitmask(HERO_CONTACT);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(ENEMY_BULLET | ENEMY_CONTACT);
	heroContact->setPhysicsBody(physicsBody);
	heroContact->setVisible(false);

	globalHero->addChild(heroContact, -1);

	status = true;
}

void initNPC(Sprite *npc) {
	npc->setAnchorPoint(Vec2(0.5f, 0.1f));

	auto physicsBody = cocos2d::PhysicsBody::createBox(
		npc->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(NPC);
	physicsBody->setCollisionBitmask(HERO);
	physicsBody->setContactTestBitmask(HERO | MY_BULLET);

	npc->setPhysicsBody(physicsBody);

	npc->setScale(2.5);
}

void initEnemy(std::shared_ptr<Monster> monster, int roomNum) {
	//init enemy
	monster->setAnchorPoint(Vec2(0.38f, 0.1f));

	auto monsterPhyBody = cocos2d::PhysicsBody::createBox(
		Size(monster->getContentSize().width, monster->getContentSize().height * 2 / 5),
		PhysicsMaterial(10000.0f, 0.0f, 0.0f), Vec2(0.0f, -0.3f*monster->getContentSize().height));
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
	physicsBody->setTag(ENEMY_CONTACT);
	physicsBody->setCategoryBitmask(ENEMY_CONTACT);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(MY_BULLET | HERO_CONTACT | ENEMY_CONTACT);

	monsterContact->setPhysicsBody(physicsBody);
	monsterContact->setVisible(false);

	monster->addChild(monsterContact, -1);

	monster->setTag(++globalRoomsIndex[roomNum]);
	globalRooms[roomNum][monster->getTag()] = monster;
}

void initMyBullet(std::shared_ptr<Bullet> bullet) {
	auto physicsBody = PhysicsBody::createBox(
		bullet->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setTag(MY_BULLET);
	physicsBody->setCategoryBitmask(MY_BULLET);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(DOOR | ENEMY_CONTACT | WALL | BOX);

	bullet->setPhysicsBody(physicsBody);
	bullet->setTag(++globalBulletIndex);
	globalBulletManagement[bullet->getTag()] = bullet;
}

void initEnemyBullet(std::shared_ptr<Bullet> bullet) {
	auto physicsBody = PhysicsBody::createBox(
		bullet->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setTag(ENEMY_BULLET);
	physicsBody->setCategoryBitmask(ENEMY_BULLET);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(DOOR | HERO_CONTACT | WALL);

	bullet->setPhysicsBody(physicsBody);
	bullet->setTag(++globalBulletIndex);
	globalBulletManagement[bullet->getTag()] = bullet;
}

void initWall(Sprite *wall) {
	auto physicsBody = PhysicsBody::createBox(
		wall->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(WALL);
	physicsBody->setCategoryBitmask(WALL);
	physicsBody->setCollisionBitmask(HERO | ENEMY | ITEM);
	physicsBody->setContactTestBitmask(MY_BULLET | ENEMY_BULLET);

	wall->setPhysicsBody(physicsBody);
}

void initDoor(Sprite *door) {
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

void initBox(Sprite *box) {
	auto physicsBody = PhysicsBody::createBox(
		box->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(BOX);
	physicsBody->setCategoryBitmask(BOX);
	physicsBody->setCollisionBitmask(HERO | ITEM);
	physicsBody->setContactTestBitmask(MY_BULLET | HERO);

	box->setPhysicsBody(physicsBody);
}

void initConductor(Sprite *conductor) {
	auto physicsBody = PhysicsBody::createBox(
		conductor->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(CONDUCTOR);
	physicsBody->setCategoryBitmask(CONDUCTOR);
	physicsBody->setCollisionBitmask(0x00);
	physicsBody->setContactTestBitmask(HERO);

	conductor->setPhysicsBody(physicsBody);
}

void initItem(std::shared_ptr<Item> item) {
	auto physicsBody = PhysicsBody::createBox(
		item->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setTag(ITEM);
	physicsBody->setCategoryBitmask(ITEM);
	physicsBody->setCollisionBitmask(WALL | BOX);
	physicsBody->setContactTestBitmask(HERO);

	item->setPhysicsBody(physicsBody);
	item->setTag(++globalItemIndex);
	globalItemManagement[item->getTag()] = item;
}