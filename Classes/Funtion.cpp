//
//  Funtion.cpp
//  Markbels
//
//  Created by YUN on 15-1-12.
//
//

#include "Funtion.h"


void setSpriteScale(CCNode* sp)
{
    sp->setScale(mWinScale);
}

CCPoint getacPos(float pox,float poy)
{
    return ccp((mWinsize.width-acWinsizeWidth*mWinScale)/2+mWinScale*pox,(mWinsize.height-acWinsizeHeight*mWinScale)/2+mWinScale*(acWinsizeHeight-poy));
}

CCPoint getpos(float pox, float poy)
{
    return ccp(mWinsize.width*(pox/acWinsizeWidth),mWinsize.height*((acWinsizeHeight-poy)/acWinsizeHeight));
}

CCPoint getwinPos(float pox,float poy)
{
    float wpox = (pox-(mWinsize.width-acWinsizeWidth*mWinScale)/2)/mWinScale;
    float wpoy = acWinsizeHeight-(poy-(mWinsize.height-acWinsizeHeight*mWinScale)/2)/mWinScale;
    
    return ccp(wpox,wpoy);
}
CCPoint getwinPos(CCPoint pot)
{
    float pox = pot.x;
    float poy = pot.y;
    float wpox = (pox-(mWinsize.width-acWinsizeWidth*mWinScale)/2)/mWinScale;
    float wpoy = acWinsizeHeight-(poy-(mWinsize.height-acWinsizeHeight*mWinScale)/2)/mWinScale;
    
    return ccp(wpox,wpoy);
}

CCAction* createAction(const char* actioname,int actionNum,int bloop,float delay,bool bframe)
{
    CCArray* actionarray = CCArray::create();
    char buff[128];
    for(int i = 1;i <= actionNum;i++)
    {
        sprintf(buff, "%s%d.png",actioname,i);
        
        if(!bframe)
        {
            CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(buff);
            CCSpriteFrame* spriteframe = CCSpriteFrame::createWithTexture(texture, CCRectMake(0,0,texture->getContentSize().width,texture->getContentSize().height));
            actionarray->addObject(spriteframe);
        }
        else
            actionarray->addObject(cache->spriteFrameByName(buff));
    }
    
    float num = delay;
    if(delay == -1)
        num = CCRANDOM_0_1() * 0.15 + 0.1;
    
    CCAnimation* action = CCAnimation::createWithSpriteFrames(actionarray,num);
    CCAnimate* animate = CCAnimate::create(action);
    
    if(bloop == -1)
    {
        CCRepeatForever* repeat = CCRepeatForever::create(animate);
        return repeat;
    }
    else
    {
        CCRepeat* repeat = CCRepeat::create(animate, bloop);
        return repeat;
    }
}
CCSprite* createPlistSprite(const char* spname,float pox,float poy)
{
    CCSprite* sp = CCSprite::createWithSpriteFrameName(spname);
    sp->setPosition(getacPos(pox, poy));
    setSpriteScale(sp);
    return sp;
}

//重写，根据x y 轴返回精灵
CCSprite* createSprite(const char* spname,float pox,float poy,bool bscale)
{
    CCSprite* sp = CCSprite::create(spname);
    sp->setPosition(ccp(pox, poy));
    if(bscale) sp->setPosition(getacPos(pox, poy));
    if(bscale) setSpriteScale(sp);
    return sp;
}

CCMenuItemImage* createPlistMenu(const char* normalImage,const char* seleteImage,CCObject *rec,SEL_MenuHandler callfun,float pox,float poy)
{
    CCMenuItemImage* menuitem = CCMenuItemImage::create();
    menuitem->setNormalSpriteFrame(cache->spriteFrameByName(normalImage));
    menuitem->setSelectedSpriteFrame(cache->spriteFrameByName(seleteImage));
    menuitem->initWithTarget(rec, callfun);
    menuitem->setPosition(getacPos(pox, poy));
    setSpriteScale(menuitem);
    
    return menuitem;
}
CCMenuItemImage* createMenu(const char* normalImage,const char* seleteImage,CCObject *rec,SEL_MenuHandler callfun,float pox,float poy,bool bscale)
{
    CCMenuItemImage* menuitem = CCMenuItemImage::create(normalImage,seleteImage);
    menuitem->initWithTarget(rec, callfun);
    menuitem->setPosition(ccp(pox,poy));
    if(bscale) menuitem->setPosition(getacPos(pox, poy));
    if(bscale) setSpriteScale(menuitem);
    
    return menuitem;
}

