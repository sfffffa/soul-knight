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

	//加载武器库
	initGlobalWeaponRepository();

	//场景设置
	//bgm
	AudioEngine::resume(0);

	//标题
	auto selectLabel = Label::createWithTTF("Select Hero", "fonts/Marker Felt.ttf", 48);
	float x1 = origin.x + visibleSize.width / 2;
	float y1 = origin.y + visibleSize.height - selectLabel->getContentSize().width / 2;
	selectLabel->setPosition(Point(x1, y1));
	this->addChild(selectLabel);

	//选择按钮
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
	weaponDefault->setName(globalWeaponRepository[0]->getName());
	offHandWeaponDefault->setAnchorPoint(Vec2(0.35, 0.5));
	offHandWeaponDefault->setScale(3.5f);
	offHandWeaponDefault->setName(globalWeaponRepository[3]->getName());

	globalHero = Hero::createWithSpriteFrameName(
		"hero1right.png", 8, 8, 120, 600.0f, weaponDefault, offHandWeaponDefault);
	weaponDefault->setPosition(
		Vec2(globalHero->getContentSize().width, globalHero->getContentSize().height / 2));
	offHandWeaponDefault->setPosition(
		Vec2(globalHero->getContentSize().width, globalHero->getContentSize().height / 2));
	globalHero->setScale(0.3f);
	globalHero->addChild(weaponDefault.get(), 1);
	globalHero->setName("hero1");
	globalHero->setAnchorPoint(Vec2(0.38f, 0.1f));
}

inline void SelectingScene::initWizard()
{
	auto weaponDefault = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[3]->clone());
	auto offHandWeaponDefault = static_cast<std::shared_ptr<Weapon>>(globalWeaponRepository[1]->clone());
	weaponDefault->setAnchorPoint(Vec2(0.35, 0.5));
	weaponDefault->setScale(3.5f);
	weaponDefault->setName(globalWeaponRepository[3]->getName());

	offHandWeaponDefault->setAnchorPoint(Vec2(0.35, 0.5));
	offHandWeaponDefault->setScale(3.5f);
	offHandWeaponDefault->setName(globalWeaponRepository[1]->getName());

	globalHero = Hero::createWithSpriteFrameName(
		"hero2right.png", 6, 9, 200, 500.0f, weaponDefault, offHandWeaponDefault);
	weaponDefault->setPosition(
		Vec2(globalHero->getContentSize().width, globalHero->getContentSize().height / 2));
	globalHero->setScale(0.3f);
	globalHero->addChild(weaponDefault.get(), 1);
	globalHero->setName("hero2");
	globalHero->setAnchorPoint(Vec2(0.28f, 0.1f));
}