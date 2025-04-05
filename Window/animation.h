


#ifndef ANIMATION_H
#define ANIMATION_H


typedef struct star_struct STAR_STRUCT;

struct star_struct {
	int x, y, r, g, b, speed, blinkrate,blink;
	  };


//STAR_STRUCT *stars;
extern STAR_STRUCT stars[320];

void fillstars( int width, int height);
void movestars();
void placestars();
void clearstars();

#endif 