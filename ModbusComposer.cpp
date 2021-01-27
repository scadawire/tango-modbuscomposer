/*----- PROTECTED REGION ID(ModbusComposer.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";
//=============================================================================
//
// file :        ModbusComposer.cpp
//
// description : C++ source for the ModbusComposer class and its commands.
//               The class is derived from Device. It represents the
//               CORBA servant object which will be accessed from the
//               network. All commands which can be executed on the
//               ModbusComposer are implemented in this file.
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
#include <ModbusComposerClass.h>

/*----- PROTECTED REGION END -----*/	//	ModbusComposer.cpp

/**
 *  ModbusComposer class description:
 *    A class to create dynamic attributes above a Modbus interface
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name  |  Method name
//================================================================
//  State         |  dev_state
//  Status        |  Inherited (no method)
//================================================================

//================================================================
//  Attributes managed is:
//================================================================
//================================================================

namespace ModbusComposer_ns
{
/*----- PROTECTED REGION ID(ModbusComposer::namespace_starting) ENABLED START -----*/

//	static initializations

/*----- PROTECTED REGION END -----*/	//	ModbusComposer::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : ModbusComposer::ModbusComposer()
 *	Description : Constructors for a Tango device
 *                implementing the classModbusComposer
 */
//--------------------------------------------------------
ModbusComposer::ModbusComposer(Tango::DeviceClass *cl, string &s)
 : TANGO_BASE_CLASS(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(ModbusComposer::constructor_1) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::constructor_1
}
//--------------------------------------------------------
ModbusComposer::ModbusComposer(Tango::DeviceClass *cl, const char *s)
 : TANGO_BASE_CLASS(cl, s)
{
	/*----- PROTECTED REGION ID(ModbusComposer::constructor_2) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::constructor_2
}
//--------------------------------------------------------
ModbusComposer::ModbusComposer(Tango::DeviceClass *cl, const char *s, const char *d)
 : TANGO_BASE_CLASS(cl, s, d)
{
	/*----- PROTECTED REGION ID(ModbusComposer::constructor_3) ENABLED START -----*/
	init_device();
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : ModbusComposer::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void ModbusComposer::delete_device()
{
	DEBUG_STREAM << "ModbusComposer::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(ModbusComposer::delete_device) ENABLED START -----*/
	
	//	Delete device allocated objects
	if(modbusDS) delete modbusDS;
	for(int i=0;i<(int)stateMap.size();i++)
          delete stateMap[i].ep;          
        stateMap.clear();
	if( useCache || useCoilCache ) {
	  useCache=false;
	  useCoilCache=false;
	  cacheThread->join(NULL);
	}

	if( Tango::Util::instance()->is_svr_shutting_down() ||
            Tango::Util::instance()->is_device_restarting(get_name()) ) {
	  attMap.clear();
	  cmdMap.clear();
	}
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::delete_device
}

//--------------------------------------------------------
/**
 *	Method      : ModbusComposer::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void ModbusComposer::init_device()
{
	DEBUG_STREAM << "ModbusComposer::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(ModbusComposer::init_device_before) ENABLED START -----*/
	
	//	Initialization before get_device_property() call
        modbusDS = NULL;
	set_state(Tango::ON);
	set_status("Device is ON");
        stateMap.clear();
	useCache = false;
	useCoilCache = false;
	cacheThread = NULL;

	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::init_device_before
	

	//	Get the device properties from database
	get_device_property();
	
	/*----- PROTECTED REGION ID(ModbusComposer::init_device) ENABLED START -----*/
	
	// Modbus connection ----------------------------------------------------------

        if( modbus_name.length()==0 ) {
            cerr << "ERROR: modbus_device property not defined" << endl;
            exit(0);
        }

        try
        {
            modbusDS = new Tango::DeviceProxy(modbus_name);
        }
        catch(Tango::DevFailed &e)
        {
            cerr << "ERROR: cannot import modbus device " << e.errors[0].desc << endl;
            exit(0);
        }

	// Self connection ----------------------------------------------------------

        try
        {
            selfDS = new Tango::DeviceProxy(get_name());
        }
        catch(Tango::DevFailed &e)
        {
            cerr << "ERROR: cannot import self " << e.errors[0].desc << endl;
            exit(0);
        }

	// Dynamic states ----------------------------------------------------------

	for(int i=0;i<(int)dynamicStates.size();i++) {

          STATEITEM item;
          item.ep = new ExpParser(this);
	  
          try
          {
	    item.ep->SetExpression((char *)dynamicStates[i].c_str());
            item.ep->ParseState();
	  }
	  catch( Tango::DevFailed &e ) 
	  {
	    cerr << device_name << ":DynamicStates Parse Error in : " << dynamicStates[i] << endl;
	    cerr << e.errors[0].desc << endl;
            exit(0);
	  }

          item.state = item.ep->GetState();
          stateMap.push_back(item);
          
        }

	// Cache config ----------------------------------------------------------
	
	if( cacheConfig.size()>0 || coilCacheConfig.size()>0 ) {

          cacheOK = false;
	  cacheError = "Cache not initialized";

	  if( cacheConfig.size()==3 ) {
  	    useCache = true;
	    cacheStartAddress = cacheConfig[0];
	    cacheLength = cacheConfig[1];
	    cachePeriod = cacheConfig[2];
	  }

	  if( coilCacheConfig.size()==3 ) {
	    useCoilCache = true;
	    cacheCoilStartAddress = coilCacheConfig[0];
	    cacheCoilLength = coilCacheConfig[1];
	    cachePeriod = coilCacheConfig[2];
	  }

	  if( !useCache && !useCoilCache ) {
	    cerr << device_name << ": Invalid cache configuration, check  CacheConfig or CoilCacheConfig propery" << endl;
            exit(0);
          }

	  cacheThread = new ModbusComposerThread(this,cacheMutex);

        }

	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::init_device
}

