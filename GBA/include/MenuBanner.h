////////////////////////////////////////////////////////////
//	File Name:		MenuBanner.h
//	Description:	The banner object in the menu, it's stored
//					as one object and then later split into 9
//					since it's too big for one object
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

#ifndef _MENUBANNER_H
#define _MENUBANNER_H

#define MenuBannerTileWidth		3
#define MenuBannerTileHeight	3

#define MenuBannerTilesSegmentLen 9

// Menu banner with text
#define MenuBannerTilesLen 9216
#define MenuBanner64X32TileLen 1024
extern const unsigned short MenuBannerTiles[4608];

#define MenuBannerPalLen 32
extern const unsigned short MenuBannerPal[16];

#endif //_MENUBANNER_H