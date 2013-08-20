/**
  *  Copyright (C) 2013  Handsome Zhou
  */
  
#include <stdlib.h>
#include <math.h>
#include "handle_tank_battle.h"

static BOOL new_tank_pos(coordinate_t *coordinate, dir_t *dir,const tank_battle_t *tank_battle);
static BOOL new_barrier_pos(coordinate_t *coordinate,const tank_battle_t *tank_battle);

static object_type_t *add_tank(coordinate_t coordinate,dir_t dir,standpoint_t standpoint,int number,object_type_t *tank);
static object_type_t *add_bullet(coordinate_t coordinate,dir_t dir,standpoint_t standpoint,object_type_t *bullet);
static object_type_t *add_barrier(coordinate_t coordinate,object_type_t *barrier);
static void del_object_type(object_type_t *obj_type,object_type_t *head);

static BOOL is_out_bounds(object_type_t *obj_type);
static BOOL is_tank_out_bounds(coordinate_t coordinate);
static BOOL is_bullet_out_bounds(coordinate_t coordinate);
static BOOL is_barrier_out_bounds(coordinate_t coordinate);

static object_type_t *tank_on_tank(object_type_t *tank1,object_type_t *tank2);
static object_type_t *bullet_on_tank(object_type_t *bullet,object_type_t *tank);
static object_type_t *barrier_on_tank(object_type_t *barrier,object_type_t *tank);

static object_type_t *bullet_on_bullet(object_type_t *bullet1,object_type_t *bullet2);
static object_type_t *barrier_on_bullet(object_type_t *barrier,object_type_t *bullet);

static object_type_t *barrier_on_barrier(object_type_t *barrier1,object_type_t *barrier2);

//tank:Moved after the first judgment
static object_type_t *deal_automatic_tank_collision(object_type_t *tank,tank_battle_t *tank_battle);
static void deal_automatic_tank_motion(object_type_t *tank,tank_battle_t *tank_battle,BOOL random);
object_type_t *deal_manual_tank_collision(dir_t direction,object_type_t *tank,tank_battle_t *tank_battle);
static tank_head_yx_t get_tank_head_yxoff(const object_type_t *tank);
static BOOL get_tank_next_pos(object_type_t *next_pos,const object_type_t *tank);
BOOL can_rotate_direction(dir_t direction,object_type_t *tank,tank_battle_t *tank_battle);
object_type_t *rotate_direction(dir_t direction,object_type_t *tank);
BOOL can_move_tank(object_type_t *tank,tank_battle_t *tank_battle);
object_type_t *move_tank(object_type_t *tank);
static BOOL is_fire(int speed);
object_type_t *fire(object_type_t *tank,object_type_t *bullet);
static BOOL is_change_direction(int seed);
static dir_t random_direction(void);


static object_type_t *deal_tank_to_bounds(object_type_t *tank);
static object_type_t *deal_tank_to_tank(object_type_t *tank,object_type_t *head_tank);
static object_type_t *deal_tank_to_bullet(object_type_t *tank,object_type_t *head_bullet);
static object_type_t *deal_tank_to_barrier(object_type_t *tank,object_type_t *head_barrier);
static int get_tank_num(standpoint_t standpoint,const object_type_t *head_tank);

//bullet:First move after the judge
static object_type_t *move_all_bullet(object_type_t *bullet);
static object_type_t *deal_all_bullet_collision(object_type_t *bullet,tank_battle_t *tank_battle);
static object_type_t *deal_bullet_to_tank(object_type_t *bullet,object_type_t *head_tank,tank_battle_t *tank_battle);
static object_type_t *deal_bullet_to_bullet(object_type_t *bullet,object_type_t *head_bullet);
static object_type_t *deal_bullet_to_barrier(object_type_t *bullet,object_type_t *head_barrier);

int handle_tank_battle(tank_battle_t *tank_battle)
{
	tank_battle_t *tb=tank_battle;

	do{
		if(NULL==tb){
			break;
		}
		if(NULL==tb->bullet||NULL==tb->tank){
			break;
		}
#if 1
		//deal bullet event
		object_type_t *bt=NULL;
		bt=move_all_bullet(tb->bullet);
		if(NULL!=bt){
			deal_all_bullet_collision(tb->bullet,tb);
		}else{//just for test
			mvwprintw(stdscr,1,1,"%s","hihi");
			wrefresh(stdscr);
		}

#endif
		
		//deal tank event
		deal_automatic_tank_collision(tb->tank,tb);
		
	}while(0);
	return TB_FAILED;
}

BOOL new_object_pos(coordinate_t *coordinate, dir_t *dir,object_t object,const tank_battle_t *tank_battle)
{
	const tank_battle_t *tb=tank_battle;
	if(NULL==tb||NULL==coordinate){
		return FALSE;
	}

	switch(object){
		case OBJECT_TANK:
			if(dir==NULL){
				return FALSE;
			}
			new_tank_pos(coordinate,dir,tb);
			break;
		case OBJECT_BULLET:
			break;
		case OBJECT_BARRIER:
			new_barrier_pos(coordinate,tb);
			break;
	}
	
	return TRUE;
}

