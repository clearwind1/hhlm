#include "DialogSuperGift.h"
#include "UserData.h"
#include "AudioPlayer.h"
#include "MainMenu.h"
#include "DialogLogonGift.h"


USING_NS_CC;

#define DIALOG_SUPERGIFT_X_MARGIN		(0.1f)
#define DIALOG_SUPERGIFT_Y_MARGIN		(0.1f)

#define DIALOG_SUPERGIFT_X_SPACE		(0.1f)
#define DIALOG_SUPERGIFT_Y_SPACE		(0.05f)

#define DIALOG_SUPERGIFT_Y_CONTENT		(0.8f)
#define DIALOG_SUPERGIFT_Y_BUTTON		(0.25f)


bool DialogSuperGift::init(void)
{
	//if(!DialogBase::init("images/dialog_super_gift_title.png", 0.95f, 0.45f, 0.05f))
    //    return false;
 
	if(!DialogBase::init(NULL,
		"images/dialog_level_title_bg1.png",
		NULL, NULL, NULL, 0.95f, 0.55f, 0.15f))
        return false;

	float scale, xScale, yScale;
	CCSize titleBarSize = DialogBase::getTitleSize();
	CCSize clientSize = DialogBase::getClientSize();
	CCPoint clientPos = DialogBase::getClientBasePoint();

	//自定义标题
	CCSprite *titleSprite = CCSprite::create("images/dialog_super_gift_title.png");
	CCSize titleSize = titleSprite->getContentSize();
	xScale = titleBarSize.width / titleSize.width;
	yScale = titleBarSize.height * 0.3f / titleSize.height;
	if(xScale < yScale)
		scale = xScale;
	else
		scale = yScale;
	titleSprite->setScale(scale);
	titleSprite->setPosition(ccp(clientPos.x + titleBarSize.width * 0.5f, 
		clientPos.y + clientSize.height + titleBarSize.height * (0.5f + 0.1f)));
	this->setTitle(titleSprite);

	//菜单项
	CCMenuItemImage *backMenuItem = CCMenuItemImage::create(
		"images/dialog_button_back.png",
		NULL,
		this,
		menu_selector(DialogSuperGift::backMenuCallback));
	CCSize backMenuSize = backMenuItem->getContentSize();
	CCMenuItemImage *continueMenuItem = CCMenuItemImage::create(
		"images/button_continue1.png",
		NULL,
		this,
		menu_selector(DialogSuperGift::continueMenuCallback));
	CCSize continueMenuSize = continueMenuItem->getContentSize();

	xScale = clientSize.width * (1 - DIALOG_SUPERGIFT_X_MARGIN * 2 - DIALOG_SUPERGIFT_X_SPACE * 1)
		/ (backMenuSize.width + continueMenuSize.width);
	yScale = clientSize.height * DIALOG_SUPERGIFT_Y_BUTTON / backMenuSize.height;
	if(xScale < yScale)
		scale = xScale;
	else
		scale = yScale;

	backMenuSize = backMenuSize * scale; 
	continueMenuSize = continueMenuSize * scale;

	float xStart = clientPos.x 
		+ (clientSize.width - (backMenuSize.width + continueMenuSize.width + clientSize.width * DIALOG_SUPERGIFT_X_SPACE * 1)) * 0.5f;
	float yStart = clientPos.y + clientSize.height * DIALOG_SUPERGIFT_Y_BUTTON * 0.4f;

	yStart += backMenuSize.height * 0.5f;
	backMenuItem->setScale(scale);
	backMenuItem->setPosition(ccp(xStart + backMenuSize.width * 0.5f, yStart));
	xStart += backMenuSize.width + clientSize.width * DIALOG_SUPERGIFT_X_SPACE;

	continueMenuItem->setScale(scale);
	continueMenuItem->setPosition(ccp(xStart + continueMenuSize.width * 0.5f, yStart));
	xStart += continueMenuSize.width + clientSize.width * DIALOG_SUPERGIFT_X_SPACE;

	CCMenu *menu = CCMenu::create(backMenuItem, continueMenuItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 2);
    
    m_buyid = 6;
    const char* picName = {"images/dialog_super_gift_content2.png"};
    if(!UserData::shared()->getFirstRun())
    {
        picName = "images/dialog_super_gift_content.png";
        UserData::shared()->setFirstRun(true);
        
        m_buyid = 0;
    }

	CCSprite *contentSprite = CCSprite::create(picName);
	CCSize contentSize = contentSprite->getContentSize();
	xScale = clientSize.width * (1 - DIALOG_SUPERGIFT_X_MARGIN * 2) / contentSize.width;
	yScale = clientSize.height * DIALOG_SUPERGIFT_Y_CONTENT * (1 - DIALOG_SUPERGIFT_Y_MARGIN * 2) / contentSize.height;
	if(xScale < yScale)
		scale = xScale;
	else
		scale = yScale;
	contentSize = contentSize * scale;
	contentSprite->setScale(scale);
	contentSprite->setPosition(ccp(clientPos.x + clientSize.width * 0.5f, 
		clientPos.y + clientSize.height - contentSize.height * 0.5f));
	this->addChild(contentSprite, 1);

	return true;
}

DialogSuperGift *DialogSuperGift::create(void)
{
	DialogSuperGift *pRet = new DialogSuperGift();
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

void DialogSuperGift::backMenuCallback(CCObject* pSender)
{
	close();
}

void DialogSuperGift::continueMenuCallback(CCObject* pSender)
{
//	UserData::shared()->setGoldCoins(UserData::shared()->getGoldCoins() + 880);
	close();
	UserData::shared()->payReslut(m_buyid);
}

void DialogSuperGift::close(void)
{
	if(UserData::shared()->getGiftCount() != UserData::shared()->getLogonCount())
		this->getParent()->addChild(DialogLogonGift::create(), 100, 101);
	this->removeFromParent();
}

void DialogSuperGift::closeClicked(void)
{
	backMenuCallback(NULL);
}

