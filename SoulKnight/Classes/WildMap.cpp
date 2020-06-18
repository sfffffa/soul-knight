#include "WildMap.h"

USING_NS_CC;

Scene* WildMap::createScene()
{
	return WildMap::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in WildMapScene.cpp\n");
}

// on "init" you need to initialize your instance
bool WildMap::init()
{
	//////////////////////////////
	// 1. 预备初始化
	//
	/////////////////////
	// 1.1 父类初始化
	//
	if (!Scene::init())
	{
		return false;
	}
	/////////////////////
	// 1.2 基础信息提取
	//
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////
	// 1.3 资源加载											hth
	//

	/////////////////////
	// 1.3.1 武器库初始化（相应子弹初始化）					hth
	//想法大概是在一个vector<std::shared_ptr<Weapon>>里提前初始化好所有武器，以供小怪爆武器时直接复制Weapon

	/////////////////////////////
	// 2. 背景初始化（不是地图）（类似于skyworld）				hth
	//

	/////////////////////////////
	// 3. 地图初始化											hth、cyf
	//

	/////////////////////////////
	// 4. 小怪（及Boss）初始化								xyc
	//在绘制地图时，额外添加一个小怪的配置文件，在进入地图初始化的时候就将小怪加载到地图上，在玩家进入战斗区域时再进入攻击模式

	/////////////////////////////
	// 5. Hero 初始化										cyf
	//

	/////////////////////
	// 5.1 Hero 与 Weapon 组装								xyc
	//

	/////////////////////
	// 5.2 键盘监听（NPC与Hero对话）							cyf
	//
	/////////////////////
	// 5.2.1 键盘监听（移动）（WASD）							cyf
	//

	/////////////////////
	// 5.2.2 键盘监听（攻击）（J）							cyf
	//

	/////////////////////////////
	// 5.2.3 键盘监听（交互键）（space）						cyf
	//

	/////////////////////
	// 5.2.3 键盘监听（技能）（K）							cyf
	//

	/////////////////////
	// 5.2.4 键盘监听（换武器）（L）							xyc、cyf
	//

	/////////////////////
	// 5.3 碰撞检测
	//
	/////////////////////
	// 5.3.1 碰撞检测（进入地图块）							cyf
	//

	/////////////////////
	// 5.3.1.1 小怪激活（开始自由移动、攻击）					xyc
	//（随机移动，跟随或远离玩家）对于可以移动的小怪：	1.玩家进入其瞄准范围（10*10），套用自动瞄准，并且移动方向跟随瞄准方向；
	//													2.玩家不在瞄准范围内，用随机数生成1-8，选择一个方向移动一定距离（5格）

	/////////////////////
	// 5.3.2 碰撞检测（我方子弹与敌人）（敌方子弹与我方）（双方子弹与墙体）		cyf
	//

	/////////////////////
	// 5.3.3 碰撞检测（出口）									cyf
	//

	/////////////////////////////
	// 6. 属性面板初始化（Hero的血，蓝，盾以及金币，魔法币这一类）				hth
	//

	/////////////////////////////
	// 7. 菜单初始化											hth
	//

	/////////////////////////////
	// 7.1 鼠标的监听										hth
	//

	/////////////////////////////
	// 8 camera 初始化										cyf
	//

	return true;
}