object_type_t *add_object(coordinate_t coordinate,object_t object,dir_t dir,standpoint_t standpoint,int number,object_type_t *object_type)
{
	object_type_t *ot=object_type;
	object_type_t *ret=NULL;

	do{
		if(NULL==ot){
			break;
		}

		switch(object){
			case OBJECT_TANK:
				ret=add_tank(coordinate,dir,standpoint,number,ot);
				break;
			case OBJECT_BULLET:
				ret=add_bullet(coordinate,dir,standpoint,ot);
				break;
			case OBJECT_BARRIER:
				ret=add_barrier(coordinate,ot);
				break;
		}
	}while(0);
	
	return ret;
}

static BOOL new_tank_pos(coordinate_t *coordinate, dir_t *dir,const tank_battle_t *tank_battle)
{
	const tank_battle_t *tb=tank_battle;
	if(NULL==tb||NULL==dir||NULL==coordinate){
		return FALSE;
	}

	object_type_t *tk=tb->tank;
	object_type_t *bt=tb->bullet;
	object_type_t *br=tb->barrier;
	if(NULL==tk||NULL==bt||NULL==br){
		return FALSE;
	}
	
	object_type_t *cur_tk=NULL;
	object_type_t *tmp_tk=NULL;
	object_type_t *cur_bt=NULL;
	object_type_t *tmp_bt=NULL;
	object_type_t *cur_br=NULL;
	object_type_t *tmp_br=NULL;
	object_type_t tank;
	tank.size.h=H_TANK;
	tank.size.w=W_TANK;
	
	do{
		
		tank.coordinate.y=rand()%(MAX_TANK_Y-MIN_TANK_Y+1)+MIN_TANK_Y;
		tank.coordinate.x=rand()%(MAX_TANK_X-MIN_TANK_X+1)+MIN_TANK_X;
		tank.dir=rand()%(TANK_MODEL_NUM);	//4-0
		
		cur_tk=tk->next;
		while(NULL!=cur_tk){
			tmp_tk=tank_on_tank(&tank,cur_tk);
			if(NULL!=tmp_tk){
				break;
			}
			cur_tk=cur_tk->next;
		}

		if(NULL!=tmp_tk){
			continue;
		}

		cur_bt=bt->next;
		while(NULL!=cur_bt){
			tmp_bt=bullet_on_tank(cur_bt,&tank);
			if(NULL!=tmp_bt){
				break;
			}
			cur_bt=cur_bt->next;
		}
		if(NULL!=tmp_bt){
			continue;
		}

		cur_br=br->next;
		while(NULL!=cur_br){
			tmp_br=barrier_on_tank(cur_br,&tank);
			if(NULL!=tmp_br){
				break;
			}
			cur_br=cur_br->next;
		}
		if(NULL!=tmp_br){
			continue;
		}
		
	}while(NULL!=tmp_tk||NULL!=tmp_br);

	coordinate->y=tank.coordinate.y;
	coordinate->x=tank.coordinate.x;
	*dir=tank.dir;
	return TRUE;
}

static BOOL new_barrier_pos(coordinate_t *coordinate,const tank_battle_t *tank_battle)
{
	const tank_battle_t *tb=tank_battle;
	if(NULL==tb||NULL==coordinate){
		return FALSE;
	}

	object_type_t *tk=tb->tank;
	object_type_t *bt=tb->bullet;
	object_type_t *br=tb->barrier;
	if(NULL==tk||NULL==bt||NULL==br){
		return FALSE;
	}
	
	object_type_t *cur_tk=NULL;
	object_type_t *tmp_tk=NULL;
	object_type_t *cur_bt=NULL;
	object_type_t *tmp_bt=NULL;
	object_type_t *cur_br=NULL;
	object_type_t *tmp_br=NULL;
	object_type_t barrier;
	
	do{
		barrier.coordinate.y=rand()%(MAX_BARRIER_Y-MIN_BARRIER_Y+1)+MIN_BARRIER_Y;
		barrier.coordinate.x=rand()%(MAX_BARRIER_X-MIN_BARRIER_X+1)+MIN_BARRIER_X;

		cur_tk=tk->next;
		while(NULL!=cur_tk){
			tmp_tk=barrier_on_tank(&barrier,cur_tk);
			if(NULL!=tmp_tk){
				break;
			}
			cur_tk=cur_tk->next;
		}
		if(NULL!=tmp_tk){
			continue;
		}

		cur_bt=bt->next;
		while(NULL!=cur_bt){
			tmp_bt=barrier_on_bullet(&barrier,cur_bt);
			if(NULL!=tmp_bt){
				break;
			}
			cur_bt=cur_bt->next;
		}
		if(NULL!=tmp_bt){
			continue;
		}

		cur_br=br->next;
		while(NULL!=cur_br){
			tmp_br=barrier_on_barrier(&barrier,cur_br);
			if(NULL!=tmp_br){
				break;
			}
			cur_br=cur_br->next;
		}	
	}while(NULL!=tmp_tk||NULL!=tmp_bt||NULL!=tmp_br);
	
	coordinate->y=barrier.coordinate.y;
	coordinate->x=barrier.coordinate.x;
	
	return TRUE;
}

