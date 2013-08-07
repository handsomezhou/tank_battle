/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include <unistd.h>
#include <pthread.h>
#include "tank_battle.h"
#include "input_tank_battle.h"
#include "handle_tank_battle.h"
#include "output_tank_battle.h"

int main(int argc, char *argv[])
{
	tank_battle_t *tank_battle=NULL;
	int ret=TB_FAILED;

	ret=init_tank_battle(&tank_battle);
	if(TB_FAILED==ret){
		exit_tank_battle(tank_battle);

		return TB_FAILED;
	}

	while(1){
		paint_tank_battle(tank_battle);
		usleep(1000*1000);
	}
	
	exit_tank_battle(tank_battle);

	return TB_SUCCESS;
}