/*
 * CAudioSettings.cpp
 *
 *  Created on: 28.11.2009
 *      Author: gerstrong
 */

#include "OverlaySettings.h"
#include <base/interface/StringUtils.h>
#include <base/CInput.h>
#include <base/video/CVideoDriver.h>

#include "engine/core/CBehaviorEngine.h"
#include "engine/core/CSettings.h"
#include "engine/core/VGamepads/vgamepadsimple.h"


OverlaySettings::OverlaySettings(const Style &style) :
GameMenu(GsRect<float>(0.075f, 0.24f, 0.85f, 0.4f), style )
{    
    setMenuLabel("KEYBMENULABEL");

    mUsersConf = gVideoDriver.getVidConfig();

    mpShowCursorSwitch =
            mpMenuDialog->add( new Switch("Cursor", style) );
#ifdef USE_VIRTUALPAD
    mpVPadSwitch  = new Switch( "VirtPad", style );
    mpMenuDialog->add( mpVPadSwitch );

    const auto iHeightFac = mUsersConf.mVPadHeight;
    const auto iWidthFac  = mUsersConf.mVPadWidth;

    mpVPadWidth =
        mpMenuDialog->add(
            new NumberControl( "Width", 100, 400, 10, iWidthFac,
                              false, getStyle() ) );
    mpVPadHeight =
        mpMenuDialog->add(
            new NumberControl( "Height", 100, 400, 10, iHeightFac,
                              false, getStyle() ) );
#endif
    mpMenuDialog->fit();
    select(1);
}


void OverlaySettings::refresh()
{
    mUsersConf = gVideoDriver.getVidConfig();

    mpShowCursorSwitch->enable( mUsersConf.mShowCursor );


#ifdef USE_VIRTUALPAD
    mpVPadSwitch->enable(mUsersConf.mVPad);
    mpVPadWidth->setSelection(mUsersConf.mVPadWidth);
    mpVPadHeight->setSelection(mUsersConf.mVPadHeight);
#endif
}


void OverlaySettings::ponder(const float /*deltaT*/)
{
    GameMenu::ponder(0);

#ifdef USE_VIRTUALPAD
    auto &activeCfg = gVideoDriver.getVidConfig();

    activeCfg.mVPad = mpVPadSwitch->isEnabled();
    activeCfg.mVPadWidth = mpVPadWidth->getSelection();
    activeCfg.mVPadHeight = mpVPadHeight->getSelection();
#endif
}


void OverlaySettings::release()
{           
    mUsersConf = gVideoDriver.getVidConfig();

    mUsersConf.mShowCursor = mpShowCursorSwitch->isEnabled();

    gVideoDriver.setVidConfig(mUsersConf);

    gSettings.saveDrvCfg();
}

