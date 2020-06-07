#ifndef _SETTING_SCENE_H_
#define _SETTING_SCENE_H_

#include "cocos2d.h"

#include "ui/CocosGUI.h"

#include "AudioEngine.h"
using namespace cocos2d;
class Setting:public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	void menuCloseCallback(cocos2d::Ref* pSender);
	

	void Setting::SliderCallBack(Ref *pSender, ui::Slider::EventType type);

	CREATE_FUNC(Setting);
};


#endif 

