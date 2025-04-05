#ifndef INPTPORT_H
#define INPTPORT_H
#include "osdepend.h"
//#include "deftypes.h"

/* input ports handling */


/***************************************************************************

Don't confuse this with the I/O ports in memory.h. This is used to handle game
inputs (joystick, coin slots, etc). Typically, you will read them using
input_port_[n]_r(), which you will associate to the appropriate memory
address or I/O port.

***************************************************************************/
struct InputPort
{
	unsigned short mask;			/* bits affected */
	unsigned short default_value;	/* def value for the bits affected */
							        /* you can also use one of the IP_ACTIVE defines below */
	unsigned int type;			    /* see defines below */
	const char *name;		        /* name to display */
	unsigned short keyboard;		/* key affecting the input bits */
	unsigned short joystick;		/* joystick command affecting the input bits */
//	struct
//	{
//		const char *tag;		/* used to tag PORT_START declarations */
//	} start;
};


typedef struct machine
{
    struct InputPort *input_ports;
}
machine;

extern machine Machine;



#define IP_ACTIVE_HIGH 0x0000
#define IP_ACTIVE_LOW 0xffff

enum { IPT_END=1,IPT_PORT,
	/* use IPT_JOYSTICK for panels where the player has one single joystick */
	IPT_JOYSTICK_UP, IPT_JOYSTICK_DOWN, IPT_JOYSTICK_LEFT, IPT_JOYSTICK_RIGHT,
	/* use IPT_JOYSTICKLEFT and IPT_JOYSTICKRIGHT for dual joystick panels */
	IPT_JOYSTICKRIGHT_UP, IPT_JOYSTICKRIGHT_DOWN, IPT_JOYSTICKRIGHT_LEFT, IPT_JOYSTICKRIGHT_RIGHT,
	IPT_JOYSTICKLEFT_UP, IPT_JOYSTICKLEFT_DOWN, IPT_JOYSTICKLEFT_LEFT, IPT_JOYSTICKLEFT_RIGHT,
	IPT_BUTTON1, IPT_BUTTON2, IPT_BUTTON3, IPT_BUTTON4,	/* action buttons */
	IPT_BUTTON5, IPT_BUTTON6, IPT_BUTTON7, IPT_BUTTON8,

	/* analog inputs */
	/* the "arg" field contains the def sensitivity expressed as a percentage */
	/* (100 = def, 50 = half, 200 = twice) */
	IPT_ANALOG_START,
	IPT_PADDLE, IPT_DIAL, IPT_TRACKBALL_X, IPT_TRACKBALL_Y, IPT_AD_STICK_X, IPT_AD_STICK_Y,
	IPT_ANALOG_END,

	IPT_COIN1, IPT_COIN2, IPT_COIN3, IPT_COIN4,	/* coin slots */
	IPT_START1, IPT_START2, IPT_START3, IPT_START4,	/* start buttons */
	IPT_SERVICE, IPT_TILT,
	IPT_DIPSWITCH_NAME, IPT_DIPSWITCH_SETTING,
/* Many games poll an input bit to check for vertical blanks instead of using */
/* interrupts. This special value allows you to handle that. If you set one of the */
/* input bits to this, the bit will be inverted while a vertical blank is happening. */
	IPT_VBLANK,
	IPT_UNKNOWN,
	IPT_EXTENSION,	/* this is an extension on the previous InputPort, not a real inputport. */
					/* It is used to store additional parameters for analog inputs */

	/* the following are special codes for user interface handling - not to be used by drivers! */
	IPT_UI_PAUSE,
	IPT_EXIT
};

#define IPT_UNUSED     IPF_UNUSED

#define IPF_MASK       0xffffff00
#define IPF_UNUSED     0x80000000	/* The bit is not used by this game, but is used */
									/* by other games running on the same hardware. */
									/* This is different from IPT_UNUSED, which marks */
									/* bits not connected to anything. */
#define IPF_COCKTAIL   IPF_PLAYER2	/* the bit is used in cocktail mode only */

#define IPF_CHEAT      0x40000000	/* Indicates that the input bit is a "cheat" key */
									/* (providing invulnerabilty, level advance, and */
									/* so on). MAME will not recognize it when the */
									/* -nocheat command line option is specified. */

#define IPF_PLAYERMASK 0x00030000	/* use IPF_PLAYERn if more than one person can */
#define IPF_PLAYER1    0         	/* play at the same time. The IPT_ should be the same */
#define IPF_PLAYER2    0x00010000	/* for all players (e.g. IPT_BUTTON1 | IPF_PLAYER2) */
#define IPF_PLAYER3    0x00020000	/* IPF_PLAYER1 is the def and can be left out to */
#define IPF_PLAYER4    0x00030000	/* increase readability. */

