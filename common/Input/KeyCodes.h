#pragma once

//
//  list of key codes
//  should match DirectInput codes
//

#define KEY_CODE_ESCAPE          0x01
#define KEY_CODE_1               0x02
#define KEY_CODE_2               0x03
#define KEY_CODE_3               0x04
#define KEY_CODE_4               0x05
#define KEY_CODE_5               0x06
#define KEY_CODE_6               0x07
#define KEY_CODE_7               0x08
#define KEY_CODE_8               0x09
#define KEY_CODE_9               0x0A
#define KEY_CODE_0               0x0B
#define KEY_CODE_MINUS           0x0C    /* - on main keyboard */
#define KEY_CODE_EQUALS          0x0D
#define KEY_CODE_BACK            0x0E    /* backspace */
#define KEY_CODE_TAB             0x0F
#define KEY_CODE_Q               0x10
#define KEY_CODE_W               0x11
#define KEY_CODE_E               0x12
#define KEY_CODE_R               0x13
#define KEY_CODE_T               0x14
#define KEY_CODE_Y               0x15
#define KEY_CODE_U               0x16
#define KEY_CODE_I               0x17
#define KEY_CODE_O               0x18
#define KEY_CODE_P               0x19
#define KEY_CODE_LBRACKET        0x1A
#define KEY_CODE_RBRACKET        0x1B
#define KEY_CODE_RETURN          0x1C    /* Enter on main keyboard */
#define KEY_CODE_LCONTROL        0x1D
#define KEY_CODE_A               0x1E
#define KEY_CODE_S               0x1F
#define KEY_CODE_D               0x20
#define KEY_CODE_F               0x21
#define KEY_CODE_G               0x22
#define KEY_CODE_H               0x23
#define KEY_CODE_J               0x24
#define KEY_CODE_K               0x25
#define KEY_CODE_L               0x26
#define KEY_CODE_SEMICOLON       0x27
#define KEY_CODE_APOSTROPHE      0x28
#define KEY_CODE_GRAVE           0x29    /* accent grave */
#define KEY_CODE_LSHIFT          0x2A
#define KEY_CODE_BACKSLASH       0x2B
#define KEY_CODE_Z               0x2C
#define KEY_CODE_X               0x2D
#define KEY_CODE_C               0x2E
#define KEY_CODE_V               0x2F
#define KEY_CODE_B               0x30
#define KEY_CODE_N               0x31
#define KEY_CODE_M               0x32
#define KEY_CODE_COMMA           0x33
#define KEY_CODE_PERIOD          0x34    /* . on main keyboard */
#define KEY_CODE_SLASH           0x35    /* / on main keyboard */
#define KEY_CODE_RSHIFT          0x36
#define KEY_CODE_MULTIPLY        0x37    /* * on numeric keypad */
#define KEY_CODE_LMENU           0x38    /* left Alt */
#define KEY_CODE_SPACE           0x39
#define KEY_CODE_CAPITAL         0x3A
#define KEY_CODE_F1              0x3B
#define KEY_CODE_F2              0x3C
#define KEY_CODE_F3              0x3D
#define KEY_CODE_F4              0x3E
#define KEY_CODE_F5              0x3F
#define KEY_CODE_F6              0x40
#define KEY_CODE_F7              0x41
#define KEY_CODE_F8              0x42
#define KEY_CODE_F9              0x43
#define KEY_CODE_F10             0x44
#define KEY_CODE_NUMLOCK         0x45
#define KEY_CODE_SCROLL          0x46    /* Scroll Lock */
#define KEY_CODE_NUMPAD7         0x47
#define KEY_CODE_NUMPAD8         0x48
#define KEY_CODE_NUMPAD9         0x49
#define KEY_CODE_SUBTRACT        0x4A    /* - on numeric keypad */
#define KEY_CODE_NUMPAD4         0x4B
#define KEY_CODE_NUMPAD5         0x4C
#define KEY_CODE_NUMPAD6         0x4D
#define KEY_CODE_ADD             0x4E    /* + on numeric keypad */
#define KEY_CODE_NUMPAD1         0x4F
#define KEY_CODE_NUMPAD2         0x50
#define KEY_CODE_NUMPAD3         0x51
#define KEY_CODE_NUMPAD0         0x52
#define KEY_CODE_DECIMAL         0x53    /* . on numeric keypad */
#define KEY_CODE_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define KEY_CODE_F11             0x57
#define KEY_CODE_F12             0x58
#define KEY_CODE_F13             0x64    /*                     (NEC PC98) */
#define KEY_CODE_F14             0x65    /*                     (NEC PC98) */
#define KEY_CODE_F15             0x66    /*                     (NEC PC98) */
#define KEY_CODE_KANA            0x70    /* (Japanese keyboard)            */
#define KEY_CODE_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define KEY_CODE_CONVERT         0x79    /* (Japanese keyboard)            */
#define KEY_CODE_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define KEY_CODE_YEN             0x7D    /* (Japanese keyboard)            */
#define KEY_CODE_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define KEY_CODE_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define KEY_CODE_PREVTRACK       0x90    /* Previous Track (KEY_CODE_CIRCUMFLEX on Japanese keyboard) */
#define KEY_CODE_AT              0x91    /*                     (NEC PC98) */
#define KEY_CODE_COLON           0x92    /*                     (NEC PC98) */
#define KEY_CODE_UNDERLINE       0x93    /*                     (NEC PC98) */
#define KEY_CODE_KANJI           0x94    /* (Japanese keyboard)            */
#define KEY_CODE_STOP            0x95    /*                     (NEC PC98) */
#define KEY_CODE_AX              0x96    /*                     (Japan AX) */
#define KEY_CODE_UNLABELED       0x97    /*                        (J3100) */
#define KEY_CODE_NEXTTRACK       0x99    /* Next Track */
#define KEY_CODE_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define KEY_CODE_RCONTROL        0x9D
#define KEY_CODE_MUTE            0xA0    /* Mute */
#define KEY_CODE_CALCULATOR      0xA1    /* Calculator */
#define KEY_CODE_PLAYPAUSE       0xA2    /* Play / Pause */
#define KEY_CODE_MEDIASTOP       0xA4    /* Media Stop */
#define KEY_CODE_VOLUMEDOWN      0xAE    /* Volume - */
#define KEY_CODE_VOLUMEUP        0xB0    /* Volume + */
#define KEY_CODE_WEBHOME         0xB2    /* Web home */
#define KEY_CODE_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define KEY_CODE_DIVIDE          0xB5    /* / on numeric keypad */
#define KEY_CODE_SYSRQ           0xB7
#define KEY_CODE_RMENU           0xB8    /* right Alt */
#define KEY_CODE_PAUSE           0xC5    /* Pause */
#define KEY_CODE_HOME            0xC7    /* Home on arrow keypad */
#define KEY_CODE_UP              0xC8    /* UpArrow on arrow keypad */
#define KEY_CODE_PRIOR           0xC9    /* PgUp on arrow keypad */
#define KEY_CODE_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define KEY_CODE_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define KEY_CODE_END             0xCF    /* End on arrow keypad */
#define KEY_CODE_DOWN            0xD0    /* DownArrow on arrow keypad */
#define KEY_CODE_NEXT            0xD1    /* PgDn on arrow keypad */
#define KEY_CODE_INSERT          0xD2    /* Insert on arrow keypad */
#define KEY_CODE_DELETE          0xD3    /* Delete on arrow keypad */
#define KEY_CODE_LWIN            0xDB    /* Left Windows key */
#define KEY_CODE_RWIN            0xDC    /* Right Windows key */
#define KEY_CODE_APPS            0xDD    /* AppMenu key */
#define KEY_CODE_POWER           0xDE    /* System Power */
#define KEY_CODE_SLEEP           0xDF    /* System Sleep */
#define KEY_CODE_WAKE            0xE3    /* System Wake */
#define KEY_CODE_WEBSEARCH       0xE5    /* Web Search */
#define KEY_CODE_WEBFAVORITES    0xE6    /* Web Favorites */
#define KEY_CODE_WEBREFRESH      0xE7    /* Web Refresh */
#define KEY_CODE_WEBSTOP         0xE8    /* Web Stop */
#define KEY_CODE_WEBFORWARD      0xE9    /* Web Forward */
#define KEY_CODE_WEBBACK         0xEA    /* Web Back */
#define KEY_CODE_MYCOMPUTER      0xEB    /* My Computer */
#define KEY_CODE_MAIL            0xEC    /* Mail */
#define KEY_CODE_MEDIASELECT     0xED    /* Media Select */

