
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <stdint.h>

#define MQ_CMDS_NAME        "/mq_cmds"	//! Use a slash conform man page (7) mq_overview
#define MAX_CMD_SIZE		50			//! Max. length of a command message
#define MESSAGE_PRIORITY	0
#define MAX_MESSAGE			50			

#define CMD_RUMBLE			"rumble"
#define CMD_LED				"led"		
#define CMD_BTNS			"btns"		

typedef struct
{
	uint8_t id;
	char cmd[MAX_CMD_SIZE];
	int16_t val;
}command;


void initQueue(char *name);
void closeQueue();

void startQueue(char *name); 
void stopQueue();

void sendCommand(char *id, char *cmd, char *val);
void getCommand(command *cmd);

#endif