CCMenuItemImage* createMenu(const char* normalImage,const char* seleteImage,const char* ttf,CCObject *rec,SEL_MenuHandler callfun,float pox,float poy,bool bscale)
{
    CCMenuItemImage* menuitem = CCMenuItemImage::create(normalImage,seleteImage);
    menuitem->initWithTarget(rec, callfun);
    menuitem->setPosition(ccp(pox,poy));
    if(bscale) menuitem->setPosition(getacPos(pox, poy));
    if(bscale) setSpriteScale(menuitem);
    
    CCSprite* text = CCSprite::create(ttf);
    text->setPosition(ccp(menuitem->getContentSize().width*0.5,menuitem->getContentSize().height*0.5));
    menuitem->addChild(text);
    
    return menuitem;
}

CCLabelTTF* createTTF(const char* ttf,float size,float pox,float poy,CCPoint anpot,bool bscale)
{
    CCLabelTTF* labelttf = CCLabelTTF::create(ttf, "Marker Felt", size);
    labelttf->setAnchorPoint(anpot);
    labelttf->setPosition(ccp(pox,poy));
    if(bscale) labelttf->setPosition(getacPos(pox, poy));
    if(bscale) setSpriteScale(labelttf);
    
    return labelttf;
}

CCProgressTimer* createProgress(const char* spname,float pox,float poy,float percent,bool bscale)
{
    CCSprite* timeprosp = createSprite(spname, 0, 0);
    CCProgressTimer* timepro = CCProgressTimer::create(timeprosp);
    timepro->setType(kCCProgressTimerTypeBar);
    timepro->setMidpoint(ccp(0,0));
    timepro->setBarChangeRate(ccp(1,0));
    timepro->setPercentage(percent);
    timepro->setPosition(ccp(pox,poy));
    if(bscale) timepro->setPosition(getacPos(pox, poy));
    if(bscale) setSpriteScale(timepro);
    
    return timepro;
}

CCSprite* createRect(float rectwidth,float rectheight,ccColor3B col,float pox,float poy,bool bscale)
{
    CCSprite* rectsp = CCSprite::create();
    rectsp->setTextureRect(CCRect(0, 0, rectwidth, rectheight));
    rectsp->setColor(col);
    rectsp->setPosition(ccp(pox,poy));
    if(bscale) rectsp->setPosition(getacPos(pox, poy));
    if(bscale) setSpriteScale(rectsp);
    
    return rectsp;
}

void showPicNum(const char* numName,int mNum,CCNode* mNode,float pox,float poy,int tag,bool sc)
{
    char buff[32];
    sprintf(buff, "%d",mNum);
    string mNumstr = buff;
    
    int numCount = (int)mNumstr.length();
    for(int i = 0;i < numCount;i++)
    {
        string signum = mNumstr.substr(i,1);
        sprintf(buff, "%s%s.png",numName,signum.c_str());
        
        if(!sc)
        {
            CCSprite* numsp = CCSprite::create(buff);
            CCSize numsize = numsp->getContentSize();
            float posx = pox+(i+0.5-numCount/2.0)*numsize.width;
            
            numsp->setPosition(ccp(posx,poy));
            mNode->addChild(numsp,9,tag);
        }
        else
        {
            CCSprite* numsp = CCSprite::create(buff);
            numsp->setScale(mWinScale);
            CCSize numsize = numsp->getContentSize();
            float posx = pox+(i+0.5-numCount/2.0)*numsize.width;
            
            numsp->setPosition(getacPos(posx,poy));
            mNode->addChild(numsp,9,tag);
        }
    }
}

