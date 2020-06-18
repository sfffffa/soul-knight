#include "SettingScene.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;


Scene* Setting::createScene()
{
	return Setting::create();
}

bool Setting::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//auto backgroundAudioID = AudioEngine::play2d("bgm2.mp3", true,0.1f);
	AudioEngine::resume(0);

	auto back = MenuItemLabel::create(Label::createWithTTF("back", "fonts/Marker Felt.ttf", 48), CC_CALLBACK_1(Setting::menuCloseCallback, this));
	float x = origin.x + back->getContentSize().width / 2;
	float y = origin.y+visibleSize.height - back->getContentSize().height / 2;
	back->setPosition(Point(x, y));

	auto menu = Menu::create(back, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);//

	auto slider = ui::Slider::create();
	slider->loadBarTexture("slider1.png"); // what the slider looks like
	//slider->loadSlidBallTextures("slider3.png", "slider3.png", "slider3.png");
	slider->loadProgressBarTexture("slider2.png");
	slider->setScale(0.3f, 0.2f);

	slider->setPosition(Point(origin.x+visibleSize.width / 2, origin.y+visibleSize.height / 2));
	//log("%f", AudioEngine::getVolume(1));
	//slider->setPercent((int)(AudioEngine::getVolume(AudioEngine::getPlayingAudioCount()) * 100));
	slider->setPercent((int)(AudioEngine::getVolume(0) * 100));

	slider->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			printf("slider moved");
			break;
		default:
			break;
		}
	});

	/*slider->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	log("%d", (int)(AudioEngine::getVolume(AudioEngine::getPlayingAudioCount()) * 100));
	slider->setPercent((int)(AudioEngine::getVolume(AudioEngine::getPlayingAudioCount())*100));*/

	auto volume = Label::createWithTTF("Volume", "fonts/Marker Felt.ttf", 48);
	float x1 =origin.x+ visibleSize.width / 2;
	float y1 = origin.y+visibleSize.height - volume->getContentSize().height / 2;
	volume->setPosition(Point(x1, y1));
	this->addChild(volume);


	slider->addEventListener(CC_CALLBACK_2(Setting::SliderCallBack, this));
	this->addChild(slider);

	//auto map = TMXTiledMap::create("safemap.tmx");
	//addChild(map);

	/*auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("spritesheet.plist");
	auto hunter = Sprite::createWithSpriteFrameName("hunternpc.png");
	hunter->setPosition(512, 480);
	this->addChild(hunter);*/
	//map->addChild(hunter);
}

void Setting::menuCloseCallback(Ref* pSender)
{
	//AudioEngine::stopAll();
	AudioEngine::pause(0);
	//Scene* helloScene = HelloWorld::createScene();
	//Scene* replaceScreen = TransitionSlideInL::create(1.0f, helloScene);
	//Director::getInstance()->replaceScene(replaceScreen);
	Director::getInstance()->popScene();
	Director::getInstance()->resume();
	AudioEngine::resume(0);
}

void Setting::SliderCallBack(Ref *pSender, ui::Slider::EventType type)
{
	auto item = (ui::Slider*)(pSender);
	log("%i", item->getPercent());
	/*if (item->getPercent() == 100)
	{
		item->setEnabled(false);
	}
	else
	{
		AudioEngine::setVolume(0,item->getPercent() / 100.0f);
	}*/
	//AudioEngine::setVolume(AudioEngine::getPlayingAudioCount(), item->getPercent() / 600.0f);
	AudioEngine::setVolume(0, item->getPercent() / 100.0f);
}