static object_type_t *add_tank(coordinate_t coordinate,dir_t dir,standpoint_t standpoint,int number,object_type_t *tank)
{
	object_type_t *ot=tank;
	object_type_t *cur=NULL;
	
	do{
		if(NULL==ot){
			break;
		}
		
		cur=ot;
		while(NULL!=cur->next){
			cur=cur->next;
		}

		cur->next=(object_type_t *)malloc(sizeof(object_type_t));
		if(NULL==cur->next){
			printf("Failed to allocate space for the pointer of the tank!\n");
			ot=NULL;
			break;
		}

		cur->next->object=OBJECT_TANK;
		cur->next->coordinate.y=coordinate.y;
		cur->next->coordinate.x=coordinate.x;
		cur->next->size.h=H_TANK;
		cur->next->size.w=W_TANK;
		cur->next->dir=dir;
		cur->next->standpoint=standpoint;
		cur->next->hp=HP_TANK;
		if(number>=0||number<NUMBER_TANK_GREEN3){
			cur->next->number=NUMBER;
		}else if(number>=NUMBER_TANK_GREEN3&&number<=NUMBER_TANK_GREEN1){
			cur->next->number=number;
		}
		
		cur->next->canmove=FALSE;
		cur->next->next=NULL;

		ot=cur->next;
	}while(0);
	
	return ot;
}

static object_type_t *add_bullet(coordinate_t coordinate,dir_t dir,standpoint_t standpoint,object_type_t *bullet)
{
	object_type_t *ot=bullet;
	object_type_t *cur=NULL;
	
	do{
		if(NULL==ot){
			break;
		}
		
		cur=ot;
		while(NULL!=cur->next){
			cur=cur->next;
		}

		cur->next=(object_type_t *)malloc(sizeof(object_type_t));
		if(NULL==cur->next){
			printf("Failed to allocate space for the pointer of the bullet!\n");
			ot=NULL;
			break;
		}

		cur->next->object=OBJECT_BULLET;
		cur->next->coordinate.y=coordinate.y;
		cur->next->coordinate.x=coordinate.x;
		cur->next->size.h=H_BULLET;
		cur->next->size.w=W_BULLET;
		cur->next->dir=dir;
		cur->next->standpoint=standpoint;
		cur->next->hp=HP_BULLET;
		cur->next->number=cur->number+1;
		cur->next->canmove=TRUE;
		cur->next->next=NULL;

		ot=cur->next;
	}while(0);
	
	return ot;
}

static object_type_t *add_barrier(coordinate_t coordinate,object_type_t *barrier)
{
	object_type_t *ot=barrier;
	object_type_t *cur=NULL;
	
	do{
		if(NULL==ot){
			break;
		}
		
		cur=ot;
		while(NULL!=cur->next){
			cur=cur->next;
		}

		cur->next=(object_type_t *)malloc(sizeof(object_type_t));
		if(NULL==cur->next){
			printf("Failed to allocate space for the pointer of the barrier!\n");
			ot=NULL;
			break;
		}

		cur->next->object=OBJECT_BARRIER;
		cur->next->coordinate.y=coordinate.y;
		cur->next->coordinate.x=coordinate.x;
		cur->next->size.h=H_BARRIER;
		cur->next->size.w=W_BARRIER;
		cur->next->dir=DIR_NONE;
		cur->next->standpoint=STANDPOINT_WHITE;
		cur->next->hp=HP_BARRIER;
		cur->next->number=cur->number+1;
		cur->next->canmove=FALSE;
		cur->next->next=NULL;

		ot=cur->next;
	}while(0);
	
	return ot;
}

static void del_object_type(object_type_t *obj_type,object_type_t *head)
{
	object_type_t *ot=obj_type;
	object_type_t *hd=head;
	if(NULL==ot||NULL==hd){
		return;
	}
	object_type_t *cur=hd->next;
	object_type_t *prev=hd;

	while(NULL!=cur){
		if(cur->coordinate.x==ot->coordinate.x&&cur->coordinate.y==ot->coordinate.y){
			prev->next=cur->next;
			free(cur);
			break;
		}
		prev=cur;
		cur=cur->next;
	}
	
	return ;
}

static BOOL is_out_bounds(object_type_t *obj_type)
{
	object_type_t *ot=obj_type;

	if(NULL==ot){
		return TRUE;
	}

	switch(ot->object){
		case OBJECT_TANK:
			if(is_tank_out_bounds(ot->coordinate)==FALSE){
				return FALSE;
			}
			break;
		case OBJECT_BULLET:
			if(is_bullet_out_bounds(ot->coordinate)==FALSE){
				return FALSE;
			}
			break;
		case OBJECT_BARRIER:
			if(is_barrier_out_bounds(ot->coordinate)==FALSE){
				return FALSE;
			}
			break;
	}
	
	return TRUE;
}

static BOOL is_tank_out_bounds(coordinate_t coordinate)
{
	if((coordinate.x>=MIN_TANK_X)&&(coordinate.x<=MAX_TANK_X)&&\
		(coordinate.y>=MIN_TANK_Y)&&(coordinate.y<=MAX_TANK_Y)){
		return FALSE;
	}
	
	return TRUE;
}

static BOOL is_bullet_out_bounds(coordinate_t coordinate)
{
	if((coordinate.x>=MIN_BULLET_X)&&(coordinate.x<=MAX_BULLET_X)&&\
		(coordinate.y>=MIN_BULLET_Y)&&(coordinate.y<=MAX_BULLET_Y)){
		return FALSE;
	}
		
	return TRUE;
}

