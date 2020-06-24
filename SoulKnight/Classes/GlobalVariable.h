#ifndef _GLOBAL_VARIABLE_
#define _GLOBAL_VARIABLE_

#include "cocos2d.h"
#include "class/Item.h"
#include "class/ElementalBullet.h"
#include "class/CloseInWeapon.h"
#include "class/RangedWeapon.h"
#include "class/Boss.h"
#include "class/Hero.h"
#include <string>
#include <vector>
#include <map>

#define HERO 0x0001
#define HERO_CONTACT 0x0002
#define ENEMY 0x0004
#define ENEMY_CONTACT 0x0008
#define BOSS 0x0010
#define BOSS_CONTACT 0x0020
#define MY_BULLET 0x0040
#define ENEMY_BULLET 0x0080
#define WALL 0x0100
#define DOOR 0x0200
#define BOX 0x0400
#define CONDUCTOR 0x0800
#define ITEM 0x1000
#define NPC 0x6000
#define NPC_SPEED 0x2000
#define NPC_ASPD 0x4000

extern int globalCoin;
extern int globalMagicStone;

extern std::shared_ptr<Hero> globalHero;//英雄

extern std::vector<std::shared_ptr<Weapon>> globalWeaponRepository;//武器库
extern std::vector<std::shared_ptr<Monster>> globalMonsterRepository;//小怪库
extern std::vector<std::shared_ptr<Boss>> globalBossRepository;//boss 库

extern int globalBulletIndex;//子弹计数器
extern std::map<int, std::shared_ptr<Bullet>> globalBulletManagement;//子弹管理器

extern int globalItemIndex;//道具计数器
extern std::map<int, std::shared_ptr<Item>> globalItemManagement;//道具管理器

extern std::map<int, char> roomStatus;
extern std::map<int, int> globalRoomsIndex;
extern std::map<int, std::map<int, std::shared_ptr<Monster>>> globalRooms;

void initGlobalWeaponRepository();
void initGlobalMonsterRepository();
void initGlobalBossRepository();

void initHero();
void initNPC(Sprite *npc);
void initEnemy(std::shared_ptr<Monster> monster, int roomNum);
void initBoss(std::shared_ptr<Boss> boss, int roomNum);
void initMyBullet(std::shared_ptr<Bullet> bullet);
void initEnemyBullet(std::shared_ptr<Bullet> bullet);
void initWall(Sprite *wall);
void initDoor(Sprite *door);
void initBox(Sprite *box);
void initConductor(Sprite *conductor);
void initItem(std::shared_ptr<Item> item);

#endif // _GLOBAL_VARIABLE_
