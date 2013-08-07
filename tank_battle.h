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
#define TANK_DECK_WIDTH		(TANK_WIDTH*8+2)

#define SCREEN_HEIGHT	(TANK_DECK_HEIGHT)
#define SCREEN_WIDTH	(TANK_DECK_WIDTH+10)
#define TANK_MODEL_NUM	4
#define TANK_SPACE_NUM	3

#define TANK_BLUE_NUM	6
#define TANK_GREEN_NUM	20

#define TANK_BLUE_FIGHTING	2
#define TANK_GREEN_FIGHTING	5

#define H_TANK		3
#define H_BULLET	1
#define H_BARRIER	1

#define W_TANK		3
#define W_BULLET	1
#define W_BARRIER	1

#define HP_TANK		5
#define HP_BULLET	1
#define HP_BARRIER	0


#define PAINT_TANK		'*'
#define PAINT_BULLTE	'*'
#define PAINT_BARRIER	'*'	

#define SPEED_DELAY_TIME (500*1000)	//500ms

#define TRUE 	1
#define FALSE	0
typedef int BOOL;

enum color{
	COLOR_SCREEN=1,
	COLOR_TANK_BLUE,
	COLOR_TANK_GREEN,
	COLOR_BULLET_BLUE,
	COLOR_BULLET_GREEN,
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
	int side_blue;			//Blue side of tank number
	int side_green;			//Green side of tank number
	int speed;				//the move speed of the tank
	status_t status;		//the status of tank battle
}tank_battle_t;

typedef struct tank_model{
	int y_off;
	int x_off;	
}tank_model_t;		//The offset relative to the coordinate(y,x)

extern tank_model_t tk_model[TANK_MODEL_NUM][TANK_SPACE_NUM];

int init_tank_battle(tank_battle_t **tank_battle);
void exit_tank_battle(tank_battle_t *tank_battle);

#endif	/*TANK_BATTLE_H*/
