
#include "LabelTextLine.h"


USING_NS_CC;


LabelTextLine::LabelTextLine()
{
	m_batchNode = NULL;
}


LabelTextLine *LabelTextLine::create(const char *fntFile)
{
    LabelTextLine *pRet = new LabelTextLine();
    if(pRet && pRet->init(fntFile))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}


LabelTextLine *LabelTextLine::create(const char *charMapFile, unsigned int charWidth, unsigned int charHeight, unsigned int startCharMap)
{
    LabelTextLine *pRet = new LabelTextLine();
    if(pRet && pRet->init(charMapFile, charWidth, charHeight, startCharMap))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}


LabelTextLine *LabelTextLine::create(CCTexture2D *texture, const CCRect &rect, unsigned int charWidth, unsigned int charHeight, unsigned int startCharMap)
{
    LabelTextLine *pRet = new LabelTextLine();
    if(pRet && pRet->init(texture, rect, charWidth, charHeight, startCharMap))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}


bool LabelTextLine::init(const char *fntFile)
{
	std::string pathStr = CCFileUtils::sharedFileUtils()->fullPathForFilename(fntFile);
	std::string relPathStr = pathStr.substr(0, pathStr.find_last_of("/"))+"/";
	CCDictionary *dict = CCDictionary::createWithContentsOfFile(pathStr.c_str());
  
	CCAssert(((CCString*)dict->objectForKey("version"))->intValue() == 1, "Unsupported version. Upgrade cocos2d version");
    
	std::string texturePathStr = relPathStr + ((CCString*)dict->objectForKey("textureFilename"))->getCString();
	CCString *textureFilename = CCString::create(texturePathStr);
	unsigned int charWidth = ((CCString*)dict->objectForKey("itemWidth"))->intValue() / CC_CONTENT_SCALE_FACTOR();
	unsigned int charHeight = ((CCString*)dict->objectForKey("itemHeight"))->intValue() / CC_CONTENT_SCALE_FACTOR();
	unsigned int startCharMap = ((CCString*)dict->objectForKey("firstChar"))->intValue();

	return init(textureFilename->getCString(), charWidth, charHeight, startCharMap);
}


bool LabelTextLine::init(const char *charMapFile, unsigned int charWidth, unsigned int charHeight, unsigned int startCharMap)
{
	m_batchNode = CCSpriteBatchNode::create(charMapFile);
	CCAssert(m_batchNode != NULL, "ERROR: m_batchNode != NULL");

	CCSize size = m_batchNode->getContentSize();
	if(init(m_batchNode->getTexture(), CCRectMake(0, 0, size.width, size.height), charWidth, charHeight, startCharMap))
	{
		m_batchNode->setPosition(ccp(0, 0));
		this->addChild(m_batchNode);
		return true;
	}

	return false;
}


bool LabelTextLine::init(CCTexture2D *texture, const CCRect &rect, unsigned int charWidth, unsigned int charHeight, unsigned int startCharMap)
{
	CCAssert(texture != NULL, "ERROR: texture != NULL");

	//if(CCSprite::initWithTexture(texture, CCRectMake(0, 0, 0 ,0)))
	if(CCNode::init())
	{
		m_charWidth = charWidth;
		m_charHeight = charHeight;
		m_startCharMap = startCharMap;
		m_textureRect = rect;
		m_texture = texture;

		return true;
	}

	return false;
}


CCSprite *LabelTextLine::text(const char *str)
{
	return textWithLength(str, strlen(str));
}


CCSprite *LabelTextLine::textWithFormat(const char *fmt, ...)
{
    va_list args;
	char temp[256];

	CCAssert(fmt != NULL, "ERROR: fmt != NULL");

    va_start(args, fmt);
	unsigned int length = vsnprintf(temp, 256, fmt, args);
    va_end(args);

	return textWithLength(temp, length);
}


CCSprite *LabelTextLine::textWithLength(const char *str, unsigned int length)
{
	CCAssert(str != NULL, "ERROR: str != NULL");

	unsigned int size = strlen(str);
	CCAssert(size != 0, "ERROR: size != 0");

	if(length == 0)
		length = size;
	if(length > size)
		length = size;

	CCSprite *textSprite = CCSprite::createWithTexture(m_texture, CCRectMake(0, 0, 0, 0));
	CCAssert(textSprite != NULL, "ERROR: textSprite != NULL");
	//textSprite->setAnchorPoint(ccp(0, 0));

	float positionX = 0;
	unsigned int textWidth = 0;
	unsigned int rectWidth = (unsigned int )m_textureRect.size.width;

	while(length-- != 0)
	{
		unsigned int charValue = ((unsigned int)*str++ - m_startCharMap);
		unsigned int offsetX = charValue * m_charWidth;
		if(offsetX >= rectWidth)
			continue;

		CCRect rect = CCRectMake(m_textureRect.origin.x + offsetX, m_textureRect.origin.y, m_charWidth, m_charHeight);
		CCSprite *charSprite = CCSprite::createWithTexture(m_texture, rect);
		CCAssert(charSprite != NULL, "ERROR: charSprite != NULL");
		charSprite->setPosition(ccp(positionX + m_charWidth * 0.5f, 0.0f));
		textSprite->addChild(charSprite);
		positionX += m_charWidth;
		textWidth += m_charWidth;
	}

	textSprite->setContentSize(CCSizeMake(textWidth, m_charHeight));
	if(m_batchNode != NULL)
		m_batchNode->addChild(textSprite);

	return textSprite;
}

void LabelTextLine::remove(CCSprite *textSprite)
{
	CCAssert(textSprite != NULL, "ERROR: textSprite != NULL");

	if(m_batchNode != NULL)
		m_batchNode->removeChild(textSprite, true);
}


void LabelTextLine::addChild(CCNode * child)
{
}


void LabelTextLine::addChild(CCNode * child, int zOrder)
{
}


void LabelTextLine::addChild(CCNode * child, int zOrder, int tag)
{
}


void LabelTextLine::reorderChild(CCNode * child, int zOrder)
{
}

        
void LabelTextLine::removeChild(CCNode* child, bool cleanup)
{
}


void LabelTextLine::removeAllChildrenWithCleanup(bool cleanup)
{
}


void LabelTextLine::sortAllChildren()
{
}



