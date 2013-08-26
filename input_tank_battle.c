/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include <unistd.h>
#include "input_tank_battle.h"
#include "handle_tank_battle.h"


// just for test 
void show_tank(int y, int x, const object_type_t *tank);
void show_tank(int y, int x, const object_type_t *tank)
{
	const object_type_t *tk=tank;
	mvwprintw(stdscr,y,x,"(%d,%d)(dir:%d)=%d=(cm:%d)=%p=%p",tk->coordinate.y,\
		tk->coordinate.x,tk->dir,tk->number,tk->canmove,tk,tk->next);
	wrefresh(stdscr);
}

void *input_tank_battle(void *arg)
{
	tank_battle_t *tb=(tank_battle_t *)arg;
	if(NULL==tb){
		return tb;
	}
	int ch=-1;
	int tank_num;
	int manual_tank_num;
	coordinate_t coordinate;
	dir_t dir;
	BOOL rotate=FALSE;
	object_type_t *tk1=NULL;
	object_type_t *tk2=NULL;
	object_type_t *tk3=NULL;
	object_type_t *cur=NULL;
	while(STATUS_QUIT!=tb->status){
		ch=-1;
		ch=wgetch(tb->screen.win);
		if(ch!=-1){
			switch(ch){
				case 'w':
				case 'W':
				case 'a':
				case 'A':
				case 's':
				case 'S':
				case 'd':
				case 'D':
				case 'e':
				case 'E':
					if(tb->manual_tank[0]==FALSE){
						tk1=NULL;
						cur=tb->tank->next;
						while(NULL!=cur){
							if(cur->number==NUMBER_TANK1){
								tk1=cur;
								tb->manual_tank[0]=TRUE;
								break;
							}
							cur=cur->next;
						}
					}
					
					if(tb->manual_tank[0]==TRUE){
						switch(ch){
							case 'w':
							case 'W':
								deal_manual_tank_collision(DIR_UP,tk1,tb);
								break;
							case 'a':
							case 'A':
								deal_manual_tank_collision(DIR_LEFT,tk1,tb);
								break;
							case 's':
							case 'S':
								deal_manual_tank_collision(DIR_DOWN,tk1,tb);
								break;
							case 'd':
							case 'D':
								deal_manual_tank_collision(DIR_RIGHT,tk1,tb);
								break;
							case 'e':
							case 'E':
								if(STATUS_PAUSE!=tb->status){
									fire(tk1,tb->bullet);
								}
								break;
						}
					}
					break;

				case 'i':
				case 'I':
				case 'j':
				case 'J':
				case 'k':
				case 'K':
				case 'l':
				case 'L':
				case 'u':
				case 'U':
					if(tb->manual_tank[1]==FALSE){
						tk2=NULL;
						cur=tb->tank->next;
						while(NULL!=cur){
							if(cur->number==NUMBER_TANK2){
								tk2=cur;
								tb->manual_tank[1]=TRUE;
								break;
							}
							cur=cur->next;
						}
					}

					if(tb->manual_tank[1]==TRUE){
						switch(ch){
							case 'i':
							case 'I':
								deal_manual_tank_collision(DIR_UP,tk2,tb);
								break;
							case 'j':
							case 'J':
								deal_manual_tank_collision(DIR_LEFT,tk2,tb);
								break;
							case 'k':
							case 'K':
								deal_manual_tank_collision(DIR_DOWN,tk2,tb);
								break;
							case 'l':
							case 'L':
								deal_manual_tank_collision(DIR_RIGHT,tk2,tb);
								break;
							case 'u':
							case 'U':
								if(STATUS_PAUSE!=tb->status){
									fire(tk2,tb->bullet);
								}
								break;
						}
					}
					break;

				case KEY_UP:
				case KEY_LEFT:
				case KEY_DOWN:
				case KEY_RIGHT:
				case ' ':
					if(tb->manual_tank[2]==FALSE){
						tk3=NULL;
						cur=tb->tank->next;
						while(NULL!=cur){
							if(cur->number==NUMBER_TANK3){
								tk3=cur;
								tb->manual_tank[2]=TRUE;
								break;
							}
							cur=cur->next;
						}
					}

					if(tb->manual_tank[2]==TRUE){
						switch(ch){
							case KEY_UP:
								deal_manual_tank_collision(DIR_UP,tk3,tb);
								break;
							case KEY_LEFT:
								deal_manual_tank_collision(DIR_LEFT,tk3,tb);
								break;
							case KEY_DOWN:
								deal_manual_tank_collision(DIR_DOWN,tk3,tb);
								break;
							case KEY_RIGHT:
								deal_manual_tank_collision(DIR_RIGHT,tk3,tb);
								break;
							case ' ':
								if(STATUS_PAUSE!=tb->status){
									fire(tk3,tb->bullet);
								}
								break;
						}
					}
					break;

				case 'p'://pause
				case 'P':
					if(tb->status!=STATUS_PAUSE){
						tb->status=STATUS_PAUSE;
					}else{
						tb->status=STATUS_START;
					}
					break;
				
				case '+'://speed up
					if(tb->speed<SPEED_MAX_LEVEL){
						tb->speed++;
					}else{
						tb->speed=SPEED_MAX_LEVEL;
					}
					break;
				case '-'://slow down
					if(tb->speed>SPEED_MIN_LEVEL){
						tb->speed--;
					}else{
						tb->speed=SPEED_MIN_LEVEL;
					}
					break;

				case 'G'://add green manual tank
					//break;
				case 'g'://and green automatic tank
					tank_num=get_tank_num(STANDPOINT_GREEN,tb->tank);
					if(tank_num<GREEN_MAX_FIGHTING_TANK&&tb->side_blue>=GREEN_MIN_FIGHTING_TANK){
						if('g'==ch){
							new_object_pos(&coordinate,&dir,OBJECT_TANK,tb);
							add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_GREEN,0,tb->tank);
						}else{
							manual_tank_num=get_manual_tank_num(tb->tank);
							if(manual_tank_num!=(NUMBER_TANK1+NUMBER_TANK2+NUMBER_TANK3)){
								new_object_pos(&coordinate,&dir,OBJECT_TANK,tb);
								if((-manual_tank_num)&(-NUMBER_TANK1)==0){
									add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_GREEN,NUMBER_TANK1,tb->tank);
								}else if((-manual_tank_num)&(-NUMBER_TANK2)==0){
									add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_GREEN,NUMBER_TANK2,tb->tank);
								}else{
									add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_GREEN,NUMBER_TANK3,tb->tank);
								}
							}
							
						}
						
					}
					
					break;
				case 'B'://add blue manual tank
					//break;
				case 'b'://add blue automatic tank
					tank_num=get_tank_num(STANDPOINT_BLUE,tb->tank);
					if(tank_num<BLUE_MAX_FIGHTING_TANK&&tb->side_blue>=BLUE_MIN_FIGHTING_TANK){
						new_object_pos(&coordinate,&dir,OBJECT_TANK,tb);
						add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_BLUE,0,tb->tank);
					}
					break;
				case 'q':
				case 'Q':
				case KEY_ESC:
					tb->status=STATUS_QUIT;
					break;
				default:
					break;
			}
		}
		usleep(SPEED_DELAY_TIME);
	}
	return tb;
}