//--------------------------------------------------------
/**
 *	Method      : ModbusComposer::get_device_property()
 *	Description : Read database to initialize property data members.
 */
//--------------------------------------------------------
void ModbusComposer::get_device_property()
{
	/*----- PROTECTED REGION ID(ModbusComposer::get_device_property_before) ENABLED START -----*/
	
	//	Initialize property data members
	modbus_name = "";
	dynamicAttributes.clear();
	dynamicCommands.clear();
	dynamicStates.clear();
	addressOffset = 0;
	defaultReadCommand = "ReadHoldingRegisters";
        cacheConfig.clear();
        coilCacheConfig.clear();

	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::get_device_property_before


	//	Read device properties from database.
	Tango::DbData	dev_prop;
	dev_prop.push_back(Tango::DbDatum("Modbus_name"));
	dev_prop.push_back(Tango::DbDatum("DynamicAttributes"));
	dev_prop.push_back(Tango::DbDatum("DynamicCommands"));
	dev_prop.push_back(Tango::DbDatum("DynamicStates"));
	dev_prop.push_back(Tango::DbDatum("AddressOffset"));
	dev_prop.push_back(Tango::DbDatum("DefaultReadCommand"));
	dev_prop.push_back(Tango::DbDatum("CacheConfig"));
	dev_prop.push_back(Tango::DbDatum("CoilCacheConfig"));

	//	is there at least one property to be read ?
	if (dev_prop.size()>0)
	{
		//	Call database and extract values
		if (Tango::Util::instance()->_UseDb==true)
			get_db_device()->get_property(dev_prop);
	
		//	get instance on ModbusComposerClass to get class property
		Tango::DbDatum	def_prop, cl_prop;
		ModbusComposerClass	*ds_class =
			(static_cast<ModbusComposerClass *>(get_device_class()));
		int	i = -1;

		//	Try to initialize Modbus_name from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  modbus_name;
		else {
			//	Try to initialize Modbus_name from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  modbus_name;
		}
		//	And try to extract Modbus_name value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  modbus_name;

		//	Try to initialize DynamicAttributes from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  dynamicAttributes;
		else {
			//	Try to initialize DynamicAttributes from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  dynamicAttributes;
		}
		//	And try to extract DynamicAttributes value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  dynamicAttributes;

		//	Try to initialize DynamicCommands from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  dynamicCommands;
		else {
			//	Try to initialize DynamicCommands from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  dynamicCommands;
		}
		//	And try to extract DynamicCommands value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  dynamicCommands;

		//	Try to initialize DynamicStates from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  dynamicStates;
		else {
			//	Try to initialize DynamicStates from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  dynamicStates;
		}
		//	And try to extract DynamicStates value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  dynamicStates;

		//	Try to initialize AddressOffset from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  addressOffset;
		else {
			//	Try to initialize AddressOffset from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  addressOffset;
		}
		//	And try to extract AddressOffset value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  addressOffset;

		//	Try to initialize DefaultReadCommand from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  defaultReadCommand;
		else {
			//	Try to initialize DefaultReadCommand from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  defaultReadCommand;
		}
		//	And try to extract DefaultReadCommand value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  defaultReadCommand;

		//	Try to initialize CacheConfig from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  cacheConfig;
		else {
			//	Try to initialize CacheConfig from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  cacheConfig;
		}
		//	And try to extract CacheConfig value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  cacheConfig;

		//	Try to initialize CoilCacheConfig from class property
		cl_prop = ds_class->get_class_property(dev_prop[++i].name);
		if (cl_prop.is_empty()==false)	cl_prop  >>  coilCacheConfig;
		else {
			//	Try to initialize CoilCacheConfig from default device value
			def_prop = ds_class->get_default_device_property(dev_prop[i].name);
			if (def_prop.is_empty()==false)	def_prop  >>  coilCacheConfig;
		}
		//	And try to extract CoilCacheConfig value from database
		if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  coilCacheConfig;

	}

	/*----- PROTECTED REGION ID(ModbusComposer::get_device_property_after) ENABLED START -----*/
	
	//	Check device property data members init
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::get_device_property_after
}

