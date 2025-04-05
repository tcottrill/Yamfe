
#include "animation.h"
#include "globals.h"
#include "config.h"
#include "gl_basics.h"


int num_stars=80;
int max_star_speed=3;
int h, w;

STAR_STRUCT stars[320];


//stars = (STAR_STRUCT *) malloc(num_stars * sizeof(STAR_STRUCT));//init stars
void clearstars()
{
     int i;
	for (i = 0; i <=num_stars-1; i++)
		{
		}
}


void placestars()
{
	int i;
		
	for (i = 0; i <=num_stars-1; i++)
		{  
		  glPointSize(3.0f);
		  glColor4ub(stars[i].r,stars[i].g,stars[i].b,255);
		  glBegin(GL_POINTS);
		  glVertex2i(stars[i].x , stars[i].y);
		  glEnd();
		  glPointSize(2.3f); //RESET ORIG POINTSIZE  config.pointsize
		}
}

void movestars()
{
	int i;
	
	switch (rotation)
	{
	case 0: 
		   for(i = 0; i <= num_stars-1; i++)
			{
				stars[i].y-=stars[i].speed;
				if(stars[i].y < 0 ){
		
				stars[i].y = h;
				}
			}
		    break;
	case 1: 
		//Rotated Left (1)
		   for(i = 0; i <= num_stars-1; i++)
			{
				stars[i].x+=stars[i].speed;
				if(stars[i].x > w ){
		
				stars[i].x = 0;
				}
			}
		    break;
	case 2: 
		   //Rotated Right (2)
			for(i = 0; i <= num_stars-1; i++)
			{
				stars[i].x-=stars[i].speed;
				if(stars[i].x < 0 ){
		
				stars[i].x = w;
				}
			}
		    break;
	case 3: 
		    for(i = 0; i <= num_stars-1; i++)
			{
				stars[i].y-=stars[i].speed;
				if(stars[i].y < 0 ){
		
				stars[i].y = h;
				}
			}
		    break;
	}
}

void fillstars( int width, int height)
{
	int i,r;
	h=height;
	w=width;

	if (width > 640) num_stars= width/10;
	else num_stars= width/8;


	for (i = 0; i <= num_stars-1; i++)
		{
		 stars[i].y = rand()%h; stars[i].x = rand()%w; 
		 stars[i].speed = rand()%max_star_speed + 2; 
		 stars[i].r =rand()%255;
		 stars[i].g	=rand()%255;
		 stars[i].b	=rand()%255;
		 r=rand()%1000; if (r > 400 && r < 325){stars[i].blink=1;} 
		}
}