#define IPF_8WAY       0         	/* Joystick modes of operation. 8WAY is the def, */
#define IPF_4WAY       0x00080000	/* it prevents left/right or up/down to be pressed at */
#define IPF_2WAY       0         	/* the same time. 4WAY prevents diagonal directions. */
									/* 2WAY should be used for joysticks wich move only */
                                 	/* on one axis (e.g. Battle Zone) */

#define IPF_IMPULSE    0x00100000	/* When this is set, when the key corrisponding to */
									/* the input bit is pressed it will be reported as */
									/* pressed for a certain number of video frames and */
									/* then released, regardless of the real status of */
									/* the key. This is useful e.g. for some coin inputs. */
									/* The number of frames the signal should stay active */
									/* is specified in the "arg" field. */
#define IPF_COUNTER    0x04000000	/* Countdown Timer */
#define IPF_SPULSE     0x08000000
#define IPF_TOGGLE     0x00200000	/* When this is set, the key acts as a toggle - press */
									/* it once and it goes on, press it again and it goes off. */
									/* useful e.g. for sone Test Mode dip switches. */
#define IPF_REVERSE    0x00400000	/* By def, analog inputs like IPT_TRACKBALL increase */
									/* when going right/up. This flag inverts them. */

#define IPF_CENTER     0x00800000	/* always preload in->def, autocentering the STICK/TRACKBALL */

#define IPF_CUSTOM_UPDATE 0x01000000 /* normally, analog ports are updated when they are accessed. */
									/* When this flag is set, they are never updated automatically, */
									/* it is the responsibility of the driver to call */
									/* update_analog_port(int port). */

#define IPF_RESETCPU   0x02000000	/* when the key is pressed, reset the first CPU */


/* The "arg" field contains 4 bytes fields */
#define IPF_SENSITIVITY(percent)	((percent & 0xff) << 8)
#define IPF_DELTA(val)				((val & 0xff) << 16)
#define IPF_CLIP(clip)				((clip & 0xff) << 24)

#define IP_GET_IMPULSE(port) (((port)->type >> 8) & 0xff)
#define IP_GET_SENSITIVITY(port) ((((port)+1)->type >> 8) & 0xff)
#define IP_SET_SENSITIVITY(port,val) ((port)+1)->type = ((port+1)->type & 0xffff00ff)|((val&0xff)<<8)
#define IP_GET_DELTA(port) ((((port)+1)->type >> 16) & 0xff)
#define IP_SET_DELTA(port,val) ((port)+1)->type = ((port+1)->type & 0xff00ffff)|((val&0xff)<<16)
#define IP_GET_CLIP(port) ((((port)+1)->type >> 24) & 0xff)
#define IP_GET_MIN(port) (((port)+1)->mask)
#define IP_GET_MAX(port) (((port)+1)->default_value)

//#define PORT_START_TAG(tag_) { 0, 0, IPT_PORTTAG, tag_ },//	port->start.tag = (tag_)
//#define PORT_START_TAG(tag_) ((port)->start.tag = tag_)
#define IP_NAME_DEFAULT ((const char *)-1)

#define IP_KEY_DEFAULT	0xffff
#define IP_KEY_NONE		0xfffe
#define IP_KEY_PREVIOUS	0xfffd	/* use the same key as the previous input bit */

#define IP_JOY_DEFAULT	0xffff
#define IP_JOY_NONE		0xfffe
#define IP_JOY_PREVIOUS	0xfffd	/* use the same joy as the previous input bit */

/* start of table */
#define INPUT_PORTS_START(name) static struct InputPort input_ports_##name[] = {
/* end of table */
#define INPUT_PORTS_END { 0, 0, IPT_END, 0, 0, 0 } };
/* start of a new input port */
#define PORT_START { 0, 0, IPT_PORT, 0, 0, 0 },
/* input bit definition */
#define PORT_BIT(mask,def,type) { mask, def, type, IP_NAME_DEFAULT, IP_KEY_DEFAULT, IP_JOY_DEFAULT },
/* impulse input bit definition */
#define PORT_BIT_IMPULSE(mask,def,type,duration) { mask, def, \
		type | IPF_IMPULSE | ((duration & 0xff) << 8), IP_NAME_DEFAULT, IP_KEY_DEFAULT, IP_JOY_DEFAULT },