//--------------------------------------------------------
/**
 *	Method      : ModbusComposer::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void ModbusComposer::always_executed_hook()
{
	DEBUG_STREAM << "ModbusComposer::always_executed_hook()  " << device_name << endl;
	/*----- PROTECTED REGION ID(ModbusComposer::always_executed_hook) ENABLED START -----*/
	
	//	code always executed before all requests
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : ModbusComposer::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void ModbusComposer::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "ModbusComposer::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(ModbusComposer::read_attr_hardware) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::read_attr_hardware
}


//--------------------------------------------------------
/**
 *	Method      : ModbusComposer::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void ModbusComposer::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(ModbusComposer::add_dynamic_attributes) ENABLED START -----*/


	for(int i=0;i<(int)dynamicAttributes.size();i++) {

    DEBUG_STREAM << "ModbusComposer::add_dynamic_attributes() Parse: " <<  dynamicAttributes[i] << endl;

	  // Expression parser
	  ExpParser *ep = new ExpParser(this);
	  ep->SetExpression((char *)dynamicAttributes[i].c_str());
	  
	  try {	  
	    ep->ParseAttribute();
    } catch( Tango::DevFailed &e ) {
	    cerr << device_name << ":Parse Error in : " << dynamicAttributes[i] << endl;
	    cerr << e.errors[0].desc << endl;
            Tango::Except::throw_exception(
              (const char *)"ModbusComposer::error_read",
              (const char *)"Wrong dynamic attribute configuration",
              (const char *)"ModbusComposer::add_dynamic_attributes");
	  }

	  // READ/WRITE type
	  Tango::AttrWriteType rwType;
	  if( ep->HasWriteExpression() ) {
      rwType = Tango::READ_WRITE;
    } else {
      rwType = Tango::READ;
    }
	    
	  // Create attribute	  	  
    try {

	    if( !ep->IsSpectrum() ) {
	      DynAttribute *att = new DynAttribute(ep->GetName(),ep->GetType(),rwType);
        add_attribute(att);
	    } else {
	      DynSpecAttribute *att = new DynSpecAttribute(ep->GetName(),ep->GetType(),MAXVALUELENGTH);
	      add_attribute(att);
	    }

	    attMap.add(string(ep->GetName()),ep);

    } catch(Tango::DevFailed &e) {
      cerr << "Warning: Cannot add " << ep->GetName() << " : " << e.errors[0].desc << endl;
    }

	}
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Command State related method
 *	Description: This command gets the device state (stored in its device_state data member) and returns it to the caller.
 *
 *	@returns Device state
 */
