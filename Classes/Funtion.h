//
//  Funtion.h
//  Markbels
//
//  Created by YUN on 15-1-12.
//
//

#ifndef __Markbels__Funtion__
#define __Markbels__Funtion__

#include <iostream>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <time.h>

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;


#define GameName        "JIGSAW"

#define CanBuyGold      1
#define testMode        0
#define HaveGameCenter  0
#define ANDGAMEMODE     0


#define ClassicMode     1
#define EndlessMode     2

#define VirgourShop     4
#define ToolsShop       3
#define MagcryShop      1
#define RoleShop        2
#define FirstGiftShop   9
#define BigGiftShop     10
#define FGameGiftShop   11

enum{GameStart,GamePause,GameOver};
enum{FireballTool,IceballTool,ColorballTool,BoomballTool,TotalToolNum};

const float acWinsizeWidth  = 720.0;
const float acWinsizeHeight = 1280.0;

const float spriteWidth     = 60.0;
const float spriteHeight    = 60.0;

const int   SCORELISTNUM    = 20;

#define mWinsize CCDirector::sharedDirector()->getWinSize()
#define cache CCSpriteFrameCache::sharedSpriteFrameCache()
#define changeChar  CCString::createWithFormat

#define LayeronEnter(p) \
void onEnter()\
{ \
    CCLayer::onEnter(); \
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,kCCMenuHandlerPriority-p,true); \
}

#define LayeronExit() \
void onExit()\
{ \
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this); \
    CCLayer::onExit(); \
}

#define mWinScale MIN(mWinsize.width/acWinsizeWidth,mWinsize.height/acWinsizeHeight)
#define mMaxWinScale MAX(mWinsize.width/acWinsizeWidth,mWinsize.height/acWinsizeHeight)
#define mWinScaleX mWinsize.width/acWinsizeWidth
#define mWinScaleY mWinsize.height/acWinsizeHeight

#define SaveStringToXML     CCUserDefault::sharedUserDefault()->setStringForKey
#define SaveIntegerToXML    CCUserDefault::sharedUserDefault()->setIntegerForKey
#define SaveBooleanToXML    CCUserDefault::sharedUserDefault()->setBoolForKey
#define LoadStringFromXML   CCUserDefault::sharedUserDefault()->getStringForKey
#define LoadIntegerFromXML  CCUserDefault::sharedUserDefault()->getIntegerForKey
#define LoadBooleanFromXML  CCUserDefault::sharedUserDefault()->getBoolForKey
#define SaveFlush           CCUserDefault::sharedUserDefault()->flush()

#define PlaySound           SimpleAudioEngine::sharedEngine()->playEffect

#define NORMALBTNIMAGE  "Normalbtn.png"
#define SELECTBTNIMAGE  "NormalbtnD.png"

#define LONGNORMALBTNIMAGE  "LongNormalbtn.png"
#define LONGSELECTBTNIMAGE  "LongNormalbtnD.png"

enum{NUMBERLEFT,NUMBERCENTER,NUMBERRIGHT};

void setSpriteScale(CCNode* sp);
CCPoint getacPos(float pox,float poy);
CCPoint getwinPos(float pox,float poy);
CCPoint getwinPos(CCPoint pot);
CCPoint getpos(float pox,float poy);
CCAction* createAction(const char* actioname,int actionNum,int bloop,float delay = -1,bool bframe = true);
CCSprite* createPlistSprite(const char* spname,float pox,float poy);
CCSprite* createSprite(const char* spname,float pox,float poy,bool bscale = true);
CCMenuItemImage* createPlistMenu(const char* normalImage,const char* seleteImage,CCObject *rec,SEL_MenuHandler callfun,float pox,float poy);
CCMenuItemImage* createMenu(const char* normalImage,const char* seleteImage,CCObject *rec,SEL_MenuHandler callfun,float pox,float poy,bool bscale = true);

CCMenuItemImage* createMenu(const char* normalImage,const char* seleteImage,const char* ttf,CCObject *rec,SEL_MenuHandler callfun,float pox,float poy,bool bscale = true);

CCLabelTTF* createTTF(const char* ttf,float size, float pox,float poy,CCPoint anpot = ccp(0.5,0.5),bool bscale = true);
CCProgressTimer* createProgress(const char* spname,float pox,float poy,float percent,bool bscale = true);
CCSprite* createRect(float rectwidth,float rectheight,ccColor3B col,float pox,float poy,bool bscale = true);
void showPicNum(const char* numName,int mNum,CCNode* mNode,float pox,float poy,int tag = -1,bool sc = false);
void showOnePicNum(const char* numName,int mNum,CCNode* mNode,float pox,float poy,int dir,int tag = -1,bool sc = true);

void encode(string &str,int addcode);
void decode(string &str,int addcode);

int getSystemTime();
void saveLastTime();
int readLastTime();

int readSystemYear();
int readSystemMonth();
int readSystemDay();

bool getHaveFirstGameRun();
void setHaveFirstGameRun(bool e = true);

bool getHaveFirstShopRun();
void setHaveFirstShopRun(bool e = true);

void saveHistoryScore(int score);
int readHightestScore();
void saveHightestScore(int score);

int readGameVirgour();
void saveGameVirgour(int virgour);
void addGameVirgour(int addnum);
void subGameVirgour(int subnum = 1);

int readGameTools(int tooltag);
void saveGameTools(int tooltag,int toolnum);
void addGameTools(int tooltag,int addnum);
void subGameTools(int tooltag,int subnum = 1);

int readGameLevel(int bigtask);
void saveGameLevel(int bigtask,int gamelevel);

void saveBackStepTimes(int times);
int readBackStepTimes();

void saveHaveSoundEffect(bool e);
void saveHaveMuicsEffect(bool e);

bool haveSoundEffect();
bool haveMuicsEffect();

bool readAchievement(int tag);
void saveAchievement(int tag);

int readGameStarNum();
void saveGameStarNum(int num);

int readGameTaskLevelStarNum(int taskid,int level);
void saveGameTaskLevelStarNum(int taskid,int level,int num);

//魔晶
int readGameMagCry();
void saveGameMagCry(int magcry);
void addGameMagCry(int addnum);
void subGameMagCry(int subnum = 1);

//人物
bool readIsGetRole(int roleID);
void saveIsGetRole(int roleID);

int readRoleLevel(int roleID);
void saveRoleLevel(int roleID,int lev);

//任务
bool readMissionCompletion(int year,int month,int day,int missionID);
void saveMissionComplettion(int year,int month,int day,int missionID,bool iscompletion);

bool readIsGetReWard(int year,int month,int day,int missionID);
void saveIsGetReWard(int year,int month,int day,int missionID,bool isget);

//签到
bool readIsSigned(int year,int month,int day);
void saveIsSigned(int year,int month,int day,bool isSigned);

//转盘
int readTurnTabelCount(int year,int month,int day);
void saveTurnTableCount(int year,int month,int day,int times);

#endif /* defined(__Markbels__Funtion__) */
