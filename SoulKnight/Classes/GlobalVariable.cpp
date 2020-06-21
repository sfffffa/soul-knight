#include "GlobalVariable.h"

std::shared_ptr<Hero> globalHero;
std::vector<std::shared_ptr<Weapon>> globalWeaponRepository;
std::vector<std::shared_ptr<Monster>> globalMonsterRepository;
int globalCoin = 0;

std::map<int, std::shared_ptr<Bullet>> bulletManagement;
int bulletIndex = 0;