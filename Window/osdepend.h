#ifndef OSDEPEND_H
#define OSDEPEND_H

#include "rawinput.h"

#define    OSD_KEY_NONE		     0
#define    OSD_KEY_A             KEY_A//1
#define    OSD_KEY_B             KEY_B//2
#define    OSD_KEY_C             KEY_C//3
#define    OSD_KEY_D             KEY_D//4
#define    OSD_KEY_E             KEY_E//5
#define    OSD_KEY_F             KEY_F//6
#define    OSD_KEY_G             KEY_G//7
#define    OSD_KEY_H             KEY_H//8
#define    OSD_KEY_I             KEY_I//9
#define    OSD_KEY_J             KEY_J//1//0
#define    OSD_KEY_K             KEY_K//11
#define    OSD_KEY_L             KEY_J//12
#define    OSD_KEY_M             KEY_M//13
#define    OSD_KEY_N             KEY_N//14
#define    OSD_KEY_O             KEY_O//15
#define    OSD_KEY_P             KEY_P//16
#define    OSD_KEY_Q             KEY_Q//17
#define    OSD_KEY_R             KEY_R//18
#define    OSD_KEY_S             KEY_S//19
#define    OSD_KEY_T             KEY_T//20
#define    OSD_KEY_U             KEY_U//21
#define    OSD_KEY_V             KEY_V//22
#define    OSD_KEY_W             KEY_W//23
#define    OSD_KEY_X             KEY_X//24
#define    OSD_KEY_Y             KEY_Y//25
#define    OSD_KEY_Z             KEY_Z//26
#define    OSD_KEY_0             KEY_0//27
#define    OSD_KEY_1             KEY_1//28
#define    OSD_KEY_2             KEY_2//29
#define    OSD_KEY_3             KEY_3//30
#define    OSD_KEY_4             KEY_4//31
#define    OSD_KEY_5             KEY_5//32
#define    OSD_KEY_6             KEY_6 // 33
#define    OSD_KEY_7             KEY_7//34
#define    OSD_KEY_8             KEY_8//3/5
#define    OSD_KEY_9             KEY_9//36
#define    OSD_KEY_0_PAD         KEY_0_PAD//3/7
#define    OSD_KEY_1_PAD         KEY_1_PAD//38
#define    OSD_KEY_2_PAD         KEY_2_PAD//39
#define    OSD_KEY_3_PAD         KEY_3_PAD//40
#define    OSD_KEY_4_PAD         KEY_4_PAD//41
#define    OSD_KEY_5_PAD         KEY_5_PAD//42
#define    OSD_KEY_6_PAD         KEY_6_PAD//43
#define    OSD_KEY_7_PAD         KEY_7_PAD//44
#define    OSD_KEY_8_PAD         KEY_8_PAD//45
#define    OSD_KEY_9_PAD         KEY_9_PAD//46
#define    OSD_KEY_F1            KEY_F1//47
#define    OSD_KEY_F2            KEY_F2//48
#define    OSD_KEY_F3            KEY_F3//49
#define    OSD_KEY_F4            KEY_F4//50
#define    OSD_KEY_F5            KEY_F5//51
#define    OSD_KEY_F6            KEY_F6//52
#define    OSD_KEY_F7            KEY_F7//53
#define    OSD_KEY_F8            KEY_F8 //54
#define    OSD_KEY_F9            KEY_F9//55
#define    OSD_KEY_F10           KEY_F10//56
#define    OSD_KEY_F11           KEY_F11//57
#define    OSD_KEY_F12           KEY_F12//58
#define    OSD_KEY_ESC           KEY_ESC//59
#define    OSD_KEY_TILDE         KEY_TILDE//60
#define    OSD_KEY_MINUS         KEY_MINUS//61
#define    OSD_KEY_EQUALS        KEY_EQUALS//62
#define    OSD_KEY_BACKSPACE     KEY_BACKSPACE//63
#define    OSD_KEY_TAB           KEY_TAB//64
#define    OSD_KEY_OPENBRACE     KEY_OPENBRACE//65
#define    OSD_KEY_CLOSEBRACE    KEY_CLOSEBRACE//66
#define    OSD_KEY_ENTER         KEY_ENTER//67
#define    OSD_KEY_COLON         KEY_COLON//68
#define    OSD_KEY_QUOTE         KEY_QUOTE//69
#define    OSD_KEY_BACKSLASH     KEY_BACKSLASH//70
#define    OSD_KEY_BACKSLASH2    KEY_BACKSLASH2//71
#define    OSD_KEY_COMMA         KEY_COMMA//72
#define    OSD_KEY_STOP          KEY_STOP//73
#define    OSD_KEY_SLASH         KEY_SLASH//74
#define    OSD_KEY_SPACE         KEY_SPACE//75
#define    OSD_KEY_INSERT        KEY_INSERT//76
#define    OSD_KEY_DEL           KEY_DEL//77
#define    OSD_KEY_HOME          KEY_HOME//78
#define    OSD_KEY_END           KEY_END//79
#define    OSD_KEY_PGUP          KEY_PGUP//80
#define    OSD_KEY_PGDN          KEY_PGDN//81
#define    OSD_KEY_LEFT          KEY_LEFT//82
#define    OSD_KEY_RIGHT         KEY_RIGHT//83
#define    OSD_KEY_UP            KEY_UP//84
#define    OSD_KEY_DOWN          KEY_DOWN// 85
#define    OSD_KEY_SLASH_PAD     KEY_SLASH_PAD//86
#define    OSD_KEY_ASTERISK      KEY_ASTERISK//87
#define    OSD_KEY_MINUS_PAD     KEY_MINUS_PAD//88
#define    OSD_KEY_PLUS_PAD      KEY_PLUS_PAD //89
#define    OSD_KEY_DEL_PAD       KEY_DEL_PAD//90
#define    OSD_KEY_ENTER_PAD     KEY_ENTER_PAD//91
#define    OSD_KEY_PRTSCR        KEY_PRTSCR//92
#define    OSD_KEY_PAUSE         KEY_PAUSE//93
#define    OSD_KEY_ABNT_C1       KEY_ABNT_C1//94
#define    OSD_KEY_YEN           KEY_YEN//95
#define    OSD_KEY_KANA          KEY_KANA//96
#define    OSD_KEY_CONVERT       KEY_CONVERT//97
#define    OSD_KEY_NOCONVERT     KEY_NOCONVERT//98
#define    OSD_KEY_AT            KEY_AT//99
#define    OSD_KEY_CIRCUMFLEX    KEY_CIRCUMFLEX //100
#define    OSD_KEY_COLON2        KEY_COLON2//101
#define    OSD_KEY_KANJI         KEY_KANJI//102
#define    OSD_KEY_EQUALS_PAD    KEY_EQUALS_PAD//103  /* MacOS X */
#define    OSD_KEY_BACKQUOTE     KEY_BACKQUOTE//104  /* MacOS X */
#define    OSD_KEY_SEMICOLON     KEY_SEMICOLON//105  /* MacOS X */
#define    OSD_KEY_SEMICOLON     KEY_SEMICOLON   //106  /* MacOS X */
#define    OSD_KEY_UNKNOWN1      0//107
#define    OSD_KEY_UNKNOWN2      0//108
#define    OSD_KEY_UNKNOWN3      0//109
#define    OSD_KEY_UNKNOWN4      0//110
#define    OSD_KEY_UNKNOWN5      0//111
#define    OSD_KEY_UNKNOWN6      0//112
#define    OSD_KEY_UNKNOWN7      0//113
#define    OSD_KEY_UNKNOWN8      0//114

