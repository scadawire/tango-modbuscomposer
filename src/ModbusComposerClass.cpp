/*----- PROTECTED REGION ID(ModbusComposerClass.cpp) ENABLED START -----*/
static const char *RcsId      = "$Id:  $";
static const char *TagName    = "$Name:  $";
static const char *CvsPath    = "$Source:  $";
static const char *SvnPath    = "$HeadURL:  $";
static const char *HttpServer = "http://www.esrf.eu/computing/cs/tango/tango_doc/ds_doc/";
//=============================================================================
//
// file :        ModbusComposerClass.cpp
//
// description : C++ source for the ModbusComposerClass.
//               A singleton class derived from DeviceClass.
//               It implements the command and attribute list
//               and all properties and methods required
//               by the ModbusComposer once per process.
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


#include "ModbusComposerClass.h"

/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass.cpp

//-------------------------------------------------------------------
/**
 *	Create ModbusComposerClass singleton and
 *	return it in a C function for Python usage
 */
//-------------------------------------------------------------------
extern "C" {
#ifdef _TG_WINDOWS_

__declspec(dllexport)

#endif

	Tango::DeviceClass *_create_ModbusComposer_class(const char *name) {
		return ModbusComposer_ns::ModbusComposerClass::init(name);
	}
}

namespace ModbusComposer_ns
{
//===================================================================
//	Initialize pointer for singleton pattern
//===================================================================
ModbusComposerClass *ModbusComposerClass::_instance = NULL;

//===================================================================
//	Class constants
//===================================================================
//--------------------------------------------------------
/**
 * method : 		ModbusComposerClass::ModbusComposerClass(std::string &s)
 * description : 	constructor for the ModbusComposerClass
 *
 * @param s	The class name
 */
//--------------------------------------------------------
ModbusComposerClass::ModbusComposerClass(std::string &s):Tango::DeviceClass(s)
{
	TANGO_LOG_INFO << "Entering ModbusComposerClass constructor" << std::endl;
	set_default_property();
	write_class_property();

	/*----- PROTECTED REGION ID(ModbusComposerClass::constructor) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::constructor

	TANGO_LOG_INFO << "Leaving ModbusComposerClass constructor" << std::endl;
}

//--------------------------------------------------------
/**
 * method : 		ModbusComposerClass::~ModbusComposerClass()
 * description : 	destructor for the ModbusComposerClass
 */
//--------------------------------------------------------
ModbusComposerClass::~ModbusComposerClass()
{
	/*----- PROTECTED REGION ID(ModbusComposerClass::destructor) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::destructor

	_instance = NULL;
}


//--------------------------------------------------------
/**
 * method : 		ModbusComposerClass::init
 * description : 	Create the object if not already done.
 *                  Otherwise, just return a pointer to the object
 *
 * @param	name	The class name
 */
//--------------------------------------------------------
ModbusComposerClass *ModbusComposerClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			std::string s(name);
			_instance = new ModbusComposerClass(s);
		}
		catch (std::bad_alloc &)
		{
			throw;
		}
	}
	return _instance;
}

//--------------------------------------------------------
/**
 * method : 		ModbusComposerClass::instance
 * description : 	Check if object already created,
 *                  and return a pointer to the object
 */
//--------------------------------------------------------
ModbusComposerClass *ModbusComposerClass::instance()
{
	if (_instance == NULL)
	{
		std::cerr << "Class is not initialized !!" << std::endl;
		exit(-1);
	}
	return _instance;
}



//===================================================================
//	Command execution method calls
//===================================================================
//--------------------------------------------------------
/**
 * method : 		DynCommandClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *DynCommandClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	TANGO_LOG_INFO << "DynCommandClass::execute(): arrived" << std::endl;
	((static_cast<ModbusComposer *>(device))->dyn_command(*this));
	return new CORBA::Any();
}


//===================================================================
//	Properties management
//===================================================================
//--------------------------------------------------------
/**
 *	Method     : ModbusComposerClass::get_class_property()
 *	Description: Get the class property for specified name.
 */
