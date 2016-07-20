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
	//����з���ʯ
	Vector <Sprite *> allAttacker;
	Vector <Sprite *> allAttacker2;
	Vector <Sprite *> allBonus;
	//��ӱ�ʯ
	void newAttacker(float t);
	void newAttacker2(float t);
	void newBonus(float t);
	//�ƶ���ʯ
	void moveAttacker(float t);
	void moveAttacker2(float t);
	void moveBonus(float t);
	//����ٶ�
	int distance;
	//�����ƶ��ٶ�
	int speed;
	//�¼����
	void update(float t);
	//����ӷֱ�ʯ
	void bonusCrystal(float t);
    //�����Ƴ�
	void killself(Node *node);
	//����ʱ��
	int time;
	//�������
	int score;
	//������߷���
	int _highScore;
	int num;
	//���ʱ��
	void timePlus(float t);
	//��ӷ���
	void scorePlus(float t);
	//�ӷֱ�ʯ
	Sprite* _Bonus;
	//����
	bool flag;
	//���������
	ProgressTimer *progress;
	Button * _btnMusic;
	Label * _labelHelp;
	void btnReStart(Ref *pSender,Widget::TouchEventType event);
    CREATE_FUNC(GameScene);
	//static create();
};
#endif // __HELLOWORLD_SCENE_H__
