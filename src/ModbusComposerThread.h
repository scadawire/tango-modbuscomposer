//+=============================================================================
//
// file :         ModbusComposerThread.h
//
// description :  Include for the ModbusComposerThread class.
//                This class is used for ModbusComposer cache thread
//
// project :      ModbusComposer TANGO Device Server 
//
// $Author: pons$
//
// $Revision: $
//
// $HeadURL: $
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================
#ifndef _MODBUSCOMPOSERTHREAD_H
#define _MODBUSCOMPOSERTHREAD_H

#include "ModbusComposer.h"

namespace ModbusComposer_ns
{

class ModbusComposerThread : public omni_thread, public Tango::LogAdapter
{
    
public:
	// Constructor.
	ModbusComposerThread(ModbusComposer *, omni_mutex &);	
	void *run_undetached(void *);
  time_t get_ticks();
			
private:
	ModbusComposer *ds;
  omni_mutex &mutex;
	time_t tickStart;
  string readError;
	bool readOK;
    
}; // class ModbusComposerThread

} // namespace ModbusComposer_ns

#endif // _MODBUSCOMPOSERTHREAD_H
