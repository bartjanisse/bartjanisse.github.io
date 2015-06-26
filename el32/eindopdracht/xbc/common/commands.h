/** \file commands.h
 *  \ingroup common
 * 	\brief Functions and definitions for commands
 *
 * 	\author A.W Janisse
 * 	\bug No known bugs.
 *
 * 	\version 1.0 	First release.
*/
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <stdint.h>

#define MQ_CMDS_NAME        "/mq_cmds"	//!< Constant for naming the message queue
#define MAX_CMD_LEN			50			//!< Max. length of a command message
#define MESSAGE_PRIORITY	0			//!< Priorty for the messages
#define MAX_MESSAGES		50			//!< MAx. number for the message queue

#define CMD_RUMBLE			"rumble"	//!< Constant for the rumble command
#define CMD_LED				"led"		//!<* Constant for the led command
#define CMD_BTNS			"btns"		//!<* Constant for the buttons command

/** \ingroup common
 *  \brief Command structure  
 */
typedef struct
{
	uint8_t id;				//!< Field for controller identification
	char cmd[MAX_CMD_LEN];	//!< Field for holding a command
	int16_t val;			//!< Field for holding the command value
}command;

/** \ingroup common
 * \brief function to create and open a message queue.
 *
 * \param name for the message queue
 */
void initQueue(char *name);

/** \ingroup common
 * \brief function for closing a opened message queue.
 */
void closeQueue();

/** \ingroup common
 * \brief function to open the existing message queue
 */
void startQueue(); 
/** \ingroup common
 * \brief Function to close the existing message queue
 */
void stopQueue();

/** \ingroup common
 * \brief function to send a command over the message queue
 * 
 * \param cmd is the command to send
 */ 
void sendCommand(command *cmd);

/** \ingroup common
 *  \brief function to receive a command over the message queue
 * 
 * \param cmd is filled with the received command
 */ 
void getCommand(command *cmd);

/** \ingroup common
 * \brief function to create a command out of a string
 * 
 * /param cmd pointer te return the created command
 * /param msg is a pointer to the command string
 */
void retreiveCommand(command *cmd, char *msg);

#endif
