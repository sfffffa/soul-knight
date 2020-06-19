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
#define ENEMY 0x0002
#define MY_BULLET 0x0004
#define ENEMY_BULLET 0x0008
#define WALL 0x0010
#define DOOR 0x0020
#define BOX 0x0040
#define NPC 0x0180
#define SPEEDNPC 0x0080
#define ASPDNPC 0x0100

extern std::shared_ptr<Hero> globalHero;
extern std::vector<std::shared_ptr<Weapon>> globalWeaponRepository;
extern std::vector<std::shared_ptr<Monster>> globalMonsterRepository;
extern int globalCoin;

#endif // _GLOBAL_VARIABLE_