//--------------------------------------------------------
Tango::DevState ModbusComposer::dev_state()
{
	DEBUG_STREAM << "ModbusComposer::State()  - " << device_name << endl;
	/*----- PROTECTED REGION ID(ModbusComposer::dev_state) ENABLED START -----*/
	
	Tango::DevState	argout = Tango::ON;
	string statusStr = "";

	bool found = false;
	int i=0;
        VALUE r;
        try
        {

	  // Find the first active state
	  while(!found && i<(int)stateMap.size()) {
	  
            stateMap[i].ep->EvaluateRead(&r);
            found = stateMap[i].ep->GetBoolResult(r);
            if(!found) i++;

	  }

	  if( !found ) {

	    argout = Tango::ON;
            statusStr = "The device is ON";

          } else {

	    string epStatus;
	    argout = stateMap[i].ep->GetState();
            statusStr = "The device is " + string(Tango::DevStateName[argout]) + "\n";

	    // Concat status string of the current state
	    epStatus = string(stateMap[i].ep->GetStatus());
            if(epStatus.length()>0) statusStr = epStatus + "\n";
	    i++;

	    // Concact status of other active state
            while(i<(int)stateMap.size() && stateMap[i].ep->GetState()==argout) {
          
              stateMap[i].ep->EvaluateRead(&r);
	      if( stateMap[i].ep->GetBoolResult(r) ) {
	        epStatus = string(stateMap[i].ep->GetStatus());
                if(epStatus.length()>0) statusStr += epStatus + "\n";
	      }
	      i++;

	    }
	  
          }

        } catch(Tango::DevFailed &e) {

	  argout = Tango::UNKNOWN;
	  statusStr = string(e.errors[0].desc);

        }

	set_status(statusStr);

	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::dev_state
	set_state(argout);    // Give the state to Tango.
	if (argout!=Tango::ALARM)
		DeviceImpl::dev_state();
	return get_state();  // Return it after Tango management.
}
//--------------------------------------------------------
/**
 *	Command DynCommand related method
 *	Description: 
 *
 */
//--------------------------------------------------------
void ModbusComposer::dyn_command(Tango::Command &command)
{
	DEBUG_STREAM << "ModbusComposer::" << command.get_name() << "  - " << device_name << endl;
	/*----- PROTECTED REGION ID(ModbusComposer::dyn_command) ENABLED START -----*/
	
	ATTITEM *item = cmdMap.get(command.get_name());
	if( item==NULL ) {
	  Tango::Except::throw_exception(	    
	  (const char *)"ModbusComposer::error_write",
	  (const char *)"Unknown command",
	  (const char *)"ModbusComposer::dyn_command");	
	  }

 	item->ep->EvaluateWrite(0.0);
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::dyn_command
}
//--------------------------------------------------------
/**
 *	Method      : ModbusComposer::add_dynamic_commands()
 *	Description : Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void ModbusComposer::add_dynamic_commands()
{
	//	Example to add dynamic command:
	//	Copy inside the folowing protected area to instanciate at startup.
	//	add_DynCommand_dynamic_command("MyDynCommandCommand", true);
	
	/*----- PROTECTED REGION ID(ModbusComposer::add_dynamic_commands) ENABLED START -----*/
	
	for(int i=0;i<(int)dynamicCommands.size();i++) {

	  // Expression parser
	  ExpParser *ep = new ExpParser(this);
	  ep->SetExpression((char *)dynamicCommands[i].c_str());
	  
	  try {	  
	    ep->ParseCommand();
          } catch( Tango::DevFailed &e ) {	  
	    cerr << device_name << ":Parse Error in : " << dynamicCommands[i] << endl;
	    cerr << e.errors[0].desc << endl;
            Tango::Except::throw_exception(
              (const char *)"ModbusComposer::error_read",
              (const char *)"Wrong dynamic command configuration",
              (const char *)"ModbusComposer::add_dynamic_commands");
	  }
	    
	  // Create command	  	  
	  DynCommandClass *cmd = new DynCommandClass(ep->GetName(),Tango::DEV_VOID,Tango::DEV_VOID);
          add_command(cmd);
	  
	  cmdMap.add(string(ep->GetName()),ep);
	
	}

	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposer::add_dynamic_commands
}

