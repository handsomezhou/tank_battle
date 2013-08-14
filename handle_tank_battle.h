/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#ifndef HANDLE_TANK_BATTLE
#define HANDLE_TANK_BATTLE

#include "tank_battle.h"
/**
 *@brief Handle tank battle event
 *@param [in/out] tank_battle:Pass background Processing relevant parameters
 *@return TB_SUCCESS/TB_FAILED 
 *@author handsomezhou  
 *@date 2013-08-12
 */
int handle_tank_battle(tank_battle_t *tank_battle);

/**
 *@brief	Determine the new tank or barrier location
 *@param [in/out] coordinate:Get coordinate of the new object
 *@param [in/out] dir:Get dir of the new object
 *@param [in] object:Depending on the object type judgment is  tank or barrier
 *@param [in] tank_battle:Determine whether the new position and tank_battle conflict
 *@return TRUE/FALSE
 *@author handsomezhou  
 *@date 2013-08-12
 */
BOOL new_object_pos(coordinate_t *coordinate, dir_t *dir,object_t object,const tank_battle_t *tank_battle);

/**
 *@brief	Add new object(tank,bullet or barrier)
 *@param [in] coordinate:The added object coordinate
 *@param [in] object:The added object type(tank,bullet or barrier)
 *@param [in] dir:The added object direction
 *@param [in] standpoint:The added object standpoint
 *@param [in] number:The added number,only used to distinguish between manual tank
 *@param [in] tank_battle:Add a new object to the corresponding object list
 *@return The address of added object
 *@author handsomezhou  
 *@date 2013-08-12
 */
object_type_t *add_object(coordinate_t coordinate,object_t object,dir_t dir,standpoint_t standpoint,int number,object_type_t *object_type);

/**
 *@brief Tank fire
 *@param [in] tank:Tank fire
 *@param [in/out] bullet:Add the bullet object to the bullet list
 *@return The address of the object of bullet
 *@author handsomezhou  
 *@date 2013-08-12
 */
object_type_t *fire(object_type_t *tank,object_type_t *bullet);

object_type_t *move_tank(object_type_t *tank);


#endif	/*HANDLE_TANK_BATTLE*/

