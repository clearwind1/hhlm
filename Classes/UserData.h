#ifndef __USER_DATA_H__	
#define __USER_DATA_H__


#include "AppConfig.h"
#include "GameInfo.h"
static const int Coins[7]={5,1,5,20,80,300,880};
static int BUYID = -1;

enum
{
    REFRESHTOOL,FOUNDTOOL,TIMETOOL,BOMBTOOL
};

struct GameData
{
	unsigned int m_magic;
	int m_gameLevel;					// µ±«∞”Œœ∑πÿ ˝
	int m_starLevel[LINK_LEVEL_MAX];	// –«º∂
	int m_goldCoins;					// µ±«∞Ω±“ ˝¡ø
	bool m_musicMute;					// ±≥æ∞“Ù¿÷ «∑Òπÿ±’
	bool m_effectMute;					// Ãÿ–ß…˘“Ù «∑Òπÿ±’
	int m_logonCount;					// ¡¨–¯µ«¬ºÃÏ ˝
	int m_giftCount;					// ¡Ï»°¿Ò∞¸ÃÏ ˝
	long m_logonDate;					// ◊Ó∫Ûµ«¬º»’∆⁄(ÃÏ)
    int m_toolsnum[TOOS_NUM];           //道具
    bool m_bfirstrun;                   //第一次跑
    int m_startNum;                     //星星数
    
};


class UserData
{
public:
	UserData();
	~UserData();

	static UserData *shared();

	void setLevel(int level);
	int getLevel();

	void setStarLevel(int level, int starLevel);
	int getStarLevel(int starLevel);

	void setGoldCoins(int gold);
	int getGoldCoins();

	void setGiftCount();
	int getGiftCount();
	int getLogonCount();

	void setMusicMute(bool mute);
	bool isMusicMute();

	void setEffectMute(bool mute);
	bool isEffectMute();
    
    void setToolNum(int ktool,int tnum);
    int getToolNum(int ktool);

    void setFirstRun(bool e);
    bool getFirstRun();
    
    void setStartNum(int num);
    int getStartNum();
    
	void flush();

	void payReslut(int num);


private:
	unsigned char checkSum(void *buffer,int length);
	bool readData(void);
	bool writeData(void);

	void encrypt(void *buffer,int length,unsigned char offset);
	void decrypt(void *buffer,int length,unsigned char offset);

private:
	struct GameData m_gameData;
};


#endif //__USER_DATA_H__