static BOOL is_barrier_out_bounds(coordinate_t coordinate)
{
	if((coordinate.x>=MIN_BARRIER_X)&&(coordinate.x<=MAX_BARRIER_X)&&\
		(coordinate.y>=MIN_BARRIER_Y)&&(coordinate.y<=MAX_BARRIER_Y)){
		return FALSE;
	}
	
	return TRUE;
}

static object_type_t *tank_on_tank(object_type_t *tank1,object_type_t *tank2)
{
	object_type_t *tk1=tank1;
	object_type_t *tk2=tank2;
	object_type_t *ot=NULL;
	int i=0;
	int j=0;
	int y1=0;
	int x1=0;
	int y2=0;
	int x2=0;
	int z=0;
	int flag_overlap=FALSE;

	do{
		if(NULL==tk1||NULL==tk2){
			break;
		}
		if((fabs(tk1->coordinate.y-tk2->coordinate.y)<TANK_HEIGHT)&&(fabs(tk1->coordinate.x-tk2->coordinate.x)<TANK_WIDTH)){
			if((fabs(tk1->coordinate.y-tk2->coordinate.y)<TANK_HEIGHT-1)&&(fabs(tk1->coordinate.x-tk2->coordinate.x)<TANK_WIDTH-1)){
				ot=tk1;
				break;
			}else{
				//judge whether the two tanks overlap
				for(i=0; i<TANK_LATTICE_NUM; i++){
					y1=tk1->coordinate.y+i/TANK_WIDTH;
					x1=tk1->coordinate.x+i%TANK_WIDTH;
				
					if((y1==tk1->coordinate.y+tk_model[tk1->dir][z].y_off&&x1==tk1->coordinate.x+tk_model[tk1->dir][z].x_off)||\
						(y1==tk1->coordinate.y+tk_model[tk1->dir][z+1].y_off&&x1==tk1->coordinate.x+tk_model[tk1->dir][z+1].x_off)||\
						(y1==tk1->coordinate.y+tk_model[tk1->dir][z+2].y_off&&x1==tk1->coordinate.x+tk_model[tk1->dir][z+2].x_off)){
						continue;
					}
					for(j=0; j<TANK_LATTICE_NUM; j++){
						y2=tk2->coordinate.y+j/TANK_WIDTH;
						x2=tk2->coordinate.x+j%TANK_WIDTH;
						if((y2==tk2->coordinate.y+tk_model[tk2->dir][z].y_off&&x2==tk2->coordinate.x+tk_model[tk2->dir][z].x_off)||\
							(y2==tk2->coordinate.y+tk_model[tk2->dir][z+1].y_off&&x2==tk2->coordinate.x+tk_model[tk2->dir][z+1].x_off)||\
							(y2==tk2->coordinate.y+tk_model[tk2->dir][z+2].y_off&&x2==tk2->coordinate.x+tk_model[tk2->dir][z+2].x_off)){
							continue;
						}
						if(y1==y2&&x1==x2){//tanks overlay where in non-empty place
							flag_overlap=TRUE;
							ot=tk1;
							break;
						}
					}
					if(TRUE==flag_overlap){
						break;
					}
				
				}
			}
		}else{
			break;
		}
		
	}while(0);
	
	return ot;
}

static object_type_t *bullet_on_tank(object_type_t *bullet,object_type_t *tank)
{
	object_type_t *ot=NULL;
	ot=barrier_on_tank(bullet,tank);
	return ot;
}

static object_type_t *barrier_on_tank(object_type_t *barrier,object_type_t *tank)
{
	object_type_t *br=barrier;
	object_type_t *tk=tank;
	object_type_t *ot=NULL;

	do{
		if(NULL==br||NULL==tk){
			break;
		}

		if((br->coordinate.x>=tk->coordinate.x)&&(br->coordinate.x<tk->coordinate.x+tk->size.w)&&\
			(br->coordinate.y>=tk->coordinate.y)&&(br->coordinate.y<tk->coordinate.y+tk->size.h)){
			if(((br->coordinate.x==tk->coordinate.x+tk_model[tk->dir][0].x_off)&&\
				(br->coordinate.y==tk->coordinate.y+tk_model[tk->dir][0].y_off))||\
				((br->coordinate.x==tk->coordinate.x+tk_model[tk->dir][1].x_off)&&\
				(br->coordinate.y==tk->coordinate.y+tk_model[tk->dir][1].y_off))||\
				((br->coordinate.x==tk->coordinate.x+tk_model[tk->dir][2].x_off)&&\
				(br->coordinate.y==tk->coordinate.y+tk_model[tk->dir][2].y_off))){
				break;
			}else{
				ot=br;
				break;
			}
		}else{
			break;
		}
	}while(0);
	
	return ot;
}


