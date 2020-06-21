#include "GlobalVariable.h"

std::shared_ptr<Hero> globalHero;
std::vector<std::shared_ptr<Weapon>> globalWeaponRepository;
std::vector<std::shared_ptr<Monster>> globalMonsterRepository;
std::vector<std::shared_ptr<Boss>> globalBossRepository;
int globalCoin = 0;

int bulletIndex = 0;
std::map<int, std::shared_ptr<Bullet>> bulletManagement;

int itemIndex = 0;
std::map<int, std::shared_ptr<Item>> itemManagement;

std::vector<char> roomStatus;
int room2Index = 0;
std::map<int, std::shared_ptr<Monster>> room2;
int room3Index = 0;
std::map<int, std::shared_ptr<Monster>> room3;
int room4Index = 0;
std::map<int, std::shared_ptr<Monster>> room4;
int room5Index = 0;
std::map<int, std::shared_ptr<Boss>> room5;