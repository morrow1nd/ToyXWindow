#include "ToyXWindow/details/WinWGLWindow.h"


namespace ToyXWindow
{


void WinWGLWindow::PresentBackBuffer(uint32 syncInterval)
{
    // TODOM: handle destop composition(https://msdn.microsoft.com/en-us/library/windows/desktop/aa969540(v=vs.85).aspx)
    // see glfw:wgl_context.c:L260

    if (syncInterval == 0)
    {
        SwapBuffers(m_WglDC);
    }
    else
    {
        if (c_Context.wglEXT.EXT_swap_control)
            c_Context.wglEXT.SwapIntervalEXT(syncInterval);
    }
}

#define setWGLattrib(attribName, attribValue) \
{ \
    attribs[index++] = attribName; \
    attribs[index++] = attribValue; \
    assert((size_t) index < sizeof(attribs) / sizeof(attribs[0])); \
}

ToyXResult WinWGLWindow::InitContext()
{
    int attribs[40];
    int pixelFormat;
    PIXELFORMATDESCRIPTOR pfd;
    HGLRC share = NULL;

    // TODOM: sharing with specified OpenGL context
    //if (ctxconfig.share)
    //    share = ctxconfig.share->context.wgl.handle;

    m_WglDC = GetDC(GetHWND());
    if (!m_WglDC)
    {
        // WGL: Failed to retrieve DC for window
        return ToyXResult::Error_WinWGL_GetDC_Fail;
    }

    FrameBufferConfig fbConfig = FrameBufferConfig::Default; // TODOH
    pixelFormat = _ChoosePixelFormat(fbConfig);
    if (!pixelFormat)
        return ToyXResult::Error_WinWGL_ChoosePixelFormat_Fail;

    if (!DescribePixelFormat(m_WglDC, pixelFormat, sizeof(pfd), &pfd))
    {
        // WGL: Failed to retrieve PFD for selected pixel format
        return ToyXResult::Error_WinWGL_DescribePixelFormat_Fail;
    }

    if (!SetPixelFormat(m_WglDC, pixelFormat, &pfd))
        return ToyXResult::Error_WinWGL_SetPixelFormat_WhileInitContext_Fail;

    ContextConfig ctxconfig = ContextConfig::WinWGLDefault;

    //if (ctxconfig.client == GLFW_OPENGL_API)
    {
        if (ctxconfig.forward)
        {
            if (!c_Context.wglEXT.ARB_create_context)
            {
                // WGL: A forward compatible OpenGL context requested but WGL_ARB_create_context is unavailable
                return ToyXResult::Error_Others;
            }
        }

        if (ctxconfig.profile != ProfileType::Any)
        {
            if (!c_Context.wglEXT.ARB_create_context_profile)
            {
                // WGL: OpenGL profile requested but WGL_ARB_create_context_profile is unavailable
                return ToyXResult::Error_Others;
            }
        }
    }
    //else // OpenGL ES
    //{
    //    if (!c_Context.wglEXT.ARB_create_context ||
    //        !c_Context.wglEXT.ARB_create_context_profile ||
    //        !c_Context.wglEXT.EXT_create_context_es2_profile)
    //    {
    //        _glfwInputError(GLFW_API_UNAVAILABLE,
    //            "WGL: OpenGL ES requested but WGL_ARB_create_context_es2_profile is unavailable");
    //        return GLFW_FALSE;
    //    }
    //}

    if (c_Context.wglEXT.ARB_create_context)
    {
        int index = 0, mask = 0, flags = 0;

        //if (ctxconfig.client == GLFW_OPENGL_API)
        {
            if (ctxconfig.forward)
                flags |= WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;

            if (ctxconfig.profile == ProfileType::OpenGL_Core)
                mask |= WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
            else if (ctxconfig.profile == ProfileType::OpenGL_Compat)
                mask |= WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
        }
        //else
        //    mask |= WGL_CONTEXT_ES2_PROFILE_BIT_EXT;

        if (ctxconfig.debug)
            flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
        if (ctxconfig.noerror)
            flags |= GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR;

        if (ctxconfig.robustness != RobustnessType::None)
        {
            if (c_Context.wglEXT.ARB_create_context_robustness)
            {
                if (ctxconfig.robustness == RobustnessType::OpenGL_NO_RESET_NOTIFICATION)
                {
                    setWGLattrib(WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB,
                        WGL_NO_RESET_NOTIFICATION_ARB);
                }
                else if (ctxconfig.robustness == RobustnessType::OpenGL_LOSE_CONTEXT_ON_RESET)
                {
                    setWGLattrib(WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB,
                        WGL_LOSE_CONTEXT_ON_RESET_ARB);
                }

                flags |= WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB;
            }
        }

        if (ctxconfig.release != ReleaseType::Any)
        {
            if (c_Context.wglEXT.ARB_context_flush_control)
            {
                if (ctxconfig.release == ReleaseType::OpenGL_None)
                {
                    setWGLattrib(WGL_CONTEXT_RELEASE_BEHAVIOR_ARB,
                        WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB);
                }
                else if (ctxconfig.release == ReleaseType::OpenGL_Flush)
                {
                    setWGLattrib(WGL_CONTEXT_RELEASE_BEHAVIOR_ARB,
                        WGL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB);
                }
            }
        }

        // NOTE: Only request an explicitly versioned context when necessary, as
        //       explicitly requesting version 1.0 does not always return the
        //       highest version supported by the driver
        if (ctxconfig.major != 1 || ctxconfig.minor != 0)
        {
            setWGLattrib(WGL_CONTEXT_MAJOR_VERSION_ARB, ctxconfig.major);
            setWGLattrib(WGL_CONTEXT_MINOR_VERSION_ARB, ctxconfig.minor);
        }

        if (flags)
            setWGLattrib(WGL_CONTEXT_FLAGS_ARB, flags);

        if (mask)
            setWGLattrib(WGL_CONTEXT_PROFILE_MASK_ARB, mask);

        setWGLattrib(0, 0);

        m_WglHandle = c_Context.wglEXT.CreateContextAttribsARB(m_WglDC,
                share, attribs);
        if (!m_WglHandle)
        {
            // TODOH: error handle
            //const DWORD error = GetLastError();

            //if (error == (0xc0070000 | ERROR_INVALID_VERSION_ARB))
            //{
            //    if (ctxconfig.client == GLFW_OPENGL_API)
            //    {
            //        _glfwInputError(GLFW_VERSION_UNAVAILABLE,
            //            "WGL: Driver does not support OpenGL version %i.%i",
            //            ctxconfig.major,
            //            ctxconfig.minor);
            //    }
            //    else
            //    {
            //        _glfwInputError(GLFW_VERSION_UNAVAILABLE,
            //            "WGL: Driver does not support OpenGL ES version %i.%i",
            //            ctxconfig.major,
            //            ctxconfig.minor);
            //    }
            //}
            //else if (error == (0xc0070000 | ERROR_INVALID_PROFILE_ARB))
            //{
            //    _glfwInputError(GLFW_VERSION_UNAVAILABLE,
            //        "WGL: Driver does not support the requested OpenGL profile");
            //}
            //else
            //{
            //    if (ctxconfig.client == GLFW_OPENGL_API)
            //    {
            //        _glfwInputError(GLFW_VERSION_UNAVAILABLE,
            //            "WGL: Failed to create OpenGL context");
            //    }
            //    else
            //    {
            //        _glfwInputError(GLFW_VERSION_UNAVAILABLE,
            //            "WGL: Failed to create OpenGL ES context");
            //    }
            //}

            //return GLFW_FALSE;
            return ToyXResult::Error_Others;
        }
    }
    else
    {
        m_WglHandle = c_Context.wgl.wglCreateContext(m_WglDC);
        if (!m_WglHandle)
        {
              //  "WGL: Failed to create OpenGL context"); // TODOH
            return ToyXResult::Error_Others;
        }

        // TODOM: share context
        //if (share)
        //{
        //    if (!wglShareLists(share, window->context.wgl.handle))
        //    {
        //        _glfwInputError(GLFW_PLATFORM_ERROR,
        //            "WGL: Failed to enable sharing with specified OpenGL context");
        //        return GLFW_FALSE;
        //    }
        //}
    }

    return ToyXResult::Success;
}

void WinWGLWindow::DeInitContext()
{
    if (m_WglHandle)
    {
        c_Context.wgl.wglDeleteContext(m_WglHandle);
        m_WglHandle = NULL;
    }
}

void WinWGLWindow::_MakeCurrentContext()
{
    if (c_Context.wgl.wglMakeCurrent(m_WglDC, m_WglHandle))
    {
        // Nothing
    }
    else
    {
        // TODOH: error handle
        // WGL: Failed to make context current
    }
}

int WinWGLWindow::_ChoosePixelFormat(const FrameBufferConfig & desired)
{
    FrameBufferConfig* usableConfigs;
    const FrameBufferConfig* closest;
    int i, pixelFormat, nativeCount, usableCount;

    if (c_Context.wglEXT.ARB_pixel_format)
    {
        nativeCount = _GetPixelFormatAttrib(1,
            WGL_NUMBER_PIXEL_FORMATS_ARB);
    }
    else
    {
        nativeCount = DescribePixelFormat(m_WglDC,
            1,
            sizeof(PIXELFORMATDESCRIPTOR),
            NULL);
    }

    usableConfigs = (FrameBufferConfig*)calloc(nativeCount, sizeof(FrameBufferConfig));
    usableCount = 0;

    for (i = 0; i < nativeCount; i++)
    {
        const int n = i + 1;
        FrameBufferConfig* u = usableConfigs + usableCount;

        if (c_Context.wglEXT.ARB_pixel_format)
        {
            // Get pixel format attributes through "modern" extension

            if (!_GetPixelFormatAttrib(n, WGL_SUPPORT_OPENGL_ARB) ||
                !_GetPixelFormatAttrib(n, WGL_DRAW_TO_WINDOW_ARB))
            {
                continue;
            }

            if (_GetPixelFormatAttrib(n, WGL_PIXEL_TYPE_ARB) !=
                WGL_TYPE_RGBA_ARB)
            {
                continue;
            }

            if (_GetPixelFormatAttrib(n, WGL_ACCELERATION_ARB) ==
                WGL_NO_ACCELERATION_ARB)
            {
                continue;
            }

            u->RedBits = _GetPixelFormatAttrib(n, WGL_RED_BITS_ARB);
            u->GreenBits = _GetPixelFormatAttrib(n, WGL_GREEN_BITS_ARB);
            u->BlueBits = _GetPixelFormatAttrib(n, WGL_BLUE_BITS_ARB);
            u->AlphaBits = _GetPixelFormatAttrib(n, WGL_ALPHA_BITS_ARB);

            u->DepthBits = _GetPixelFormatAttrib(n, WGL_DEPTH_BITS_ARB);
            u->StencilBits = _GetPixelFormatAttrib(n, WGL_STENCIL_BITS_ARB);

            u->AccumRedBits = _GetPixelFormatAttrib(n, WGL_ACCUM_RED_BITS_ARB);
            u->AccumGreenBits = _GetPixelFormatAttrib(n, WGL_ACCUM_GREEN_BITS_ARB);
            u->AccumBlueBits = _GetPixelFormatAttrib(n, WGL_ACCUM_BLUE_BITS_ARB);
            u->AccumAlphaBits = _GetPixelFormatAttrib(n, WGL_ACCUM_ALPHA_BITS_ARB);

            u->AuxBuffers = _GetPixelFormatAttrib(n, WGL_AUX_BUFFERS_ARB);

            if (_GetPixelFormatAttrib(n, WGL_STEREO_ARB))
                u->Stereo = true;
            if (_GetPixelFormatAttrib(n, WGL_DOUBLE_BUFFER_ARB))
                u->Doublebuffer = true;

            if (c_Context.wglEXT.ARB_multisample)
                u->Samples = _GetPixelFormatAttrib(n, WGL_SAMPLES_ARB);

            if (c_Context.wglEXT.ARB_framebuffer_sRGB ||
                c_Context.wglEXT.EXT_framebuffer_sRGB)
            {
                if (_GetPixelFormatAttrib(n, WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB))
                    u->sRGB = true;
            }
        }
        else
        {
            PIXELFORMATDESCRIPTOR pfd;

            // Get pixel format attributes through legacy PFDs

            if (!DescribePixelFormat(m_WglDC,
                n,
                sizeof(PIXELFORMATDESCRIPTOR),
                &pfd))
            {
                continue;
            }

            if (!(pfd.dwFlags & PFD_DRAW_TO_WINDOW) ||
                !(pfd.dwFlags & PFD_SUPPORT_OPENGL))
            {
                continue;
            }

            if (!(pfd.dwFlags & PFD_GENERIC_ACCELERATED) &&
                (pfd.dwFlags & PFD_GENERIC_FORMAT))
            {
                continue;
            }

            if (pfd.iPixelType != PFD_TYPE_RGBA)
                continue;

            u->RedBits = pfd.cRedBits;
            u->GreenBits = pfd.cGreenBits;
            u->BlueBits = pfd.cBlueBits;
            u->AlphaBits = pfd.cAlphaBits;

            u->DepthBits = pfd.cDepthBits;
            u->StencilBits = pfd.cStencilBits;

            u->AccumRedBits = pfd.cAccumRedBits;
            u->AccumGreenBits = pfd.cAccumGreenBits;
            u->AccumBlueBits = pfd.cAccumBlueBits;
            u->AccumAlphaBits = pfd.cAccumAlphaBits;

            u->AuxBuffers = pfd.cAuxBuffers;

            if (pfd.dwFlags & PFD_STEREO)
                u->Stereo = true;
            if (pfd.dwFlags & PFD_DOUBLEBUFFER)
                u->Doublebuffer = true;
        }

        u->Handle = n;
        usableCount++;
    }

    if (!usableCount)
    {
        // TODOH: error handle
        //_glfwInputError(GLFW_API_UNAVAILABLE,
        //    "WGL: The driver does not appear to support OpenGL");

        free(usableConfigs);
        return 0;
    }

    closest = _ChooseFrameBufferConfig(&desired, usableConfigs, usableCount);
    if (!closest)
    {
        // TODOH: error handle
        //_glfwInputError(GLFW_FORMAT_UNAVAILABLE,
        //    "WGL: Failed to find a suitable pixel format");

        free(usableConfigs);
        return 0;
    }

    pixelFormat = (int)closest->Handle;
    free(usableConfigs);

    return pixelFormat;
}

int WinWGLWindow::_GetPixelFormatAttrib(int pixelFormat, int attrib)
{
    int value = 0;

    assert(c_Context.wglEXT.ARB_pixel_format);

    if (!c_Context.wglEXT.GetPixelFormatAttribivARB(m_WglDC,
        pixelFormat,
        0, 1, &attrib, &value))
    {
        // TODOH: error handle
        //_glfwInputError(GLFW_PLATFORM_ERROR,
        //    "WGL: Failed to retrieve pixel format attribute %i",
        //    attrib);
        return 0;
    }

    return value;
}

const FrameBufferConfig * WinWGLWindow::_ChooseFrameBufferConfig(const FrameBufferConfig * desired, const FrameBufferConfig * alternatives, unsigned int count)
{

    unsigned int i;
    unsigned int missing, leastMissing = UINT_MAX;
    unsigned int colorDiff, leastColorDiff = UINT_MAX;
    unsigned int extraDiff, leastExtraDiff = UINT_MAX;
    const FrameBufferConfig* current;
    const FrameBufferConfig* closest = NULL;

    for (i = 0; i < count; i++)
    {
        current = alternatives + i;

        if (desired->Stereo && current->Stereo == false)
        {
            // Stereo is a hard constraint
            continue;
        }

        if (desired->Doublebuffer != current->Doublebuffer)
        {
            // Double buffering is a hard constraint
            continue;
        }

        // Count number of missing buffers
        {
            missing = 0;

            if (desired->AlphaBits > 0 && current->AlphaBits == 0)
                missing++;

            if (desired->DepthBits > 0 && current->DepthBits == 0)
                missing++;

            if (desired->StencilBits > 0 && current->StencilBits == 0)
                missing++;

            if (desired->AuxBuffers > 0 &&
                current->AuxBuffers < desired->AuxBuffers)
            {
                missing += desired->AuxBuffers - current->AuxBuffers;
            }

            if (desired->Samples > 0 && current->Samples == 0)
            {
                // Technically, several multisampling buffers could be
                // involved, but that's a lower level implementation detail and
                // not important to us here, so we count them as one
                missing++;
            }
        }

        // These polynomials make many small channel size differences matter
        // less than one large channel size difference

        // Calculate color channel size difference value
        {
            colorDiff = 0;

            if (desired->RedBits != XWINDOW_DONT_CARE)
            {
                colorDiff += (desired->RedBits - current->RedBits) *
                    (desired->RedBits - current->RedBits);
            }

            if (desired->GreenBits != XWINDOW_DONT_CARE)
            {
                colorDiff += (desired->GreenBits - current->GreenBits) *
                    (desired->GreenBits - current->GreenBits);
            }

            if (desired->BlueBits != XWINDOW_DONT_CARE)
            {
                colorDiff += (desired->BlueBits - current->BlueBits) *
                    (desired->BlueBits - current->BlueBits);
            }
        }

        // Calculate non-color channel size difference value
        {
            extraDiff = 0;

            if (desired->AlphaBits != XWINDOW_DONT_CARE)
            {
                extraDiff += (desired->AlphaBits - current->AlphaBits) *
                    (desired->AlphaBits - current->AlphaBits);
            }

            if (desired->DepthBits != XWINDOW_DONT_CARE)
            {
                extraDiff += (desired->DepthBits - current->DepthBits) *
                    (desired->DepthBits - current->DepthBits);
            }

            if (desired->StencilBits != XWINDOW_DONT_CARE)
            {
                extraDiff += (desired->StencilBits - current->StencilBits) *
                    (desired->StencilBits - current->StencilBits);
            }

            if (desired->AccumRedBits != XWINDOW_DONT_CARE)
            {
                extraDiff += (desired->AccumRedBits - current->AccumRedBits) *
                    (desired->AccumRedBits - current->AccumRedBits);
            }

            if (desired->AccumGreenBits != XWINDOW_DONT_CARE)
            {
                extraDiff += (desired->AccumGreenBits - current->AccumGreenBits) *
                    (desired->AccumGreenBits - current->AccumGreenBits);
            }

            if (desired->AccumBlueBits != XWINDOW_DONT_CARE)
            {
                extraDiff += (desired->AccumBlueBits - current->AccumBlueBits) *
                    (desired->AccumBlueBits - current->AccumBlueBits);
            }

            if (desired->AccumAlphaBits != XWINDOW_DONT_CARE)
            {
                extraDiff += (desired->AccumAlphaBits - current->AccumAlphaBits) *
                    (desired->AccumAlphaBits - current->AccumAlphaBits);
            }

            if (desired->Samples != XWINDOW_DONT_CARE)
            {
                extraDiff += (desired->Samples - current->Samples) *
                    (desired->Samples - current->Samples);
            }

            if (desired->sRGB && !current->sRGB)
                extraDiff++;
        }

        // Figure out if the current one is better than the best one found so far
        // Least number of missing buffers is the most important heuristic,
        // then color buffer size match and lastly size match for other buffers

        if (missing < leastMissing)
            closest = current;
        else if (missing == leastMissing)
        {
            if ((colorDiff < leastColorDiff) ||
                (colorDiff == leastColorDiff && extraDiff < leastExtraDiff))
            {
                closest = current;
            }
        }

        if (current == closest)
        {
            leastMissing = missing;
            leastColorDiff = colorDiff;
            leastExtraDiff = extraDiff;
        }
    }

    return closest;
}


} // end of namespace ToyXWindow