static object_type_t *bullet_on_bullet(object_type_t *bullet1,object_type_t *bullet2)
{
	/*
	 *Move all bullet before deal bullet collision ,Not only need we to deal with the same coordinates bullet collision,
	 *but also we need to deal  with Moving towards each other but different coordinate collision case.
	 */
	object_type_t *bt1=bullet1;
	object_type_t *bt2=bullet2;
	object_type_t *ot=NULL; 
	
	do{
		if(NULL==bt1||NULL==bt2){
			break;
		}

		if(bt1->coordinate.x==bt2->coordinate.x&&bt1->coordinate.y==bt2->coordinate.y){
			ot=bt1;
			break;
		}

		if(fabs(bt1->coordinate.x-bt2->coordinate.x)==1&&bt1->coordinate.y-bt2->coordinate.y==0){
			if(bt1->coordinate.x-bt2->coordinate.x==1){
				if(bt1->dir==DIR_RIGHT&&bt2->dir==DIR_LEFT){
					ot=bt1;
					break;
				}
			}else{
				if(bt1->dir==DIR_LEFT&&bt2->dir==DIR_RIGHT){
					ot=bt1;
					break;
				}
			}
		}

		if(fabs(bt1->coordinate.y-bt2->coordinate.y)==1&&bt1->coordinate.x-bt2->coordinate.x==0){
			if(bt1->coordinate.y-bt2->coordinate.y==1){
				if(bt1->dir==DIR_DOWN&&bt2->dir==DIR_UP){
					ot=bt1;
					break;
				}
			}else{
				if(bt1->dir==DIR_UP&&bt2->dir==DIR_DOWN){
					ot=bt1;
					break;
				}
			}
		}
	}while(0);

	return ot;
}

static object_type_t *barrier_on_bullet(object_type_t *barrier,object_type_t *bullet)
{
	object_type_t *br=barrier;
	object_type_t *bt=bullet;
	object_type_t *ot=NULL;

	do{
		if(NULL==br||NULL==bt){
			break;
		}

		if(br->coordinate.x==bt->coordinate.x&&br->coordinate.y==bt->coordinate.y){
			ot=br;
			break;
		}
	}while(0);
	
	return ot;
}

static object_type_t *barrier_on_barrier(object_type_t *barrier1,object_type_t *barrier2)
{
	object_type_t *br1=barrier1;
	object_type_t *br2=barrier2;
	object_type_t *ot=NULL;

	do{
		if(NULL==br1||NULL==br2){
			break;
		}
		if(br1->coordinate.x==br2->coordinate.x&&br1->coordinate.y==br2->coordinate.y){
			ot=br1;
			break;
		}
	}while(0);
	return ot;
}



static object_type_t *deal_automatic_tank_collision(object_type_t *tank,tank_battle_t *tank_battle)
{
	object_type_t *tk=tank;
	tank_battle_t *tb=tank_battle;
	object_type_t *cur=NULL;
	object_type_t *tmp=NULL;
	dir_t dir=DIR_NONE;
	BOOL flag_change_dir=FALSE;
	BOOL flag_rotate=FALSE;
	BOOL flag_fire=FALSE;
	
	do{
		if(NULL==tk||NULL==tb){
			break;
		}

		cur=tk->next;
		while(NULL!=cur){
			if(cur->number!=NUMBER_TANK_GREEN1&&\
				cur->number!=NUMBER_TANK_GREEN2&&\
				cur->number!=NUMBER_TANK_GREEN3){
				
				tmp=deal_tank_to_bounds(cur);
				if(NULL!=tmp){
					deal_automatic_tank_motion(cur,tb,FALSE);
					cur=cur->next;
					continue;
				}
				
				tmp=deal_tank_to_barrier(cur,tb->barrier);
				if(NULL!=tmp){
					deal_automatic_tank_motion(cur,tb,TRUE);
					cur=cur->next;
					continue;
				}

				tmp=deal_tank_to_tank(cur,tb->tank);
				if(NULL!=tmp){
					deal_automatic_tank_motion(cur,tb,TRUE);
					cur=cur->next;
					continue;
				}

				tmp=deal_tank_to_bullet(cur,tb->bullet);
				if(NULL!=tmp){
					deal_automatic_tank_motion(cur,tb,TRUE);
					cur=cur->next;
					continue;
				}
				move_tank(cur);
				deal_automatic_tank_motion(cur,tb,TRUE);
			}
			cur=cur->next;
		}
	}while(0);
	
	return tmp;
}


static void deal_automatic_tank_motion(object_type_t *tank,tank_battle_t *tank_battle,BOOL random)
{
	object_type_t *tk=tank;
	tank_battle_t *tb=tank_battle;
	dir_t dir=DIR_NONE;
	BOOL flag_change_dir=FALSE;
	BOOL flag_rotate=FALSE;
	BOOL flag_fire=FALSE;
	
	flag_change_dir=is_change_direction(TANK_MODEL_NUM);
	if((TRUE==flag_change_dir)||(random==FALSE)){
		dir=random_direction();
		flag_rotate=can_rotate_direction(dir,tk,tb);
		if(TRUE==flag_rotate){
			rotate_direction(dir,tk);
		}
	}
	flag_fire=is_fire(tb->speed);
	if(TRUE==flag_fire){
		fire(tk,tb->bullet);
	}
}

object_type_t *deal_manual_tank_collision(dir_t direction,object_type_t *tank,tank_battle_t *tank_battle)
{
	dir_t dir=direction;
	object_type_t *tk=tank;
	tank_battle_t *tb=tank_battle;
	BOOL rotate=FALSE;
	
	if(NULL==tk||NULL==tb){
		return NULL;
	}
	
	show_tank(1,1,tk);
	can_move_tank(tk,tb);
	show_tank(2,1,tk);
	if(dir==tk->dir){
		move_tank(tk);
	}else{
		rotate=can_rotate_direction(dir,tk,tb);
		if(TRUE==rotate){
			rotate_direction(dir,tk);
		}
	}

	return tk;

}

