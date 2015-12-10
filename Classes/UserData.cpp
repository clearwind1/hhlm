#include "UserData.h"
#include "cocos2d.h"
#include "GameMenu.h"
#include "LevelMap.h"
//#include "DialogBuyGift.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif


USING_NS_CC;

#define GAME_DATA_MAGIC		(0x4C494E4B)

enum
{
	USERDATA_EXTINFO_PWROFFSET = 0,
	USERDATA_EXTINFO_CHECKSUM,
	USERDATA_EXTINFO_MAX
};


static UserData *sharedUserData = NULL;


static const char *_fileName = "UserData.dat";
static const char *_dataPwd = "Zk$8M@0?c#$#@eE&D1~#:L*7t^3d@d?k"; //32

UserData *UserData::shared()
{
	if(sharedUserData == NULL)
	{
		sharedUserData = new UserData();
	}
	return sharedUserData;
}

UserData::UserData()
{
	memset(&m_gameData, 0, sizeof(struct GameData));
	if(readData() == false || m_gameData.m_magic != GAME_DATA_MAGIC)
	{
		memset(&m_gameData, 0, sizeof(struct GameData));
		m_gameData.m_magic = GAME_DATA_MAGIC;
		m_gameData.m_gameLevel = 1;
		flush();
	}
}

UserData::~UserData()
{
	if(sharedUserData != NULL)
	{
		delete sharedUserData;
		sharedUserData = NULL;
	}
}


unsigned char UserData::checkSum(void *buffer,int length)
{
	unsigned char sum = 0;
	unsigned char *p = (unsigned char *)buffer;

	while(length--)
		sum += *p++;

	return sum;
}

void UserData::encrypt(void *buffer,int length,unsigned char offset)
{
	unsigned char *key,*data;
	int keyLen,dataLen,count;

	dataLen = length;
	keyLen = strlen(_dataPwd);
	if(dataLen <= 0 || keyLen <= 0 || offset >= keyLen)
		return;
	count = keyLen - offset;
	key = (unsigned char *)_dataPwd + offset;
	data = (unsigned char *)buffer;
	while(length--)
	{
		*data = *key++ ^ *data;
		data++;
		if(++count >= keyLen)
		{
			count = 0;
			key = (unsigned char *)_dataPwd;
		}
	}
}

void UserData::decrypt(void *buffer,int length,unsigned char offset)
{
	encrypt(buffer,length,offset);
}

bool UserData::readData(void)
{
	bool result = false;
	unsigned char extInfo[USERDATA_EXTINFO_MAX];
	std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath() + _fileName;

	FILE *file = fopen(filePath.c_str(), "rb");
	if(file != NULL)
	{
		unsigned char *data = new unsigned char[sizeof(struct GameData)];
		if(fread(extInfo,1,USERDATA_EXTINFO_MAX,file) == USERDATA_EXTINFO_MAX)
		{
			if(fread(data,1,sizeof(struct GameData),file) == sizeof(struct GameData))
			{
				if((unsigned char)(checkSum(data,sizeof(struct GameData)) + checkSum(extInfo,USERDATA_EXTINFO_MAX)) == 0)
				{
					result = true;
					decrypt(data,sizeof(struct GameData),extInfo[USERDATA_EXTINFO_PWROFFSET]);
					memcpy(&m_gameData,data,sizeof(struct GameData));
				}
			}
		}
		delete []data;
		fclose(file);
	}

	return result;
}

bool UserData::writeData(void)
{
	bool result = false;
	unsigned char extInfo[USERDATA_EXTINFO_MAX];
	unsigned char *data = new unsigned char[sizeof(struct GameData)];
	std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath() + _fileName;

	memcpy(data,&m_gameData,sizeof(struct GameData));
	extInfo[USERDATA_EXTINFO_PWROFFSET] = (unsigned char)(rand() % strlen(_dataPwd));
	encrypt(data,sizeof(struct GameData),extInfo[USERDATA_EXTINFO_PWROFFSET]);
	extInfo[USERDATA_EXTINFO_CHECKSUM] = 0;
	extInfo[USERDATA_EXTINFO_CHECKSUM] = 0 - (checkSum(data,sizeof(struct GameData)) + checkSum(extInfo,USERDATA_EXTINFO_MAX));

	FILE *file = fopen(filePath.c_str(), "wb");
	if(file != NULL)
	{
		if(fwrite(extInfo,1,USERDATA_EXTINFO_MAX,file) == USERDATA_EXTINFO_MAX)
			if(fwrite(data,1,sizeof(struct GameData),file) == sizeof(struct GameData))
				result = true;
		fclose(file);
	}
	delete []data;

	return result;
}

