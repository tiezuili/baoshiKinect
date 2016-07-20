#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__
#include "cocos2d.h"
#include "GameScene.h"
#include "Loading.h"
#include "GameOver.h"
#include "ui\CocosGUI.h"
#include "SocketHelper.h"
USING_NS_CC;
using namespace ui;
class Loading : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
	void onEnter();
	void onExit();
	void kinectCallback(cocos2d::EventCustom * event);
    virtual bool init();
    CREATE_FUNC(Loading);
	void tiaozhuan(float t);


	//DesignerBtn
	void btnDesign(Ref * pSender,Widget::TouchEventType event);
	//music
	void btnMusic(Ref *pSender,Widget::TouchEventType event);
	//StartBtn
	void btnStart(Ref *pSender,Widget::TouchEventType event);
	//计划任务
	void Timer(float t);
	bool onTouchBegan(Touch *touch, Event *unused_event);
    void onTouchMoved(Touch *touch, Event *unused_event);
    void onTouchEnded(Touch *touch, Event *unused_event);
	Label * LabelJewel;
	Label * LabelBCU;
	Button * _btnMusic;
	Button * _btnDesign;
	bool flag;
};

#endif // __HELLOWORLD_SCENE_H__
