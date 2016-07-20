#include "GameScene.h"
#include "Loading.h"
#include "GameOver.h"
#include "SimpleAudioEngine.h"
#include <iostream>
//#include "ProgressBar.h"
#define MaxSize 10
using namespace CocosDenshion;
using namespace std;
USING_NS_CC;
Scene* GameScene::createScene()
{
	auto scene=Scene::create();
	auto layer=GameScene::create();
	scene->addChild(layer);
	return scene;
	
}
void GameScene::onEnter()
{
	Layer::onEnter();
	Director::getInstance()->getEventDispatcher()->addCustomEventListener("KinectData",CC_CALLBACK_1(GameScene::kinectCallback, this));
	_highScore = UserDefault::getInstance()->getIntegerForKey("HighScore",0);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("stones_v.4_loop.mp3",true);
}
void GameScene::onExit()
{
    Layer::onExit();
}
bool GameScene::init()
{
	if(!Layer::init())
	{
		return false;
	}
	//时间
	time=0;
	//分数
	score=0;  
	//开关
	flag=true;
	//初始化移动的distance
	distance=1;
	//初始化敌方方块speed
	speed=4;
	//判断方块number
	num=1;
	//添加星空背景
	auto size=Director::getInstance()->getVisibleSize();
	auto _bg=Sprite::create("003-StarlitSky01.jpg");
	//_bg->setScaleX(0.43);
	//_bg->setScaleY(0.4);
	_bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(_bg,1);
	//添加透明层
	auto _bg2=Sprite::create("eleZameten.png");
	_bg2->setPosition(Vec2(size.width/2-4,size.height/2-5));
	_bg2->setScale(1.8);
	//_bg2->setScaleY(3);
	this->addChild(_bg2,2);
	//添加透明层2
	auto _bg3=Sprite::create("activeCell.png");
	_bg3->setPosition(Vec2(size.width/2-5,size.height/2));
	//添加透明层3
	auto layer=LayerColor::create(Color4B(255,255,255,210),size.width,size.height);
	layer->setName("layer");
	this->addChild(layer,2);
	layer->setOpacity(0);
	//透明层2Tag=150
	_bg3->setTag(150);
	_bg3->setScale(2);
	_bg3->setOpacity(10);
	this->addChild(_bg3,2);
	//添加player1
	auto _player1=Sprite::create("jewel01.png");
	_player1->setPosition(Vec2(size.width/2-50,size.height/2));
	this->addChild(_player1,1.5);
	_player1->setTag(100);
	_player1->setScale(0.09);
	//添加player2
	auto _player2=Sprite::create("jewel01.png");
	_player2->setPosition(Vec2(size.width/2+50,size.height/2));
	this->addChild(_player2,1.5);
	_player2->setTag(101);
	_player2->setScale(0.09);
	//粒子特效1
	auto particle1=ParticleSystemQuad::create("19552.plist");
	particle1->setTag(0);
	particle1->setScale(0.1);
	this->addChild(particle1,1);
	auto particle2=ParticleSystemQuad::create("19552.plist");
	particle2->setTag(1);
	particle2->setScale(0.1);
	this->addChild(particle2,1);
    //进度条背景
    Sprite *progressbgSprite;
    progressbgSprite = Sprite::create("bonusbar.png") ;
    //progressbgSprite->setAnchorPoint(ccp(0,0));//修改定点对应点
    progressbgSprite->setPosition(Vec2(size.width/2,size.height/3-140));
    this->addChild(progressbgSprite,3);
    progressbgSprite->setVisible(true);
    //进度条正面
    Sprite *progressSprite = Sprite::create("bonusbar_fill.png");
    progress = ProgressTimer::create(progressSprite);
    //创建进度条.
    //progress->setAnchorPoint(ccp(0,0));
    progress->setType(kCCProgressTimerTypeBar);  //中间为从左向右的进度条，类型为水平
    progress->setPosition(Vec2(size.width/2,size.height/3-140));
    //进度动画运动方向
    progress->setMidpoint(ccp(0,0));
    //进度条宽高变化
    progress->setBarChangeRate(ccp(1,0)); //设置进度条为从左向右随进度增长而显现
    progress->setPercentage(100.0f);//值
    this->addChild(progress,3);
    progress->setVisible(true);
	//分数
	auto _labelScore=Label::createWithSystemFont("分数:0","DFGirl",40);
	_labelScore->setName("_labelScore");
	_labelScore->setPosition(Vec2(size.width/2-20,size.height/2+100));
	_labelScore->setColor(Color3B::BLUE);
	_labelScore->setPosition(Vec2(size.width/2,50));
	this->addChild(_labelScore,3);
	//music
	_btnMusic=Button::create("btnMusic.png");
	_btnMusic->setTag(200);
	_btnMusic->addTouchEventListener(CC_CALLBACK_2(GameScene::btnMusic,this));
	_btnMusic->setPosition(Vec2(20,size.height-20));
	//_btnMusic->setPressedActionEnabled(true);
	_btnMusic->setScale(0.22);
	_btnMusic->setOpacity(200);
	this->addChild(_btnMusic,1);
	bool isMusicOn=UserDefault::getInstance()->getBoolForKey("isMusicOn",true);
	if (!isMusicOn)
	{
		_btnMusic->loadTextureNormal("btnMusic_off.png");
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
		SimpleAudioEngine::getInstance()->setEffectsVolume(0);
	}
	//滚动字幕
	_labelHelp=Label::createWithSystemFont("控制屏幕中的宝石来躲避移动的陨石,每碰一回,控制的宝石就会被放大,并且屏幕就会变模糊;可通过吃钻石来得分.如果进度条为0%,那么游戏结束.祝您游戏愉快.","DFGirl",30);
	_labelHelp->setPosition(Vec2(size.width+1100,size.height-70));
	this->addChild(_labelHelp,4);
	_labelHelp->setTextColor(Color4B::GREEN);
	//bunding
	
	//添加触摸监听
	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(GameScene::onTouchBegan,this);
	listener->onTouchMoved=CC_CALLBACK_2(GameScene::onTouchMoved,this);
	listener->onTouchEnded=CC_CALLBACK_2(GameScene::onTouchEnded,this);
	//bunding
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	//添加敌方方块
	this->schedule(schedule_selector(GameScene::newAttacker),speed);
	this->schedule(schedule_selector(GameScene::newAttacker2),speed);
	this->schedule(schedule_selector(GameScene::newBonus),10);
	//移动敌方方块
	this->schedule(schedule_selector(GameScene::moveAttacker),0.01);
	this->schedule(schedule_selector(GameScene::moveAttacker2),0.01);
	//添加时间
	this->schedule(schedule_selector(GameScene::timePlus),1);
	//添加分数
	this->schedule(schedule_selector(GameScene::scorePlus),0.01);
	//加分宝石
	this->schedule(schedule_selector(GameScene::moveBonus),0.01);
	this->scheduleUpdate();
	return true;
}
void GameScene::kinectCallback(cocos2d::EventCustom * event)
{
    auto vecMap =*static_cast<std::map<std::string,Vec3> *>(event->getUserData());
    auto pos = vecMap["nearstHand"];   
    if (progress->getPercentage()<=0)
    {
        //最后取消自定义事件
        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("KinectData");
		auto scene = GameScene::createScene();
        auto actionScene = TransitionCrossFade::create(0.5, scene);
        Director::getInstance()->replaceScene(actionScene);
    }
    else
    {
		auto vecPre = Vec2(this->getChildByTag(100)->getPositionX(),this->getChildByTag(100)->getPositionY());
		auto vecPre2 = Vec2(this->getChildByTag(101)->getPositionX(),this->getChildByTag(101)->getPositionY());
        auto vecNow = Vec2(pos.x,pos.y);
		//auto vecNow2 = Vec2(pos.x,pos.y);
		auto offset1=Vec2(vecPre-vecNow);
		auto offset2=Vec2(vecNow-vecPre2);
		auto next1=this->getChildByTag(100)->getPosition()-offset1;
		auto next2=this->getChildByTag(101)->getPosition()-offset2;
		this->getChildByTag(100)->setPosition(next1);
		this->getChildByTag(101)->setPosition(next2);
		this->getChildByTag(0)->setPosition(next1);
		this->getChildByTag(1)->setPosition(next2);
	if (next1.x>=280)
	{
		next1.x=280;
	}
	if (next1.x<=40)
	{
		next1.x=40;
	}
	if (next1.y<=120)
	{
		next1.y=120;
	}
	if (next1.y>=355)
	{
		next1.y=355;
	}
	this->getChildByTag(100)->setPosition(next1);
	if (next2.x>=280)
	{
		next2.x=280;
	}
	if (next2.x<=40)
	{
		next2.x=40;
	}
	if (next2.y<=120)
	{
		next2.y=120;
	}
	if (next2.y>=355)
	{
		next2.y=355;
	}
	this->getChildByTag(101)->setPosition(next2);
    }
}
bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}
void GameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	//_player1的移动触摸
	auto BeginPoint1=touch->getLocationInView();//获取当前触摸点坐标
	BeginPoint1=Director::sharedDirector()->convertToGL(BeginPoint1);
	auto endPoint1=touch->getPreviousLocationInView();//获取下一个触摸点坐标
	endPoint1=Director::sharedDirector()->convertToGL(endPoint1);
	auto offset1=Vec2(BeginPoint1-endPoint1);

	auto nextpoint1=Vec2(this->getChildByTag(100)->getPosition()+offset1);
	this->getChildByTag(100)->setPosition(nextpoint1);
	this->getChildByTag(0)->setPosition(nextpoint1);
	//_player2的移动触摸
	auto BeginPoint2=touch->getLocationInView();
	BeginPoint2=Director::sharedDirector()->convertToGL(BeginPoint2);
	auto endPoint2=touch->getPreviousLocationInView();
	endPoint2=Director::sharedDirector()->convertToGL(endPoint2);
	auto offset2=Vec2(BeginPoint2-endPoint2);

	auto nextpoint2=Vec2(this->getChildByTag(101)->getPosition()-offset2);
	this->getChildByTag(101)->setPosition(nextpoint2);
	this->getChildByTag(1)->setPosition(nextpoint2);
	//边界设定
	auto player1=this->getChildByTag(100);
	auto player2=this->getChildByTag(101);
	if (nextpoint1.x>=280)
	{
		nextpoint1.x=280;
	}
	if (nextpoint1.x<=40)
	{
		nextpoint1.x=40;
	}
	if (nextpoint1.y<=120)
	{
		nextpoint1.y=120;
	}
	if (nextpoint1.y>=355)
	{
		nextpoint1.y=355;
	}
	player1->setPosition(nextpoint1);
	if (nextpoint2.x>=280)
	{
		nextpoint2.x=280;
	}
	if (nextpoint2.x<=40)
	{
		nextpoint2.x=40;
	}
	if (nextpoint2.y<=120)
	{
		nextpoint2.y=120;
	}
	if (nextpoint2.y>=355)
	{
		nextpoint2.y=355;
	}
	player2->setPosition(nextpoint2);
}
void GameScene::onTouchEnded(Touch *touch, Event *unused_event)
{

}
//检测事件
void GameScene::update(float t)
{
	auto size=Director::getInstance()->getVisibleSize();
	_labelHelp->setPositionX(_labelHelp->getPositionX()-1);
	if (_labelHelp->getPositionX()<=-1100)
	{
		_labelHelp->setPositionX(1300);
	}
	//检测碰撞事件
	auto player1=this->getChildByTag(100);
	auto player2=this->getChildByTag(101);
	auto bonus=this->getChildByTag(110);
	Rect playerBounding1(player1->getPositionX(),player1->getPositionY(),30,30);
	Rect playerBounding2(player2->getPositionX(),player2->getPositionY(),30,30);
	for (int i = 0; i < allAttacker.size(); i++)
	{
		auto attacker1=allAttacker.at(i);
		Rect A(attacker1->getPositionX(),attacker1->getPositionY(),30,30);
		if((playerBounding1.intersectsRect(A)||playerBounding2.intersectsRect(A))&& flag)
		{
			SimpleAudioEngine::getInstance()->playEffect("crush.mp3");
			player1->runAction(RotateBy::create(0.5,360));
			player2->runAction(RotateBy::create(0.5,360));
			//player1->runAction(Blink::create(1,4));
			//player2->runAction(Blink::create(1,4));
			player1->setVisible(true);
			player2->setVisible(true);
			player1->setScale(player1->getScale()+0.01);
			player2->setScale(player2->getScale()+0.01);
			progress->setPercentage(progress->getPercentage()-15);
			if (this->getChildByName("layer")->getOpacity()<=255)
			{
				this->getChildByName("layer")->setOpacity(this->getChildByName("layer")->getOpacity()+30);
			}
			if (this->getChildByName("layer")->getOpacity()>240)
			{
				flag=false;
			}
			flag=false;
		}
	}
	for (int i = 0; i < allAttacker2.size(); i++)
	{
		auto attacker2=allAttacker2.at(i);
		Rect B(attacker2->getPositionX(),attacker2->getPositionY(),30,30);
		if ((playerBounding1.intersectsRect(B)||playerBounding2.intersectsRect(B) )&& flag)
		{
			SimpleAudioEngine::getInstance()->playEffect("crush.mp3");
			player1->runAction(RotateBy::create(0.5,360));
			player2->runAction(RotateBy::create(0.5,360));
			//->runAction(Blink::create(1,4));
			////->runAction(Blink::create(1,4));
			player1->setVisible(true);
			player2->setVisible(true);
			player1->setScale(player1->getScale()+0.01);
			player2->setScale(player2->getScale()+0.01);
			progress->setPercentage(progress->getPercentage()-15);
			if (this->getChildByName("layer")->getOpacity()<=255 && flag)
			{
				this->getChildByName("layer")->setOpacity(this->getChildByName("layer")->getOpacity()+30);
			}
			if (this->getChildByName("layer")->getOpacity()>240)
			{
				flag=false;
			}
			flag=false;
		}
	}
	for (int i = 0; i < allBonus.size(); i++)
	{
		auto bonus=allBonus.at(i);
		Rect C(bonus->getPositionX(),bonus->getPositionY(),30,30);
		if ((playerBounding1.intersectsRect(C)||playerBounding2.intersectsRect(C) )&& flag)
		{
			SimpleAudioEngine::getInstance()->playEffect("chishuijing.mp3");
			score+=500;
			progress->setPercentage(progress->getPercentage()+15);
			bonus->removeFromParent();
			allAttacker.eraseObject(bonus);
			i--;
			flag=false;
		}
	}
	auto _scoreLabel=(Label *)this->getChildByName("_labelScore");
	_scoreLabel->setString(String::createWithFormat("分数:%d",score)->_string);
    //进度条递减
	if (progress->getPercentage()<=0)
	{
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		this->pauseSchedulerAndActions();
		this->removeAllChildrenWithCleanup(true);
		SimpleAudioEngine::getInstance()->playEffect("fs_lose.wav");
    //背景
	auto bc1=Sprite::create("backComix01.jpg");
	bc1->setAnchorPoint(Vec2::ZERO);
	bc1->setPosition(Vec2(0,0));
	bc1->setScale(0.3);
	//bc1->setOpacity(0);
	this->addChild(bc1,5);
	//btnReStart
	Button *btnReStart=Button::create("pauseRestart_lock.png","pauseRestart.png","pauseRestart_lock.png");
	btnReStart->addTouchEventListener(CC_CALLBACK_2(GameScene::btnReStart,this));
	btnReStart->setPosition(Vec2(size.width/2,size.height/2));
	btnReStart->setPressedActionEnabled(true);
	//btnReStart->setOpacity(0);
	btnReStart->setScale(0.8);
	this->addChild(btnReStart,6);
	//分数
	auto labelScore=Label::createWithSystemFont("分数:0","DFGirl",40);
	labelScore->setName("labelScore");
	labelScore->setPosition(Vec2(size.width/2-20,size.height/2+90));
	labelScore->setColor(Color3B::BLACK);
	//labelScore->setOpacity(0);
	this->addChild(labelScore,6);
	auto scoreLabel=(Label *)this->getChildByName("labelScore");
	scoreLabel->setString(String::createWithFormat("分数:%d",score)->_string);

	Label * highScoreLabel=nullptr;
	if (score>_highScore)
	{
		UserDefault::getInstance()->setIntegerForKey("HighScore",score);
		_highScore=score;
		highScoreLabel=Label::createWithSystemFont("恭喜您突破自我!","DFGirl",20);
		highScoreLabel->setColor(Color3B::RED);
		highScoreLabel->setHorizontalAlignment(TextHAlignment::CENTER);
		highScoreLabel->setPosition(Vec2(size.width/2-20,size.height/2+140));
	}
	else
	{
		highScoreLabel=Label::createWithSystemFont(StringUtils::format("你的最高分:%d",_highScore),"DFgirl",20);
		highScoreLabel->setColor(Color3B::RED);
		highScoreLabel->setPosition(Vec2(size.width/2-20,size.height/2+145));
	}
	this->addChild(highScoreLabel,6);

	}
	//
}
void  GameScene::btnReStart(Ref *pSender,Widget::TouchEventType event)
{
	if (event == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("fs_win.wav");
        auto scene=Loading::createScene();
	    Director::getInstance()->replaceScene(scene);
	}
	
}
void GameScene::btnMusic(Ref *pSender,Widget::TouchEventType event)
{
	if (event == Widget::TouchEventType::ENDED)
	{
		bool isMusicOn = UserDefault::getInstance()->getBoolForKey("isMusicOn",true);
            if (isMusicOn)
            {
				_btnMusic->loadTextureNormal("btnMusic_off.png");
                UserDefault::getInstance()->setBoolForKey("isMusicOn",false);
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
				SimpleAudioEngine::getInstance()->pauseAllEffects();
            }
            else
            {
                _btnMusic->loadTextureNormal("btnMusic.png");
                UserDefault::getInstance()->setBoolForKey("isMusicOn",true);
				SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
				SimpleAudioEngine::getInstance()->resumeAllEffects();
            }
	}
}
//添加横向宝石
void GameScene::newAttacker(float t)
{
	flag=true;
	if (time>20)
	{
		num=2;
	}
	if (time>30)
	{
		num=3;
	}
	for (int i = 0; i<num; i++)
	{
		Sprite *newAttacker=nullptr;
	    newAttacker=Sprite::create("bombBlue.png");
	    newAttacker->setScale(0.18);
	    this->addChild(newAttacker,1.5);
	    int y=rand()%200+120;
	    newAttacker->setPosition(Vec2(15,y));
	    allAttacker.pushBack(newAttacker);	
		auto randomAttacker=allAttacker.at(rand()%allAttacker.size());
		randomAttacker->setPositionY(this->getChildByTag(100)->getPositionY());
	}
}
//添加纵向宝石
void GameScene::newAttacker2(float t)
{
	flag=true;
	if (time>20)
	{
		num=2;
	}
	if (time>30)
	{
		num=3;
	}
	for(int i=0;i<num;i++)
	{
		Sprite* newAttacker2=nullptr;
		newAttacker2=Sprite::create("bombRed.png");
		newAttacker2->setScale(0.18);
		int x=rand()%200+50;
		newAttacker2->setPosition(Vec2(x,380));
		this->addChild(newAttacker2,1.5);
		allAttacker2.pushBack(newAttacker2);
	}
}
//添加加分道具
void GameScene::newBonus(float t)
{
	flag=true;
	//int _BonusX=rand()%200+50;
	int _BonusY=rand()%200+120;
	Sprite *nowBonus=nullptr;
	nowBonus=Sprite::create("bonusCrystal.png");
	nowBonus->setScale(0.2);
	nowBonus->setName("nowBonus");
	this->addChild(nowBonus,1.5);
	nowBonus->setPosition(Vec2(15,_BonusY));
	allBonus.pushBack(nowBonus);
}
//移动横向宝石
void GameScene::moveAttacker(float t)
{
	for(int i=0;i<allAttacker.size();i++){
		auto nowAttacker=allAttacker.at(i);
		nowAttacker->setPositionX(nowAttacker->getPositionX()+1.5);
		if(nowAttacker->getPositionX()>300)
		{
			nowAttacker->removeFromParent();
			allAttacker.eraseObject(nowAttacker);
			i--;
		}
	}
}

//移动纵向宝石
void GameScene::moveAttacker2(float t)
{
	for (int i = 0; i < allAttacker2.size(); i++)
	{
		auto attacker2=allAttacker2.at(i);
		//this->getChildByTag(3)->setPosition(attacker2->getPosition());
		attacker2->setPositionY(attacker2->getPositionY()-distance);
		if(attacker2->getPositionY()<=95)
		{
			attacker2->removeFromParent();
			allAttacker2.eraseObject(attacker2);
			i--;
		}
	}
}
//移动加分道具
void GameScene::moveBonus(float t)
{
	for (int i = 0; i < allBonus.size(); i++)
	{
		auto nowBonus=allBonus.at(i);
		nowBonus->setPositionX(nowBonus->getPositionX()+2);
		if (nowBonus->getPositionX()>=300)
 		{
		   nowBonus->removeFromParent();
           allBonus.eraseObject(nowBonus);
		   i--;
		}
	}
}
//自我移除
void GameScene::killself(Node *node)
{   
	node->removeFromParentAndCleanup(true);
}
//添加分数
void GameScene::timePlus(float t)
{
	time++;
}
void GameScene::scorePlus(float t)
{
	score++;
}
