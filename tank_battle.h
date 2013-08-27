/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#ifndef TANK_BATTLE_H
#define TANK_BATTLE_H
#include <ncurses.h>

#define TB_SUCCESS		0
#define TB_FAILED		-1

#define TANK_HEIGHT		3
#define TANK_WIDTH		3
#define TANK_LATTICE_NUM	(TANK_HEIGHT*TANK_WIDTH)
#define TANK_DECK_HEIGHT	(TANK_HEIGHT*8+2)
#define TANK_DECK_WIDTH		(TANK_WIDTH*10+2)

#define TANK_PROMPT_HEIGHT	(TANK_DECK_HEIGHT)
#define TANK_PROMPT_WIDTH	(15)//coordinate+standpoint+hp
#define SCREEN_HEIGHT		(TANK_DECK_HEIGHT)
#define SCREEN_WIDTH		(TANK_DECK_WIDTH+TANK_PROMPT_WIDTH)
#define TANK_MODEL_NUM	4
#define TANK_SPACE_NUM	3

#define BLUE_TANK_NUM	12//36
#define BLUE_MIN_FIGHTING_TANK	6
#define BLUE_MAX_FIGHTING_TANK	9
#define BLUE_AUTO_TANK			3
#define BLUE_MANUAL_TANK		3

#define GREEN_TANK_NUM	12//36
#define GREEN_MIN_FIGHTING_TANK	6
#define GREEN_MAX_FIGHTING_TANK	9
#define GREEN_AUTO_TANK			3
#define GREEN_MANUAL_TANK		3


#define H_TANK		3
#define H_BULLET	1
#define H_BARRIER	1

#define W_TANK		3
#define W_BULLET	1
#define W_BARRIER	1

#define HP_TANK		5
#define HP_BULLET	1
#define HP_BARRIER	5

#define NUMBER		0

#define MAX_TANK_Y		(TANK_DECK_HEIGHT-3-1)
#define MAX_TANK_X		(TANK_DECK_WIDTH-3-1)
#define MIN_TANK_Y		1
#define MIN_TANK_X		1

#define MAX_BULLET_Y	(TANK_DECK_HEIGHT-2)	
#define MAX_BULLET_X	(TANK_DECK_WIDTH-2)
#define MIN_BULLET_Y	1
#define MIN_BULLET_X	1

#define MAX_BARRIER_Y	(TANK_DECK_HEIGHT-2-1)
#define MAX_BARRIER_X	(TANK_DECK_WIDTH-2-1)
#define MIN_BARRIER_Y	2
#define MIN_BARRIER_X	2

#define NUMBER_TANK1		(-1)
#define NUMBER_TANK2		(-2)
#define NUMBER_TANK3		(-4)
#define NUMBER_BULLTE1		(-1)
#define NUMBER_BULLTE2		(-2)
#define NUMBER_BULLTE3		(-4)

#define PAINT_TANK		'*'
#define PAINT_BULLTE	'*'
#define PAINT_BARRIER	'*'	

#define MANUAL_TANK_NUM		3
#define SPEED_DELAY_TIME (50*1000)	//50ms
#define SPEED_MIN_LEVEL			1
#define SPEED_MAX_LEVEL			20

#define KEY_ESC  			27

#if 0	//just for test
#define TIME_MIN_UNIT	100000 //us
#define TIME_MAX_DELAY	1200000 //us
#endif 

#define TRUE 	1
#define FALSE	0
typedef int BOOL;

enum color{
	COLOR_SCREEN=1,
		
	COLOR_TANK_BLUE,
	COLOR_TANK_GREEN,
	COLOR_TANK1,//Manual control (GREEN1~GREEN3)
	COLOR_TANK2,
	COLOR_TANK3,
	
	COLOR_BULLET_BLUE,
	COLOR_BULLET_GREEN,
	COLOR_BULLET1,//Manual control (GREEN1~GREEN3)
	COLOR_BULLET2,
	COLOR_BULLET3,
	
	COLOR_BARRIER,
	
	COLOR_NORMAL_PROMPT,
	COLOR_ERROR_PROMPT,
};

typedef struct screen{
	WINDOW *win;
	int nlines;
	int ncols;
	int begin_y;
	int begin_x;
}screen_t;

typedef enum object{
	OBJECT_TANK=0,
	OBJECT_BULLET,
	OBJECT_BARRIER,
}object_t;

typedef struct coordinate{
	int y;
	int x;		
}coordinate_t;//The object coordinates (y, x)

typedef struct size{
	int h;	//the height of the object
	int w;	//the width of the object
}siz_t;	

typedef enum dir{
	DIR_UP=0,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_NONE,
}dir_t;

typedef enum standpoint{
	STANDPOINT_WHITE,	//Neutral side
	STANDPOINT_GREEN,	//Blue side
	STANDPOINT_BLUE,	//Green side
}standpoint_t;

typedef struct object_type{
	object_t object;	//To determine the type of object
	coordinate_t coordinate;
	siz_t size;
	dir_t dir;
	standpoint_t standpoint;
	int hp;				//Hit Point Health Points
	int number;			//Object No.
	BOOL canmove;		//Judge whether the object can move
	struct object_type *next;
}object_type_t;

typedef enum status{
	STATUS_INIT=0,
	STATUS_START,
	STATUS_PAUSE,
	STATUS_QUIT,
}status_t;

typedef struct tank_battle{
	screen_t screen;		//the background of tank battle
	object_type_t *tank;	//The head pointer points to tank
	object_type_t *bullet;	//The head pointer points to bullet
	object_type_t *barrier;	//The head pointer points to barrier
	BOOL manual_tank[MANUAL_TANK_NUM];//manual tank(which number is NUMBER_TANK_GREEN1 to NUMBER_TANK_GREEN3)
	int side_blue;			//Blue side of tank number
	int side_green;			//Green side of tank number
	int speed;				//the move speed of the tank()
	status_t status;		//the status of tank battle
}tank_battle_t;

typedef struct tank_model{
	int y_off;
	int x_off;	
}tank_model_t;		//The offset relative to the coordinate(y,x)

typedef tank_model_t tank_head_yx_t;//The next coordinate offset of the tank head
extern tank_model_t tk_model[TANK_MODEL_NUM][TANK_SPACE_NUM];

/**
 *@brief Init tank battle 
 *@param [in/out] tank_battle:Pass resources parameters
 *@return TB_SUCCESS/TB_FAILED 
 *@author handsomezhou  
 *@date 2013-08-12
 */
int init_tank_battle(tank_battle_t **tank_battle);

/**
 *@brief Release tank battle resources
 *@param [in/out] tank_battle:Pass resources parameters
 *@return TB_SUCCESS/TB_FAILED 
 *@author handsomezhou  
 *@date 2013-08-12
 */
void exit_tank_battle(tank_battle_t *tank_battle);


//basic function
coordinate_t *coordinate_copy(coordinate_t *dest,const coordinate_t *src);
siz_t *size_copy(siz_t *dest,const siz_t *src);
object_type_t *object_type_copy(object_type_t *dest,const object_type_t *src);
#endif	/*TANK_BATTLE_H*/

