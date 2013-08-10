/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "tank_battle.h"
#include "input_tank_battle.h"
#include "handle_tank_battle.h"
#include "output_tank_battle.h"

int main(int argc, char *argv[])
{
	tank_battle_t *tank_battle=NULL;
	int ret=TB_FAILED;
	pthread_t tid_input=-1;
	
	srand((unsigned int)(time(NULL)));
	ret=init_tank_battle(&tank_battle);
	if(TB_FAILED==ret){
		exit_tank_battle(tank_battle);

		return TB_FAILED;
	}

	if(pthread_create(&tid_input,NULL,input_tank_battle,(void *)tank_battle)!=0){
		exit_tank_battle(tank_battle);
		perror("pthread_create() for tid_input!");

		return TB_FAILED;
	}
	
	while(STATUS_QUIT!=tank_battle->status){
		paint_tank_battle(tank_battle);
		handle_tank_battle(tank_battle);
		
		usleep(1000*1000);
	}

	if(pthread_join(tid_input,NULL)!=0){
		exit_tank_battle(tank_battle);
		perror("pthread_join() for tid_input");

		return TB_FAILED;
	}
	
	exit_tank_battle(tank_battle);

	return TB_SUCCESS;
}
