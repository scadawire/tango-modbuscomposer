//+=============================================================================
//
// file :         ModbusComposerThread.cpp
//
//
// description :  This class is used for ModbusComposer cache thread
//
// project :      ModbusComposer TANGO Device Server 
//
// $Author: pons$
//
// $Revision: $
//
// $Log: $
//
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================
namespace ModbusComposer_ns
{
class ModbusComposerThread;
}

#include <ModbusComposerThread.h>


namespace ModbusComposer_ns
{

  // Constructor:
  ModbusComposerThread::ModbusComposerThread(ModbusComposer *modbuscomposer, omni_mutex &m):
    Tango::LogAdapter(modbuscomposer), mutex(m), ds(modbuscomposer)
  {
    INFO_STREAM << "ModbusComposerThread::ModbusComposerThread(): entering." << endl;
    tickStart = -1;
    start_undetached();
    readError = "Cache not yet initialised";
    readOK = false;
  }

  // ----------------------------------------------------------------------------------------

  void *ModbusComposerThread::run_undetached(void *arg)
  {
      
    while(ds->useCache) {
          
      time_t t0 = get_ticks();

      Tango::DeviceData argout;
      Tango::DeviceData argin;
      vector<short> input;
      vector<short> output;

      try {
      
        input.push_back(ds->cacheStartAddress);
        input.push_back(ds->cacheLength);
        argin << input;
        argout = ds->modbusDS->command_inout(ds->defaultReadCommand.c_str(),argin);
        argout >> output;
	readOK = true;

      } catch( Tango::DevFailed &e ) {

	readError = e.errors[0].desc;
	readOK = false;

      }

      // Update share part
      mutex.lock();
      ds->cacheOK =  readOK;
      if( readOK ) {
        ds->cacheBuffer = output;
	ds->cacheError = "";
      } else {
	ds->cacheError = readError;
	ds->cacheBuffer.clear();
      }
      mutex.unlock();

      time_t t1 = get_ticks();    
      time_t toSleep = (time_t)(ds->cachePeriod) - (t1-t0);
      usleep(toSleep*1000);
      
    }
            
  }

  //----------------------------------------------------------------------------
  // Return number of milliseconds
  //-----------------------------------------------------------------------------
  time_t ModbusComposerThread::get_ticks() {
      
    if(tickStart < 0 )
      tickStart = time(NULL);
    
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return ( (tv.tv_sec-tickStart)*1000 + tv.tv_usec/1000 );

  }  

} // namespace ModbusComposer_ns
