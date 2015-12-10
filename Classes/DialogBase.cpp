

#include "DialogBase.h"


USING_NS_CC;


DialogBase *DialogBase::create()
{
	return DialogBase::create(NULL, NULL, NULL, NULL, NULL, 1.0f, 1.0f, 0.05f);
}

DialogBase *DialogBase::create(float width, float height, float titleHeight)
{
	return DialogBase::create(NULL, NULL, NULL, NULL, NULL, width, height, titleHeight);
}

DialogBase *DialogBase::create(const char *titleImage, float width, float height, float titleHeight)
{
	return DialogBase::create(titleImage, NULL, NULL, NULL, NULL, width, height, titleHeight);
}

DialogBase *DialogBase::create(const char *titleImage, const char *titleBgImage,
	const char *closeImage, const char *closePressedImage,
	const char *clientBgImage, float width, float height, float titleHeight)
{
	DialogBase *pRet = new DialogBase();
	if(pRet != NULL &&
		pRet->init(titleImage, titleBgImage, closeImage, closePressedImage, 
		clientBgImage, width, height, titleHeight))
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

bool DialogBase::init()
{
	return DialogBase::init(NULL, NULL, NULL, NULL, NULL, 1.0f, 1.0f, 0.05f);
}

bool DialogBase::init(float width, float height, float titleHeight)
{
	return DialogBase::init(NULL, NULL, NULL, NULL, NULL, width, height, titleHeight);
}

bool DialogBase::init(const char *titleImage, float width, float height, float titleHeight)
{
	return DialogBase::init(titleImage, NULL, NULL, NULL, NULL, width, height, titleHeight);
}

bool DialogBase::init(const char *titleImage, const char *titleBgImage,
	const char *closeImage, const char *closePressedImage,
	const char *clientBgImage, float width, float height, float titleHeight)
{
    if(!CCLayer::init())
        return false;
	if(width <= 0.0f || width > 1.0f 
		|| height <= 0.0f || height > 1.0f 
		|| titleHeight <= 0.0f || titleHeight > 1.0f)
		return false;

	if(titleBgImage == NULL)
		titleBgImage = "images/dialog_title_bg.png";
	if(clientBgImage == NULL)
		clientBgImage = "images/dialog_client_bg2.png";
  
	float wScale, hScale, scale;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	titleHeight = winSize.height * titleHeight;
	m_dialogSize.width = winSize.width * width;
	m_dialogSize.height = winSize.height * height;
	float xStart = (winSize.width - m_dialogSize.width) / 2;
	float yStart = (winSize.height - m_dialogSize.height) / 2 + m_dialogSize.height;
	m_clientBasePoint.x = xStart;
	m_clientBasePoint.y = yStart - m_dialogSize.height;

	/* 对话框标题栏背景 */
	CCSprite *titleBgSprite = CCSprite::create(titleBgImage);
	m_titleSize = titleBgSprite->getContentSize();
	wScale = m_dialogSize.width / m_titleSize.width;
	hScale = titleHeight / m_titleSize.height;
	titleBgSprite->setScaleX(wScale);
	titleBgSprite->setScaleY(hScale);
	m_titleSize.width *= wScale;
	m_titleSize.height *= hScale;
	titleBgSprite->setPosition(ccp(xStart + m_titleSize.width / 2, yStart - m_titleSize.height / 2));
	this->addChild(titleBgSprite, 1);

	/* 对话框标题 */
	m_titleSprite = NULL;
	if(titleImage != NULL)
	{
		m_titleSprite = CCSprite::create(titleImage);
		CCSize titleSize = m_titleSprite->getContentSize();
		wScale = m_titleSize.width / titleSize.width;
		hScale = m_titleSize.height * 0.8f / titleSize.height;
		if(wScale < hScale)
			scale = wScale;
		else
			scale = hScale;
		m_titleSprite->setScale(scale);
		CCPoint titlePos = titleBgSprite->getPosition();
		m_titleSprite->setPosition(titlePos);
		this->addChild(m_titleSprite, 1);
	}

	/* 关闭按钮 */
	if(closeImage != NULL)
	{
		CCMenuItemImage *closeMenuItem = CCMenuItemImage::create(
			closeImage, 
			closePressedImage, 
			this, 
			menu_selector(DialogBase::menuCloseCallback));
		CCSize closeMenuSize = closeMenuItem->getContentSize();
		wScale = m_titleSize.width / closeMenuSize.width;
		hScale = m_titleSize.height / closeMenuSize.height;
		if(wScale < hScale)
			scale = wScale;
		else
			scale = hScale;
		closeMenuSize = closeMenuSize * scale;
		closeMenuItem->setScale(scale);
		CCPoint closeMenuPos = titleBgSprite->getPosition();
		closeMenuPos.x += m_titleSize.width / 2 - m_titleSize.height;
		closeMenuItem->setPosition(closeMenuPos);

		CCMenu *menu = CCMenu::create(closeMenuItem, NULL);
		menu->setPosition(CCPointZero);
		this->addChild(menu, 1);
	}

	yStart -= m_titleSize.height * 0.5f;

	m_clientSize.width = m_dialogSize.width;
	m_clientSize.height = m_dialogSize.height - m_titleSize.height * 0.5f;

	/* 对话框背景 */
	CCSprite *clientBgSprite = CCSprite::create(clientBgImage);
	CCSize clientBgSize = clientBgSprite->getContentSize();
	//wScale = m_clientSize.width / clientBgSize.width;
	wScale = titleBgSprite->getScaleX();
	hScale = m_clientSize.height / clientBgSize.height;
	clientBgSprite->setScaleX(wScale);
	clientBgSprite->setScaleY(hScale);
	clientBgSprite->setPosition(ccp(xStart + m_clientSize.width / 2, yStart - m_clientSize.height / 2));
	this->addChild(clientBgSprite, 0);

	//m_clientSize.height -= m_titleSize.height * 0.5f;
	m_clientSize.height = m_dialogSize.height - m_titleSize.height;

	this->setTouchEnabled(true);    
	//this->setKeypadEnabled(true);

	return true;
}

CCSprite *DialogBase::getTitle(void)
{
	return m_titleSprite;
}

void DialogBase::setTitle(CCSprite *titleSprite)
{
	if(m_titleSprite != NULL)
	{
		this->removeChild(m_titleSprite);
	}
	m_titleSprite = titleSprite;
	if(m_titleSprite != NULL)
	{
		this->addChild(m_titleSprite, 1);
	}
}

const CCSize &DialogBase::getSize(void)
{
	return m_dialogSize;
}

const CCSize &DialogBase::getTitleSize(void)
{
	return m_titleSize;
}

const CCSize &DialogBase::getClientSize(void)
{
	return m_clientSize;
}

const CCPoint &DialogBase::getClientBasePoint(void)
{
	return m_clientBasePoint;
}

void DialogBase::closeClicked(void)
{
	hide();
}

void DialogBase::menuCloseCallback(CCObject* pSender)
{
	closeClicked();
}

void DialogBase::keyBackClicked(void)
{
	closeClicked();
}

void DialogBase::show(void)
{
	this->setVisible(true);
}

void DialogBase::hide(void)
{
	this->setVisible(false);
}

bool DialogBase::ccTouchBegan(cocos2d::CCTouch *pTouch,  cocos2d::CCEvent *pEvent)
{
	if(isVisible())
		return true;
	return false;
}

void DialogBase::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this,  kCCMenuHandlerPriority,  true);
}