void showOnePicNum(const char* numName,int mNum,CCNode* mNode,float pox,float poy,int dir,int tag ,bool sc)
{
    char buff[32];
    sprintf(buff, "%d",mNum);
    string mNumstr = buff;
    
    int numCount = (int)mNumstr.length();
    for(int i = 0;i < numCount;i++)
    {
        string signum = mNumstr.substr(i,1);
        sprintf(buff, "%s%s.png",numName,signum.c_str());
        
        if(!sc)
        {
            CCSprite* numsp = CCSprite::create(buff);
            CCSize numsize = numsp->getContentSize();
            float posx;
            if(dir == NUMBERCENTER)
                posx = pox+(i+0.5-numCount/2.0)*numsize.width;
            else if(dir == NUMBERLEFT)
                posx = pox + i*(numsize.width+3);
            
            numsp->setPosition(ccp(posx,poy));
            mNode->addChild(numsp,9,tag);
        }
        else
        {
            CCSprite* numsp = CCSprite::create(buff);
            numsp->setScale(mWinScale);
            CCSize numsize = numsp->getContentSize();
            float posx;
            if(dir == NUMBERCENTER)
                posx = pox+(i+0.5-numCount/2.0)*numsize.width;
            else if(dir == NUMBERLEFT)
                posx = pox + i*(numsize.width+3);
            
            numsp->setPosition(getacPos(posx,poy));
            mNode->addChild(numsp,9,tag);
        }
    }
}

int getSystemTime()
{
    struct cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return (int)tv.tv_sec;
}

void saveLastTime()
{
    char buff[32];
    sprintf(buff, "%s_LastTime",GameName);
    SaveIntegerToXML(buff,getSystemTime());
    SaveFlush;
}
int readLastTime()
{
    char buff[32];
    sprintf(buff, "%s_LastTime",GameName);
    return LoadIntegerFromXML(buff);
}

int readSystemYear()
{
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    
    struct tm *tm;
    time_t timep = now.tv_sec;
    tm = localtime(&timep);
    int year = tm->tm_year + 1900;
    
    return year;
}
int readSystemMonth()
{
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    
    struct tm *tm;
    time_t timep = now.tv_sec;
    tm = localtime(&timep);
    int month = tm->tm_mon + 1;
    
    return month;
}
int readSystemDay()
{
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    
    struct tm *tm;
    time_t timep = now.tv_sec;
    tm = localtime(&timep);
    int day = tm->tm_mday;
    
    return day;
}

void encode(string &str,int addcode)
{
    for(int i = 0; i < str.length(); i++) {
        int ch = str[i];
        ch += (24+addcode*i+addcode);
        str[i] = ch;
    }
}
void decode(string &str,int addcode)
{
    for(int i = 0; i < str.length(); i++) {
        int ch = str[i];
        ch -= (24+addcode*i+addcode);
        str[i] = ch;
    }
    
}

bool getHaveFirstGameRun()
{
    char buff[32];
    sprintf(buff, "%s_FirstRun",GameName);
    return LoadBooleanFromXML(buff);
}
void setHaveFirstGameRun(bool e)
{
    char buff[32];
    sprintf(buff, "%s_FirstRun",GameName);
    SaveBooleanToXML(buff,e);
    SaveFlush;
}

bool getHaveFirstShopRun()
{
    char buff[32];
    sprintf(buff, "%s_FirstShopRun",GameName);
    return LoadBooleanFromXML(buff);
}
void setHaveFirstShopRun(bool e)
{
    char buff[32];
    sprintf(buff, "%s_FirstShopRun",GameName);
    SaveBooleanToXML(buff,e);
    SaveFlush;
}

void saveHistoryScore(int score)
{
    char buff[64];
    for(int i = SCORELISTNUM;i >= 0;i--)
    {
        sprintf(buff, "%s_HisScore_%d",GameName,i);
        int hisscore = LoadIntegerFromXML(buff);
        if(score <= hisscore)
            break;
        else
        {
            SaveIntegerToXML(buff,score);
            sprintf(buff, "%s_HisScore_%d",GameName,i+1);
            SaveIntegerToXML(buff,hisscore);
            SaveFlush;
        }
    }
}
int readHightestScore()
{
    char buff[64];
    sprintf(buff, "%s_HisScore_0",GameName);
    return LoadIntegerFromXML(buff);
}
void saveHightestScore(int score)
{
    char buff[64];
    sprintf(buff, "%s_HisScore_0",GameName);
    SaveIntegerToXML(buff,score);
    SaveFlush;
}