static tank_head_yx_t get_tank_head_yxoff(const object_type_t *tank)
{
	const object_type_t *tk=tank;
	tank_head_yx_t tank_head={.y_off=0,.x_off=0};

	do{
		if(NULL==tk){
			break;
		}

		switch(tk->dir){
			case DIR_UP:
				tank_head.y_off = +0-1;
				tank_head.x_off = +1;
				break;
				
			case DIR_RIGHT:
				tank_head.y_off = +1;
				tank_head.x_off = +2+1;
				break;
				
			case DIR_DOWN:
				tank_head.y_off = +2+1;
				tank_head.x_off = +1;
				break;
				
			case DIR_LEFT:
				tank_head.y_off = +1;
				tank_head.x_off = +0-1;
				break;
			default:
				break;
		}
	}while(0);

	return tank_head;
}


static BOOL get_tank_next_pos(object_type_t *next_pos,const object_type_t *tank)
{
	const object_type_t *tk=tank;
	object_type_t *np=next_pos;
	BOOL flag=FALSE;
	do{
		if(NULL==tk||NULL==np){
			break;
		}
		object_type_copy(np,tk);
		switch(tk->dir){
			case DIR_UP:
				np->coordinate.y=tk->coordinate.y-1;
				np->coordinate.x=tk->coordinate.x;
				break;

			case DIR_RIGHT:
				np->coordinate.y=tk->coordinate.y;
				np->coordinate.x=tk->coordinate.x+1;
				break;

			case DIR_DOWN:
				np->coordinate.y=tk->coordinate.y+1;
				np->coordinate.x=tk->coordinate.x;
				break;

			case DIR_LEFT:
				np->coordinate.y=tk->coordinate.y;
				np->coordinate.x=tk->coordinate.x-1;
				break;
			default:break;
		}
		
	}while(0);

	return flag;
}

BOOL can_rotate_direction(dir_t direction,object_type_t *tank,tank_battle_t *tank_battle)
{
	dir_t dir=direction;
	object_type_t *tk=tank;
	tank_battle_t *tb=tank_battle;
	object_type_t *cur=NULL;
	object_type_t *ret=NULL;
	object_type_t tmp;
	BOOL rotate=TRUE;
	
	do{
		if(NULL==tk||NULL==tb){
			break;
		}

		if(NULL==tb->tank){
			break;
		}

		if(dir==tk->dir){
			break;
		}
		tmp.dir=dir;
		coordinate_copy(&tmp.coordinate,&tk->coordinate);
		size_copy(&tmp.size,&tk->size);
		cur=tb->tank->next;
		while(NULL!=cur){
			if(tk!=cur){
				ret=tank_on_tank(&tmp,cur);
				if(NULL!=ret){
					rotate=FALSE;
					break;
				}
				
			}
			cur=cur->next;
		}
		if(rotate==FALSE){
			break;
		}
		
		cur=tb->barrier->next;
		while(NULL!=cur){
			ret=barrier_on_tank(cur,&tmp);
			if(NULL!=ret){
				rotate=FALSE;
				break;
			}
			cur=cur->next;
		}
		if(rotate==FALSE){
			break;
		}
	}while(0);
		
	return rotate;
}

object_type_t *rotate_direction(dir_t direction,object_type_t *tank)
{
	object_type_t *tk=tank;
	if(NULL==tk){
		return tk;
	}

	tk->dir=direction;

	return tk;
}

BOOL can_move_tank(object_type_t *tank,tank_battle_t *tank_battle)
{
	object_type_t *tk=tank;
	tank_battle_t *tb=tank_battle;
	object_type_t *tmp=NULL;
	BOOL move=FALSE;
	
	do{
		if(NULL==tk||NULL==tb){
			break;
		}

		tmp=deal_tank_to_bounds(tk);
		if(NULL!=tmp){
			break;
		}

		tmp=deal_tank_to_tank(tk,tb->tank);
		if(NULL!=tmp){
			break;
		}

		tmp=deal_tank_to_barrier(tk,tb->barrier);
		if(NULL!=tmp){
			break;
		}
		
		move=TRUE;
	}while(0);
	
	return move;
}

object_type_t *move_tank(object_type_t *tank)
{
	object_type_t *tk=tank;
	object_type_t *ot=NULL;
	do{
		if(NULL==tk){
			break;
		}

		if(FALSE==tk->canmove){
			break;
		}

		switch(tk->dir){
			case DIR_UP:
				tk->coordinate.y--;
				break;
			case DIR_RIGHT:
				tk->coordinate.x++;
				break;
			case DIR_DOWN:
				tk->coordinate.y++;
				break;
			case DIR_LEFT:
				tk->coordinate.x--;
				break;
			default:
				break;
		}
		
		ot=tk;
	}while(0);

	return ot;
}

static BOOL is_fire(int speed)
{
	int sd=speed;
	if(sd<SPEED_MIN_LEVEL||sd>SPEED_MAX_LEVEL){
		sd=TANK_MODEL_NUM;
	}

	return (rand()%TANK_MODEL_NUM==TANK_MODEL_NUM/2)?TRUE:FALSE;
}