/*----- PROTECTED REGION ID(ModbusComposer::namespace_ending) ENABLED START -----*/

//	Additional Methods

short ModbusComposer::coil(short address) {
  
  short ret;

  if( useCoilCache ) {

    omni_mutex_lock sync(cacheMutex);

    int idx = address-cacheCoilStartAddress;
      
    if( !cacheOK ) {
       Tango::Except::throw_exception(
         (const char *)"ModbusComposer::error_cache_read",
         (const char *)cacheError.c_str(),
         (const char *)"ModbusComposer::coil");
    }

    if(idx<0 || idx>=(int)cacheCoilBuffer.size()) {
       Tango::Except::throw_exception(
         (const char *)"ModbusComposer::error_cahce_read",
         (const char *)"Coil reading out of cache range",
         (const char *)"ModbusComposer::coil");
    }

    ret = cacheCoilBuffer[idx];

  } else {
  
    Tango::DeviceData argout;
    Tango::DeviceData argin;
    vector<short> input;
    vector<short> output;
          
    input.push_back(address);
    input.push_back(1);
    argin << input;
    argout = modbusDS->command_inout("ReadMultipleCoilsStatus",argin);
    argout >> output;

    ret = output[0];

  }

  return ret;

}

vector<short> ModbusComposer::coils(short address,int length) {
  
  vector<short> ret;

  if( useCoilCache ) {

    omni_mutex_lock sync(cacheMutex);

    int idx = address-cacheCoilStartAddress;
      
    if( !cacheOK ) {
       Tango::Except::throw_exception(
         (const char *)"ModbusComposer::error_cache_read",
         (const char *)cacheError.c_str(),
         (const char *)"ModbusComposer::coils");
    }

    if(idx<0 || idx+length>(int)cacheCoilBuffer.size()) {
       Tango::Except::throw_exception(
         (const char *)"ModbusComposer::error_cahce_read",
         (const char *)"Register reading out of cache range",
         (const char *)"ModbusComposer::coils");
    }

    for(int i=0;i<length;i++)
      ret.push_back(cacheCoilBuffer[idx+i]);

  } else {
  
    Tango::DeviceData argout;
    Tango::DeviceData argin;
    vector<short> input;
    vector<short> output;
          
    input.push_back(address);
    input.push_back(length);
    argin << input;
    argout = modbusDS->command_inout("ReadMultipleCoilsStatus",argin);
    argout >> output;

    ret = output;

  }

  return ret;

}

short ModbusComposer::reg(short address) {
  
  short ret;

  if( useCache ) {

    omni_mutex_lock sync(cacheMutex);

    int idx = (address+addressOffset)-cacheStartAddress;
      
    if( !cacheOK ) {
       Tango::Except::throw_exception(
         (const char *)"ModbusComposer::error_cache_read",
         (const char *)cacheError.c_str(),
         (const char *)"ModbusComposer::reg");
    }

    if(idx<0 || idx>=(int)cacheBuffer.size()) {
       Tango::Except::throw_exception(
         (const char *)"ModbusComposer::error_cahce_read",
         (const char *)"Register reading out of cache range",
         (const char *)"ModbusComposer::reg");
    }

    ret = cacheBuffer[idx];

  } else {
  
    Tango::DeviceData argout;
    Tango::DeviceData argin;
    vector<short> input;
    vector<short> output;
          
    input.push_back(address+addressOffset);
    input.push_back(1);
    argin << input;
    argout = modbusDS->command_inout(defaultReadCommand.c_str(),argin);
    argout >> output;

    ret = output[0];

  }

  return ret;

}

