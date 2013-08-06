/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#ifndef TANK_BATTLE_H
#define TANK_BATTLE_H

#define TB_SUCCESS		0
#define TB_FAILED		-1

#define TANK_HEIGHT		3
#define TANK_WIDTH		3
#define TANK_DECK_HEIGHT	(TANK_HEIGHT*8+2)
#define TANK_DECK_WIDTH		(TANK_WIDTH*8+2)

#define SCREEN_HEIGHT	(TANK_DECK_HEIGHT+6)
#define SCREEN_WIDTH	(TANK_DECK_WIDTH)

#define PAINT_TANK		'*'
#define PAINT_BULLTE	'*'
#define PAINT_BARRIER	'*'	

#define TRUE 	1
#define FALSE	0
typedef int BOOL;

typedef struct screen{
	WINDOW *win;
	int nlines;
	int ncols;
	int begin_y;
	int begin_x;
}screen_t;

typedef enum object{
	TANK=0,
	BULLET,
	BARRIER,
}object_t;

typedef enum standpoint{
	STANDPOINT_WHITE,	//means neutral
	STANDPOINT_GREEN,	//
	STANDPOINT_BLUE,	//
}standpoint_t;

typedef enum dir{
	UP=0,
	RIGHT,
	DOWN,
	LEFT,
	NONE,
}dir_t;

typedef enum status{
	STATUS_INIT=0,
	STATUS_START,
	STATUS_PAUSE,
	STATUS_QUIT,
}status_t;

typedef struct tank_battle{
	screen_t screen;
	
	status_t status;
}tank_battle_t;

int init_tank_battle(tank_battle_t **tank_battle);
void exit_tank_battle(tank_battle_t *tank_battle);

#endif	/*TANK_BATTLE_H*/

