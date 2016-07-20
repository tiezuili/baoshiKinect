#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Loading.h"
#include "GameOver.h"
//#include "ProgressBar.h"
#include "ui\CocosGUI.h"
USING_NS_CC;
class GameScene : public cocos2d::Layer
{	
public:
    static cocos2d::Scene* createScene();
	void onEnter();
	void kinectCallback(cocos2d::EventCustom * event);
	void onExit();
	Vec3 _perVec3;
    virtual bool init();   
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
	//music
	void btnMusic(Ref *pSender,Widget::TouchEventType event);
	//保存敌方宝石
	Vector <Sprite *> allAttacker;
	Vector <Sprite *> allAttacker2;
	Vector <Sprite *> allBonus;
	//添加宝石
	void newAttacker(float t);
	void newAttacker2(float t);
	void newBonus(float t);
	//移动宝石
	void moveAttacker(float t);
	void moveAttacker2(float t);
	void moveBonus(float t);
	//添加速度
	int distance;
	//定义移动速度
	int speed;
	//事件检测
	void update(float t);
	//定义加分宝石
	void bonusCrystal(float t);
    //自我移除
	void killself(Node *node);
	//定义时间
	int time;
	//定义分数
	int score;
	//定义最高分数
	int _highScore;
	int num;
	//添加时间
	void timePlus(float t);
	//添加分数
	void scorePlus(float t);
	//加分宝石
	Sprite* _Bonus;
	//开关
	bool flag;
	//定义进度条
	ProgressTimer *progress;
	Button * _btnMusic;
	Label * _labelHelp;
	void btnReStart(Ref *pSender,Widget::TouchEventType event);
    CREATE_FUNC(GameScene);
	//static create();
};
#endif // __HELLOWORLD_SCENE_H__
