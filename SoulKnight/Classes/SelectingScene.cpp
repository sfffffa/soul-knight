#include "HelloWorldScene.h"
#include "SelectingScene.h"
#include "SecureMap.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#include "class\Hero.h"
#include"memory"

USING_NS_CC;
extern std::shared_ptr<Hero> globalHero;
extern std::vector<std::shared_ptr<Weapon>> globalWeaponRepository;

Scene* SelectingScene::createScene()
{
	return SelectingScene::create();
}

bool SelectingScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("new.plist");

	//武器组装
	//bullet
	auto bulletright = Bullet::createWithSpriteFrameName("bulletright.png", 2, 2000.0f);
	auto elementalbulletright = Bullet::createWithSpriteFrameName("bluefireright.png", 2, 2000.0f);
	//weapon
	auto gun1right = RangedWeapon::createWithSpriteFrameName("gun1right.png", 0, 0.3f, 0.1f, 2.5f, bulletright);
	gun1right->setWeaponName("gun1");
	auto gun2right = RangedWeapon::createWithSpriteFrameName("gun2right.png", 2, 0.5f, 0.2f, 3.0f, bulletright);
	gun2right->setWeaponName("gun2");
	auto gun3right = RangedWeapon::createWithSpriteFrameName("gun3right.png", 4, 0.5f, 0.3f, 5.0f, bulletright);
	gun3right->setWeaponName("gun3");
	auto swordright = CloseInWeapon::createWithSpriteFrameName("swordright.png", 0, 8, 500.0f, 0.6f, 0.3f, 3.0f);
	swordright->setWeaponName("sword");
	auto blade1right = CloseInWeapon::createWithSpriteFrameName("blade1right.png", 2, 10, 550.0f, 0.6f, 0.4f, 4.0f);
	blade1right->setWeaponName("blade1");
	auto meteorhammerright = CloseInWeapon::createWithSpriteFrameName("meteorhammerright.png", 5, 25, 550.0f, 0.8f, 0.5f, 5.0f);
	meteorhammerright->setWeaponName("meteorhammer");
	auto wandright = RangedWeapon::createWithSpriteFrameName("wandright.png", 0, 0.3f, 0.1f, 2.5f, elementalbulletright);
	wandright->setWeaponName("wand");
	wandright->setAnchorPoint(Vec2(0.35, 0.5));
	wandright->setScale(3.5f);

	globalWeaponRepository.push_back(gun1right);
	globalWeaponRepository.push_back(gun2right);
	globalWeaponRepository.push_back(gun3right);
	globalWeaponRepository.push_back(swordright);
	globalWeaponRepository.push_back(blade1right);
	globalWeaponRepository.push_back(meteorhammerright);
	globalWeaponRepository.push_back(wandright);

	//场景设置
	AudioEngine::resume(0);

	auto selectLabel = Label::createWithTTF("Select Hero", "fonts/Marker Felt.ttf", 48);
	float x1 = origin.x + visibleSize.width / 2;
	float y1 = origin.y + visibleSize.height - selectLabel->getContentSize().width / 2;
	selectLabel->setPosition(Point(x1, y1));
	this->addChild(selectLabel);

	globalHero = Hero::createWithSpriteFrameName("hero1right.png");

	auto hero1 = MenuItemImage::create(
		"rawmaterials/hero1right.png", "rawmaterials/hero1right.png",
		CC_CALLBACK_1(SelectingScene::hero1Chosen, this));
	auto hero2 = MenuItemImage::create(
		"rawmaterials/hero2left.png", "rawmaterials/hero2left.png",
		CC_CALLBACK_1(SelectingScene::hero2Chosen, this));
	auto menu = Menu::create();
	menu->setPosition(0, 0);
	menu->addChild(hero1);
	menu->addChild(hero2);
	hero1->setPosition(Vec2(origin.x + visibleSize.width / 2 - hero1->getContentSize().width / 2 - 100, origin.y + visibleSize.height / 2));
	hero2->setPosition(Vec2(origin.x + visibleSize.width / 2 + hero2->getContentSize().width / 2 + 100, origin.y + visibleSize.height / 2));
	this->addChild(menu, 1);

	return true;
}

void SelectingScene::hero1Chosen(Ref* pSender)
{
	initKnight();
	Scene* securemap = SecureMap::createScene();
	Director::getInstance()->replaceScene(securemap);
}

void SelectingScene::hero2Chosen(Ref* pSender)
{
	initWizard();
	Scene* securemap = SecureMap::createScene();
	Director::getInstance()->replaceScene(securemap);
}

inline void SelectingScene::initKnight() {
	auto weaponDefault = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[0]->clone());
	auto offHandWeaponDefault = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[3]->clone());
	weaponDefault->setAnchorPoint(Vec2(0.35, 0.5));
	weaponDefault->setScale(3.5f);
	offHandWeaponDefault->setAnchorPoint(Vec2(0.35, 0.5));
	offHandWeaponDefault->setScale(3.5f);
	globalHero = Hero::createWithSpriteFrameName("hero1right.png", 8, 8, 120, 600.0f, weaponDefault, offHandWeaponDefault);
	weaponDefault->setPosition(Vec2(globalHero->getContentSize().width, globalHero->getContentSize().height / 2));
	offHandWeaponDefault->setPosition(Vec2(globalHero->getContentSize().width, globalHero->getContentSize().height / 2));
	globalHero->addChild(weaponDefault.get(), 1);
	globalHero->setHeroName("hero1");
	globalHero->setAnchorPoint(Vec2(0.38f, 0.1f));
}

inline void SelectingScene::initWizard()
{
	auto weaponDefault = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[6]->clone());
	auto offHandWeaponDefault = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[1]->clone());
	weaponDefault->setAnchorPoint(Vec2(0.35, 0.5));
	weaponDefault->setScale(3.5f);
	offHandWeaponDefault->setAnchorPoint(Vec2(0.35, 0.5));
	offHandWeaponDefault->setScale(3.5f);
	globalHero = Hero::createWithSpriteFrameName("hero2right.png", 6, 9, 200, 500.0f, weaponDefault, offHandWeaponDefault);
	weaponDefault->setPosition(Vec2(globalHero->getContentSize().width, globalHero->getContentSize().height / 2));
	globalHero->addChild(weaponDefault.get(), 1);
	globalHero->setHeroName("hero2");
	globalHero->setAnchorPoint(Vec2(0.28f, 0.1f));
}