/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#ifndef HANDLE_TANK_BATTLE
#define HANDLE_TANK_BATTLE

#include "tank_battle.h"

int handle_tank_battle(tank_battle_t *tank_battle);
BOOL new_object_pos(coordinate_t *coordinate, dir_t *dir,object_t object,const tank_battle_t *tank_battle);
object_type_t *add_object(coordinate_t coordinate,object_t object,dir_t dir,standpoint_t standpoint,int number,object_type_t *object_type);
object_type_t *fire(object_type_t *tank,object_type_t *bullet);

#endif	/*HANDLE_TANK_BATTLE*/

