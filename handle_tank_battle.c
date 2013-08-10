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
static object_type_t *move_tank(object_type_t *tank);

//bullet:First move after the judge
static object_type_t *move_all_bullet(object_type_t *bullet);

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
		//about bullet event
		move_all_bullet(tb->bullet);
		
		//about tank event
		
		
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
		tank.dir=rand()%(DIR_NONE-DIR_UP);	//4-0
#if 0
		//just for test
		tank.coordinate.y=8;
		tank.coordinate.x=7;
#endif
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

static object_type_t *move_tank(object_type_t *tank)
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


