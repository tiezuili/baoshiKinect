#include "ProgressBar.h"
#include "Loading.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;
void  ProgressBar1::_ProgressBar()
{
	//��Ļ�ĳߴ�
	auto size=Director::getInstance()->getVisibleSize();
	 //����������
    Sprite *progressbgSprite;
	//�����Դ��������������
    progressbgSprite = Sprite::create("bonusbar.png") ;
    //progressbgSprite->setAnchorPoint(ccp(0,0));//�޸Ķ����Ӧ��
    progressbgSprite->setPosition(Vec2(size.width/2,size.height/3-100));
	//����������ӵ���������
    addChild(progressbgSprite,3);
	//���ÿɼ���Ϊtrue�����ǿɼ�
    progressbgSprite->setVisible(true);
    //����������
    Sprite *progressSprite = Sprite::create("bonusbar_fill.png");
    progress = ProgressTimer::create(progressSprite);
    //����������.
    //progress->setAnchorPoint(ccp(0,0));
    progress->setType(kCCProgressTimerTypeBar);  //�м�Ϊ�������ҵĽ�����������Ϊˮƽ
	//���ý�����������
    progress->setPosition(Vec2(size.width/2,size.height/3-100));
    //���ȶ����˶�����
    progress->setMidpoint(ccp(0,0));
    //��������߱仯
    progress->setBarChangeRate(ccp(1,0)); //���ý�����Ϊ�����������������������
	//Ϊ��������ӽ���ֵ
    progress->setPercentage(100.0f);//ֵ
    addChild(progress,3);
    progress->setVisible(true);
}
void ProgressBar1::update(float t)
{
	//�������ݼ�
	progress->setPercentage(progress->getPercentage()-0.1);
	if (progress->getPercentage()<=50)
	{
		progress->setPercentage(100);
	}
}




