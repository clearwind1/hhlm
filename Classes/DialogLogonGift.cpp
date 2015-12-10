#include "DialogLogonGift.h"
#include "UserData.h"
#include "AudioPlayer.h"
#include "MainMenu.h"
#include "LinkCard.h"
#include "DialogGameEnd.h"
#include "LevelMap.h"


USING_NS_CC;

#define DIALOG_LOGON_X_MARGIN		(0.05f)
#define DIALOG_LOGON_Y_MARGIN		(0.05f)

#define DIALOG_LOGON_X_SPACE		(0.05f)
#define DIALOG_LOGON_Y_SPACE		(0.1f)

#define DIALOG_LOGON_Y_DAYS			(0.75f)
#define DIALOG_LOGON_Y_BUTTON		(0.25f)


#define DIALOG_LOGON_IMAGE_WIDTH	(120)
#define DIALOG_LOGON_IMAGE_HEIGHT	(180)

static const int logonGiftGoins[7] = 
{
	1, 2, 3, 5, 6, 8, 10
};

bool DialogLogonGift::init(void)
{
	if(!DialogBase::init("images/dialog_logon_gift_title.png", "images/dialog_title_bg1.png", 
		NULL, NULL, "images/dialog_client_bg4.png",
		0.95f, 0.55f, 0.05f))
        return false;

	CCSize titleBarSize = DialogBase::getTitleSize();
	CCSize clientSize = DialogBase::getClientSize();
	CCPoint clientPos = DialogBase::getClientBasePoint();

	float width = clientSize.width * (1 - DIALOG_LOGON_X_MARGIN * 2 - DIALOG_LOGON_X_SPACE * 3);
	float height = clientSize.height * DIALOG_LOGON_Y_DAYS * (1 - DIALOG_LOGON_Y_MARGIN * 2 - DIALOG_LOGON_Y_SPACE * 1);
	float xScale = width * (1.0f / 4.0f) / DIALOG_LOGON_IMAGE_WIDTH;
	float yScale = height * (1.0f / 2.0f) / DIALOG_LOGON_IMAGE_HEIGHT;
	float scale;
	if(xScale < yScale)
		scale = xScale;
	else
		scale = yScale;
	float imgWidth = DIALOG_LOGON_IMAGE_WIDTH * scale + clientSize.width * DIALOG_LOGON_X_SPACE;
	float imgHeight = DIALOG_LOGON_IMAGE_HEIGHT * scale + clientSize.height * DIALOG_LOGON_Y_DAYS * DIALOG_LOGON_Y_SPACE;

	int index = 0;
	int giftCount = UserData::shared()->getGiftCount();
	int logonCount = UserData::shared()->getLogonCount();
	for(index = 0; index < logonCount; index++)
	{
		CCString *str = CCString::createWithFormat("images/button_day_%d.png", index + 1);
		logonSprite[index] = CCSprite::create(str->getCString());
		logonSprite[index]->setScale(scale);
		if(index < giftCount)
		{
			CCSprite *selSprite = CCSprite::create("images/button_sel_flag.png");
			selSprite->setPosition(ccp(DIALOG_LOGON_IMAGE_WIDTH - selSprite->getContentSize().width * 0.3f, selSprite->getContentSize().height * 0.5f));
			logonSprite[index]->addChild(selSprite);
		}
	}

	for(; index < LOGONGIFT_DAY_MAX; index++)
	{
		CCString *str = CCString::createWithFormat("images/button_day_%d_disable.png", index + 1);
		logonSprite[index] = CCSprite::create(str->getCString());
		logonSprite[index]->setScale(scale);
	}

	float xStart = clientPos.x + clientSize.width * 0.5f - (DIALOG_LOGON_IMAGE_WIDTH * scale * 4 + clientSize.width * DIALOG_LOGON_X_SPACE * 3) * 0.5f + DIALOG_LOGON_IMAGE_WIDTH * 0.5f * scale;
	//float xStart = clientPos.x + clientSize.width * DIALOG_LOGON_X_MARGIN + DIALOG_LOGON_IMAGE_WIDTH * 0.5f * scale;
	//float yStart = clientPos.y + clientSize.height - clientSize.height * DIALOG_LOGON_Y_DAYS * DIALOG_LOGON_Y_MARGIN - DIALOG_LOGON_IMAGE_HEIGHT * 0.5f * scale;
	float yStart = clientPos.y + clientSize.height 
		- clientSize.height * DIALOG_LOGON_Y_DAYS * 0.5f 
		+ DIALOG_LOGON_IMAGE_HEIGHT * 0.5f * scale
		+ clientSize.height * DIALOG_LOGON_Y_DAYS * DIALOG_LOGON_Y_SPACE * 0.5f;
	//yStart = clientPos.y + clientSize.height - DIALOG_LOGON_IMAGE_HEIGHT * 0.5f * scale;
	for(index = 0; index < 4; index++)
	{
		logonSprite[index]->setPosition(ccp(xStart + imgWidth * (float)index, yStart));
		this->addChild(logonSprite[index]);
	}

	xStart = clientPos.x + clientSize.width * 0.5f - (DIALOG_LOGON_IMAGE_WIDTH * scale * 3 + clientSize.width * DIALOG_LOGON_X_SPACE * 2) * 0.5f + DIALOG_LOGON_IMAGE_WIDTH * 0.5f * scale;
	//xStart = clientPos.x + clientSize.width * DIALOG_LOGON_X_MARGIN + DIALOG_LOGON_IMAGE_WIDTH * scale;
	yStart -= imgHeight;
	for(index = 0; index < 3; index++)
	{
		logonSprite[index + 4]->setPosition(ccp(xStart + imgWidth * (float)index, yStart));
		this->addChild(logonSprite[index + 4]);
	}

	//ÁìÈ¡
	CCMenuItemImage *continueMenuItem = CCMenuItemImage::create(
		"images/button_continue.png",
		NULL,
		this,
		menu_selector(DialogLogonGift::continueMenuCallback));
	CCSize continueMenuSize = continueMenuItem->getContentSize();

	width = clientSize.width * (1 - DIALOG_LOGON_X_MARGIN * 2);
	height = clientSize.height * DIALOG_LOGON_Y_BUTTON * (1 - DIALOG_LOGON_Y_MARGIN * 2);
	xScale = width / continueMenuSize.width;
	yScale = height / continueMenuSize.height;
	if(xScale < yScale)
		scale = xScale;
	else
		scale = yScale;

	continueMenuSize = continueMenuSize * scale;
	continueMenuItem->setScale(scale);
	xStart = clientPos.x + clientSize.width * 0.5f;
	yStart = clientPos.y + clientSize.height * DIALOG_LOGON_Y_BUTTON * 0.5f;
	//yStart = clientPos.y + clientSize.height - clientSize.height * DIALOG_LOGON_Y_DAYS - continueMenuSize.height * 0.5f;
	continueMenuItem->setPosition(ccp(xStart, yStart));

	CCMenu *menu = CCMenu::create(continueMenuItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	return true;
}

DialogLogonGift *DialogLogonGift::create(void)
{
	DialogLogonGift *pRet = new DialogLogonGift();
	if(pRet != NULL && pRet->init())
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

void DialogLogonGift::continueMenuCallback(CCObject* pSender)
{
	int goins = 0;
	int giftCount = UserData::shared()->getGiftCount();
	int logonCount = UserData::shared()->getLogonCount();

	while(giftCount < logonCount)
		goins += logonGiftGoins[giftCount++];

	UserData::shared()->setGiftCount();
	if(goins != 0)
		UserData::shared()->setGoldCoins(UserData::shared()->getGoldCoins() + goins);
    
    if(LevelMap::shared())
        LevelMap::shared()->setnumSprite();
    
	close();
}


void DialogLogonGift::close(void)
{
	this->removeFromParent();
}

void DialogLogonGift::closeClicked(void)
{
	close();
}