#define    OSD_KEY_LSHIFT        KEY_LSHIFT//115
#define    OSD_KEY_RSHIFT        KEY_RSHIFT//116
#define    OSD_KEY_LCONTROL      KEY_LCONTROL//117
#define    OSD_KEY_RCONTROL      KEY_RCONTROL//1////18
#define    OSD_KEY_ALT           KEY_ALT//119
#define    OSD_KEY_ALTGR         KEY_ALTGR//120
#define    OSD_KEY_LWIN          KEY_LWIN //121
#define    OSD_KEY_RWIN          KEY_RWIN //122
#define    OSD_KEY_MENU          KEY_MENU//123
#define    OSD_KEY_SCRLOCK       KEY_SCRLOCK//124
#define    OSD_KEY_NUMLOCK       KEY_NUMLOCK//125
#define    OSD_KEY_CAPSLOCK      /KEY_CAPSLOCK//126

#define    OSD_MAX_KEY          0xAF

/* 116 - 119 */

/* The following are defined in Allegro */
/* 120 KEY_RCONTROL */
/* 121 KEY_ALTGR */
/* 122 KEY_SLASH2 */
/* 123 KEY_PAUSE */

/*
 * ASG 980730: these are pseudo-keys that the os-dependent code can
 * map to whatever they see fit
 * HJB 980812: added some more names and used higher values because
 * there were some clashes with Allegro's scancodes (see above)
 */