int readGameVirgour()
{
    string xx = LoadStringFromXML(CCString::createWithFormat("%s_GameVirgour",GameName)->getCString());
    decode(xx, 12);
    return atoi(xx.c_str());
}
void saveGameVirgour(int virgour)
{
    string xx = CCString::createWithFormat("%d",virgour)->getCString();
    encode(xx, 12);
    SaveStringToXML(CCString::createWithFormat("%s_GameVirgour",GameName)->getCString(),xx);
    SaveFlush;
}
void addGameVirgour(int addnum)
{
    int curvirgour = readGameVirgour();
    curvirgour += addnum;
    saveGameVirgour(curvirgour);
}
void subGameVirgour(int subnum)
{
    int curvirgour = readGameVirgour();
    curvirgour -= subnum;
    if(curvirgour <= 0) curvirgour = 0;
    saveGameVirgour(curvirgour);
}

int readGameTools(int tooltag)
{
    string xx = LoadStringFromXML(CCString::createWithFormat("%s_GameTools_%d",GameName,tooltag)->getCString());
    decode(xx, 9);
    return atoi(xx.c_str());
}
void saveGameTools(int tooltag,int toolnum)
{
    string xx = CCString::createWithFormat("%d",toolnum)->getCString();
    encode(xx, 9);
    SaveStringToXML(CCString::createWithFormat("%s_GameTools_%d",GameName,tooltag)->getCString(),xx);
    SaveFlush;
}
void addGameTools(int tooltag,int addnum)
{
    int curvirgour = readGameTools(tooltag);
    curvirgour += addnum;
    saveGameTools(tooltag,curvirgour);
}
void subGameTools(int tooltag,int subnum)
{
    int curvirgour = readGameTools(tooltag);
    curvirgour -= subnum;
    if(curvirgour <= 0) curvirgour = 0;
    saveGameTools(tooltag,curvirgour);
}

int readGameLevel(int bigtask)
{
    string xx = LoadStringFromXML(CCString::createWithFormat("%s_GameLevel_%d",GameName,bigtask)->getCString());
    decode(xx, 8);
    return atoi(xx.c_str());
}
void saveGameLevel(int bigtask,int gamelevel)
{
    string xx = CCString::createWithFormat("%d",gamelevel)->getCString();
    encode(xx, 8);
    SaveStringToXML(CCString::createWithFormat("%s_GameLevel_%d",GameName,bigtask)->getCString(),xx);
    SaveFlush;
}

void saveBackStepTimes(int times)
{
    SaveIntegerToXML("BackStepTimes",times);
    SaveFlush;
}
int readBackStepTimes()
{
    return LoadIntegerFromXML("BackStepTimes");
}

bool haveSoundEffect()
{
    char buff[64];
    sprintf(buff, "%s_GameSound",GameName);
    return LoadBooleanFromXML(buff);
}
bool haveMuicsEffect()
{
    char buff[64];
    sprintf(buff, "%s_GameMuics",GameName);
    return LoadBooleanFromXML(buff);
}
void saveHaveSoundEffect(bool e)
{
    char buff[64];
    sprintf(buff, "%s_GameSound",GameName);
    SaveBooleanToXML(buff,e);
    SaveFlush;
    
    if(e)
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
    else
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
    
}
void saveHaveMuicsEffect(bool e)
{
    char buff[64];
    sprintf(buff, "%s_GameMuics",GameName);
    SaveBooleanToXML(buff,e);
    SaveFlush;
    
    if(e)
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
    else
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
}

bool readAchievement(int tag)
{
    char buff[64];
    sprintf(buff, "IsGotAchievement_%d",tag);
    return LoadBooleanFromXML(buff);
}
void saveAchievement(int tag)
{
    char buff[64];
    sprintf(buff, "IsGotAchievement_%d",tag);
    SaveBooleanToXML(buff,true);
    SaveFlush;
}

int readGameStarNum()
{
    string xx = LoadStringFromXML(CCString::createWithFormat("%s_GameStarNum",GameName)->getCString());
    decode(xx, 4);
    return atoi(xx.c_str());
}
void saveGameStarNum(int num)
{
    string xx = CCString::createWithFormat("%d",num)->getCString();
    encode(xx, 4);
    SaveStringToXML(CCString::createWithFormat("%s_GameStarNum",GameName)->getCString(),xx);
    SaveFlush;
}

