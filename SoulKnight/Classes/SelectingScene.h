#ifndef __SELECTING_SCENE_H__
#define __SELECTING_SCENE_H__

#include "cocos2d.h"
#include "class\Hero.h"
#include"memory"
class SelectingScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	void hero1Chosen(cocos2d::Ref* pSender);
	void hero2Chosen(cocos2d::Ref* pSender);
	CREATE_FUNC(SelectingScene);
private:
	std::shared_ptr<Hero> _hero;
};
#endif //__SELECTING_SCENE_H__