#define OSD_KEY_FAST_EXIT			128
#define OSD_KEY_CANCEL				129
#define OSD_KEY_RESET_MACHINE		130
#define OSD_KEY_CONFIGURE			133
#define OSD_KEY_ON_SCREEN_DISPLAY	134
#define OSD_KEY_SHOW_GFX			135
#define OSD_KEY_FRAMESKIP_INC		136
#define OSD_KEY_FRAMESKIP_DEC		137
#define OSD_KEY_THROTTLE			138
#define OSD_KEY_SHOW_FPS			139
#define OSD_KEY_SHOW_PROFILE		140
#define OSD_KEY_SHOW_TOTAL_COLORS	141
#define OSD_KEY_SNAPSHOT			142
#define OSD_KEY_CHEAT_TOGGLE		143
#define OSD_KEY_DEBUGGER			144
#define OSD_KEY_UI_LEFT				145
#define OSD_KEY_UI_RIGHT			146
#define OSD_KEY_UI_UP				147
#define OSD_KEY_UI_DOWN				148
#define OSD_KEY_UI_SELECT			149
#define OSD_KEY_ANY					150
#define OSD_KEY_CHAT_ENABLE         151

#define OSD_MAX_PSEUDO				151

#define OSD_JOY_LEFT    1
#define OSD_JOY_RIGHT   2
#define OSD_JOY_UP      3
#define OSD_JOY_DOWN    4
#define OSD_JOY_FIRE1   5
#define OSD_JOY_FIRE2   6
#define OSD_JOY_FIRE3   7
#define OSD_JOY_FIRE4   8
#define OSD_JOY_FIRE5   9
#define OSD_JOY_FIRE6   10
#define OSD_JOY_FIRE7   11
#define OSD_JOY_FIRE8   12
#define OSD_JOY_FIRE9   13
#define OSD_JOY_FIRE10  14
#define OSD_JOY_FIRE    15      /* any of the first joystick fire buttons */
#define OSD_JOY2_LEFT   16
#define OSD_JOY2_RIGHT  17
#define OSD_JOY2_UP     18
#define OSD_JOY2_DOWN   19
#define OSD_JOY2_FIRE1  20
#define OSD_JOY2_FIRE2  21
#define OSD_JOY2_FIRE3  22
#define OSD_JOY2_FIRE4  23
#define OSD_JOY2_FIRE5  24
#define OSD_JOY2_FIRE6  25
#define OSD_JOY2_FIRE7  26
#define OSD_JOY2_FIRE8  27
#define OSD_JOY2_FIRE9  28
#define OSD_JOY2_FIRE10 29
#define OSD_JOY2_FIRE   30      /* any of the second joystick fire buttons */
#define OSD_JOY3_LEFT   31
#define OSD_JOY3_RIGHT  32
#define OSD_JOY3_UP     33
#define OSD_JOY3_DOWN   34
#define OSD_JOY3_FIRE1  35
#define OSD_JOY3_FIRE2  36
#define OSD_JOY3_FIRE3  37
#define OSD_JOY3_FIRE4  38
#define OSD_JOY3_FIRE5  39
#define OSD_JOY3_FIRE6  40
#define OSD_JOY3_FIRE7  41
#define OSD_JOY3_FIRE8  42
#define OSD_JOY3_FIRE9  43
#define OSD_JOY3_FIRE10 44
#define OSD_JOY3_FIRE   45      /* any of the third joystick fire buttons */
#define OSD_JOY4_LEFT   46
#define OSD_JOY4_RIGHT  47
#define OSD_JOY4_UP     48
#define OSD_JOY4_DOWN   49
#define OSD_JOY4_FIRE1  50
#define OSD_JOY4_FIRE2  51
#define OSD_JOY4_FIRE3  52
#define OSD_JOY4_FIRE4  53
#define OSD_JOY4_FIRE5  54
#define OSD_JOY4_FIRE6  55
#define OSD_JOY4_FIRE7  56
#define OSD_JOY4_FIRE8  57
#define OSD_JOY4_FIRE9  58
#define OSD_JOY4_FIRE10 59
#define OSD_JOY4_FIRE   60      /* any of the fourth joystick fire buttons */
#define OSD_MAX_JOY     60