int readGameMagCry()
{
    string xx = LoadStringFromXML(CCString::createWithFormat("%s_GameMagCry",GameName)->getCString());
    decode(xx, 6);
    return atoi(xx.c_str());
}
void saveGameMagCry(int magcry)
{
    string xx = CCString::createWithFormat("%d",magcry)->getCString();
    encode(xx, 6);
    SaveStringToXML(CCString::createWithFormat("%s_GameMagCry",GameName)->getCString(),xx);
    SaveFlush;

}
void addGameMagCry(int addnum)
{
    int curvirgour = readGameMagCry();
    curvirgour += addnum;
    saveGameMagCry(curvirgour);
}
void subGameMagCry(int subnum)
{
    int curvirgour = readGameMagCry();
    curvirgour -= subnum;
    if(curvirgour <= 0) curvirgour = 0;
    saveGameMagCry(curvirgour);
}

int readGameTaskLevelStarNum(int taskid,int level)
{
    string xx = LoadStringFromXML(CCString::createWithFormat("%s_GameStarNum_%d_%d",GameName,taskid,level)->getCString());
    decode(xx, 7);
    return atoi(xx.c_str());
}
void saveGameTaskLevelStarNum(int taskid,int level,int num)
{
    string xx = CCString::createWithFormat("%d",num)->getCString();
    encode(xx, 7);
    SaveStringToXML(CCString::createWithFormat("%s_GameStarNum_%d_%d",GameName,taskid,level)->getCString(),xx);
    SaveFlush;
}

bool readIsGetRole(int roleID)
{
    return LoadBooleanFromXML(CCString::createWithFormat("%s_IsGetRole_%d",GameName,roleID)->getCString());
}
void saveIsGetRole(int roleID)
{
    SaveBooleanToXML(CCString::createWithFormat("%s_IsGetRole_%d",GameName,roleID)->getCString(),true);
    SaveFlush;
}

int readRoleLevel(int roleID)
{
    string xx = LoadStringFromXML(CCString::createWithFormat("%s_GameRoleLevel_%d",GameName,roleID)->getCString());
    decode(xx, 3);
    return atoi(xx.c_str());
}
void saveRoleLevel(int roleID,int lev)
{
    string xx = CCString::createWithFormat("%d",lev)->getCString();
    encode(xx, 3);
    SaveStringToXML(CCString::createWithFormat("%s_GameRoleLevel_%d",GameName,roleID)->getCString(),xx);
    SaveFlush;
}

bool readMissionCompletion(int year,int month,int day,int missionID)
{
    return LoadBooleanFromXML(CCString::createWithFormat("%s_IsMissionComplete_%d-%d-%d_%d",GameName,year,month,day,missionID)->getCString());
}
void saveMissionComplettion(int year,int month,int day,int missionID,bool iscompletion)
{
    SaveBooleanToXML(CCString::createWithFormat("%s_IsMissionComplete_%d-%d-%d_%d",GameName,year,month,day,missionID)->getCString(),iscompletion);
    SaveFlush;
}

bool readIsGetReWard(int year,int month,int day,int missionID)
{
    return LoadBooleanFromXML(CCString::createWithFormat("%s_IsMissionRewardGet_%d-%d-%d_%d",GameName,year,month,day,missionID)->getCString());
}
void saveIsGetReWard(int year,int month,int day,int missionID,bool isget)
{
    SaveBooleanToXML(CCString::createWithFormat("%s_IsMissionRewardGet_%d-%d-%d_%d",GameName,year,month,day,missionID)->getCString(),isget);
    SaveFlush;
}

bool readIsSigned(int year,int month,int day)
{
    return LoadBooleanFromXML(CCString::createWithFormat("%s_IsGameSigned_%d-%d-%d",GameName,year,month,day)->getCString());
}
void saveIsSigned(int year,int month,int day,bool isSigned)
{
    SaveBooleanToXML(CCString::createWithFormat("%s_IsGameSigned_%d-%d-%d",GameName,year,month,day)->getCString(),isSigned);
    SaveFlush;
}

int readTurnTabelCount(int year,int month,int day)
{
    return LoadIntegerFromXML(CCString::createWithFormat("%s_TurnTabelCount_%d-%d-%d",GameName,year,month,day)->getCString());
}
void saveTurnTableCount(int year,int month,int day,int times)
{
    SaveBooleanToXML(CCString::createWithFormat("%s_TurnTabelCount_%d-%d-%d",GameName,year,month,day)->getCString(),times);
    SaveFlush;
}




