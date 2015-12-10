

#ifndef __LABEL_TEXT_LINE_H__
#define __LABEL_TEXT_LINE_H__


#include "cocos2d.h"

USING_NS_CC;

class LabelTextLine : public CCNode
{
public:
	LabelTextLine();
 
	static LabelTextLine *create(const char *fntFile);
    static LabelTextLine *create(const char *charMapFile, unsigned int charWidth, unsigned int charHeight, unsigned int startCharMap);
    static LabelTextLine *create(CCTexture2D *texture, const CCRect &rect, unsigned int charWidth, unsigned int charHeight, unsigned int startCharMap);

    bool init(const char *fntFile);
    bool init(const char *charMapFile, unsigned int charWidth, unsigned int charHeight, unsigned int startCharMap);
    bool init(CCTexture2D *texture, const CCRect &rect, unsigned int charWidth, unsigned int charHeight, unsigned int startCharMap);

    virtual void addChild(CCNode * child);
    virtual void addChild(CCNode * child, int zOrder);
    virtual void addChild(CCNode * child, int zOrder, int tag);
    virtual void reorderChild(CCNode * child, int zOrder);
        
    virtual void removeChild(CCNode* child, bool cleanup);
    virtual void removeAllChildrenWithCleanup(bool cleanup);
    virtual void sortAllChildren();

	CCSprite *text(const char *str);
	CCSprite *textWithFormat(const char *fmt, ...);
	CCSprite *textWithLength(const char *str, unsigned int length);
	void remove(CCSprite *textSprite);

private:


private:
	unsigned int m_charWidth;
	unsigned int m_charHeight;
	unsigned int m_startCharMap;
	CCRect m_textureRect;
	CCTexture2D *m_texture;
	CCSpriteBatchNode *m_batchNode;
};

#endif //__LABEL_TEXT_LINE_H__

