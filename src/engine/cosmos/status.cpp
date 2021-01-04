#include <stdio.h>
#include "status.h"
#include "defines.h"
#include "tile.h"
#include "video.h"
#include "map.h"
#include "game.h"
#include "font.h"
#include "player.h"

#include <base/video/CVideoDriver.h>

Tile *status_tiles = nullptr;

const int  STATUS_BAR_HEIGHT = 6;
const int  STATUS_BAR_WIDTH = 38;


namespace cosmos_engine
{

void Status::loadTiles()
{
    uint16 num_tiles;
    status_tiles = load_tiles("STATUS.MNI", SOLID, &num_tiles);
    printf("Loading %d status tiles.\n", num_tiles);
}

void Status::init()
{
    numStarsCollected = 0;
}

void Status::initPanel()
{
    video_fill_screen_with_black();
    display();
    addToScoreUpdateOnDisplay(0, GsVec2D<int>(9, 0x16));
    updateHealthBarDisplay();
    displayNumStarsCollected();
    displayNumBombsLeft();
}


void Status::display()
{
    for(int y=0; y < STATUS_BAR_HEIGHT; y++)
    {
        for (int x = 0; x < STATUS_BAR_WIDTH; x++)
        {
            video_draw_tile(&status_tiles[x + y * STATUS_BAR_WIDTH], (x + 1) * TILE_WIDTH, (y+MAP_WINDOW_HEIGHT+1) * TILE_HEIGHT);
        }
    }
}

void Status::displayEverything()
{
    GsWeakSurface blitSfc(gVideoDriver.getBlitSurface());
    const GsRect<Uint16> rect(0, 144, 320, 56);

    GsColor color(0, 0, 0);
    blitSfc.fill(rect, color);

    display();
    addToScoreUpdateOnDisplay(0, GsVec2D<int>(9, 0x16));
    updateHealthBarDisplay();
    displayNumStarsCollected();
    displayNumBombsLeft();
}

void Status::addToScoreUpdateOnDisplay(const int amount_to_add_low,
                                       const GsVec2D<int> pos)
{
    score += amount_to_add_low;
    display_number(pos.x, pos.y, score);
}

void Status::updateHealthBarDisplay()
{
    int x = 0x11;
    int y = 0x16;
    for(int i=0;i< num_health_bars;i++)
    {
        if(health - 1 > i)
        {
            video_draw_tile(&font_tiles[95], (x - i) * TILE_WIDTH, y * TILE_HEIGHT);
            video_draw_tile(&font_tiles[96], (x - i) * TILE_WIDTH, (y+1) * TILE_HEIGHT);
        }
        else
        {
            video_draw_tile(&font_tiles[9], (x - i) * TILE_WIDTH, y * TILE_HEIGHT);
            video_draw_tile(&font_tiles[8], (x - i) * TILE_WIDTH, (y+1) * TILE_HEIGHT);
        }
    }
}

void Status::displayNumStarsCollected()
{
    display_number(0x23,0x16, numStarsCollected);
}


void Status::displayNumBombsLeft()
{
    video_draw_tile(&font_tiles[97], 0x18 * TILE_WIDTH, 0x17 * TILE_HEIGHT);
    display_number(0x18, 0x17, num_bombs);
}

int Status::numStartsCollected() const
{
    return numStarsCollected;
}

void Status::setNumStartsCollected(const int value)
{
    numStarsCollected = value;
}

void Status::addStar()
{
    numStarsCollected++;
}

};
