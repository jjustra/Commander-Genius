#include "GsScrollsurface.h"

#include "base/video/CVideoDriver.h"

GsScrollSurface::GsScrollSurface()
{

}

bool GsScrollSurface::create(const unsigned int mode,
                             const unsigned int squareSize)
{
    mScrollSurface.create(mode,
                          squareSize,
                          squareSize, 32,
                          0x00FF0000,
                          0x0000FF00,
                          0x000000FF,
                          0xFF000000);

    // TODO: Create proper Texture here.
    //mScrollTexture

    return !mScrollSurface.empty();
}

void GsScrollSurface::resetScrollbuffer()
{
    mSbufferx = 0;
    mSbuffery = 0;
}

void GsScrollSurface::updateScrollBuf(const GsVec2D<int> SBuffer)
{
    const int drawMask = getSquareSize()-1;
    mSbufferx = SBuffer.x&drawMask;
    mSbuffery = SBuffer.y&drawMask;
}

void GsScrollSurface::UpdateScrollBufX(const Sint16 SBufferX)
{
    const int drawMask = getSquareSize()-1;
    mSbufferx = SBufferX&drawMask;
}

void GsScrollSurface::UpdateScrollBufY(const Sint16 SBufferY)
{
    const int drawMask = getSquareSize()-1;
    mSbuffery = SBufferY&drawMask;
}

void GsScrollSurface::readScrollBuffer(Sint16 &x, Sint16 &y) const
{
    x = mSbufferx; y = mSbuffery;
}

int GsScrollSurface::getSquareSize() const
{
    assert(mScrollSurface);
    return mScrollSurface.width();
}

GsSurface &GsScrollSurface::getScrollSurface()
{
    return mScrollSurface;
}

Uint32 GsScrollSurface::getFlags() const
{
    assert(mScrollSurface);
    return mScrollSurface.getSDLSurface()->flags;
}

void GsScrollSurface::setScale(const float scale)
{
    mScale = scale;
}

void GsScrollSurface::blitScrollSurface(GsWeakSurface &blitSfc) // This is only for tiles
{									                            // The name should be changed
    SDL_Rect srcRect, dstRect;
    int sbufferx, sbuffery;

    CVidConfig &vidConf = gVideoDriver.getVidConfig();
    SDL_Rect Gamerect = vidConf.mGameRect.SDLRect();

    const int scrollSfcSize = getSquareSize();
    const auto &visGA = gVideoDriver.mpVideoEngine->mRelativeVisGameArea;

    Gamerect.x = visGA.pos.x;
    Gamerect.y = visGA.pos.y;
    Gamerect.w = visGA.dim.x+16;
    Gamerect.h = visGA.dim.y+16;

    dstRect.x = Gamerect.x;
    dstRect.y = Gamerect.y;
    //dstRect.w = Gamerect.w;
    //dstRect.h = Gamerect.h;

    sbufferx = mSbufferx + dstRect.x;
    sbuffery = mSbuffery + dstRect.y;

    // Clip the scrollbuffer correctly
    if(sbufferx > scrollSfcSize)
        sbufferx -= scrollSfcSize;
    if(sbuffery > scrollSfcSize)
        sbuffery -= scrollSfcSize;

    srcRect.x = sbufferx;
    srcRect.y = sbuffery;

    const bool wraphoz = (sbufferx+Gamerect.w > scrollSfcSize);
    const bool wrapvrt = (sbuffery+Gamerect.h > scrollSfcSize);

    // Upper-Left Part to draw from the Scrollbuffer
    srcRect.w = wraphoz ? (scrollSfcSize-sbufferx) : Gamerect.w;
    srcRect.h = wrapvrt ? (scrollSfcSize-sbuffery) : Gamerect.h;

    dstRect.w = srcRect.w*mScale;
    dstRect.h = srcRect.h*mScale;

    mScrollSurface.blitScaledTo(blitSfc, srcRect, dstRect);

    const Uint16 upperLeftW = srcRect.w;
    const Uint16 upperLeftH = srcRect.h;

    // upper-right part
    if (wraphoz)
    {
        srcRect.w = Gamerect.w - upperLeftW;
        srcRect.x = 0;
        dstRect.x = (Gamerect.x + upperLeftW)*mScale;
        dstRect.w = srcRect.w*mScale;

        mScrollSurface.blitScaledTo(blitSfc, srcRect, dstRect);
    }

    // lower-right part
    if (wrapvrt)
    {
        srcRect.h = Gamerect.h - upperLeftH;
        srcRect.y = 0;
        dstRect.y = Gamerect.y + upperLeftH;
        dstRect.h = srcRect.h*mScale;

        mScrollSurface.blitScaledTo(blitSfc, srcRect, dstRect);
    }

    if(!wraphoz || !wrapvrt)
        return;

    // and lower-left part
    srcRect.x = sbufferx;
    srcRect.y = 0;
    srcRect.w = upperLeftW;

    dstRect.x = Gamerect.x;
    dstRect.y = Gamerect.y+upperLeftH;
    dstRect.w = srcRect.w*mScale;
    dstRect.h = srcRect.h*mScale;

    mScrollSurface.blitScaledTo(blitSfc, srcRect, dstRect);
}

