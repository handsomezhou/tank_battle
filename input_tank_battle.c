/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include <unistd.h>
#include "input_tank_battle.h"
#include "handle_tank_battle.h"

void *input_tank_battle(void *arg)
{
	tank_battle_t *tb=(tank_battle_t *)arg;
	if(NULL==tb){
		return tb;
	}
	int ch=-1;
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
						cur=tb->tank->next;
						while(NULL!=cur){
							if(cur->number==NUMBER_TANK_GREEN1){
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
								if(DIR_UP==tk1->dir){
									move_tank(tk1);
								}else{
									rotate=can_rotate_direction(DIR_UP,tk1,tb);
									if(TRUE==rotate){
										rotate_direction(DIR_UP,tk1);
										tk1->canmove=TRUE;
									}
								}
								break;
							case 'a':
							case 'A':
								if(DIR_LEFT==tk1->dir){
									move_tank(tk1);
								}else{
									rotate=can_rotate_direction(DIR_LEFT,tk1,tb);
									if(TRUE==rotate){
										rotate_direction(DIR_LEFT,tk1);
										tk1->canmove=TRUE;
									}
								}
								break;
							case 's':
							case 'S':
								if(DIR_DOWN==tk1->dir){
									move_tank(tk1);
								}else{
									rotate=can_rotate_direction(DIR_DOWN,tk1,tb);
									if(TRUE==rotate){
										rotate_direction(DIR_DOWN,tk1);
										tk1->canmove=TRUE;
									}
								}
								
								break;
							case 'd':
							case 'D':
								if(DIR_RIGHT==tk1->dir){
									move_tank(tk1);
								}else{
									rotate=can_rotate_direction(DIR_RIGHT,tk1,tb);
									if(TRUE==rotate){
										rotate_direction(DIR_RIGHT,tk1);
										tk1->canmove=TRUE;
									}
								}
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
						cur=tb->tank->next;
						while(NULL!=cur){
							if(cur->number==NUMBER_TANK_GREEN2){
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
								tk2->coordinate.y--;
								break;
							case 'j':
							case 'J':
								tk2->coordinate.x--;
								break;
							case 'k':
							case 'K':
								tk2->coordinate.y++;
								break;
							case 'l':
							case 'L':
								tk2->coordinate.x++;
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
						cur=tb->tank->next;
						while(NULL!=cur){
							if(cur->number==NUMBER_TANK_GREEN3){
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
								tk3->coordinate.y--;
								break;
							case KEY_LEFT:
								tk3->coordinate.x--;
								break;
							case KEY_DOWN:
								tk3->coordinate.y++;
								break;
							case KEY_RIGHT:
								tk3->coordinate.x++;
								break;
							case ' ':
								if(STATUS_PAUSE!=tb->status){
									fire(tk3,tb->bullet);
								}
								break;
						}
					}
					break;

				case 'p':
				case 'P':
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
		usleep(tb->speed);
	}
	return tb;
}

