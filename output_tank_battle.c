/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include "output_tank_battle.h"
static void draw_screen(screen_t *screen);
static void clear_screen(screen_t *screen);


void paint_tank_battle(tank_battle_t *tank_battle)
{
	tank_battle_t *tb=tank_battle;
	if(NULL==tb){
		return;
	}
	clear_screen(&(tb->screen));
	draw_screen(&(tb->screen));
	
	return ;
}

static void draw_screen(screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return;
	}
	int scr_y,scr_x;

	getmaxyx(stdscr,scr_y,scr_x);
	scr->begin_y=(scr_y-scr->nlines)/2;
	scr->begin_x=(scr_x-scr->ncols)/2;
}

static void clear_screen(screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return;
	}

	werase(scr->win);
	return;
}


