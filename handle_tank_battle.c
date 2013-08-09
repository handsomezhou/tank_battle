/**
  *  Copyright (C) 2013  Handsome Zhou
  */
  
#include <stdlib.h>
#include "handle_tank_battle.h"

static BOOL new_tank_pos(coordinate_t *coordinate, dir_t *dir,const tank_battle_t *tank_battle);
static BOOL new_barrier_pos(coordinate_t *coordinate,const tank_battle_t *tank_battle);

static object_type_t *add_tank(coordinate_t coordinate,dir_t dir,standpoint_t standpoint,object_type_t *tank);
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


int handle_tank_battle(tank_battle_t *tank_battle)
{
	return TB_SUCCESS;
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

object_type_t *add_object_type(coordinate_t coordinate,object_t object,dir_t dir,standpoint_t standpoint,object_type_t *object_type)
{
	object_type_t *ot=object_type;
	object_type_t *ret=NULL;

	do{
		if(NULL==ot){
			break;
		}

		switch(object){
			case OBJECT_TANK:
				ret=add_tank(coordinate,dir,standpoint,ot);
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

static object_type_t *add_tank(coordinate_t coordinate,dir_t dir,standpoint_t standpoint,object_type_t *tank)
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
		cur->next->canmove=TRUE;
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
		cur->hp=HP_BULLET;
		cur->canmove=TRUE;
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
		cur->hp=HP_BARRIER;
		cur->canmove=FALSE;
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
	return NULL;
}

static object_type_t *bullet_on_tank(object_type_t *bullet,object_type_t *tank)
{
	return NULL;
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
	return NULL;
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