void GsScrollSurface::renderScrollSurface()
{
    // TODO: iterate through a list of texture tiles to update and send those to the rendering engine

    /*
    SDL_Rect srcRect, dstRect;
    int sbufferx, sbuffery;

    CVidConfig &vidConf = gVideoDriver.getVidConfig();
    SDL_Rect Gamerect = vidConf.mGameRect.SDLRect();

    const int scrollSfcSize = getSquareSize();
    const auto &visGA = gVideoDriver.mpVideoEngine->mRelativeVisGameArea;

    Gamerect.x = visGA.pos.x;
    Gamerect.y = visGA.pos.y;
    Gamerect.w = visGA.dim.x+16;
    Gamerect.h = visGA.dim.y+16;

    dstRect.x = Gamerect.x;
    dstRect.y = Gamerect.y;
    //dstRect.w = Gamerect.w;
    //dstRect.h = Gamerect.h;

    sbufferx = mSbufferx + dstRect.x;
    sbuffery = mSbuffery + dstRect.y;

    // Clip the scrollbuffer correctly
    if(sbufferx > scrollSfcSize)
        sbufferx -= scrollSfcSize;
    if(sbuffery > scrollSfcSize)
        sbuffery -= scrollSfcSize;

    srcRect.x = sbufferx;
    srcRect.y = sbuffery;

    const bool wraphoz = (sbufferx+Gamerect.w > scrollSfcSize);
    const bool wrapvrt = (sbuffery+Gamerect.h > scrollSfcSize);

    // Upper-Left Part to draw from the Scrollbuffer
    srcRect.w = wraphoz ? (scrollSfcSize-sbufferx) : Gamerect.w;
    srcRect.h = wrapvrt ? (scrollSfcSize-sbuffery) : Gamerect.h;

    dstRect.w = srcRect.w*mScale;
    dstRect.h = srcRect.h*mScale;

    mScrollSurface.blitScaledTo(blitSfc, srcRect, dstRect);

    const Uint16 upperLeftW = srcRect.w;
    const Uint16 upperLeftH = srcRect.h;

    // upper-right part
    if (wraphoz)
    {
        srcRect.w = Gamerect.w - upperLeftW;
        srcRect.x = 0;
        dstRect.x = (Gamerect.x + upperLeftW)*mScale;
        dstRect.w = srcRect.w*mScale;

        mScrollSurface.blitScaledTo(blitSfc, srcRect, dstRect);
    }

    // lower-right part
    if (wrapvrt)
    {
        srcRect.h = Gamerect.h - upperLeftH;
        srcRect.y = 0;
        dstRect.y = Gamerect.y + upperLeftH;
        dstRect.h = srcRect.h*mScale;

        mScrollSurface.blitScaledTo(blitSfc, srcRect, dstRect);
    }

    if(!wraphoz || !wrapvrt)
        return;

    // and lower-left part
    srcRect.x = sbufferx;
    srcRect.y = 0;
    srcRect.w = upperLeftW;

    dstRect.x = Gamerect.x;
    dstRect.y = Gamerect.y+upperLeftH;
    dstRect.w = srcRect.w*mScale;
    dstRect.h = srcRect.h*mScale;

    mScrollSurface.blitScaledTo(blitSfc, srcRect, dstRect);
    */
}