int UserData::getLevel()
{
	return m_gameData.m_gameLevel;
}

void UserData::setLevel(int level)
{
	m_gameData.m_gameLevel = level;
}

void UserData::setStarLevel(int level, int starLevel)
{
	m_gameData.m_starLevel[level - 1] = starLevel;
}

int UserData::getStarLevel(int level)
{
	return m_gameData.m_starLevel[level - 1];
}

void UserData::setGoldCoins(int gold)
{
	if(gold >= 0)
	{
		m_gameData.m_goldCoins = gold;
		flush();
	}
}

int UserData::getGoldCoins()
{
	return m_gameData.m_goldCoins;
}

int UserData::getLogonCount()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	long curDate = (long)(time(NULL) / 86400);
#else
	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL); 
	long curDate = now.tv_sec / 86400;
#endif

	if(curDate == m_gameData.m_logonDate + 1)
	{
		m_gameData.m_logonCount += 1;
		if(m_gameData.m_logonCount > 7)
		{
			m_gameData.m_logonCount = 1;
			m_gameData.m_giftCount = 0;
		}
	}
	else if(curDate != m_gameData.m_logonDate)
	{
		m_gameData.m_logonCount = 1;
		m_gameData.m_giftCount = 0;
	}
	m_gameData.m_logonDate = curDate;

	return m_gameData.m_logonCount;// + 5;
}

void UserData::setGiftCount()
{
	m_gameData.m_giftCount = m_gameData.m_logonCount;// + 4;
}

int UserData::getGiftCount()
{
	return m_gameData.m_giftCount;
}

void UserData::setMusicMute(bool mute)
{
	if(m_gameData.m_musicMute != mute)
	{
		m_gameData.m_musicMute = mute;
		this->flush();
	}
}

bool UserData::isMusicMute()
{
	//return true;
	return m_gameData.m_musicMute;
}


void UserData::setEffectMute(bool mute)
{
	if(m_gameData.m_effectMute != mute)
	{
		m_gameData.m_effectMute = mute;
		this->flush();
	}
}

bool UserData::isEffectMute()
{
	//return true;
	return m_gameData.m_effectMute;
}

void UserData::setToolNum(int ktool, int tnum)
{
    if(tnum >= 0)
    {
        m_gameData.m_toolsnum[ktool] = tnum;
        this->flush();
    }
}

int UserData::getToolNum(int ktool)
{
    return m_gameData.m_toolsnum[ktool];
}

void UserData::setFirstRun(bool e)
{
    m_gameData.m_bfirstrun = e;
}
bool UserData::getFirstRun()
{
    return m_gameData.m_bfirstrun;
}

void UserData::setStartNum(int num)
{
    m_gameData.m_startNum = num;
}
int UserData::getStartNum()
{
    return m_gameData.m_startNum;
}

void UserData::flush()
{
	writeData();
}

void UserData::payReslut(int num)
{
    
    BUYID = num;
	//CCAssert(num >= 0 && num <= 2, "Invalid Number");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		JniMethodInfo minfo;
		bool is =JniHelper::getStaticMethodInfo(minfo,"IAP/IAPJni","orderBomb","(I)V");
		if (is)
		{
			CCLOG("order is exist");
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,num);
		}
		else
			CCLOG("find order fail");
	}
#else
	{
		UserData::shared()->setGoldCoins(UserData::shared()->getGoldCoins() + Coins[num]);
        
        if(num == 0)
        {
            for(int i = 0;i < TOOS_NUM;i++)
            {
                UserData::shared()->setToolNum(i, 1);
            }
        }
        
		if (GameInfo::shared())
		{
			GameInfo::shared()->setCoin(UserData::shared()->getGoldCoins());
            for(int i = 0;i < TOOS_NUM;i++)
            {
                if(GameMenu::shared())
                    GameMenu::shared()->setToolNum(i, 1);
            }
		}
        if(LevelMap::shared())
        {
            LevelMap::shared()->setnumSprite();
        }
        //if(DialogBuyGift::shared())
            //DialogBuyGift::shared()->setVirgour(UserData::shared()->getGoldCoins());

	}
#endif
}




