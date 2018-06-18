#pragma once

#include "ToyUtility/Prerequisites/PreDefine.h"
#include "ToyXWindow/IWindow.h"


namespace ToyXWindow
{


class BaseWindow : public IWindow
{
public:
    BaseWindow()
        :
        m_ShouldClose(false),
        m_Windowed(true)
    {}


private:
    bool                            m_Windowed; // windowed-mode window
    bool                            m_ShouldClose;

    ToyUtility::String              m_Title;


protected:
    // Callbacks
    ShouldCloseFunc                 m_ShouldCloseCb;
    WindowSizeFunc                  m_WindowSizeCb;
    FrameBufferSizeFunc             m_FrameBufferSizeCb;
    PosFunc                         m_PosCb;
    IconifyFunc                     m_IconifyCb;
    FocusFunc                       m_FocusCb;
    WindowNeedRefreshFunc           m_WindowNeedRefreshCb;
    KeyFunc                         m_KeyCb;
    CursorPosFunc                   m_CursorPosCb;
    CursorPosDeltaFunc              m_CursorPosDeltaCb;
    CursorEnterLeaveFunc            m_CursorEnterLeaveCb;
    MouseButtonFunc                 m_MouseButtonCb;
    ScrollFunc                      m_ScrollCb;
    DropFunc                        m_DropCb;


public:
    virtual bool Create(const WINDOW_DESC& desc) override;
    virtual bool IsWindowed() const override;
    virtual bool ShouldClose() override;
    virtual void SetShouldClose(bool b) override;
    virtual const ToyUtility::String & GetTitle() const override;
    virtual void SetTitle(const ToyUtility::String & title) override;

    // Set Callback function impl
    virtual void SetShouldCloseCallback(ShouldCloseFunc callback) override;
    virtual void SetWindowSizeCallback(WindowSizeFunc callback) override;
    virtual void SetFrameBufferSizeCallback(FrameBufferSizeFunc callback) override;
    virtual void SetPosCallback(PosFunc callback) override;
    virtual void SetIconifyCallback(IconifyFunc callback) override;
    virtual void SetFocusCallback(FocusFunc callback) override;
    virtual void SetWindowNeedRefreshCalback(WindowNeedRefreshFunc callback) override;
    virtual void SetKeyCallback(KeyFunc callback) override;
    virtual void SetCursorPosCallback(CursorPosFunc callback) override;
    virtual void SetCursorPosDeltaCallback(CursorPosDeltaFunc callback) override;
    virtual void SetCursorEnterLeaveCallback(CursorEnterLeaveFunc callback) override;
    virtual void SetMouseButtonCallback(MouseButtonFunc callback) override;
    virtual void SetScrollCallback(ScrollFunc callback) override;
    virtual void SetDropCallback(DropFunc callback) override;


    ////////////////////////////////////////////////////////////////////////////
    // Handle messages (OnRecvMsg)
protected:
    virtual void OnRecvMsg_ShouldClose(ShouldCloseFuncArguList);
    virtual void OnRecvMsg_WindowSize(WindowSizeFuncArguList);
    virtual void OnRecvMsg_FrameBufferSize(FrameBufferSizeFuncArguList);
    virtual void OnRecvMsg_Pos(PosFuncArguList);
    virtual void OnRecvMsg_Iconify(IconifyFuncArguList);
    virtual void OnRecvMsg_Focus(FocusFuncArguList);
    virtual void OnRecvMsg_WindowNeedRefresh(WindowNeedRefreshFuncArguList);
    virtual void OnRecvMsg_Key(KeyFuncArguList);
    virtual void OnRecvMsg_CursorPos(CursorPosFuncArguList);
    virtual void OnRecvMsg_CursorPosDelta(CursorPosDeltaFuncArguList);
    virtual void OnRecvMsg_CursorEnterLeave(CursorEnterLeaveFuncArguList);
    virtual void OnRecvMsg_MouseButton(MouseButtonFuncArguList);
    virtual void OnRecvMsg_Scroll(ScrollFuncArguList);
    virtual void OnRecvMsg_Drop(DropFuncArguList);
};


} // end of namespace ToyXWindow