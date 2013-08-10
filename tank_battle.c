/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include <stdlib.h>
#include "tank_battle.h"
#include "handle_tank_battle.h"

#if 0
char tank_model[TANK_MODEL_NUM][TANK_HEIGHT][TANK_WIDTH]=
{					    (y,x):tank range(y,x)~(y+h-1,x+w-1)
	{{'0','1','0'},		UP:(y,x)-(y,x+2)-(y+2,x+1)
	 {'1','1','1'},
	 {'1','0','1'},},

	{{'1','1','0'},		RIGHT:(y,x+2)-(y+2,x+2)-(y+1,x)
	 {'0','1','1'},
	 {'1','1','0'},},

	{{'1','0','1'},		DOWN:(y+2,x+2)-(y+2,x)-(y,x+1)
	 {'1','1','1'},
	 {'0','1','0'},},

	{{'0','1','1'},		LEFT:(y+2,x)-(y,x)-(y+1,x+2)
	 {'1','1','0'},
	 {'0','1','1'},},
};
#endif
tank_model_t tk_model[TANK_MODEL_NUM][TANK_SPACE_NUM]={
	{{0,0},{0,2},{2,1}},
	{{0,2},{2,2},{1,0}},
	{{2,2},{2,0},{0,1}},
	{{2,0},{0,0},{1,2}},
};

