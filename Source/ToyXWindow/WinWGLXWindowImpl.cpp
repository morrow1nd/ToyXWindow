#include "ToyXWindow/details/WinWGLXWindowImpl.h"


namespace ToyXWindow
{


ToyXResult WinWGLXWindowImpl::_InitializeWGL()
{
    if(m_Context.Opengl32Dll)
        return ToyXResult::Success;

    m_Context.Opengl32Dll = LoadLibraryA("opengl32.dll");
    if (!m_Context.Opengl32Dll)
    {
        return ToyXResult::Error_WinWGL_LoadLibrary_opengl32_dll_Fail;
    }

    m_Context.wgl.wglCreateContext = (WGLCREATECONTEXT_T)
        GetProcAddress(m_Context.Opengl32Dll, "wglCreateContext");
    m_Context.wgl.wglDeleteContext = (WGLDELETECONTEXT_T)
        GetProcAddress(m_Context.Opengl32Dll, "wglDeleteContext");
    m_Context.wgl.wglGetProcAddress = (WGLGETPROCADDRESS_T)
        GetProcAddress(m_Context.Opengl32Dll, "wglGetProcAddress");
    m_Context.wgl.wglGetCurrentDC = (WGLGETCURRENTDC_T)
        GetProcAddress(m_Context.Opengl32Dll, "wglGetCurrentDC");
    m_Context.wgl.wglMakeCurrent = (WGLMAKECURRENT_T)
        GetProcAddress(m_Context.Opengl32Dll, "wglMakeCurrent");
    m_Context.wgl.wglShareLists = (WGLSHARELISTS_T)
        GetProcAddress(m_Context.Opengl32Dll, "wglShareLists");

    // TODOM: check these pointers

    // Initialize WGL-specific extensions
    PIXELFORMATDESCRIPTOR pfd;
    HGLRC rc;
    HDC dc = GetDC(GetHelperWindowHandle());;

    m_Context.WGLExtensionsLoaded = true;

    // NOTE: A dummy context has to be created for opengl32.dll to load the
    //       OpenGL ICD, from which we can then query WGL extensions
    // NOTE: This code will accept the Microsoft GDI ICD; accelerated context
    //       creation failure occurs during manual pixel format enumeration

    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;

    if (!SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd))
    {
        return ToyXResult::Error_WinWGL_SetPixelFormatForDummyContext_Fail;
    }

    rc = m_Context.wgl.wglCreateContext(dc);
    if (!rc)
    {
        return ToyXResult::Error_WinWGL_CreateDummyContext_Fail;
    }

    if (!m_Context.wgl.wglMakeCurrent(dc, rc))
    {
        m_Context.wgl.wglDeleteContext(rc);

        return ToyXResult::Error_WinWGL_MakeDummyContextCurrent_Fail;
    }

    // NOTE: Functions must be loaded first as they're needed to retrieve the
    //       extension string that tells us whether the functions are supported
    m_Context.wglEXT.GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)
        m_Context.wgl.wglGetProcAddress("wglGetExtensionsStringEXT");
    m_Context.wglEXT.GetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)
        m_Context.wgl.wglGetProcAddress("wglGetExtensionsStringARB");
    m_Context.wglEXT.CreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)
        m_Context.wgl.wglGetProcAddress("wglCreateContextAttribsARB");
    m_Context.wglEXT.SwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)
        m_Context.wgl.wglGetProcAddress("wglSwapIntervalEXT");
    m_Context.wglEXT.GetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)
        m_Context.wgl.wglGetProcAddress("wglGetPixelFormatAttribivARB");

    // NOTE: WGL_ARB_extensions_string and WGL_EXT_extensions_string are not
    //       checked below as we are already using them
    m_Context.wglEXT.ARB_multisample =
        _WGLExtensionSupported("WGL_ARB_multisample");
    m_Context.wglEXT.ARB_framebuffer_sRGB =
        _WGLExtensionSupported("WGL_ARB_framebuffer_sRGB");
    m_Context.wglEXT.EXT_framebuffer_sRGB =
        _WGLExtensionSupported("WGL_EXT_framebuffer_sRGB");
    m_Context.wglEXT.ARB_create_context =
        _WGLExtensionSupported("WGL_ARB_create_context");
    m_Context.wglEXT.ARB_create_context_profile =
        _WGLExtensionSupported("WGL_ARB_create_context_profile");
    m_Context.wglEXT.EXT_create_context_es2_profile =
        _WGLExtensionSupported("WGL_EXT_create_context_es2_profile");
    m_Context.wglEXT.ARB_create_context_robustness =
        _WGLExtensionSupported("WGL_ARB_create_context_robustness");
    m_Context.wglEXT.EXT_swap_control =
        _WGLExtensionSupported("WGL_EXT_swap_control");
    m_Context.wglEXT.ARB_pixel_format =
        _WGLExtensionSupported("WGL_ARB_pixel_format");
    m_Context.wglEXT.ARB_context_flush_control =
        _WGLExtensionSupported("WGL_ARB_context_flush_control");

    m_Context.wgl.wglMakeCurrent(dc, NULL);
    m_Context.wgl.wglDeleteContext(rc);

    return ToyXResult::Success;
}

