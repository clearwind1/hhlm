#include "MapConfig.h"

#if (MAP_EDITOR != 0)
#include "PlayGame.h"
#include "UserData.h"
#include "MainMenu.h"


USING_NS_CC;

#define LEVELMAP_IMAGE_WIDTH		(512)
#define LEVELMAP_IMAGE_HEIGHT		(512)

static const char *mapFileName[MAP_CONFIG_MAX] = 
{
	"images/map_1.jpg",
	"images/map_2.jpg",
	"images/map_3.jpg",
	"images/map_4.jpg",
	"images/map_5.jpg",
	"images/map_6.jpg",
	"images/map_7.jpg",
	"images/map_8.jpg",
	"images/map_9.jpg",
	"images/map_10.jpg",
	"images/map_11.jpg"
};

struct LockPos
{
	int x;
	int y;
};

CCScene* MapConfig::scene()
{
    CCScene *scene = CCScene::create();

    MapConfig *layer = MapConfig::create();

    scene->addChild(layer);

    return scene;
}


MapConfig *MapConfig::create()
{
    MapConfig *pRet = new MapConfig();
    if(pRet && pRet->init()) 
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

bool MapConfig::init()
{
    if(!CCLayer::init())
        return false;

	CCMenuItemImage *addMenuItem = CCMenuItemImage::create(
		"tools/map_config_add.png",
		"tools/map_config_add_press.png",
		this,
		menu_selector(MapConfig::addMenuCallback));

	CCMenuItemImage *okMenuItem = CCMenuItemImage::create(
		"tools/map_config_ok.png",
		"tools/map_config_ok_press.png",
		this,
		menu_selector(MapConfig::okMenuCallback));

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize menuSize = addMenuItem->getContentSize();
	float scale = winSize.width * 0.1f / menuSize.width;
	menuSize = menuSize * scale;

	addMenuItem->setScale(scale);
	addMenuItem->setPosition(ccp(winSize.width - menuSize.width * 0.5f, winSize.height * 0.5f + menuSize.height));

	okMenuItem->setScale(scale);
	okMenuItem->setPosition(ccp(winSize.width - menuSize.width * 0.5f, winSize.height * 0.5f - menuSize.height));

	CCMenu *menu = CCMenu::create(addMenuItem, okMenuItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 200);

	m_lockArray = CCArray::create();
	m_lockArray->retain();

	for(int index = 0; index < MAP_CONFIG_MAX; index++)
	{
		m_map[index] = CCSprite::create((const char *)mapFileName[index]);
		this->addChild(m_map[index]);
	}

	m_winSize = CCDirector::sharedDirector()->getWinSize();
	m_imgSize = m_map[0]->getContentSize();
	m_scale = m_winSize.width / m_imgSize.width;
	m_imgSize = m_imgSize * m_scale;
	m_mapSize.width = m_imgSize.width;
	m_mapSize.height = m_imgSize.height * MAP_CONFIG_MAX;

	for(int index = 0; index < MAP_CONFIG_MAX; index++)
	{
		m_map[index]->setScale(m_scale);
		m_map[index]->setAnchorPoint(ccp(0.0f, 0.0f));
		m_map[index]->setPosition(ccp(0, m_imgSize.height * index));
	}

	m_spriteBatchNode = CCSpriteBatchNode::create("images/map_lock.png");
	m_spriteBatchNode->setPosition(ccp(0,0));
	this->addChild(m_spriteBatchNode);

	m_lockDigitSprite = DigitSprite::create(0);
	m_lockDigitSprite->output(0);
	CCSize lockDigitSize = m_lockDigitSprite->getContentSize();
	scale = winSize.height * 0.05f / lockDigitSize.height;
	lockDigitSize = lockDigitSize * scale;
	m_lockDigitSprite->setScale(scale);
	m_lockDigitSprite->setAnchorPoint(ccp(0, 1));
	m_lockDigitSprite->setPosition(ccp(10, winSize.height));
	this->addChild(m_lockDigitSprite, 201);

	m_pos = 0;
	m_moveY = 0;
	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);
    
    return true;
}