/*
*  Alternate names for keys, to facilitate transition from DOS.
*/
#define KEY_CODE_BACKSPACE       KEY_CODE_BACK            /* backspace */
#define KEY_CODE_NUMPADSTAR      KEY_CODE_MULTIPLY        /* * on numeric keypad */
#define KEY_CODE_LALT            KEY_CODE_LMENU           /* left Alt */
#define KEY_CODE_CAPSLOCK        KEY_CODE_CAPITAL         /* CapsLock */
#define KEY_CODE_NUMPADMINUS     KEY_CODE_SUBTRACT        /* - on numeric keypad */
#define KEY_CODE_NUMPADPLUS      KEY_CODE_ADD             /* + on numeric keypad */
#define KEY_CODE_NUMPADPERIOD    KEY_CODE_DECIMAL         /* . on numeric keypad */
#define KEY_CODE_NUMPADSLASH     KEY_CODE_DIVIDE          /* / on numeric keypad */
#define KEY_CODE_RALT            KEY_CODE_RMENU           /* right Alt */
#define KEY_CODE_UPARROW         KEY_CODE_UP              /* UpArrow on arrow keypad */
#define KEY_CODE_PGUP            KEY_CODE_PRIOR           /* PgUp on arrow keypad */
#define KEY_CODE_LEFTARROW       KEY_CODE_LEFT            /* LeftArrow on arrow keypad */
#define KEY_CODE_RIGHTARROW      KEY_CODE_RIGHT           /* RightArrow on arrow keypad */
#define KEY_CODE_DOWNARROW       KEY_CODE_DOWN            /* DownArrow on arrow keypad */
#define KEY_CODE_PGDN            KEY_CODE_NEXT            /* PgDn on arrow keypad */

/*
*  Alternate names for keys originally not used on US keyboards.
*/
#define KEY_CODE_CIRCUMFLEX      KEY_CODE_PREVTRACK       /* Japanese keyboard */
