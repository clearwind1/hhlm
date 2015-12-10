#ifndef __CARD_MAP_H__
#define __CARD_MAP_H__

#include "cocos2d.h"

USING_NS_CC;

enum
{
	LinkMapTypeEmpty = 0,
	LinkMapTypeNormal = 1,
	LinkMapTypeFreeze = 2,
	LinkMapTypeDisable = 3,
	LinkMapTypeMax
};

class LinkMap : public CCObject
{
public:
	bool init(int style = 1);

	static LinkMap *create(int style = 1);
	static int getMapTableMax();

	int getStyle();
	int getTime();
	int getWidth();
	int getHeight();
	int getSize();
	int getTotal();
	int getCardCount();
	int getCardType(int index);

private:
	int m_style;
	int m_count;
	unsigned char *m_map;
};

#endif //__CARD_MAP_H__