vector<short> ModbusComposer::regs(short address,int length) {

  vector<short> ret;

  if( useCache ) {

    omni_mutex_lock sync(cacheMutex);

    int idx = (address+addressOffset)-cacheStartAddress;
      
    if( !cacheOK ) {
       Tango::Except::throw_exception(
         (const char *)"ModbusComposer::error_cache_read",
         (const char *)cacheError.c_str(),
         (const char *)"ModbusComposer::regs");
    }

    if(idx<0 || idx+length>(int)cacheBuffer.size()) {
       Tango::Except::throw_exception(
         (const char *)"ModbusComposer::error_cahce_read",
         (const char *)"Register reading out of cache range",
         (const char *)"ModbusComposer::regs");
    }

    for(int i=0;i<length;i++)
      ret.push_back(cacheBuffer[idx+i]);

  } else {

    Tango::DeviceData argout;
    Tango::DeviceData argin;
    vector<short> input;
    vector<short> output;
          
    input.push_back(address+addressOffset);
    input.push_back(length);
    argin << input;
    argout = modbusDS->command_inout(defaultReadCommand.c_str(),argin);
    argout >> output;
    ret = output;

  }

  return ret;

}

void ModbusComposer::write_coil(short address,short value) {

  Tango::DeviceData argin;
  vector<short> input;

  input.push_back(address+addressOffset);
  input.push_back(value);
  argin << input;
  
  modbusDS->command_inout("ForceSingleCoil",argin);
                    
}

// Global mutex to protect the write bit method in case
// of serveral modbuscomposer devices in the same server 
// accessing the same register.
omni_mutex regAccess;

void ModbusComposer::write_bit(short address,short bitIdx,short value) {

  Tango::DeviceData argout;
  Tango::DeviceData argin;
  vector<short> input;
  vector<short> ret;
  unsigned short mask = 1 << bitIdx;
  unsigned short val = (value!=0) << bitIdx;

  omni_mutex_lock oml(regAccess);

  input.push_back(address+addressOffset);
  input.push_back(1);
  argin << input;
  argout = modbusDS->command_inout(defaultReadCommand.c_str(),argin);
  argout >> ret;

  ret[0] &= ~mask;
  ret[0] |= val;

  input.clear();
  input.push_back(address+addressOffset);
  input.push_back(ret[0]);
  argin << input;  
  modbusDS->command_inout("PresetSingleRegister",argin);
                    
}

void ModbusComposer::write_reg(short address,short value) {

  Tango::DeviceData argin;
  vector<short> input;

  input.push_back(address+addressOffset);
  input.push_back(value);
  argin << input;  
  modbusDS->command_inout("PresetSingleRegister",argin);
                    
}

void ModbusComposer::write_regs(short address,vector<short> &value) {

  Tango::DeviceData argin;
  vector<short> input;

  input.push_back(address+addressOffset);
  input.push_back((short)value.size());
  for(int i=0;i<(int)value.size();i++)
    input.push_back(value[i]);
  argin << input;
  
  modbusDS->command_inout("PresetMultipleRegisters",argin);
                    
}

