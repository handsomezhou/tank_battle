/**
  *  Copyright (C) 2013  Handsome Zhou
  */
  
#include <stdlib.h>
#include "handle_tank_battle.h"

static object_type_t *add_tank(coordinate_t coordinate,dir_t dir,standpoint_t standpoint,object_type_t *tank);
static object_type_t *add_bullet(coordinate_t coordinate,dir_t dir,standpoint_t standpoint,object_type_t *bullet);
static object_type_t *add_barrier(coordinate_t coordinate,object_type_t *barrier);
static void del_object_type(object_type_t *obj_type,object_type_t *head);

int handle_tank_battle(tank_battle_t *tank_battle)
{
	return TB_SUCCESS;
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
		cur->hp=HP_TANK;
		cur->canmove=TRUE;
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


