#pragma once

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyXWindow/details/WinWindow.h"
#include "ToyXWindow/details/WinWGLCommonType.h"


namespace ToyXWindow
{


class WinWGLWindow : public WinWindow
{
public:
    WinWGLWindow(const WinWGLContext& context)
        :
        c_Context(context)
    {}


public:
    virtual void PresentBackBuffer(uint32 syncInterval) override;


protected:
    virtual ToyXResult InitContext() override;
    virtual void DeInitContext() override;


public:
    void _MakeCurrentContext();


private:
    int _ChoosePixelFormat(const FrameBufferConfig& desired);

    // Returns the specified attribute of the specified pixel format
    int _GetPixelFormatAttrib(int pixelFormat, int attrib);

    // TODOM: this function can be placed in CommonType.h
    const FrameBufferConfig* _ChooseFrameBufferConfig(const FrameBufferConfig* desired,
        const FrameBufferConfig* alternatives,
        unsigned int count);


private:
    const WinWGLContext&                    c_Context;

    HDC                                     m_WglDC;
    HGLRC                                   m_WglHandle;
};


} // end of namespace ToyXWindow