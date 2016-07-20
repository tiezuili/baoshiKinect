#ifndef __PROGRESSBAR_SCENE_H__
#define __PROGRESSBAR_SCENE_H__
#include "cocos2d.h"
#include "GameScene.h"
#include "Loading.h"
USING_NS_CC;
class ProgressBar1 : public cocos2d::Layer
{
public:
     void  _ProgressBar();
	 void  update(float t);
	 ProgressTimer *progress;
};

#endif // __HELLOWORLD_SCENE_H__
