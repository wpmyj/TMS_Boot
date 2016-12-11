/*
This file is part of CanFestival, a library implementing CanOpen Stack.

Copyright (C): Edouard TISSERANT and Francis DUPIN
AVR Port: Andreas GLAUSER and Peter CHRISTEN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
/******************************************************************************
Project description:
Test projekt for a DS 401 slave, running on Atmel's STK500 with AT90CAN128
Short description:
  PORTA:	Inputs (Keys, low active)
  PORTB:	Outputs (LEDs, low active)
  PORTC:	Node ID (1 BCD switch, low active)

******************************************************************************/
#include "hardware.h"
#include "canfestival.h"
#include "can_STM32.h"
#include "ObjDict.h"

unsigned char timer_interrupt = 0;		// Set if timer interrupt eclapsed
unsigned char inputs;

// CAN
unsigned char nodeID;
unsigned char digital_input[1] = {0};
unsigned char digital_output[1] = {0};



// macros to handle the schedule timer
#define sys_timer			timer_interrupt
#define reset_sys_timer()		timer_interrupt = 0
#define CYCLE_TIME	        	1000     	// Sample Timebase [us]

void canOpenInit(void)
{                              // Initialize system
  canInit(CAN1,CAN_BAUDRATE_1MB);         		// Initialize the CANopen bus
  initTimer();                                 	// Start timer for the CANopen stack
  nodeID = read_bcd();				// Read node ID first
  setNodeId (&ObjDict_Data, nodeID);
  setState(&ObjDict_Data, Initialisation);	// Init the state
}
void testCanOpen()
{
	Message m;
	for(;;)		                        // forever loop
  {
    // a message was received pass it to the CANstack
    canReceive(CAN1,&m);		// a message reveived
    canDispatch(&ObjDict_Data, &m);         // process it

  }
}