/* input bit definition with extended fields */
#define PORT_BITX(mask,def,type,name,key,joy) { mask, def, type, name, key, joy },
/* analog input */
#define PORT_ANALOG(mask,def,type,sensitivity,delta,clip,min,max) \
	{ mask, def, type, IP_NAME_DEFAULT, IP_KEY_DEFAULT, IP_JOY_DEFAULT }, \
	{ min, max, IPT_EXTENSION | IPF_SENSITIVITY(sensitivity) | IPF_DELTA(delta) | IPF_CLIP(clip), \
			IP_NAME_DEFAULT, IP_KEY_DEFAULT, IP_JOY_DEFAULT },
/* analog input with extended fields for defining def keys & sensitivities */
#define PORT_ANALOGX(mask,def,type,sensitivity,delta,clip,min,max,keydec,keyinc,joydec,joyinc) \
	{ mask, def, type, IP_NAME_DEFAULT, keydec, joydec }, \
	{ min, max, IPT_EXTENSION | IPF_SENSITIVITY(sensitivity) | IPF_DELTA(delta) | IPF_CLIP(clip), \
			IP_NAME_DEFAULT, keyinc, joyinc },

/* dip switch definition */
#define PORT_DIPNAME(mask,def,name) { mask, def, IPT_DIPSWITCH_NAME, name, IP_KEY_NONE, IP_JOY_NONE },
#define PORT_DIPSETTING(def,name) { 0, def, IPT_DIPSWITCH_SETTING, name, IP_KEY_NONE, IP_JOY_NONE },

#define PORT_SERVICE(mask,def)	\
	PORT_BITX(    mask, mask & def, IPT_DIPSWITCH_NAME | IPF_TOGGLE, DEF_STR( Service_Mode ), OSD_KEY_F2, IP_JOY_NONE )	\
	PORT_DIPSETTING(    mask & def, DEF_STR( Off ) )	\
	PORT_DIPSETTING(    mask &~def, DEF_STR( On ) )


#define MAX_DEFSTR_LEN 20
extern char ipdn_defaultstrings[][MAX_DEFSTR_LEN];

enum {
	STR_Off,
	STR_On,
	STR_No,
	STR_Yes,
	STR_Lives,
	STR_Bonus_Life,
	STR_Difficulty,
	STR_Demo_Sounds,
	STR_Coinage,
	STR_Coin_A,
	STR_Coin_B,
	STR_9C_1C,
	STR_8C_1C,
	STR_7C_1C,
	STR_6C_1C,
	STR_5C_1C,
	STR_4C_1C,
	STR_3C_1C,
	STR_2C_1C,
	STR_3C_2C,
	STR_4C_3C,
	STR_1C_1C,
	STR_3C_4C,
	STR_2C_3C,
	STR_1C_2C,
	STR_2C_5C,
	STR_1C_3C,
	STR_1C_4C,
	STR_1C_5C,
	STR_1C_6C,
	STR_1C_7C,
	STR_1C_8C,
	STR_1C_9C,
	STR_Free_Play,
	STR_Cabinet,
	STR_Upright,
	STR_Cocktail,
	STR_Flip_Screen,
	STR_Service_Mode,
	STR_Unused,
	STR_Unknown,
	STR_TOTAL
};

#define DEF_STR(str_num) (ipdn_defaultstrings[STR_##str_num])


#define MAX_INPUT_PORTS 16

int load_input_port_settings(void);
void save_input_port_settings(void);

const char *input_port_name(const struct InputPort *in);
int input_port_type_key(int type);
int input_port_type_joy(int type);
int input_port_key(const struct InputPort *in);
int input_port_joy(const struct InputPort *in);

void update_analog_port(int port);
void update_input_ports(void);		/* called by cpuintrf.c - not for external use */
void inputport_vblank_end(void);	/* called by cpuintrf.c - not for external use */


unsigned int readinputportbytag(const char *tag);
int port_tag_to_index(const char *tag);
int KeyFlip(int keynum, int state);
int Check_Input(int in, int state);

int readinputport(int port);
int input_port_0_r(int offset);
int input_port_1_r(int offset);
int input_port_2_r(int offset);
int input_port_3_r(int offset);
int input_port_4_r(int offset);
int input_port_5_r(int offset);
int input_port_6_r(int offset);
int input_port_7_r(int offset);
int input_port_8_r(int offset);
int input_port_9_r(int offset);
int input_port_10_r(int offset);
int input_port_11_r(int offset);
int input_port_12_r(int offset);
int input_port_13_r(int offset);
int input_port_14_r(int offset);
int input_port_15_r(int offset);

struct ipd
{
	unsigned int type;
	const char *name;
	unsigned short keyboard;
	unsigned short joystick;
};

#endif