object_type_t *fire(object_type_t *tank,object_type_t *bullet)
{
	//get fire direction and coordinate
	object_type_t *tk=tank;
	object_type_t *bt=bullet;
	object_type_t *cur=NULL;

	coordinate_t coordinate;
	dir_t dir=tk->dir;
	standpoint_t standpoint=tk->standpoint;
	if(NULL==tk||NULL==bt){
		return cur;
	}

	switch(tk->dir){
		//from the head of tank instead of the next pos of the head
		case DIR_UP:
			coordinate.y=tk->coordinate.y+0;
			coordinate.x=tk->coordinate.x+1;
			break;
		case DIR_LEFT:
			coordinate.y=tk->coordinate.y+1;
			coordinate.x=tk->coordinate.x+0;
			break;
		case DIR_DOWN:
			coordinate.y=tk->coordinate.y+2;
			coordinate.x=tk->coordinate.x+1;
			break;
		case DIR_RIGHT:
			coordinate.y=tk->coordinate.y+1;
			coordinate.x=tk->coordinate.x+2;
			break;
		default:
			break;
	}
	cur=add_object(coordinate,OBJECT_BULLET,dir,standpoint,0,bt);
	
	return cur;
}

static BOOL is_change_direction(int seed)
{
	int sd=seed;
	if(sd<0){
		sd=TANK_MODEL_NUM;
	}

	return ((rand()%sd==sd/2)?TRUE:FALSE);
}

static dir_t random_direction(void)
{
	dir_t dir=rand()%(TANK_MODEL_NUM);

	return dir;
}

static object_type_t *deal_tank_to_bounds(object_type_t *tank)
{
	object_type_t next_tank;
	BOOL flag=FALSE;
	
	get_tank_next_pos(&next_tank,tank);
	tank->canmove=TRUE;
	flag=is_out_bounds(&next_tank);
	if(flag==TRUE){
		tank->canmove=FALSE;
		return tank;
	}else{
		return NULL;
	}
}

static object_type_t *deal_tank_to_tank(object_type_t *tank,object_type_t *head_tank)
{
	object_type_t *tk=tank;
	object_type_t *htk=head_tank;
	object_type_t *cur=NULL;
	object_type_t *tmp=NULL;
	object_type_t next_tank;

	do{
		if(NULL==tk||NULL==htk){
			break;
		}

		get_tank_next_pos(&next_tank,tk);
		tk->canmove=TRUE;
		cur=htk->next;
		while(NULL!=cur){
			if(tk!=cur){
				tmp=tank_on_tank(&next_tank,cur);
				if(NULL!=tmp){
					tk->canmove=FALSE;
					break;
				}
			}
			cur=cur->next;
		}
	}while(0);

	return tmp;
}

static object_type_t *deal_tank_to_bullet(object_type_t *tank,object_type_t *head_bullet)
{
	object_type_t *tk=tank;
	object_type_t *hbt=head_bullet;
	object_type_t *cur=NULL;
	object_type_t *tmp=NULL;
	object_type_t next_tank;
	do{
		if(NULL==tk||NULL==hbt){
			break;
		}
		get_tank_next_pos(&next_tank,tk);
		tk->canmove=TRUE;
		cur=hbt->next;
		while(NULL!=cur){
			tmp=bullet_on_tank(cur,&next_tank);
			if(NULL!=tmp){
				tk->canmove=FALSE;
				break;
			}
			cur=cur->next;
		}
	}while(0);
	
	return tmp;
}

static object_type_t *deal_tank_to_barrier(object_type_t *tank,object_type_t *head_barrier)
{
	object_type_t *tk=tank;
	object_type_t *hbr=head_barrier;
	object_type_t *cur=NULL;
	object_type_t *tmp=NULL;
	object_type_t next_tank;

	do{
		if(NULL==tk||NULL==hbr){
			break;
		}
		get_tank_next_pos(&next_tank,tk);
		tk->canmove=TRUE;
		cur=hbr->next;
		while(NULL!=cur){
			tmp=barrier_on_tank(cur,&next_tank);
			if(NULL!=tmp){
				tk->canmove=FALSE;
				break;
			}
			cur=cur->next;
		}
		
	}while(0);
		
	return tmp;
}

static int get_tank_num(standpoint_t standpoint,const object_type_t *head_tank)
{
	const object_type_t *htk=head_tank;
	object_type_t *cur=NULL;
	int number=0;
	if(NULL==htk){
		return TB_FAILED;
	}
	cur=htk->next;
	while(NULL!=cur){
		if(standpoint==cur->standpoint){
			number++;
		}
		cur=cur->next;
	}
	
	return number;
}

static object_type_t *move_all_bullet(object_type_t *bullet)
{
	object_type_t *ot=bullet;

		if(NULL==ot){
			return ot;
		}

		object_type_t *cur=ot->next;
		while(NULL!=cur){
			switch(cur->dir){
				case DIR_UP:
					cur->coordinate.y--;
					break;
				case DIR_RIGHT:
					cur->coordinate.x++;
					break;
				case DIR_DOWN:
					cur->coordinate.y++;
					break;
				case DIR_LEFT:
					cur->coordinate.x--;
					break;
				default:
					break;
			}
			cur=cur->next;
		}
	return ot->next;
}