void MapConfig::addMenuCallback(CCObject* pSender)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *sprite = CCSprite::createWithTexture(m_spriteBatchNode->getTexture());
	sprite->setScale(m_scale * 0.6f);
	sprite->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
	m_spriteBatchNode->addChild(sprite);

	m_lockArray->addObject(sprite);
	m_lockDigitSprite->output((int)m_lockArray->count());
}

bool MapConfig::writeData(void)
{
	bool result = false;
	int size, count = 0;
	size = m_lockArray->count() *32;
	unsigned char *data = new unsigned char[size];

	CCObject *object;
	CCARRAY_FOREACH(m_lockArray, object)
	{
		CCSprite *sprite = (CCSprite *)object;
		CCPoint position = sprite->getPosition();
		int tmp = snprintf((char *)&data[count], size - count, "{%d, %d},\r\n",(int)(position.x / m_scale), (int)((position.y + m_pos) / m_scale));
		CCAssert(tmp > 0, "Error value!!");
		count += tmp;
	}

	CCLog("count/size:%d/%d", count, size);

	std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "mapdata.txt";
	FILE *file = fopen(filePath.c_str(), "wb");
	if(file != NULL)
	{
		if(fwrite(data,1,count,file) == count)
			result = true;
		fclose(file);
	}
	delete []data;

	return result;
}

void MapConfig::okMenuCallback(CCObject* pSender)
{
	writeData();
}

void MapConfig::setPos(float pos)
{
	float tmp = m_pos + pos;

	if(tmp < 0)
		tmp = 0;
	if(tmp > m_mapSize.height - m_winSize.height)
		tmp = m_mapSize.height - m_winSize.height;

	for(int index = 0; index < MAP_CONFIG_MAX; index++)
	{
		m_map[index]->setPosition(ccp(0, m_imgSize.height * index - tmp));
	}

	CCObject *object;
	CCARRAY_FOREACH(m_lockArray, object)
	{
		CCSprite *sprite = (CCSprite *)object;
		CCPoint position = sprite->getPosition();
		position.y += m_pos - tmp;
		sprite->setPosition(position);
	}

	m_pos = tmp;
}

void MapConfig::timeUpdateCallBack(float dt)
{
	m_touchCount++;
}

void MapConfig::startMovePos(float dt)
{
	if(m_endPos != 0)
	{
		if(m_endPos > m_step)
		{
			setPos(m_step);
			m_endPos -= m_step;
		}
		else
		{
			setPos(m_endPos);
			m_endPos = 0;
		}

		this->scheduleOnce(schedule_selector(MapConfig::startMovePos), 0.1f);
	}
}

bool MapConfig::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint point = pTouch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);

	m_curLock = NULL;
	CCObject *object;
	CCARRAY_FOREACH_REVERSE(m_lockArray, object)
	{
		CCSprite *sprite = (CCSprite *)object;
		CCSize halfSize = sprite->getContentSize() * sprite->getScale() * 0.5f;
		CCPoint position = sprite->getPosition();
		if(point.x >= position.x - halfSize.width
			&& point.x <= position.x + halfSize.width
			&& point .y >= position.y - halfSize.height
			&& point .y <= position.y + halfSize.height)
		{
			m_curLock = sprite;
			break;
		}
		sprite->setPosition(position);
	}

	if(m_curLock == NULL)
	{
		m_beganY = point.y;
		m_moveY = m_beganY;
		m_touchCount = 0;
		this->schedule(schedule_selector(MapConfig::timeUpdateCallBack), 0.01f);
	}

	return true;
}

void MapConfig::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint point = pTouch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);

	if(m_curLock != NULL)
	{
		//CCPoint position = m_curLock->getPosition();
		m_curLock->setPosition(point);
	}
	else
	{
		setPos(m_moveY - point.y);
		m_moveY = point.y;
	}
}

void MapConfig::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint point = pTouch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);

	if(m_curLock == NULL)
	{
		this->unschedule(schedule_selector(MapConfig::timeUpdateCallBack));

		float ySize = (m_beganY - point.y) / (float)m_touchCount;
		if(ySize < 10)
			return;
		m_endPos = ySize * 100;
		m_step = m_endPos / 100;
		startMovePos(0);
	}
	else
	{
		m_curLock = NULL;
	}
}

void MapConfig::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

void MapConfig::keyBackClicked(void)
{
	CCScene *scene = MainMenu::scene(true);
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.5,scene));
}
#endif