static int init_screen(screen_t *screen);
static void exit_screen(screen_t *screen);
static object_type_t *init_object_type(object_type_t **obj_type, object_t object);
static void exit_object_type(object_type_t *object_type);
int init_tank_battle(tank_battle_t **tank_battle)
{
	tank_battle_t **tb=tank_battle;
	int ret=TB_FAILED;

	if(NULL!=*tb){
		exit_tank_battle(*tb);
		*tb=NULL;
	}
	int i=0;
	*tb=(tank_battle_t *)malloc(sizeof(tank_battle_t));
	do{
		if(NULL==*tb){
			break;
		}
		(*tb)->tank=NULL;
		(*tb)->bullet=NULL;
		(*tb)->barrier=NULL;
		for(i=0; i<MANUAL_TANK_NUM; i++){
			(*tb)->manual_tank[i]=FALSE;
		}
		
		(*tb)->side_blue=TANK_BLUE_NUM;
		(*tb)->side_green=TANK_GREEN_NUM;
		(*tb)->speed=SPEED_DELAY_TIME;
		(*tb)->status=STATUS_INIT;
		init_object_type(&(*tb)->tank,OBJECT_TANK);
		init_object_type(&(*tb)->bullet,OBJECT_BULLET);
		init_object_type(&(*tb)->barrier,OBJECT_BARRIER);
		ret=init_screen(&(*tb)->screen);
		if(TB_FAILED==ret){
			break;
		}

		//just for test
		//1<=x<<TANK_DECK_WIDTH-3, 1<=TANK_DECK_HEIGHT-4

		coordinate_t coordinate={.y=1+1,.x=1+1};
#if 0

		add_object(coordinate,OBJECT_TANK,DIR_UP,STANDPOINT_BLUE,(*tb)->tank);

		coordinate.y=TANK_DECK_HEIGHT-3-1;
		coordinate.x=1;
		add_object(coordinate,OBJECT_TANK,DIR_UP,STANDPOINT_BLUE,(*tb)->tank);

		coordinate.y=1;
		coordinate.x=TANK_DECK_WIDTH-3-1;
		add_object(coordinate,OBJECT_TANK,DIR_UP,STANDPOINT_GREEN,(*tb)->tank);


		coordinate.y=TANK_DECK_HEIGHT-3-1;
		coordinate.x=TANK_DECK_WIDTH-3-1;
		add_object(coordinate,OBJECT_TANK,DIR_UP,STANDPOINT_GREEN,(*tb)->tank);
#endif
		int number=0;
		coordinate.y=1;
		coordinate.x=1;
		add_object(coordinate,OBJECT_BULLET,DIR_UP,STANDPOINT_BLUE,number,(*tb)->bullet);
		coordinate.y=1;
		coordinate.x=TANK_DECK_WIDTH-2;
		add_object(coordinate,OBJECT_BULLET,DIR_UP,STANDPOINT_BLUE,number,(*tb)->bullet);
		coordinate.y=TANK_DECK_HEIGHT-2;
		coordinate.x=1;
		add_object(coordinate,OBJECT_BULLET,DIR_UP,STANDPOINT_BLUE,number,(*tb)->bullet);
		coordinate.y=9;
		coordinate.x=8;
		add_object(coordinate,OBJECT_BULLET,DIR_UP,STANDPOINT_GREEN,number,(*tb)->bullet);
		coordinate.y=8;
		coordinate.x=8;
		add_object(coordinate,OBJECT_BULLET,DIR_UP,STANDPOINT_GREEN,number,(*tb)->bullet);
		coordinate.y=TANK_DECK_HEIGHT-2;
		coordinate.x=TANK_DECK_WIDTH-2;
		add_object(coordinate,OBJECT_BULLET,DIR_UP,STANDPOINT_BLUE,number,(*tb)->bullet);
#if 0
		coordinate.y=2;
		coordinate.x=2;
		add_object(coordinate,OBJECT_BARRIER,DIR_NONE,STANDPOINT_WHITE,(*tb)->barrier);

		coordinate.y=2;
		coordinate.x=TANK_DECK_WIDTH-2-1;
		add_object(coordinate,OBJECT_BARRIER,DIR_NONE,STANDPOINT_WHITE,(*tb)->barrier);

		coordinate.y=TANK_DECK_HEIGHT-2-1;
		coordinate.x=2;
		add_object(coordinate,OBJECT_BARRIER,DIR_NONE,STANDPOINT_WHITE,(*tb)->barrier);

		coordinate.y=TANK_DECK_HEIGHT-2-1;
		coordinate.x=TANK_DECK_WIDTH-2-1;
		add_object(coordinate,OBJECT_BARRIER,DIR_NONE,STANDPOINT_WHITE,(*tb)->barrier);
#endif
#if 1
		int n;
		dir_t dir;
#if 0
		coordinate.y=8;
		coordinate.x=8;
	    add_object(coordinate,OBJECT_BARRIER,DIR_NONE,STANDPOINT_WHITE,(*tb)->barrier);
		coordinate.y=8;
		coordinate.x=9;
        add_object(coordinate,OBJECT_BARRIER,DIR_NONE,STANDPOINT_WHITE,(*tb)->barrier);
	 	coordinate.y=9;
		coordinate.x=8;
		 add_object(coordinate,OBJECT_BARRIER,DIR_NONE,STANDPOINT_WHITE,(*tb)->barrier);
#endif
		n=10;
		while(n--){
			 new_object_pos(&coordinate,NULL,OBJECT_BARRIER,*tb);
			 add_object(coordinate,OBJECT_BARRIER,DIR_NONE,STANDPOINT_WHITE,number,(*tb)->barrier);
			 //add_barrier(coordinate,tb->barrier);
		}
		n=5;
		while(n){
			new_object_pos(&coordinate,&dir,OBJECT_TANK,*tb);
			add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_BLUE,number,(*tb)->tank);
			n--;
		}
		n=5;
		while(n){
			new_object_pos(&coordinate,&dir,OBJECT_TANK,*tb);
			add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_GREEN,number,(*tb)->tank);
			n--;
		}
		n=6;
		number=0;
		while(n){
			new_object_pos(&coordinate,&dir,OBJECT_TANK,*tb);
			add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_GREEN,--number,(*tb)->tank);
			n--;
		}
		
#if 0

			coordinate.y=9;
			coordinate.x=10;
			dir=DIR_RIGHT;
			add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_BLUE,(*tb)->tank);
			//new_object_pos(&coordinate,&dir,OBJECT_TANK,*tb);
			coordinate.y=13;
			coordinate.x=12;
			dir=DIR_DOWN;
			add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_BLUE,(*tb)->tank);
			
			n--;
		//}
#endif
#endif
		return TB_SUCCESS;
	}while(0);
	
	return TB_FAILED;
}