//--------------------------------------------------------
Tango::DbDatum ModbusComposerClass::get_class_property(std::string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, returns  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method     : ModbusComposerClass::get_default_device_property()
 *	Description: Return the default value for device property.
 */
//--------------------------------------------------------
Tango::DbDatum ModbusComposerClass::get_default_device_property(std::string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method     : ModbusComposerClass::get_default_class_property()
 *	Description: Return the default value for class property.
 */
//--------------------------------------------------------
Tango::DbDatum ModbusComposerClass::get_default_class_property(std::string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}


//--------------------------------------------------------
/**
 *	Method     : ModbusComposerClass::set_default_property()
 *	Description: Set default property (class and device) for wizard.
 *                For each property, add to wizard property name and description.
 *                If default value has been set, add it to wizard property and
 *                store it in a DbDatum.
 */
//--------------------------------------------------------
void ModbusComposerClass::set_default_property()
{
	std::string	prop_name;
	std::string	prop_desc;
	std::string	prop_def;
	std::vector<std::string>	vect_data;

	//	Set Default Class Properties

	//	Set Default device Properties
	prop_name = "Modbus_name";
	prop_desc = "Name of the Modbus device";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "DynamicAttributes";
	prop_desc = "List of dynacmic attributes\nSee <a href=grammar.html>grammar.html</a>";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "DynamicCommands";
	prop_desc = "List of dynacmic attributes\nSee <a href=grammar.html>grammar.html</a>";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "DynamicStates";
	prop_desc = "State definitions (Default state is ON)\nSee <a href=grammar.html>grammar.html</a>";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "DynamicStatus";
	prop_desc = "Status definitions\nSee <a href=grammar.html>grammar.html</a>";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "AddressOffset";
	prop_desc = "Integer offset added to Addresses in every Modbus command call";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "DefaultReadCommand";
	prop_desc = "Command use to read Modbus registers\n(eg. ReadHoldingRegisters)";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "CacheConfig";
	prop_desc = "Cache configuration, the read command is the DefaultReadCommand (if any)\n\n[0] = Start address\n[1] = Number of register\n[2] = Refresh period (ms)";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
	prop_name = "CoilCacheConfig";
	prop_desc = "Coil Cache configuration, the read can be only ReadMultipleCoilStatus\n\n[0] = Start address\n[1] = Number of coil\n[2] = Refresh period (ms)";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
}

//--------------------------------------------------------
/**
 *	Method     : ModbusComposerClass::write_class_property()
 *	Description: Set class description fields as property in database
 */
//--------------------------------------------------------
void ModbusComposerClass::write_class_property()
{
	//	First time, check if database used
	if (Tango::Util::_UseDb == false)
		return;

	Tango::DbData	data;
	std::string	classname = get_name();
	std::string	header;

	//	Put title
	Tango::DbDatum	title("ProjectTitle");
	std::string	str_title("ModbusComposer");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	std::vector<std::string>	str_desc;
	str_desc.push_back("A class to create dynamic attributes above a Modbus interface");
	description << str_desc;
	data.push_back(description);

	//  Put inheritance
	Tango::DbDatum	inher_datum("InheritedFrom");
	std::vector<std::string> inheritance;
	inheritance.push_back("TANGO_BASE_CLASS");
	inher_datum << inheritance;
	data.push_back(inher_datum);

	//	Call database and and values
	get_db_class()->put_property(data);
}

//===================================================================
//	Factory methods
//===================================================================

//--------------------------------------------------------
/**
 *	Method     : ModbusComposerClass::device_factory()
 *	Description: Create the device object(s)
 *                and store them in the device list
 */
//--------------------------------------------------------
void ModbusComposerClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
	/*----- PROTECTED REGION ID(ModbusComposerClass::device_factory_before) ENABLED START -----*/
	
	//	Add your own code
	//	Create devices and add it into the device list
	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i].in() << endl;
		device_list.push_back(new ModbusComposer(this, (*devlist_ptr)[i]));
	}

	//	DO NOT CALL erase. Conflict with Yat dyn Attrs -> device crashes!!!
	// erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());

	//	Export devices to the outside world
	for (unsigned long i=1 ; i<=devlist_ptr->length() ; i++)
	{
		//	Add dynamic attributes if any
		ModbusComposer *dev = static_cast<ModbusComposer *>(device_list[device_list.size()-i]);
		dev->add_dynamic_attributes();
		dev->add_dynamic_commands();

		//	Check before if database used.
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(dev);
		else
			export_device(dev, dev->get_name().c_str());
	}
	return;	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::device_factory_before

	//	Create devices and add it into the device list
	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		TANGO_LOG_DEBUG << "Device name : " << (*devlist_ptr)[i].in() << std::endl;
		device_list.push_back(new ModbusComposer(this, (*devlist_ptr)[i]));
	}

	//	Manage dynamic attributes if any
	erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());

	//	Export devices to the outside world
	for (unsigned long i=1 ; i<=devlist_ptr->length() ; i++)
	{
		//	Add dynamic attributes if any
		ModbusComposer *dev = static_cast<ModbusComposer *>(device_list[device_list.size()-i]);
		dev->add_dynamic_attributes();
		dev->add_dynamic_commands();

		//	Check before if database used.
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(dev);
		else
			export_device(dev, dev->get_name().c_str());
	}

	/*----- PROTECTED REGION ID(ModbusComposerClass::device_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::device_factory_after
}
//--------------------------------------------------------
/**
 *	Method     : ModbusComposerClass::attribute_factory()
 *	Description: Create the attribute object(s)
 *                and store them in the attribute list
 */
