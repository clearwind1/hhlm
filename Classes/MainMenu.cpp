#include "MainMenu.h"
#include "PlayGame.h"
#include "OptionMenu.h"
#include "AudioPlayer.h"
#include "UserData.h"
#include "LevelMap.h"
#include "MapConfig.h"
#include "LogoShow.h"
#include "Funtion.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#else
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

USING_NS_CC;

CCSprite *LoadBgImage(const char *filename)
{
	float wScale,hScale,scale;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *sprite = CCSprite::create(filename);
	CCSize size = sprite->getContentSize();

	wScale = winSize.width / size.width;
	hScale = winSize.height / size.height;
	if(wScale > hScale)
		scale = wScale;
	else
		scale = hScale;

	sprite->setScale(scale);
	sprite->setAnchorPoint(ccp(0.5f,0.0f));
	sprite->setPosition(ccp(winSize.width / 2, 0));

	return sprite;
}


CCScene* MainMenu::scene(bool back)
{
    CCScene *scene = CCScene::create();

    MainMenu *layer = MainMenu::create(back);
    scene->addChild(layer);

    return scene;
}

MainMenu *MainMenu::create(bool back)
{
    MainMenu *pRet = new MainMenu();
    if(pRet && pRet->init(back)) 
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool MainMenu::init(bool back)
{
    if(!CCLayer::init())
        return false;

	//this->addChild(LoadBgImage("images/bg_start.jpg"),0);
    
    //this->addChild(LogoShow::create(),200);
    
	initMenu();
    
    bool bhavesound = true;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo jmi;
    
    jboolean jbl;
    
    if(JniHelper::getStaticMethodInfo(jmi ,"com/Mygame/hhlm/zalm" ,"isSound" ,"()Z"))
    {
        jbl = jmi.env->CallStaticBooleanMethod(jmi.classID , jmi.methodID);
        
        CCLog("jbl=====%d",jbl);
        if(jbl == 0)
            bhavesound = false;
        else
            bhavesound = true;
    }
#endif
    
    CCLog("bhavesound=======%d",bhavesound);
    
    saveHaveMuicsEffect(bhavesound);
    saveHaveSoundEffect(bhavesound);
    

	if(!back)
		AudioPlayer::shared()->playMusic("sounds/music_bg.mp3", true);

	this->setKeypadEnabled(true);
    
    return true;
}

void MainMenu::initMenu()
{
	float wScale,hScale,scale;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemImage *newMenuItem = CCMenuItemImage::create(
		"images/bg_start.jpg",
		NULL,
		this,
		menu_selector(MainMenu::newMenuCallback));
	CCSize size = newMenuItem->getContentSize();


	wScale = winSize.width / size.width;
	hScale = winSize.height / size.height;

	newMenuItem->setScaleX(wScale);
    newMenuItem->setScaleY(hScale);
	newMenuItem->setAnchorPoint(ccp(0.5f,0.0f));
	newMenuItem->setPosition(ccp(winSize.width / 2, 0));

	CCMenu *menu = CCMenu::create(newMenuItem, NULL);


	menu->setPosition(CCPointZero);
	this->addChild(menu, 1);

	CCSprite *sprite = CCSprite::create("images/bg_start_tip.png");
	size = sprite->getContentSize();

	scale = winSize.width * 0.5f / size.width;

	sprite->setScale(scale);
	sprite->setAnchorPoint(ccp(0.5f,0.0f));
	sprite->setPosition(ccp(winSize.width / 2, winSize.height * 0.2f));
	this->addChild(sprite, 1);

	CCActionInterval *delayTime = CCDelayTime::create(1);
	CCActionInterval *fadeOut = CCFadeOut::create(1);
	CCActionInterval *fadeIn = CCFadeIn::create(1);
	CCFiniteTimeAction *seq = CCSequence::create(delayTime,fadeOut,fadeIn,NULL);
	CCRepeatForever *repeatForever = CCRepeatForever::create((CCActionInterval *)seq);
	sprite->runAction(repeatForever);
}

void MainMenu::newMenuCallback(CCObject* pSender)
{
#if(MAP_EDITOR == 0)
 	//CCScene *pScene = PlayGame::scene(1, false);
	CCScene *pScene = LevelMap::scene();
#else
	//CCScene *pScene = MapConfig::scene();
#endif

	//CCLog("newMenuCallback,pScene:0x%x",(unsigned int)pScene);
	//CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInR::create(0.5f, pScene));
	//CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, pScene));
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.5f, pScene));
}

void MainMenu::keyBackClicked(void)
{
	UserData::shared()->flush();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/Mygame/hhlm/zalm","getIntence","()Ljava/lang/Object;");
    CCLog("开始调用退出!");
    jobject jobj=NULL;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    //CCLog("取得jobj");
    isHave = JniHelper::getMethodInfo(minfo,"com/Mygame/hhlm/zalm","exitGame","()V");
    //CCLog("处理中!");
    std::cout<<isHave<<std::endl;
    if (isHave)
    {
        //CCLog("接收!");
        minfo.env->CallVoidMethod(jobj, minfo.methodID);
        CCLog("完成!");
    }
    else
    {
        // CCLog("jni-java函数执行完毕");
    }
#else
    CCDirector::sharedDirector()->end();
    exit(0);
#endif
}