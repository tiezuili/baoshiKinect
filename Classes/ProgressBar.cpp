#include "ProgressBar.h"
#include "Loading.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;
void  ProgressBar1::_ProgressBar()
{
	//屏幕的尺寸
	auto size=Director::getInstance()->getVisibleSize();
	 //进度条背景
    Sprite *progressbgSprite;
	//添加资源到进度条背景中
    progressbgSprite = Sprite::create("bonusbar.png") ;
    //progressbgSprite->setAnchorPoint(ccp(0,0));//修改定点对应点
    progressbgSprite->setPosition(Vec2(size.width/2,size.height/3-100));
	//将进度条添加到根场景中
    addChild(progressbgSprite,3);
	//设置可见性为true，就是可见
    progressbgSprite->setVisible(true);
    //进度条正面
    Sprite *progressSprite = Sprite::create("bonusbar_fill.png");
    progress = ProgressTimer::create(progressSprite);
    //创建进度条.
    //progress->setAnchorPoint(ccp(0,0));
    progress->setType(kCCProgressTimerTypeBar);  //中间为从左向右的进度条，类型为水平
	//设置进度条的坐标
    progress->setPosition(Vec2(size.width/2,size.height/3-100));
    //进度动画运动方向
    progress->setMidpoint(ccp(0,0));
    //进度条宽高变化
    progress->setBarChangeRate(ccp(1,0)); //设置进度条为从左向右随进度增长而显现
	//为进度条添加进度值
    progress->setPercentage(100.0f);//值
    addChild(progress,3);
    progress->setVisible(true);
}
void ProgressBar1::update(float t)
{
	//进度条递减
	progress->setPercentage(progress->getPercentage()-0.1);
	if (progress->getPercentage()<=50)
	{
		progress->setPercentage(100);
	}
}