void WinWGLXWindowImpl::_TerminateWGL()
{
    if (m_Context.Opengl32Dll)
    {
        FreeLibrary(m_Context.Opengl32Dll);
        m_Context.Opengl32Dll = nullptr;
    }
}

bool WinWGLXWindowImpl::_WGLExtensionSupported(const char * extension)
{
    const char* extensions;

    if (m_Context.wglEXT.GetExtensionsStringEXT)
    {
        extensions = m_Context.wglEXT.GetExtensionsStringEXT();
        if (extensions)
        {
            if (_StringInExtensionString(extension, extensions))
                return true;
        }
    }

    if (m_Context.wglEXT.GetExtensionsStringARB)
    {
        extensions = m_Context.wglEXT.GetExtensionsStringARB(m_Context.wgl.wglGetCurrentDC());
        if (extensions)
        {
            if (_StringInExtensionString(extension, extensions))
                return true;
        }
    }

    return false;
}

bool WinWGLXWindowImpl::_StringInExtensionString(const char * string, const char * extensions)
{
    const char* start = extensions;

    for (;;)
    {
        const char* where;
        const char* terminator;

        where = strstr(start, string);
        if (!where)
            return false;

        terminator = where + strlen(string);
        if (where == start || *(where - 1) == ' ')
        {
            if (*terminator == ' ' || *terminator == '\0')
                break;
        }

        start = terminator;
    }

    return true;
}

ToyXResult WinWGLXWindowImpl::StartUp(const XWINDOW_API_STARTUP_DESC & desc)
{
    TOYXRESULT_RETURN_IF_FAIL(WinBaseXWindowImpl::StartUp(desc));

    TOYXRESULT_RETURN_IF_FAIL(_InitializeWGL());
    return ToyXResult::Success;
}

void WinWGLXWindowImpl::ShutDown()
{
    _TerminateWGL();

    WinBaseXWindowImpl::ShutDown();
}


WGLGETPROCADDRESS_T g_WinWGL__wglGetProcAddress = 0;
HINSTANCE g_WinWGL__wglInstance = 0;

static void* WinWGL_GetProcAddress(const char* procname)
{
    if (g_WinWGL__wglGetProcAddress)
    {
        auto proc = g_WinWGL__wglGetProcAddress(procname);
        if(proc)
            return proc;
    }

    if (g_WinWGL__wglInstance)
    {
        return GetProcAddress(g_WinWGL__wglInstance, procname);
    }

    return 0;
}

void WinWGLXWindowImpl::GetPlatformDependentData(PlatformDependentData & data)
{
    WinBaseXWindowImpl::GetPlatformDependentData(data);

    g_WinWGL__wglInstance = m_Context.Opengl32Dll;
    g_WinWGL__wglGetProcAddress = m_Context.wgl.wglGetProcAddress;
    data._.Windows._.WinWGL.GetProcAddressFuncPtr = WinWGL_GetProcAddress;
}

ToyUtility::SPtr<IAdapter> WinWGLXWindowImpl::GetDefaultAdapter()
{
    return ToyUtility::SPtr<IAdapter>();
}

const ToyUtility::List<ToyUtility::SPtr<IAdapter>>& WinWGLXWindowImpl::GetAdapters() const
{
    // TODOM
    throw "unachieved";
}

ToyUtility::SPtr<IWindow> WinWGLXWindowImpl::CreateWindow(const WINDOW_DESC & desc)
{
    ToyUtility::SPtr<WinWGLWindow> window = std::make_shared<WinWGLWindow>(m_Context);

    if (window->Create(desc) == ToyXResult::Success)
    {
        if (m_MainWindow == nullptr)
            SetMainWindow(window);

        return window;
    }

    // TODOH: how about error code?

    return ToyUtility::SPtr<IWindow>();
}

void WinWGLXWindowImpl::SetMainWindow(ToyUtility::SPtr<IWindow> window)
{
    WinBaseXWindowImpl::SetMainWindow(window);

    m_MainWindow = std::dynamic_pointer_cast<WinWGLWindow>(window);

    m_MainWindow->_MakeCurrentContext();
}


} // end of namespace ToyXWindow