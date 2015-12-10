//
//  LogoShow.cpp
//  ds2048
//
//  Created by yang on 15/4/28.
//
//

#include "LogoShow.h"
#include "Funtion.h"

LogoShow::LogoShow()
{
    
}
LogoShow::~LogoShow()
{
    
}

bool LogoShow::init()
{
    if(!CCLayer::init())
        return false;
    
    showLogo();
    
    return true;
}

void LogoShow::showLogo()
{
    CCLayerColor* clay = CCLayerColor::create(ccc4(255, 255, 255, 255));
    this->addChild(clay);
    
    CCSprite* logopic = CCSprite::create("images/logoPic.png");
    float scalex = mWinsize.width/logopic->getContentSize().width;
    logopic->setScaleX(scalex);
    logopic->setScaleY(scalex);
    logopic->setPosition(ccp(mWinsize.width*0.5,mWinsize.height*0.5));
    this->addChild(logopic,10);
    
    CCFadeOut* fadein = CCFadeOut::create(1.0f);
    CCCallFunc* call = CCCallFunc::create(this, callfunc_selector(LogoShow::endLogo));
    logopic->runAction(CCSequence::create(fadein,call,NULL));
    
}

void LogoShow::endLogo(cocos2d::CCObject *obj)
{
    this->removeFromParent();
}