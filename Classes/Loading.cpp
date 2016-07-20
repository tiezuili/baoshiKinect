#include "Loading.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "SocketHelper.h"
using namespace CocosDenshion;
USING_NS_CC;
Scene* Loading::createScene()
{
	auto scene=Scene::create();
	auto layer=Loading::create();
	scene->addChild(layer);
	return scene;
}
void Loading::onEnter()
{
	Layer::onEnter();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("gembox-ingame-music-3.1.mp3",true);
	Director::getInstance()->getEventDispatcher()->addCustomEventListener("KinectData",CC_CALLBACK_1(Loading::kinectCallback, this));  
    auto sh = SocketHelper::getInstance();//初始化Socket 
    //一次就够了
    std::string jsonStr ="{\"methodId\":100,\"argId\":1007,\"argName\":\"CloestHandData\"}";//生成初始化字符串
    sh->send(jsonStr);//发送初始化命令
}
void Loading::onExit()
{
    Layer::onExit();
}
//回调
void Loading::kinectCallback(cocos2d::EventCustom * event)
{
    //跳转Scene开始游戏
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("KinectData");//跳转之前先取消监听
    auto scene = GameScene::createScene();
    auto actionScene = TransitionCrossFade::create(0.5, scene);
    Director::getInstance()->replaceScene(actionScene);
}
bool Loading::init()
{
	flag=true;
	if(!Layer::init())
	{
		return false;
	}
	_btnMusic=nullptr;
	auto size=Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//定义背景图片
	auto _bg1=Sprite::create("worldMap_top01.jpg");
	_bg1->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(_bg1,0);
	_bg1->setScale(0.4);
	//定义标签精灵
	auto _bg2=Sprite::create("logo2.png");
	_bg2->setPosition(Vec2(size.width/2,size.height*2/3));
	this->addChild(_bg2,1);
	_bg2->runAction(Sequence::create(DelayTime::create(0.5),RotateBy::create(1,360),
	ScaleBy::create(2,1.5),FadeTo::create(3,20),FadeIn::create(2),ScaleBy::create(2,0.5),
	MoveTo::create(2,Vec2(size.width/2,size.height*2/3+80)),NULL));
	_bg2->setScale(0.3);
	//定义作者按钮
	_btnDesign = Button::create("btnSendReport.png","btnSendReport_active.png","btnSendReport.png");
	_btnDesign->addTouchEventListener(CC_CALLBACK_2(Loading::btnDesign,this));
	_btnDesign->setPosition(Vec2(size.width-20,size.height-20));
	_btnDesign->setScale(0.3);
	this->addChild(_btnDesign,1);
	//定义开始按钮
	auto btnStart=Button::create("dot.png","dot.png","dot.png");
	btnStart->addTouchEventListener(CC_CALLBACK_2(Loading::btnStart,this));
	btnStart->setPosition(Vec2(size.width/2,size.height/2-100));
	btnStart->setPressedActionEnabled(true);
	btnStart->setScale(0.8);
	btnStart->setOpacity(200);
	this->addChild(btnStart,1);
	//手
	Vector <SpriteFrame *> allFrame;
	for (int i = 1; i <=2; i++)
	{
		auto sf=SpriteFrame::create(StringUtils::format("hand%d.png",i),Rect(0,0,288,320));
		allFrame.pushBack(sf);
	}
	//定义动画
	auto animation=Animation::createWithSpriteFrames(allFrame,0.3);
	//定义动作
	auto animate=Animate::create(animation);
	auto hand=Sprite::create();
	hand->runAction(RepeatForever::create(animate));
	hand->setPosition(Vec2(size.width/2+40,size.height/2-140));
	hand->setScale(0.3);
	this->addChild(hand,1);
	//透明层
	auto layer=LayerColor::create(Color4B(255,255,255,210),size.width,size.height);
	layer->setPosition(Vec2::ZERO);
	layer->setTag(100);
	this->addChild(layer,2);
	layer->setOpacity(0);
	//music
	_btnMusic=Button::create("btnMusic.png");
	_btnMusic->setTag(200);
	_btnMusic->addTouchEventListener(CC_CALLBACK_2(Loading::btnMusic,this));
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
	//Design
	LabelJewel=Label::createWithSystemFont("宝石秘境\n   ","DFGirl",70);
	LabelJewel->setTag(110);
	LabelJewel->setTextColor(Color4B::ORANGE);
	LabelJewel->setPosition(Vec2(size.width/2,size.height*2/3-100));
	LabelJewel->setOpacity(0);
	this->addChild(LabelJewel,3);
	LabelBCU =Label::createWithSystemFont("Design By\n北京城市学院","DFGirl",40);
	LabelBCU->setTag(120);
	LabelBCU->setTextColor(Color4B::ORANGE);
	LabelBCU->setPosition(Vec2(size.width-150,100));
	LabelBCU->setOpacity(0);
	this->addChild(LabelBCU,3);
	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(Loading::onTouchBegan,this);
	listener->onTouchMoved=CC_CALLBACK_2(Loading::onTouchMoved,this);
	listener->onTouchEnded=CC_CALLBACK_2(Loading::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	//Timer计划任务
	this->schedule(schedule_selector(Loading::Timer),4);
	return true;
}
void Loading::Timer(float t)
{
	//粒子特效1
	auto particle=ParticleSystemQuad::create("16700.plist");
	int x=rand()%280+20;
	int x1=rand()%280+20;
	particle->setPosition(Vec2(x,800));
	SimpleAudioEngine::getInstance()->playEffect("chidaoju.mp3");
	particle->runAction(RepeatForever::create
		(Sequence::create(MoveTo::create(3,Vec2(x1,-200)),RemoveSelf::create(),NULL)));
	this->addChild(particle,5);
}
void Loading::btnDesign(Ref * pSender,Widget::TouchEventType event)
{
	
	if (event == Widget::TouchEventType::ENDED && flag)
	{
		this->getChildByTag(100)->runAction(Sequence::create(FadeIn::create(2),NULL));
		this->getChildByTag(110)->runAction(Sequence::create(FadeIn::create(2),NULL));
		this->getChildByTag(120)->runAction(Sequence::create(FadeIn::create(2),NULL));
		flag=false;
		_btnMusic->setTouchEnabled(false);
	   _btnDesign->setTouchEnabled(false);
	}
}
void Loading::btnStart(Ref *pSender,Widget::TouchEventType event)
{
	if (event == Widget::TouchEventType::ENDED)
	{
		auto scene=GameScene::createScene();
	    Director::getInstance()->replaceScene(TransitionFade::create(2,scene));
	    Director::getInstance()->replaceScene(scene);
		SimpleAudioEngine::getInstance()->playEffect("fs_win.wav");
	}
}
void Loading::btnMusic(Ref *pSender,Widget::TouchEventType event)
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

bool Loading::onTouchBegan(Touch *touch, Event *unused_event)
{
	this->getChildByTag(100)->runAction(Sequence::create(FadeTo::create(2,0),NULL));
	this->getChildByTag(110)->runAction(Sequence::create(FadeTo::create(2,0),NULL));
	this->getChildByTag(120)->runAction(Sequence::create(FadeTo::create(2,0),NULL));

	return true;
}
void Loading::onTouchMoved(Touch *touch, Event *unused_event)
{

}
void Loading::onTouchEnded(Touch *touch, Event *unused_event)
{
	flag=true;
	this->_btnMusic->setTouchEnabled(true);
	this->_btnDesign->setTouchEnabled(true);
}