static object_type_t *deal_all_bullet_collision(object_type_t *bullet,tank_battle_t *tank_battle)
{
	object_type_t *bt=bullet;
	tank_battle_t *tb=tank_battle;
	object_type_t *prev=NULL;
	object_type_t *cur=NULL;
	object_type_t *tmp=NULL;
	BOOL flag;

	do{
		if(NULL==bt||NULL==tb){
			break;
		}

		cur=bt->next;
		prev=bt;
		while(NULL!=cur){
			flag=is_out_bounds(cur);
			if(TRUE==flag){
				del_object_type(cur,bt);
				cur=prev->next;
				continue;
			}

			tmp=deal_bullet_to_tank(cur,tb->tank,tb);
			if(NULL!=tmp){
				del_object_type(cur,bt);
				cur=prev->next;
				continue;
			}

			tmp=deal_bullet_to_barrier(cur,tb->barrier);
			if(NULL!=tmp){
				del_object_type(cur,bt);
				cur=prev->next;
				continue;
			}

			tmp=deal_bullet_to_bullet(cur,tb->bullet);
			if(NULL!=tmp){
				del_object_type(cur,bt);
				cur=prev->next;
				continue;
			}
			
			prev=cur;
			cur=cur->next;
		}
	}while(0);
	
	return cur;
}

static object_type_t *deal_bullet_to_tank(object_type_t *bullet,object_type_t *head_tank,tank_battle_t *tank_battle)
{
	tank_battle_t *tb=tank_battle;
	object_type_t *bt=bullet;
	object_type_t *htk=head_tank;
	object_type_t *prev=NULL;
	object_type_t *cur=NULL;
	object_type_t *tmp=NULL;
	standpoint_t sp;
	coordinate_t coordinate;
	dir_t dir;
	int number=0;

	do{
		if(NULL==bt||NULL==htk||NULL==tb){
			break;
		}
		prev=htk;
		cur=htk->next;
		while(NULL!=cur){
			tmp=bullet_on_tank(bt,cur);
			if(NULL!=tmp){
				if(bt->standpoint!=cur->standpoint){
					cur->hp--;
					if(cur->hp<=0){
						prev->next=cur->next;
						sp=cur->standpoint;
						del_object_type(cur,htk);
						if(STANDPOINT_BLUE==sp){
							tb->side_blue--;
							if(tb->side_blue<=0){
								tb->status=STATUS_QUIT;
							}else{
								number=get_tank_num(STANDPOINT_BLUE,htk);
								if(number<BLUE_MIN_FIGHTING_TANK&&tb->side_blue>=BLUE_MIN_FIGHTING_TANK){
									new_object_pos(&coordinate,&dir,OBJECT_TANK,tb);
									add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_BLUE,0,tb->tank);
								}
							}
						}else if(STANDPOINT_GREEN==sp){
							tb->side_green--;
							if(tb->side_green<=0){
								tb->status=STATUS_QUIT;
							}else{
								number=get_tank_num(STANDPOINT_GREEN,htk);
								if(number<GREEN_MIN_FIGHTING_TANK&&tb->side_green>=GREEN_MIN_FIGHTING_TANK){
									new_object_pos(&coordinate,&dir,OBJECT_TANK,tb);
									add_object(coordinate,OBJECT_TANK,dir,STANDPOINT_GREEN,0,tb->tank);
								}
							}
						}
					}	
				}
				break;
			}
			prev=cur;
			cur=cur->next;
		}
	}while(0);
	
	return tmp;
}

static object_type_t *deal_bullet_to_bullet(object_type_t *bullet,object_type_t *head_bullet)
{
	object_type_t *bt=bullet;
	object_type_t *hbt=head_bullet;
	object_type_t *prev=NULL;
	object_type_t *cur=NULL;
	object_type_t *tmp=NULL;

	do{
		if(NULL==bt||NULL==hbt){
			break;
		}
		prev=hbt;
		cur=hbt->next;
		while(NULL!=cur){
			if(bt!=cur){
				tmp=bullet_on_bullet(bt,cur);
				if(NULL!=tmp){
					prev->next=cur->next;
					del_object_type(cur,hbt);
					break;
				}
			}
			prev=cur;
			cur=cur->next;
		}
	}while(0);
	
	return tmp;
}

static object_type_t *deal_bullet_to_barrier(object_type_t *bullet,object_type_t *head_barrier)
{
	object_type_t *bt=bullet;
	object_type_t *hbr=head_barrier;
	object_type_t *prev=NULL;
	object_type_t *cur=NULL;
	object_type_t *tmp=NULL;

	do{
		if(NULL==bt||NULL==hbr){
			break;
		}
		prev=hbr;
		cur=hbr->next;
		while(NULL!=cur){
			if(bt->coordinate.y==cur->coordinate.y&&bt->coordinate.x==cur->coordinate.x){
				tmp=bt;

				cur->hp--;
				if(cur->hp<=0){
					prev->next=cur->next;
					del_object_type(cur,hbr);
				}
				break;
			}
			prev=cur;
			cur=cur->next;
		}
	}while(0);
	
	return tmp;
}

