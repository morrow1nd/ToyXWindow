#include "ToyXWindow/BaseWindow.h"


namespace ToyXWindow
{


bool BaseWindow::ShouldClose()
{
    return m_ShouldClose;
}

void BaseWindow::SetShouldClose(bool b)
{
    m_ShouldClose = b;

    if (m_ShouldClose)
    {
        OnRecvMsg_ShouldClose();
    }
}

void BaseWindow::SetShouldCloseCallback(ShouldCloseFunc callback)
{
    m_ShouldCloseCb = callback;
}

void BaseWindow::SetWindowSizeCallback(WindowSizeFunc callback)
{
    m_WindowSizeCb = callback;
}

void BaseWindow::SetFrameBufferSizeCallback(FrameBufferSizeFunc callback)
{
    m_FrameBufferSizeCb = callback;
}

void BaseWindow::SetPosCallback(PosFunc callback)
{
    m_PosCb = callback;
}

void BaseWindow::SetIconifyCallback(IconifyFunc callback)
{
    m_IconifyCb = callback;
}

void BaseWindow::SetFocusCallback(FocusFunc callback)
{
    m_FocusCb = callback;
}

void BaseWindow::SetWindowNeedRefreshCalback(WindowNeedRefreshFunc callback)
{
    m_WindowNeedRefreshCb = callback;
}

void BaseWindow::SetKeyCallback(KeyFunc callback)
{
    m_KeyCb = callback;
}

void BaseWindow::SetCursorPosCallback(CursorPosFunc callback)
{
    m_CursorPosCb = callback;
}

void BaseWindow::SetCursorPosDeltaCallback(CursorPosDeltaFunc callback)
{
    m_CursorPosDeltaCb = callback;
}

void BaseWindow::SetCursorEnterLeaveCallback(CursorEnterLeaveFunc callback)
{
    m_CursorEnterLeaveCb = callback;
}

void BaseWindow::SetMouseButtonCallback(MouseButtonFunc callback)
{
    m_MouseButtonCb = callback;
}

void BaseWindow::SetScrollCallback(ScrollFunc callback)
{
    m_ScrollCb = callback;
}

void BaseWindow::SetDropCallback(DropFunc callback)
{
    m_DropCb = callback;
}

void BaseWindow::OnRecvMsg_ShouldClose(ShouldCloseFuncArguList)
{
    m_ShouldClose = true;

    if (m_ShouldCloseCb)
    {
        m_ShouldCloseCb(this);
    }
}

void BaseWindow::OnRecvMsg_WindowSize(WindowSizeFuncArguList)
{
    if (m_WindowSizeCb)
    {
        m_WindowSizeCb(this, WindowSizeFuncParaList);
    }
}

void BaseWindow::OnRecvMsg_FrameBufferSize(FrameBufferSizeFuncArguList)
{
    if (m_FrameBufferSizeCb)
    {
        m_FrameBufferSizeCb(this, FrameBufferSizeFuncParaList);
    }
}

void BaseWindow::OnRecvMsg_Pos(PosFuncArguList)
{
    if (m_PosCb)
    {
        m_PosCb(this, PosFuncParaList);
    }
}

void BaseWindow::OnRecvMsg_Iconify(IconifyFuncArguList)
{
    if (m_IconifyCb)
    {
        m_IconifyCb(this, IconifyFuncParaList);
    }
}

void BaseWindow::OnRecvMsg_Focus(FocusFuncArguList)
{
    if (m_FocusCb)
    {
        m_FocusCb(this, FocusFuncParaList);
    }
}

void BaseWindow::OnRecvMsg_WindowNeedRefresh(WindowNeedRefreshFuncArguList)
{
    if (m_WindowNeedRefreshCb)
    {
        m_WindowNeedRefreshCb(this);
    }
}

void BaseWindow::OnRecvMsg_Key(KeyFuncArguList)
{
    if (m_KeyCb)
    {
        m_KeyCb(this, KeyFuncParaList);
    }
}

void BaseWindow::OnRecvMsg_CursorPos(CursorPosFuncArguList)
{
    if (m_CursorPosCb)
    {
        m_CursorPosCb(this, CursorPosFuncParaList);
    }
}


void BaseWindow::OnRecvMsg_CursorPosDelta(CursorPosDeltaFuncArguList)
{
    if (m_CursorPosDeltaCb)
    {
        m_CursorPosDeltaCb(this, CursorPosDeltaFuncParaList);
    }
}

void BaseWindow::OnRecvMsg_CursorEnterLeave(CursorEnterLeaveFuncArguList)
{
    if (m_CursorEnterLeaveCb)
    {
        m_CursorEnterLeaveCb(this, CursorEnterLeaveFuncParaList);
    }
}

void BaseWindow::OnRecvMsg_MouseButton(MouseButtonFuncArguList)
{
    if (m_MouseButtonCb)
    {
        m_MouseButtonCb(this, MouseButtonFuncParaList);
    }
}

void BaseWindow::OnRecvMsg_Scroll(ScrollFuncArguList)
{
    if (m_ScrollCb)
    {
        m_ScrollCb(this, ScrollFuncParaList);
    }
}

void BaseWindow::OnRecvMsg_Drop(DropFuncArguList)
{
    if (m_DropCb)
    {
        m_DropCb(this, DropFuncParaList);
    }
}


} // end of namespace ToyXWindow