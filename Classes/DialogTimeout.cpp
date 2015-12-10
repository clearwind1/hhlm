#include "DialogTimeout.h"
#include "UserData.h"
#include "AudioPlayer.h"
#include "MainMenu.h"
#include "LinkCard.h"
#include "DialogGameEnd.h"


USING_NS_CC;

#define DIALOG_TIMEOUT_X_MARGIN		(0.1f)
#define DIALOG_TIMEOUT_Y_MARGIN		(0.1f)

#define DIALOG_TIMEOUT_X_SPACE		(0.05f)
#define DIALOG_TIMEOUT_Y_SPACE		(0.05f)


bool DialogTimeout::init(void)
{
	if(!DialogBase::init("images/dialog_timeout_title.png", 0.95f, 0.4f, 0.05f))
        return false;
 
	float scale, wScale, hScale;
	CCSize titleBarSize = DialogBase::getTitleSize();
	CCSize clientSize = DialogBase::getClientSize();
	CCPoint clientPos = DialogBase::getClientBasePoint();

	//²Ëµ¥Ïî
	CCMenuItemImage *backMenuItem = CCMenuItemImage::create(
		"images/dialog_button_back.png",
		NULL,
		this,
		menu_selector(DialogTimeout::backMenuCallback));
	CCSize backMenuSize = backMenuItem->getContentSize();
	CCMenuItemImage *restartMenuItem = CCMenuItemImage::create(
		"images/dialog_button_game_fail_delay.png",
		NULL,
		this,
		menu_selector(DialogTimeout::restartMenuCallback));
	CCSize restartMenuSize = restartMenuItem->getContentSize();

	scale = clientSize.width * (1 - DIALOG_TIMEOUT_X_MARGIN * 2 - DIALOG_TIMEOUT_X_SPACE * 1)
		/ (backMenuSize.width + restartMenuSize.width);

	float xStart = clientPos.x + clientSize.width * DIALOG_TIMEOUT_X_MARGIN;
	float yStart = clientPos.y + clientSize.height * DIALOG_TIMEOUT_Y_MARGIN;

	backMenuSize = backMenuSize * scale; 
	yStart += backMenuSize.height * 0.5f;
	backMenuItem->setScale(scale);
	backMenuItem->setPosition(ccp(xStart + backMenuSize.width * 0.5f, yStart));
	xStart += backMenuSize.width + clientSize.width * DIALOG_TIMEOUT_X_SPACE;

	restartMenuSize = restartMenuSize * scale;
	restartMenuItem->setScale(scale);
	restartMenuItem->setPosition(ccp(xStart + restartMenuSize.width * 0.5f, yStart));
	xStart += restartMenuSize.width + clientSize.width * DIALOG_TIMEOUT_X_SPACE;

	CCMenu *menu = CCMenu::create(backMenuItem, restartMenuItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 2);

	CCSprite *contentSprite = CCSprite::create("images/dialog_game_fail_add_time.png");
	CCSize contentSize = contentSprite->getContentSize();
	wScale = clientSize.width * (1 - DIALOG_TIMEOUT_X_MARGIN * 2) / contentSize.width;
	hScale = (clientSize.height * (1 - DIALOG_TIMEOUT_Y_MARGIN * 2) - backMenuSize.height) / contentSize.height;
	if(wScale < hScale)
		scale = wScale;
	else
		scale = hScale;
	contentSize = contentSize * scale;
	contentSprite->setScale(scale);
	contentSprite->setPosition(ccp(clientPos.x + clientSize.width * 0.5f, 
		clientPos.y + clientSize.height - contentSize.height * 0.5f));
	this->addChild(contentSprite, 1);

	return true;
}

DialogTimeout *DialogTimeout::create(void)
{
	DialogTimeout *pRet = new DialogTimeout();
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

void DialogTimeout::backMenuCallback(CCObject* pSender)
{
	close();
	LinkCard::shared()->showFailDialog();
}

void DialogTimeout::restartMenuCallback(CCObject* pSender)
{
	if(LinkCard::shared()->doubleTimeProp() == LinkCardPropSucceed)
		close();
}

void DialogTimeout::close(void)
{
	this->removeFromParent();
}

void DialogTimeout::closeClicked(void)
{
	backMenuCallback(NULL);
}

