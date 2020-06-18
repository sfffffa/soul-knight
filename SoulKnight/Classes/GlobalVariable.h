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

#define HERO 0x01
#define ENEMY 0x02
#define MY_BULLET 0x04
#define ENEMY_BULLET 0x08
#define WALL 0x10
#define DOOR 0x20
#define NPC 0x40

extern std::shared_ptr<Hero> globalHero;
extern std::vector<std::shared_ptr<Weapon>> globalWeaponRepository;
extern std::vector<std::shared_ptr<Monster>> globalMonsterRepository;
extern int globalCoin;

#endif // _GLOBAL_VARIABLE_