/* We support 4 players for each analog control */
#define OSD_MAX_JOY_ANALOG	4
#define X_AXIS          1
#define Y_AXIS          2

#define COIN_COUNTERS	4	/* total # of coin counters */
#define NUMVOICES 16 //SOUND - PUT SOMEWHERE ELSE

//void osd_update_audio(void);
#define osd_fread_msbfirst osd_fread_swap
#define osd_fwrite_msbfirst osd_fwrite_swap
#define osd_fread_lsbfirst osd_fread
#define osd_fwrite_lsbfirst osd_fwrite

//int scale_by_cycles(int val, int clock);

int osd_key_pressed(int keycode);
int osd_key_pressed_memory(int keycode);
int osd_key_pressed_memory_repeat(int keycode,int speed);
int osd_read_key_immediate();
/* the following two should return pseudo key codes if translate != 0 */

int osd_key_invalid(int keycode);
const char *osd_joy_name(int joycode);
const char *osd_key_name(int keycode);
void osd_poll_joysticks();
int osd_joy_pressed(int joycode);
void alleg_init_input ();

void osd_trak_read(int player,int *deltax,int *deltay);

/* return values in the range -128 .. 128 (yes, 128, not 127) */
void osd_analogjoy_read(int player,int *analog_x, int *analog_y);



/* file handling routines */
#define OSD_FILETYPE_ROM 1
#define OSD_FILETYPE_SAMPLE 2
#define OSD_FILETYPE_HIGHSCORE 3
#define OSD_FILETYPE_CONFIG 4
#define OSD_FILETYPE_INPUTLOG 5
#define OSD_FILETYPE_STATE 6
#define OSD_FILETYPE_ARTWORK 7
#define OSD_FILETYPE_MEMCARD 8
#define OSD_FILETYPE_SCREENSHOT 9


/* gamename holds the driver name, filename is only used for ROMs and samples. */
/* if 'write' is not 0, the file is opened for write. Otherwise it is opened */
/* for read. */
void *osd_fopen (const char *gamename,const char *filename,int filetype,int write);
int osd_fread (void *file,void *buffer,int length);
int osd_fread_swap(void *file,void *buffer,int length);
int osd_fread_scatter(void *file,void *buffer,int length,int increment);
int osd_fwrite (void *file,const void *buffer,int length);
int osd_fseek (void *file,int offset,int whence);
unsigned int osd_fcrc (void *file);
void osd_fclose (void *file);


#define bitget(p,m) ((p) & (m))
#define bitset(p,m) ((p) |= (m))
#define bitclr(p,m) ((p) &= ~(m))
#define bitflp(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))

 
#endif
