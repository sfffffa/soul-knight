#include "HelloWorldScene.h"
#include "SelectingScene.h"
#include "SecureMap.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#include "class\Hero.h"
#include"memory"

USING_NS_CC;
extern std::shared_ptr<Hero> globalHero;


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


	/*if (AudioEngine::getPlayingAudioCount())
	{
		//AudioEngine::resume(0);
		AudioEngine::setCurrentTime(0,0);
		AudioEngine::resume(0);
	}
	else
	{
		auto backgroundAudioID = AudioEngine::play2d("bgm2.mp3", true);

	}*/
	AudioEngine::resume(0);
	
	auto selectLabel = Label::createWithTTF("Select Hero", "fonts/Marker Felt.ttf", 48);
	float x1 = origin.x+visibleSize.width / 2;
	float y1 = origin.y+visibleSize.height - selectLabel->getContentSize().width / 2;
	selectLabel->setPosition(Point(x1, y1));
	this->addChild(selectLabel);

	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("new.plist");
	
	globalHero = Hero::createWithSpriteFrameName("hero1right.png");
	
	
	auto hero1 = MenuItemImage::create(
		"hero1right.png","hero1right.png",
		CC_CALLBACK_1(SelectingScene::hero1Chosen, this));
	auto hero2 = MenuItemImage::create(
		"hero2left.png", "hero2left.png",
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
	Scene* securemap = SecureMap::createScene();
	auto hero = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1right.png");
	globalHero->setSpriteFrame(hero);
	//AudioEngine::pause(0);
	Director::getInstance()->replaceScene(securemap);

}

void SelectingScene::hero2Chosen(Ref* pSender)
{

	Scene* securemap = SecureMap::createScene();
	auto hero = SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2right.png");
	globalHero->setSpriteFrame(hero);
	//AudioEngine::pause(0);
	Director::getInstance()->replaceScene(securemap);
}