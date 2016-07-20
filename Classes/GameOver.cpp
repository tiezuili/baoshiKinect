#include "GameOver.h"
#include "Loading.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;
Scene* GameOver::createScene()
{
	auto scene=Scene::create();
	auto layer=GameOver::create();
	scene->addChild(layer);
	return scene;
}
bool GameOver::init()
{
	if(!Layer::init())
	{
		return false;
	}
	auto size=Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//±³¾°
	auto bc1=Sprite::create("backComix01.jpg");
	bc1->setAnchorPoint(Vec2::ZERO);
	bc1->setPosition(Vec2(0,0));
	bc1->setScale(0.3);
	this->addChild(bc1,5);
	//btnReStart
	Button *btnReStart=Button::create("pauseRestart_lock.png","pauseRestart.png","pauseRestart_lock.png");
	btnReStart->addTouchEventListener(CC_CALLBACK_2(GameOver::btnReStart,this));
	btnReStart->setPosition(Vec2(size.width/2,size.height/2));
	btnReStart->setPressedActionEnabled(true);
	btnReStart->setScale(0.8);
	this->addChild(btnReStart,6);
	
	return true;
}
void  GameOver::btnReStart(Ref *pSender,Widget::TouchEventType event)
{
	if (event == Widget::TouchEventType::ENDED)
	{
        auto scene=Loading::createScene();
	    Director::getInstance()->replaceScene(scene);
	}
	
}




