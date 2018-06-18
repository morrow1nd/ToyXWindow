#pragma once

#include <functional>
#include "ToyUtility/Prerequisites/PreDefine.h"


namespace ToyXWindow
{


enum class KeyType
{
    Unknown = -1,

    // Printable key
    Space                   = 32,
    Apostrophe              = 39, // '
    Comma                   = 44, // ,
    Minus                   = 45, // -
    Period                  = 46, // .
    Slash                   = 47, // /
    _0                      = 48,
    _1                      = 49,
    _2                      = 50,
    _3                      = 51,
    _4                      = 52,
    _5                      = 53,
    _6                      = 54,
    _7                      = 55,
    _8                      = 56,
    _9                      = 57,
    Semicolon               = 59, // ;
    Equal                   = 61, // =
    A                       = 65,
    B                       = 66,
    C                       = 67,
    D                       = 68,
    E                       = 69,
    F                       = 70,
    G                       = 71,
    H                       = 72,
    I                       = 73,
    J                       = 74,
    K                       = 75,
    L                       = 76,
    M                       = 77,
    N                       = 78,
    O                       = 79,
    P                       = 80,
    Q                       = 81,
    R                       = 82,
    S                       = 83,
    T                       = 84,
    U                       = 85,
    V                       = 86,
    W                       = 87,
    X                       = 88,
    Y                       = 89,
    Z                       = 90,
    LeftBracket             = 91, // [
    BackSlash               = 92, // \ 
    RightBracket            = 93, // ]
    GraveAccent             = 96, // `

    // Function keys
    Escape                  = 256,
    Enter,
    Tab,
    Backspace,
    Insert,
    Delete,
    Right,
    Left,
    Down,
    Up,
    PageUp,
    PageDown,
    Home,
    End,
    CapsLock,
    ScrollLock,
    NumberLock,
    PrintScreen,
    Pause,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    F25,
    LeftShift,
    LeftControl,
    LeftAlt,
    LeftSuper,
    RightShift,
    RightControl,
    RightAlt,
    RightSuper,
    Menu,

    // Keypad
    Keypad_0,
    Keypad_1,
    Keypad_2,
    Keypad_3,
    Keypad_4,
    Keypad_5,
    Keypad_6,
    Keypad_7,
    Keypad_8,
    Keypad_9,
    Keypad_Decimal,
    Keypad_Divide,
    Keypad_Multiply,
    Keypad_Subtact,
    Keypad_Add,
    Keypad_Enter,
    Keypad_Equal,
};

enum class ModifierKeyType
{
    Shift,
    Control,
    Alt,
    Super,
};

enum class CursorMode
{
    Normal,
    Disabled,
    Hidden,
};

enum class ButtonState
{
    Press,
    Release,
    R
};

enum class MouseButtonType
{
    Left,
    Right,
    Middle
};


} // end of namespace ToyXWindow