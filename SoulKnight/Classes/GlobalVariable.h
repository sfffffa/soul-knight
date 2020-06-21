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

#define HERO 0x0001
#define HEROCONTACT 0x0002
#define ENEMY 0x0004
#define ENEMYCONTACT 0x0008
#define MY_BULLET 0x0010
#define ENEMY_BULLET 0x0020
#define WALL 0x0040
#define DOOR 0x0080
#define BOX 0x0100
#define CONDUCTOR 0x0200
#define ITEM 0x0400
#define NPC 0x1800
#define SPEEDNPC 0x0800
#define ASPDNPC 0x1000

extern std::shared_ptr<Hero> globalHero;
extern std::vector<std::shared_ptr<Weapon>> globalWeaponRepository;
extern std::vector<std::shared_ptr<Monster>> globalMonsterRepository;
extern std::vector<std::shared_ptr<Boss>> globalBossRepository;
extern int globalCoin;

extern std::map<int, std::shared_ptr<Bullet>> bulletManagement;
extern int bulletIndex;

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

#endif // _GLOBAL_VARIABLE_
