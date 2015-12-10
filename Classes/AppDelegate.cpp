#include "AppDelegate.h"
#include "MainMenu.h"
#include "AudioPlayer.h"
#include "UserData.h"


USING_NS_CC;


AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);

    
    CCFileUtils::sharedFileUtils()->addSearchPath("shop");
    
    //pDirector->setDisplayStats(true);
	pDirector->setAnimationInterval(1.0 / 60);

	cc_timeval timeVal;
	CCTime::gettimeofdayCocos2d(&timeVal,NULL);
	srand(timeVal.tv_sec * 1000 + timeVal.tv_usec / 1000);

	//Initialize..
	UserData::shared();

	CCScene *pScene = MainMenu::scene();
    pDirector->runWithScene(pScene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
	AudioPlayer::shared()->pauseMusic();
	UserData::shared()->flush();
}

void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
	AudioPlayer::shared()->resumeMusic();
}