double ModbusComposer::read_self_attribute(char *attName) {

  Tango::DeviceAttribute da;
  da = selfDS->read_attribute(attName);

  switch(da.get_type()) {

    case Tango::DEV_BOOLEAN:
    {
      Tango::DevBoolean v;
      da >> v;
      return (double)v;
    }
    break;
    case Tango::DEV_SHORT:
    {
      Tango::DevShort v;
      da >> v;
      return (double)v;
    }
    break;
    case Tango::DEV_USHORT:
    {
      Tango::DevUShort v;
      da >> v;
      return (double)v;
    }
    break;
    case Tango::DEV_LONG:
    {
      Tango::DevLong v;
      da >> v;
      return (double)v;
    }
    break;
    case Tango::DEV_ULONG:
    {
      Tango::DevULong v;
      da >> v;
      return (double)v;
    }
    break;
    case Tango::DEV_DOUBLE:
    {
      Tango::DevDouble v;
      da >> v;
      return v;
    }
    break;
    case Tango::DEV_FLOAT:
    {
      Tango::DevFloat v;
      da >> v;
      return (double)v;
    }
    break;
    default:
       Tango::Except::throw_exception(
         (const char *)"ModbusComposer::error_read",
         (const char *)"Cannot read attribute (type not supported)",
         (const char *)"ModbusComposer::read_self_attribute");

  }

  return 0.0;

}

void ModbusComposer::read_dyn_attributes(Tango::Attribute &attr,DynAttribute *src) {

  ATTITEM *item = attMap.get(attr.get_name());
  if( item==NULL ) {
    Tango::Except::throw_exception(	    
      (const char *)"ModbusComposer::error_read",
      (const char *)"Unknown attribute",
      (const char *)"ModbusComposer::read_dyn_attributes");	
  }

  src->get_value(item->ep,attr);
  
}

void ModbusComposer::write_dyn_attributes(Tango::WAttribute &attr,DynAttribute *src) {
  
  double wValue;

  switch(src->get_type()) {
       
    case Tango::DEV_BOOLEAN:
    {
      Tango::DevBoolean	w_val;
      attr.get_write_value(w_val);
      wValue = (double)w_val;
    }
    break;
	 
    case Tango::DEV_SHORT:
    {
      Tango::DevShort	w_val;
      attr.get_write_value(w_val);
      wValue = (double)w_val;
    }
    break;

    case Tango::DEV_USHORT:
    {
      Tango::DevUShort	w_val;
      attr.get_write_value(w_val);
      wValue = (double)w_val;
    }
    break;
	 
    case Tango::DEV_DOUBLE:
    {
      Tango::DevDouble	w_val;
      attr.get_write_value(w_val);
      wValue = w_val;
    }
    break;

    case Tango::DEV_FLOAT:
    {
      Tango::DevFloat	w_val;
      attr.get_write_value(w_val);
      wValue = (double)w_val;
    }
    break;

    case Tango::DEV_LONG:
    {
      Tango::DevLong	w_val;
      attr.get_write_value(w_val);
      wValue = (double)w_val;
    }
    break;

    case Tango::DEV_ULONG:
    {
      Tango::DevULong	w_val;
      attr.get_write_value(w_val);
      wValue = (double)w_val;
    }
    break;
    
    //to delete warning build
    default:
        wValue = NAN;
	        
  }

  ATTITEM *item = attMap.get(attr.get_name());
  if( item==NULL ) {
    Tango::Except::throw_exception(	    
      (const char *)"ModbusComposer::error_write",
      (const char *)"Unknown attribute",
      (const char *)"ModbusComposer::write_dyn_attributes");	
  }

  src->set_value(item->ep,wValue);

}

void ModbusComposer::read_dynspec_attributes(Tango::Attribute &attr,DynSpecAttribute *src) {
    
  ATTITEM *item = attMap.get(attr.get_name());
  if( item==NULL ) {
    Tango::Except::throw_exception(	    
      (const char *)"ModbusComposer::error_read",
      (const char *)"Unknown attribute",
      (const char *)"ModbusComposer::read_dynspec_attributes");	
  }

  src->get_value(item->ep,attr);
  
}

void ModbusComposer::write_dynspec_attributes(Tango::WAttribute &attr,DynSpecAttribute *src) {
}

/*----- PROTECTED REGION END -----*/	//	ModbusComposer::namespace_ending
} //	namespace