void exit_tank_battle(tank_battle_t *tank_battle)
{
	tank_battle_t *tb=tank_battle;
	if(NULL==tb){
		return;
	}

	exit_screen(&tb->screen);
	exit_object_type(tb->barrier);
	exit_object_type(tb->bullet);
	exit_object_type(tb->tank);
	free(tb);
	
	return;
}

static int init_screen(screen_t *screen)
{
	screen_t *scr=screen;
	int scr_y;
	int scr_x;

	if(NULL==scr){
		return TB_FAILED;
	}
	
	//init ncurses
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	clear();
	if(has_colors()){
		start_color();

		init_pair(COLOR_SCREEN,COLOR_MAGENTA,COLOR_BLACK);

		init_pair(COLOR_TANK_BLUE,COLOR_BLUE,COLOR_BLACK);
		init_pair(COLOR_TANK_GREEN,COLOR_GREEN,COLOR_BLACK);
		init_pair(COLOR_TANK_GREEN1,COLOR_YELLOW,COLOR_BLACK);
		init_pair(COLOR_TANK_GREEN2,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(COLOR_TANK_GREEN3,COLOR_WHITE,COLOR_BLACK);
		
		
		init_pair(COLOR_BULLET_BLUE,COLOR_BLUE,COLOR_BLACK);
		init_pair(COLOR_BULLET_GREEN,COLOR_GREEN,COLOR_BLACK);
		init_pair(COLOR_BULLET_GREEN1,COLOR_YELLOW,COLOR_BLACK);
		init_pair(COLOR_BULLET_GREEN2,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(COLOR_BULLET_GREEN3,COLOR_WHITE,COLOR_BLACK);

		init_pair(COLOR_BARRIER,COLOR_YELLOW,COLOR_BLACK);

		init_pair(COLOR_NORMAL_PROMPT,COLOR_GREEN,COLOR_BLACK);
		init_pair(COLOR_ERROR_PROMPT,COLOR_RED,COLOR_WHITE);
	}
	getmaxyx(stdscr,scr_y,scr_x);
	scr->win=stdscr;
	scr->nlines=SCREEN_HEIGHT;
	scr->ncols=SCREEN_WIDTH;
	scr->begin_y=(scr_y-scr->nlines)/2;
	scr->begin_x=(scr_x-scr->ncols)/2;
	wrefresh(scr->win);
	keypad(scr->win,TRUE);
	nodelay(stdscr,TRUE);
#if 0
	//just for test
	nodelay(stdscr,FALSE);
	box(stdscr,0,0);
	wrefresh(stdscr);
	int ch=wgetch(stdscr);
	mvwprintw(stdscr,1,1,"%d",ch);
	ch=wgetch(stdscr);
	nodelay(stdscr,TRUE);
#endif
	return TB_SUCCESS;
}

static void exit_screen(screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return;
	}
	werase(scr->win);
	endwin();
	
	return;
}

static object_type_t *init_object_type(object_type_t **obj_type, object_t object)
{
	if(NULL!=*obj_type){
		exit_object_type(*obj_type);
		*obj_type=NULL;
	}

	*obj_type=(object_type_t *)malloc(sizeof(object_type_t));
	do{
		if(NULL==*obj_type){
			printf("Failed to allocate space for the pointer of object_type_t!\n");
			break;
		}
		(*obj_type)->object=object;
		(*obj_type)->coordinate.y=0;
		(*obj_type)->coordinate.x=0;
		(*obj_type)->size.h=0;
		(*obj_type)->size.w=0;
		(*obj_type)->dir=DIR_NONE;
		(*obj_type)->standpoint=STANDPOINT_WHITE;
		(*obj_type)->hp=0;
		(*obj_type)->number=NUMBER;
		(*obj_type)->canmove=FALSE;
		(*obj_type)->next=NULL;
	}while(0);

	return *obj_type;
}

static void exit_object_type(object_type_t *object_type)
{
	object_type_t *ot=object_type;
	
	object_type_t *cur=ot;
	object_type_t *tmp=NULL;

	while(NULL!=cur){
		tmp=cur->next;
		free(cur);
		cur=tmp;
	}
	
	return ;
}


