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
#if 0	//just for test
	nodelay(tb->screen.win,FALSE);
	int ch=wgetch(tb->screen.win);
	nodelay(tb->screen.win,TRUE);
#endif
	draw_screen(&(tb->screen));
#if 0	//just for test
		nodelay(tb->screen.win,FALSE);
		box(tb->screen.win,0,0);
		wrefresh(tb->screen.win);
		int ch=wgetch(tb->screen.win);
		mvwprintw(tb->screen.win,1,1,"%d",ch);
		ch=wgetch(tb->screen.win);
		mvwprintw(tb->screen.win,1,1,"%c",'@');
		ch=wgetch(tb->screen.win);
		nodelay(tb->screen.win,TRUE);
#endif

	return ;
}

static void draw_screen(screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return;
	}
	int scr_y,scr_x;
	int y,x;

	getmaxyx(stdscr,scr_y,scr_x);
	scr->begin_y=(scr_y-scr->nlines)/2;
	scr->begin_x=(scr_x-scr->ncols)/2;

	for(y=scr->begin_y; y<scr->begin_y+scr->nlines; y++){
		for(x=scr->begin_x; x<scr->begin_x+scr->ncols; x++){
			if(y==scr->begin_y||y==scr->begin_y+scr->nlines-1||x==scr->begin_x||x==scr->begin_x+TANK_DECK_WIDTH||x==scr->begin_x+scr->ncols-1){
				mvwprintw(scr->win,y,x,"%c",PAINT_BARRIER);
			}
		}
	}
	wrefresh(scr->win);	
	
	return ;
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


