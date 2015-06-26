/** \file /xbcweb/main.c
 *  \ingroup xbcweb
 * 	\brief Main and helper functions for the xbcweb application
 *
 * 	\author A.W Janisse
 * 	\bug No known bugs.
 *
 * 	\version 1.0 	First release.
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "../common/commands.h"
#include "../common/common.h"
#include "../common/buttons.h"
#include "../common/shm.h"

#define WEB_LOG_NAME	"xbc-web"	//!< name used in the log file

/** \ingroup xbcweb 
 *  \brief This function print a button as a comma sepperated string.
 * 
 *  \param btn is a pointer to the button to print
 */
void
printButton(button *btn)
{	
	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		btn->avail,
		btn->D_UP,
		btn->D_DN,
		btn->D_LEFT,
		btn->D_RIGHT,
		btn->START,
		btn->BACK,
		btn->LS_PRESS,
		btn->RS_PRESS,
		btn->LB,
		btn->RB,
		btn->LOGO,
		btn->SPARE,
		btn->A,
		btn->B,
		btn->X,
		btn->Y,
		btn->Left_trigger,
		btn->Right_trigger,
		btn->Left_stick_X,
		btn->Left_stick_Y,
		btn->Right_stick_X,
		btn->Right_stick_Y);
}

/** \ingroup xbcweb 
 *  \brief This function removes any spaces
 * 
 *  \param str is the string which should be cleaned.
 */
void
removeSpaces(char *str)
{
	uint16_t i;
	char *tmpStr = (char *)malloc(strlen(str));
	char *dest = tmpStr;
	
	if(tmpStr){
		i = 0;
		while(str[i])
		{
			if(str[i] != '%') {
				*dest++ = str[i];
			} else {
				i += 2; // Skip the next 2 positions to remove "20"
			}
			i++;
		}
		*dest = '\0';
	}	
	strcpy(str, tmpStr);
	free(tmpStr);
}

/** \ingroup xbcweb 
 *  \brief This is the main function of the application.
 * 
 *  When this applicattion is called from a website this is the first function which will run.
 *  <p>
 *  The website can call this application to get information about the connected Xbox 360 controllers or to
 *  controll the rumble actuator or leds.
 *  <p>
 *  The call made from the website should have this format:
 *  <b>wbcweb?<id>,<cmd>,<val></b> 
 *  <p>
 *  The fields in here have the following meaning:
 *  <table>
 * 		<tr>
 * 			<th>Field</th>
 * 			<th>Meaning</th>
 * 		</tr>
 *		<tr>
 *        	<td>xbcweb</td> 
 * 			<td>this application</td>
 *		</tr>
 * 		<tr>
 *        	<td>id</td> 
 * 			<td>controller id</td>
 *		</tr>
 * 		<tr>
 *        	<td>cmd</td> 
 * 			<td>command</td>
 *		</tr>
 * 		<tr>
 *        	<td>val</td> 
 * 			<td>value for the given command</td>
 *		</tr>
 *  </table>
 *  <p>
 *  The command can be one of the following:
 *  <table>
 *     	<tr>
 * 			<th>Command</th>
 * 			<th>Meaning</th>
 * 		</tr>
 *		<tr>
 *        	<td>btns</td> 
 * 			<td>Will return the button states of the controller. The id field is the controller id</td>
 *		</tr>
 * 		<tr>
 *        	<td>rumble</td> 
 * 			<td>Controll the rumble actuator of the controller. The id field is the controller id, The val field is the speed</td>
 *		</tr>
 * 		<tr>
 *        	<td>led</td> 
 * 			<td>ontroll the leds of the controller. The id field is the controller id, The val field is the @ref Leds led pattern</td>
 *		</tr>
 *  </table>
 */
int
main()
{
	char 		*strCmd;
	command 	cmd;
	button 		btn;
	
	// Create a log entry in /var/log/
	openlog (WEB_LOG_NAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
	
	/* Start HTTP output */
	printf("Content-Type: text/plain\n\n");

	if ((strCmd = getenv("QUERY_STRING")) != NULL && *strCmd != '\0')
    {
		removeSpaces(strCmd);
		retreiveCommand(&cmd, strCmd);
			
		if (strstr(strCmd, CMD_BTNS))
		{
			if(cmd.id >= 0 && cmd.id < MAX_DEVS)
			{
				startButtonSHM(SHM_NAME);
				readButtonFromSHM(&btn, cmd.id);
				printButton(&btn);
			}
		}
		else if (strstr(strCmd, CMD_LED))
		{
			startQueue();
			sendCommand(&cmd);
			stopQueue();
		}
		else if (strstr(strCmd, CMD_RUMBLE))
		{
			startQueue();
			sendCommand(&cmd);
			stopQueue();
		}
	}
	return (0);
}
