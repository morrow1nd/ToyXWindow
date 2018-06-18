#pragma once

#include <Windows.h>

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyXWindow/BaseWindow.h"


namespace ToyXWindow
{


class WinWindow : public BaseWindow
{
public:
    WinWindow()
        :
        m_HWND(0)
    {}


private:
    HWND                            m_HWND;
    ToyUtility::String              m_ClassName;
    WINDOW_DESC                     m_Desc;


public:
    void SetClassName(const ToyUtility::String& className);


public:
    virtual bool Create(const WINDOW_DESC& desc) override;
    virtual void Destory() override;
    virtual void GetWindowSize(uint32 * width, uint32 * height) override;
    virtual void SetWindowSize(uint32 width, uint32 height) override;
    virtual void GetWindowFrameSize(uint32 * left, uint32 * top, uint32 * right, uint32 * bottom) override;
    virtual void GetFrameBufferSize(uint32 * width, uint32 * height) override;
    virtual void SetWindowSizeLimits(uint32 minWidth, uint32 minHeight, uint32 maxWidth, uint32 maxHeight) override;
    virtual void SetAspectRatio(uint32 width, uint32 height) override;
    virtual void SetPos(int32 xpos, int32 ypos) override;
    virtual void GetPos(int32 * xpos, int32 * pos) override;
    virtual const ToyUtility::String & GetTitle() const override;
    virtual void SetTitle(const ToyUtility::String & title) override;
    virtual void Iconify() override;
    virtual void Restore() override;
    virtual bool IsIconified() override;
    virtual void Hide() override;
    virtual void Show() override;
    virtual bool IsVisible() override;
    virtual void Focus() override;
    virtual bool IsFocused() override;
    virtual void PresentBackBuffer(uint32 syncInterval) override;
    virtual ButtonState GetKey(KeyType key) override;
    virtual const ToyUtility::String & GetKeyName(KeyType key) const override;
    virtual void GetCursorPos(float * xpos, float * ypos) override;
    virtual void GetCursorPosDelta(float * xdelta, float * ydelta) override;
    virtual void SetCursorMode(CursorMode mode) override;
    virtual CursorMode GetCursorMode() override;
    virtual ButtonState GetMouseButton(MouseButtonType type) override;
    virtual const ToyUtility::String & GetClipboardString() const override;
    virtual void SetClipboardString(const ToyUtility::String & text) override;
};


} // end of namespace ToyXWindow