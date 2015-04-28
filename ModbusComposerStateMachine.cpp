/*----- PROTECTED REGION ID(ModbusComposerStateMachine.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";
//=============================================================================
//
// file :        ModbusComposerStateMachine.cpp
//
// description : State machine file for the ModbusComposer class
//
// project :     ModbusComposer
//
// This file is part of Tango device class.
// 
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
// 
// $Author:  $
//
// $Revision:  $
// $Date:  $
//
// $HeadURL:  $
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================

#include <ModbusComposer.h>

/*----- PROTECTED REGION END -----*/	//	ModbusComposer::ModbusComposerStateMachine.cpp

//================================================================
//  States  |  Description
//================================================================


namespace ModbusComposer_ns
{
//=================================================
//		Attributes Allowed Methods
//=================================================


//=================================================
//		Commands Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : ModbusComposer::is_DynCommand_allowed()
 *	Description : Execution allowed for DynCommand attribute
 */
//--------------------------------------------------------
bool ModbusComposer::is_DynCommand_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for DynCommand command.
	/*----- PROTECTED REGION ID(ModbusComposer::DynCommandStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::DynCommandStateAllowed
	return true;
}

}	//	End of namespace