//--------------------------------------------------------
void ModbusComposerClass::attribute_factory(std::vector<Tango::Attr *> &)
{
	/*----- PROTECTED REGION ID(ModbusComposerClass::attribute_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::attribute_factory_before

	//	Create a list of static attributes
	create_static_attribute_list(get_class_attr()->get_attr_list());
	/*----- PROTECTED REGION ID(ModbusComposerClass::attribute_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::attribute_factory_after
}
//--------------------------------------------------------
/**
 *	Method     : ModbusComposerClass::pipe_factory()
 *	Description: Create the pipe object(s)
 *                and store them in the pipe list
 */
//--------------------------------------------------------
void ModbusComposerClass::pipe_factory()
{
	/*----- PROTECTED REGION ID(ModbusComposerClass::pipe_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::pipe_factory_before
	/*----- PROTECTED REGION ID(ModbusComposerClass::pipe_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::pipe_factory_after
}
//--------------------------------------------------------
/**
 *	Method     : ModbusComposerClass::command_factory()
 *	Description: Create the command object(s)
 *                and store them in the command list
 */
//--------------------------------------------------------
void ModbusComposerClass::command_factory()
{
	/*----- PROTECTED REGION ID(ModbusComposerClass::command_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::command_factory_before


	/*----- PROTECTED REGION ID(ModbusComposerClass::command_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::command_factory_after
}

//===================================================================
//	Dynamic attributes related methods
//===================================================================

//--------------------------------------------------------
/**
 * method : 		ModbusComposerClass::create_static_attribute_list
 * description : 	Create the a list of static attributes
 *
 * @param	att_list	the created attribute list
 */
//--------------------------------------------------------
void ModbusComposerClass::create_static_attribute_list(std::vector<Tango::Attr *> &att_list)
{
	for (unsigned long i=0 ; i<att_list.size() ; i++)
	{
		std::string att_name(att_list[i]->get_name());
		std::transform(att_name.begin(), att_name.end(), att_name.begin(), ::tolower);
		defaultAttList.push_back(att_name);
	}

	TANGO_LOG_INFO << defaultAttList.size() << " attributes in default list" << std::endl;

	/*----- PROTECTED REGION ID(ModbusComposerClass::create_static_att_list) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::create_static_att_list
}


//--------------------------------------------------------
/**
 * method : 		ModbusComposerClass::erase_dynamic_attributes
 * description : 	delete the dynamic attributes if any.
 *
 * @param	devlist_ptr	the device list pointer
 * @param	list of all attributes
 */
//--------------------------------------------------------
void ModbusComposerClass::erase_dynamic_attributes(const Tango::DevVarStringArray *devlist_ptr, std::vector<Tango::Attr *> &att_list)
{
	Tango::Util *tg = Tango::Util::instance();

	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		Tango::DeviceImpl *dev_impl = tg->get_device_by_name(((std::string)(*devlist_ptr)[i]).c_str());
		ModbusComposer *dev = static_cast<ModbusComposer *> (dev_impl);

		std::vector<Tango::Attribute *> &dev_att_list = dev->get_device_attr()->get_attribute_list();
		std::vector<Tango::Attribute *>::iterator ite_att;
		for (ite_att=dev_att_list.begin() ; ite_att != dev_att_list.end() ; ++ite_att)
		{
			std::string att_name((*ite_att)->get_name_lower());
			if ((att_name == "state") || (att_name == "status"))
				continue;
			std::vector<std::string>::iterator ite_str = find(defaultAttList.begin(), defaultAttList.end(), att_name);
			if (ite_str == defaultAttList.end())
			{
				TANGO_LOG_INFO << att_name << " is a UNWANTED dynamic attribute for device " << (*devlist_ptr)[i] << std::endl;
				Tango::Attribute &att = dev->get_device_attr()->get_attr_by_name(att_name.c_str());
				dev->remove_attribute(att_list[att.get_attr_idx()], true, false);
				--ite_att;
			}
		}
	}
	/*----- PROTECTED REGION ID(ModbusComposerClass::erase_dynamic_attributes) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::erase_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method     : ModbusComposerClass::get_attr_object_by_name()
 *	Description: returns Tango::Attr * object found by name
 */
//--------------------------------------------------------
Tango::Attr *ModbusComposerClass::get_attr_object_by_name(std::vector<Tango::Attr *> &att_list, std::string attname)
{
	std::vector<Tango::Attr *>::iterator it;
	for (it=att_list.begin() ; it<att_list.end() ; ++it)
		if ((*it)->get_name()==attname)
			return (*it);
	//	Attr does not exist
	return NULL;
}


/*----- PROTECTED REGION ID(ModbusComposerClass::Additional Methods) ENABLED START -----*/

/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::Additional Methods
} //	namespace
