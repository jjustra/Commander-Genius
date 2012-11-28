/*
 * CPlatformVertical.h
 *
 *  Created on: 06.04.2011
 *      Author: gerstrong
 *
 *  This is the code for the vertical platforms in galaxy
 */

#ifndef CPLATFORMVERTICAL_H_
#define CPLATFORMVERTICAL_H_

#include "CPlatform.h"
#include <memory>

namespace galaxy {

class CPlatformVertical : public CPlatform
{
public:
	CPlatformVertical(CMap *pmap, const Uint16 foeID, const Uint32 x, const Uint32 y,
			const direction_t vertdir, const int actionOffset);

	void draw();
	
	void process();

private:
    
	bool drawFire;
	int m_FireSprite;
	int m_fireTimer;
};


}

#endif /* CPLATFORMVERTICAL_H_ */

