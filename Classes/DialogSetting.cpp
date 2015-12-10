#include "DialogSetting.h"
#include "UserData.h"
#include "AudioPlayer.h"
#include "MainMenu.h"
#include "LinkCard.h"
#include "DialogGameEnd.h"


USING_NS_CC;

#define DIALOG_SETTING_X_MARGIN		(0.1f)
#define DIALOG_SETTING_Y_MARGIN		(0.1f)

#define DIALOG_SETTING_X_SPACE		(0.35f)
#define DIALOG_SETTING_Y_SPACE		(0.2f)


bool DialogSetting::init(void)
{
	if(!DialogBase::init("images/dialog_setting_title.png", NULL, 
		"images/dialog_button_back1.png", NULL, NULL,
		0.95f, 0.4f, 0.05f))
        return false;

	CCSize titleBarSize = DialogBase::getTitleSize();
	CCSize clientSize = DialogBase::getClientSize();
	CCPoint clientPos = DialogBase::getClientBasePoint();

	//菜单项
	CCMenuItemImage *backMenuItem = CCMenuItemImage::create(
		"images/dialog_button_back.png",
		NULL,
		this,
		menu_selector(DialogSetting::backMenuCallback));
	CCSize backMenuSize = backMenuItem->getContentSize();

	/* 静音开关 */
	CCSprite *musicLabelSprite = CCSprite::create("images/setting_music_label.png");
	CCSize musicLabelSize = musicLabelSprite->getContentSize();
	CCMenuItemImage *musicMenuItem = CCMenuItemImage::create(
		"images/button_sound_on.png",
		"images/button_sound_off.png",
		this,
		menu_selector(DialogSetting::musicMenuCallback));
	CCSize musicMenuSize = musicMenuItem->getContentSize();
	if(UserData::shared()->isMusicMute())
		musicMenuItem->selected();

	/* 音效开关 */
	CCSprite *effectLabelSprite = CCSprite::create("images/setting_effect_label.png");
	CCSize effectLabelSize = effectLabelSprite->getContentSize();
	CCMenuItemImage *effectMenuItem = CCMenuItemImage::create(
		"images/button_sound_on.png",
		"images/button_sound_off.png",
		this,
		menu_selector(DialogSetting::effectMenuCallback));
	CCSize effectMenuSize = effectMenuItem->getContentSize();
	if(UserData::shared()->isEffectMute())
		effectMenuItem->selected();

		float scale = clientSize.width * (1 - DIALOG_SETTING_X_MARGIN * 2 - DIALOG_SETTING_X_SPACE * 1)
		/ (musicLabelSize.width + musicMenuSize.width);
	float xStart = clientPos.x + clientSize.width * DIALOG_SETTING_X_MARGIN;
	//float yStart = clientPos.y + clientSize.height - clientSize.height * DIALOG_SETTING_Y_MARGIN;
	float yStart = clientPos.y + clientSize.height 
		- clientSize.height * (DIALOG_SETTING_Y_MARGIN + DIALOG_SETTING_Y_SPACE / 2);

	/* 音乐静音开关 */
	musicLabelSize = musicLabelSize * scale;
	musicLabelSprite->setScale(scale);
	musicLabelSprite->setPosition(ccp(xStart + musicLabelSize.width * 0.5f, yStart - musicLabelSize.height * 0.5f));
	this->addChild(musicLabelSprite);
	musicMenuSize = musicMenuSize * scale;
	musicMenuItem->setScale(scale);
	musicMenuItem->setPosition(ccp(xStart + musicLabelSize.width 
		+ clientSize.width * DIALOG_SETTING_X_SPACE 
		+ musicMenuSize.width * 0.5f, yStart - musicMenuSize.height * 0.5f));
	yStart -= musicMenuSize.height + clientSize.height * DIALOG_SETTING_Y_SPACE;


	/* 音效静音开关 */
	effectLabelSize = effectLabelSize * scale;
	effectLabelSprite->setScale(scale);
	effectLabelSprite->setPosition(ccp(xStart + effectLabelSize.width * 0.5f, yStart - effectLabelSize.height * 0.5f));
	this->addChild(effectLabelSprite);
	effectMenuSize = effectMenuSize * scale;
	effectMenuItem->setScale(scale);
	effectMenuItem->setPosition(ccp(xStart + effectLabelSize.width 
		+ clientSize.width * DIALOG_SETTING_X_SPACE 
		+ effectMenuSize.width * 0.5f, yStart - effectMenuSize.height * 0.5f));
	yStart -= effectMenuSize.height + clientSize.height * DIALOG_SETTING_Y_SPACE;


	CCMenu *menu = CCMenu::create(musicMenuItem, effectMenuItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	return true;
}

DialogSetting *DialogSetting::create(void)
{
	DialogSetting *pRet = new DialogSetting();
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

void DialogSetting::backMenuCallback(CCObject* pSender)
{
	close();
}

void DialogSetting::restartMenuCallback(CCObject* pSender)
{
	close();
}

void DialogSetting::musicMenuCallback(CCObject *pSender)
{
	bool isMute = UserData::shared()->isMusicMute();
	isMute = !isMute;
	if(isMute)
		((CCMenuItemImage *)pSender)->selected();
	else
		((CCMenuItemImage *)pSender)->unselected();
	AudioPlayer::shared()->setMusicMute(isMute);
}


void DialogSetting::effectMenuCallback(CCObject *pSender)
{
	bool isMute = UserData::shared()->isEffectMute();
	isMute = !isMute;
	if(isMute)
		((CCMenuItemImage *)pSender)->selected();
	else
		((CCMenuItemImage *)pSender)->unselected();
	AudioPlayer::shared()->setEffectMute(isMute);
}


void DialogSetting::close(void)
{
	this->removeFromParent();
}

void DialogSetting::closeClicked(void)
{
	backMenuCallback(NULL);
}

