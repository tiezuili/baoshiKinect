#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__
#include "cocos2d.h"
#include "GameScene.h"
#include "Loading.h"
#include "ui\CocosGUI.h"
USING_NS_CC;
using namespace ui;
class GameOver : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
	void btnReStart(Ref *pSender,Widget::TouchEventType event);
    CREATE_FUNC(GameOver);
};

#endif // __HELLOWORLD_SCENE_H__
