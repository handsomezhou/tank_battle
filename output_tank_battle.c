/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include "output_tank_battle.h"
static void open_colors(object_t object,standpoint_t standpoint,int number);
static void close_colors(object_t object,standpoint_t standpoint,int number);
static void draw_screen(screen_t *screen);
static void clear_screen(screen_t *screen);
static void draw_tank(object_type_t *tank,const screen_t *screen);
static void draw_bullet(object_type_t *bullet,const screen_t *screen);
static void draw_barrier(object_type_t *barrier,const screen_t *screen);


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
	draw_tank(tb->tank,&(tb->screen));
	draw_bullet(tb->bullet,&(tb->screen));
	draw_barrier(tb->barrier,&(tb->screen));
	
	
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

static void open_colors(object_t object,standpoint_t standpoint,int number)
{
	switch(object){
		case OBJECT_TANK:
			switch(standpoint){
				case STANDPOINT_BLUE:
					switch(number){
						case NUMBER_TANK1:
							if(has_colors()){attron(COLOR_PAIR(COLOR_TANK1)|A_BOLD);}
							break;
						case NUMBER_TANK2:
							if(has_colors()){attron(COLOR_PAIR(COLOR_TANK2)|A_BOLD);}
							break;
						case NUMBER_TANK3:
							if(has_colors()){attron(COLOR_PAIR(COLOR_TANK3)|A_BOLD);}
							break;
						default:
							if(has_colors()){attron(COLOR_PAIR(COLOR_TANK_BLUE)|A_BOLD);}
							break;
					}
					break;
				case STANDPOINT_GREEN:
					switch(number){
						case NUMBER_TANK1:
							if(has_colors()){attron(COLOR_PAIR(COLOR_TANK1)|A_BOLD);}
							break;
						case NUMBER_TANK2:
							if(has_colors()){attron(COLOR_PAIR(COLOR_TANK2)|A_BOLD);}
							break;
						case NUMBER_TANK3:
							if(has_colors()){attron(COLOR_PAIR(COLOR_TANK3)|A_BOLD);}
							break;
						default:
							if(has_colors()){attron(COLOR_PAIR(COLOR_TANK_GREEN)|A_BOLD);}
							break;
					}
					break;
				default:
					break;

			}
			break;
		case OBJECT_BULLET:
			switch(standpoint){
				case STANDPOINT_BLUE:
					if(has_colors()){attron(COLOR_PAIR(COLOR_BULLET_BLUE)|A_BOLD);}
					break;
				case STANDPOINT_GREEN:
					switch(number){
						case NUMBER_BULLTE1:
							if(has_colors()){attron(COLOR_PAIR(COLOR_BULLET1)|A_BOLD);}
							break;
						case NUMBER_BULLTE2:
							if(has_colors()){attron(COLOR_PAIR(COLOR_BULLET2)|A_BOLD);}
							break;
						case NUMBER_BULLTE3:
							if(has_colors()){attron(COLOR_PAIR(COLOR_BULLET3)|A_BOLD);}
							break;
						default:
							if(has_colors()){attron(COLOR_PAIR(COLOR_BULLET_GREEN)|A_BOLD);}
							break;
					}
					break;
				default:
					break;

			}
			break;
		case OBJECT_BARRIER:
			switch(standpoint){
				case STANDPOINT_WHITE:
					if(has_colors()){attron(COLOR_PAIR(COLOR_BARRIER)|A_BOLD);}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

	return;
}

static void close_colors(object_t object, standpoint_t standpoint,int number)
{
		switch(object){
		case OBJECT_TANK:
			switch(standpoint){
				case STANDPOINT_BLUE:
					switch(number){
						case NUMBER_TANK1:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_TANK1)|A_BOLD);}
							break;
						case NUMBER_TANK2:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_TANK2)|A_BOLD);}
							break;
						case NUMBER_TANK3:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_TANK3)|A_BOLD);}
							break;
						default:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_TANK_GREEN)|A_BOLD);}
							break;
					}
					break;
				case STANDPOINT_GREEN:
					switch(number){
						case NUMBER_TANK1:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_TANK1)|A_BOLD);}
							break;
						case NUMBER_TANK2:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_TANK2)|A_BOLD);}
							break;
						case NUMBER_TANK3:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_TANK3)|A_BOLD);}
							break;
						default:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_TANK_GREEN)|A_BOLD);}
							break;
					}
					break;
				default:
					break;

			}
			break;
		case OBJECT_BULLET:
			switch(standpoint){
				case STANDPOINT_BLUE:
					if(has_colors()){attroff(COLOR_PAIR(COLOR_BULLET_BLUE)|A_BOLD);}
					break;
				case STANDPOINT_GREEN:
					switch(number){
						case NUMBER_BULLTE1:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_BULLET1)|A_BOLD);}
							break;
						case NUMBER_BULLTE2:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_BULLET2)|A_BOLD);}
							break;
						case NUMBER_BULLTE3:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_BULLET3)|A_BOLD);}
							break;
						default:
							if(has_colors()){attroff(COLOR_PAIR(COLOR_BULLET_BLUE)|A_BOLD);}
							break;
						
					}
					break;
				default:
					break;

			}
			break;
		case OBJECT_BARRIER:
			switch(standpoint){
				case STANDPOINT_WHITE:
					if(has_colors()){attroff(COLOR_PAIR(COLOR_BARRIER)|A_BOLD);}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

	return;
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

	if(has_colors()){attron(COLOR_PAIR(COLOR_SCREEN)|A_BOLD);}
	for(y=scr->begin_y; y<scr->begin_y+scr->nlines; y++){
		for(x=scr->begin_x; x<scr->begin_x+scr->ncols; x++){
			if(y==scr->begin_y||y==scr->begin_y+scr->nlines-1||x==scr->begin_x||x==scr->begin_x+TANK_DECK_WIDTH-1||x==scr->begin_x+scr->ncols-1){
				mvwprintw(scr->win,y,x,"%c",PAINT_BARRIER);
			}
		}
	}
	if(has_colors()){attroff(COLOR_PAIR(COLOR_SCREEN)|A_BOLD);}
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

static void draw_tank(object_type_t *tank,const screen_t *screen)
{
	object_type_t *ot=tank;
	const screen_t *scr=screen;
	if(NULL==ot||NULL==scr){
		return;
	}
	
	int x=0;
	int y=0;
	int z=0;
	object_type_t *cur=ot->next;
	while(NULL!=cur){
		open_colors(cur->object,cur->standpoint,cur->number);
		for(y=scr->begin_y+cur->coordinate.y; y<scr->begin_y+cur->coordinate.y+cur->size.h; y++){
			for(x=scr->begin_x+cur->coordinate.x; x<scr->begin_x+cur->coordinate.x+cur->size.w; x++){
				if((y==scr->begin_y+cur->coordinate.y+tk_model[cur->dir][z].y_off&&x==scr->begin_x+cur->coordinate.x+tk_model[cur->dir][z].x_off)||\
					(y==scr->begin_y+cur->coordinate.y+tk_model[cur->dir][z+1].y_off&&x==scr->begin_x+cur->coordinate.x+tk_model[cur->dir][z+1].x_off)||\
					(y==scr->begin_y+cur->coordinate.y+tk_model[cur->dir][z+2].y_off&&x==scr->begin_x+cur->coordinate.x+tk_model[cur->dir][z+2].x_off)){
				}else{
					mvwaddch(scr->win,y,x,PAINT_TANK);
				}
			}
		}	
		close_colors(cur->object,cur->standpoint,cur->number);
		
		cur=cur->next;
	}
	wrefresh(scr->win);
	
	return;
}

static void draw_bullet(object_type_t *bullet,const screen_t *screen)
{
	object_type_t *ot=bullet;
	const screen_t *scr=screen;
	if(NULL==ot||NULL==scr){
		return;
	}

	object_type_t *cur=ot->next;
	while(NULL!=cur){
		open_colors(cur->object,cur->standpoint,cur->number);
		mvwaddch(scr->win,scr->begin_y+cur->coordinate.y,scr->begin_x+cur->coordinate.x,PAINT_BULLTE);
		close_colors(cur->object,cur->standpoint,cur->number);

		cur=cur->next;
	}
	wrefresh(scr->win);
		
	return;
}


static void draw_barrier(object_type_t *barrier,const screen_t *screen)
{
	draw_bullet(barrier,screen);
}

