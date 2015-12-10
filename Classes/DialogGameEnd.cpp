#include "DialogGameEnd.h"
#include "UserData.h"
#include "AudioPlayer.h"
#include "LevelMap.h"
#include "LinkCard.h"
#include "Funtion.h"
#include "Shop.h"

USING_NS_CC;

#define DIALOG_GAMEEND_X_MARGIN		(0.1f)
#define DIALOG_GAMEEND_Y_MARGIN		(0.1f)

#define DIALOG_GAMEEND_X_SPACE		(0.02f)
#define DIALOG_GAMEEND_Y_SPACE		(0.05f)

#define DIALOG_GAMEEND_STAR_MAX		(3)

bool DialogGameEnd::init(int mode, int level, int stars, int time)
{
	if(mode < DialogGameEndModePass || mode >= DialogGameEndModeMax)
		return false;
	if(level < 0 || level >= LINK_LEVEL_MAX)
		return false;
	if(stars < 0 || stars >= DIALOG_GAMEEND_STAR_MAX)
		return false;
	if(time < 0)
		return false;

	if(!DialogBase::init(NULL, //"images/dialog_level_title.png",
		"images/dialog_level_title_bg1.png",
		NULL, NULL, NULL, 0.95f, 0.55f, 0.15f))
        return false;
 
	m_mode = mode;

	float scale, wScale, hScale;
	CCSize titleBarSize = DialogBase::getTitleSize();
	CCSize clientSize = DialogBase::getClientSize();
	CCPoint clientPos = DialogBase::getClientBasePoint();

	//自定义标题
	CCSprite *titleSprite = CCSprite::create("images/dialog_level_title.png");
	CCSize titleSize = titleSprite->getContentSize();
	wScale = titleBarSize.width / titleSize.width;
	hScale = titleBarSize.height * 0.3f / titleSize.height;
	if(wScale < hScale)
		scale = wScale;
	else
		scale = hScale;
	titleSprite->setScale(scale);
	titleSprite->setPosition(ccp(clientPos.x + titleBarSize.width * 0.5f, 
		clientPos.y + clientSize.height + titleBarSize.height * (0.5f + 0.1f)));

	DigitSprite *levelDigitSprite = DigitSprite::create(0);
	levelDigitSprite->output(level);
	CCSize levelDigitSize = levelDigitSprite->getContentSize();
	levelDigitSprite->setPosition(ccp(titleSize.width * 0.5f,titleSize.height * 0.55f));
	scale =  titleSize.height / levelDigitSize.height;
	levelDigitSprite->setScale(scale);
	titleSprite->addChild(levelDigitSprite);
	this->setTitle(titleSprite);

	//菜单项
	CCMenuItemImage *backMenuItem = CCMenuItemImage::create(
		"images/dialog_button_back.png",
		NULL,
		this,
		menu_selector(DialogGameEnd::backMenuCallback));
	CCSize backMenuSize = backMenuItem->getContentSize();
	CCMenuItemImage *restartMenuItem = CCMenuItemImage::create(
		"images/dialog_level_button_play.png",
		NULL,
		this,
		menu_selector(DialogGameEnd::restartMenuCallback));
	CCSize restartMenuSize = restartMenuItem->getContentSize();
	const char *nextMenuImage;
	if(m_mode == DialogGameEndModePass)
		nextMenuImage = "images/dialog_level_button_next.png";
	else
		nextMenuImage = "images/dialog_level_button_jump.png";
	CCMenuItemImage *nextMenuItem = CCMenuItemImage::create(
		(const char *)nextMenuImage,
		NULL,
		this,
		menu_selector(DialogGameEnd::nextMenuCallback));
	CCSize nextMenuSize = nextMenuItem->getContentSize();
	
	scale = clientSize.width * (1 - DIALOG_GAMEEND_X_MARGIN * 2 - DIALOG_GAMEEND_X_SPACE * 2)
		/ (backMenuSize.width + restartMenuSize.width + nextMenuSize.width);

	float xStart = clientPos.x + clientSize.width * DIALOG_GAMEEND_X_MARGIN;
	float yStart = clientPos.y + clientSize.height * DIALOG_GAMEEND_Y_MARGIN;

	backMenuSize = backMenuSize * scale; 
	yStart += backMenuSize.height * 0.5f;
	backMenuItem->setScale(scale);
	backMenuItem->setPosition(ccp(xStart + backMenuSize.width * 0.5f, yStart));
	//xStart += backMenuSize.width + clientSize.width * DIALOG_GAMEEND_X_SPACE;
	xStart += backMenuSize.width;

	restartMenuSize = restartMenuSize * scale;
	restartMenuItem->setScale(scale);
	restartMenuItem->setPosition(ccp(xStart + restartMenuSize.width * 0.5f, yStart));
	xStart += restartMenuSize.width + clientSize.width * DIALOG_GAMEEND_X_SPACE;

	nextMenuSize = nextMenuSize * scale;
	nextMenuItem->setScale(scale);
	nextMenuItem->setPosition(ccp(xStart + nextMenuSize.width * 0.5f, yStart));

	CCMenu *menu = CCMenu::create(backMenuItem, restartMenuItem, nextMenuItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 2);

	const char *contentImage;
	if(m_mode == DialogGameEndModePass)
		contentImage = "images/dialog_game_pass_content.png";
	else
		contentImage = "images/dialog_game_fail_content.png";
	CCSprite *contentSprite = CCSprite::create((const char *)contentImage);
	CCSize contentSize = contentSprite->getContentSize();
	wScale = clientSize.width * (1 - DIALOG_GAMEEND_X_MARGIN * 2) / contentSize.width;
	hScale = (clientSize.height * (1 - DIALOG_GAMEEND_Y_MARGIN * 2) - backMenuSize.height) / contentSize.height;
	if(wScale < hScale)
		scale = wScale;
	else
		scale = hScale;
	contentSize = contentSize * scale;
	contentSprite->setScale(scale);
	contentSprite->setPosition(ccp(clientPos.x + clientSize.width * 0.5f, 
		clientPos.y + clientSize.height - contentSize.height * 0.5f));
	this->addChild(contentSprite, 1);

	if(m_mode == DialogGameEndModePass)
	{
		//显示星星数
		int index;
		CCSprite *starSprite[DIALOG_GAMEEND_STAR_MAX];
		for(index = 0; index <= stars; index++)
		{
			starSprite[index] = CCSprite::create("images/dialog_game_pass_content_star.png");
		}
		for(; index < DIALOG_GAMEEND_STAR_MAX; index++)
		{
			starSprite[index] = CCSprite::create("images/dialog_game_pass_content_star_1.png");
		}

		CCPoint starPos = contentSprite->getPosition();
		CCSize starSize = starSprite[0]->getContentSize() * scale;
		starPos.x -= starSize.width;
		starPos.y -= contentSize.height * 0.01f;
		for(index = 0; index < DIALOG_GAMEEND_STAR_MAX; index++)
		{
			starSprite[index]->setScale(scale);
			starSprite[index]->setPosition(starPos);
			starPos.x += starSize.width;
			this->addChild(starSprite[index], 1);
		}

		//显示时间
		DigitSprite *timeDigitSprite = DigitSprite::create(0);
		timeDigitSprite->outputWithFormat("%d.%02d",time / 100, time % 100);
		CCSize timeDigitSize = timeDigitSprite->getContentSize();
		scale =  46 * scale * 0.8f / timeDigitSize.height;
		timeDigitSprite->setScale(scale);

		CCPoint timePos = contentSprite->getPosition();
		timePos.x += contentSize.width * 0.1f;
		timePos.y -= contentSize.height * 0.33f;
		timeDigitSprite->setPosition(timePos);
		this->addChild(timeDigitSprite);
	}

	return true;
}

DialogGameEnd *DialogGameEnd::create(int mode, int level, int stars, int time)
{
	DialogGameEnd *pRet = new DialogGameEnd();
	if(pRet != NULL && pRet->init(mode, level, stars, time))
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

void DialogGameEnd::backMenuCallback(CCObject* pSender)
{
	close();
	UserData::shared()->flush();
	CCScene *scene = LevelMap::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.5,scene));
}

void DialogGameEnd::nextMenuCallback(CCObject* pSender)
{
	if(m_mode == DialogGameEndModePass)
	{
		LinkCard::shared()->nextGame();
		close();
	}
	else
	{
		if(LinkCard::shared()->nextLevelProp() == LinkCardPropSucceed)
			close();
	}
}

void DialogGameEnd::restartMenuCallback(CCObject* pSender)
{
    
    if(readGameVirgour() == 0)
    {
        this->addChild(Shop::create(),100);
        return;
    }
    
	close();
	LinkCard::shared()->restartGame();
}

void DialogGameEnd::close(void)
{
	this->removeFromParent();
}

void DialogGameEnd::closeClicked(void)
{
	backMenuCallback(NULL);
}

