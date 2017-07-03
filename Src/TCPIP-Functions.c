/*
 * TCPIP-Functions.c
 *
 *  Created on: 02.07.2017
 *      Author: Philipp
 */

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "stm32f7xx_hal.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
//#include "FreeRTOS_tcp_server.h"
#include "FreeRTOS_DHCP.h"

/* Use by the pseudo random number generator. */
static UBaseType_t ulNextRand;

/* Define names that will be used for DNS, LLMNR and NBNS searches.  This allows
mainHOST_NAME to be used when the IP address is not known.  For example
"ping RTOSDemo" to resolve RTOSDemo to an IP address then send a ping request. */
#define mainHOST_NAME					"RTOSDemo"
#define mainDEVICE_NICK_NAME			"stm32"


const char *pcApplicationHostnameHook( void )
{
	/* Assign the name "rtosdemo" to this network node.  This function will be
	called during the DHCP: the machine will be registered with an IP address
	plus this name. */
	return mainHOST_NAME;
}

static void prvSRand( UBaseType_t ulSeed )
{
	/* Utility function to seed the pseudo random number generator. */
	ulNextRand = ulSeed;
}

//Utility Functions
//Random Number Generator for TCP Arbiter
UBaseType_t uxRand( void )
{
const uint32_t ulMultiplier = 0x015a4e35UL, ulIncrement = 1UL;
static BaseType_t xInitialised = pdFALSE;

	/* Don't initialise until the scheduler is running, as the timeout in the
	random number generator uses the tick count. */
	if( xInitialised == pdFALSE )
	{
		if( xTaskGetSchedulerState() !=  taskSCHEDULER_NOT_STARTED )
		{
		RNG_HandleTypeDef xRND;
		uint32_t ulSeed;

			/* Generate a random number with which to seed the local pseudo random
			number generating function. */
			HAL_RNG_Init( &xRND );
			HAL_RNG_GenerateRandomNumber( &xRND, &ulSeed );
			prvSRand( ulSeed );
			xInitialised = pdTRUE;
		}
	}

	/* Utility function to generate a pseudo random number. */

	ulNextRand = ( ulMultiplier * ulNextRand ) + ulIncrement;
	return( ( int ) ( ulNextRand >> 16UL ) & 0x7fffUL );
}



/* Called by FreeRTOS+TCP when the network connects or disconnects.  Disconnect
events are only received if implemented in the MAC driver. */
void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{
uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
char cBuffer[ 16 ];
static BaseType_t xTasksAlreadyCreated = pdFALSE;

	FreeRTOS_printf( ( "vApplicationIPNetworkEventHook: event %ld\n", eNetworkEvent ) );

	/* If the network has just come up...*/
	if( eNetworkEvent == eNetworkUp )
	{
		/* Create the tasks that use the IP stack if they have not already been
		created. */
		if( xTasksAlreadyCreated == pdFALSE )
		{
			/* Tasks that use the TCP/IP stack can be created here. */

			/* Start a new task to fetch logging lines and send them out. */
			#if( mainCREATE_UDP_LOGGING_TASK == 1 )
			{
				vUDPLoggingTaskCreate();
			}
			#endif

			#if( ( mainCREATE_FTP_SERVER == 1 ) || ( mainCREATE_HTTP_SERVER == 1 ) )
			{
				/* Let the server work task now it can now create the servers. */
				xTaskNotifyGive( xServerWorkTaskHandle );
			}
			#endif

			#if( mainCREATE_TCP_ECHO_CLIENT_TASKS_SINGLE == 1 )
			{
				vStartTCPEchoClientTasks_SingleTasks( mainECHO_CLIENT_TASK_STACK_SIZE, mainECHO_CLIENT_TASK_PRIORITY );
			}
			#endif

			#if( mainCREATE_SIMPLE_TCP_ECHO_SERVER == 1 )
			{
				vStartSimpleTCPServerTasks( mainECHO_SERVER_STACK_SIZE, mainECHO_SERVER_TASK_PRIORITY );
			}
			#endif

			/* Register example commands with the FreeRTOS+CLI command
			interpreter via the UDP port specified by the
			mainUDP_CLI_PORT_NUMBER constant.  If the HTTP or FTP servers are
			being created then file system related commands will also be
			registered from the TCP server task. */
			#if( mainCREATE_UDP_CLI_TASKS == 1 )
			{
				vRegisterSampleCLICommands();
				vRegisterTCPCLICommands();
				vStartUDPCommandInterpreterTask( mainUDP_CLI_TASK_STACK_SIZE, mainUDP_CLI_PORT_NUMBER, mainUDP_CLI_TASK_PRIORITY );
			}
			#endif

			xTasksAlreadyCreated = pdTRUE;
		}

		/* Print out the network configuration, which may have come from a DHCP
		server. */
		FreeRTOS_GetAddressConfiguration( &ulIPAddress, &ulNetMask, &ulGatewayAddress, &ulDNSServerAddress );
		FreeRTOS_inet_ntoa( ulIPAddress, cBuffer );
		FreeRTOS_printf( ( "IP Address: %s\n", cBuffer ) );

		FreeRTOS_inet_ntoa( ulNetMask, cBuffer );
		FreeRTOS_printf( ( "Subnet Mask: %s\n", cBuffer ) );

		FreeRTOS_inet_ntoa( ulGatewayAddress, cBuffer );
		FreeRTOS_printf( ( "Gateway Address: %s\n", cBuffer ) );

		FreeRTOS_inet_ntoa( ulDNSServerAddress, cBuffer );
		FreeRTOS_printf( ( "DNS Server Address: %s\n", cBuffer ) );
	}
}
/*-----------------------------------------------------------*/

void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier )
{
}
/*-----------------------------------------------------------*/

BaseType_t xApplicationDNSQueryHook( const char *pcName )
{
BaseType_t xReturn;

	/* Determine if a name lookup is for this node.  Two names are given
	to this node: that returned by pcApplicationHostnameHook() and that set
	by mainDEVICE_NICK_NAME. */
	if( strcasecmp( pcName, pcApplicationHostnameHook() ) == 0 )
	{
		xReturn = pdPASS;
	}
	else if( strcasecmp( pcName, mainDEVICE_NICK_NAME ) == 0 )
	{
		xReturn = pdPASS;
	}
	else
	{
		xReturn = pdFAIL;
	}

	return xReturn;
}


