/*----- PROTECTED REGION ID(ModbusComposerClass.h) ENABLED START -----*/
//=============================================================================
//
// file :        ModbusComposerClass.h
//
// description : Include for the ModbusComposer root class.
//               This class is the singleton class for
//                the ModbusComposer device class.
//               It contains all properties and methods which the 
//               ModbusComposer requires only once e.g. the commands.
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
// $Author: pons $
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


#ifndef ModbusComposerClass_H
#define ModbusComposerClass_H

// #include <tango.h>
#include "ModbusComposer.h"
#include "ExpParser.h"


/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass.h


namespace ModbusComposer_ns
{
/*----- PROTECTED REGION ID(ModbusComposerClass::classes for dynamic creation) ENABLED START -----*/

// Dynamic scalar attribute
class DynAttribute: public Tango::Attr {
   
   public:
   
   //-----------------------------------------------------------------------------
   // Constructor
   
   DynAttribute(const char *attName,long data_type,Tango::AttrWriteType rwType)
	       :Attr(attName,data_type,rwType) {    

   };

   void set_value(ExpParser *_ep,double wValue) {

        _ep->EvaluateWrite(wValue);
      
   }

    void get_value(ExpParser *_ep,Tango::Attribute &attr) {

     if( get_type()==Tango::DEV_STRING ) {

       _ep->EvaluateString();
       attr.set_value(&_ep->strValue);

     } else {

       VALUE r;
       _ep->EvaluateRead(&r);

       switch (get_type()) {

         case Tango::DEV_BOOLEAN: {
           _ep->bValue = _ep->GetBoolResult(r);
           attr.set_value(&_ep->bValue);
         }
           break;

         case Tango::DEV_SHORT: {
           _ep->sValue = (Tango::DevShort) _ep->GetIntegerValue(r.value[0]);
           attr.set_value(&_ep->sValue);
         }
           break;

         case Tango::DEV_USHORT: {
           _ep->usValue = (Tango::DevUShort) _ep->GetIntegerValue(r.value[0]);
           attr.set_value(&_ep->usValue);
         }
           break;

         case Tango::DEV_DOUBLE: {
           _ep->dValue = r.value[0];
           attr.set_value(&_ep->dValue);
         }
           break;

         case Tango::DEV_FLOAT: {
           _ep->fValue = (Tango::DevFloat) r.value[0];
           attr.set_value(&_ep->fValue);
         }
           break;

         case Tango::DEV_LONG: {
           _ep->lValue = (Tango::DevLong) _ep->GetIntegerValue(r.value[0]);
           attr.set_value(&_ep->lValue);
         }
           break;

         case Tango::DEV_ULONG: {
           _ep->ulValue = (Tango::DevULong) _ep->GetIntegerValue(r.value[0]);
           attr.set_value(&_ep->ulValue);
         }
           break;

       }

     }
     
   }
   
   
   ~DynAttribute() {
   };
      
   
   //-----------------------------------------------------------------------------
   // Write/Read mehod
   
   virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
        (static_cast<ModbusComposer *>(dev))->read_dyn_attributes(att,this);}
   virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
        (static_cast<ModbusComposer *>(dev))->write_dyn_attributes(att,this);}
	  
};

// Dynamic spectrum attribute
class DynSpecAttribute: public Tango::SpectrumAttr {
   
   public:
   
   //-----------------------------------------------------------------------------
   // Constructor
   
   DynSpecAttribute(const char *attName,long data_type,long max)
	       :SpectrumAttr(attName,data_type,max) {
	                
   };
   
   void get_value(ExpParser *_ep,Tango::Attribute &attr) {

     VALUE r;
     _ep->EvaluateRead(&r);
     
     switch(get_type()) {
       	 
       case Tango::DEV_DOUBLE:
       {
	 Tango::DevDouble *_retDoubleArray = new Tango::DevDouble[r.lgth];
	 for(int i=0;i<r.lgth;i++) _retDoubleArray[i] = r.value[i];
	 attr.set_value(_retDoubleArray,r.lgth,0,true);
       }
       break;
       
     }
            
   }
   
   
   ~DynSpecAttribute() {
   };
      
   
   //-----------------------------------------------------------------------------
   // Write/Read mehod
   
   virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att) {
        (static_cast<ModbusComposer *>(dev))->read_dynspec_attributes(att,this);}
   virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att) {
        (static_cast<ModbusComposer *>(dev))->write_dynspec_attributes(att,this);}
	  
};

/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::classes for dynamic creation

//=========================================
//	Define classes for dynamic commands
//=========================================
//	Command DynCommand class definition
class DynCommandClass : public Tango::Command
{
public:
	DynCommandClass(const char   *cmd_name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(cmd_name,in,out,in_desc,out_desc, level)	{}

	DynCommandClass(const char   *cmd_name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(cmd_name,in,out)	{}
	~DynCommandClass() {}

	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<ModbusComposer *>(dev))->is_DynCommand_allowed(any);}
};


/**
 *	The ModbusComposerClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  ModbusComposerClass : public Tango::DeviceClass
#else
class ModbusComposerClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(ModbusComposerClass::Additional DServer data members) ENABLED START -----*/
	
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::Additional DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
		//	Method prototypes
		static ModbusComposerClass *init(const char *);
		static ModbusComposerClass *instance();
		~ModbusComposerClass();
		Tango::DbDatum	get_class_property(std::string &);
		Tango::DbDatum	get_default_device_property(std::string &);
		Tango::DbDatum	get_default_class_property(std::string &);

	protected:
		ModbusComposerClass(std::string &);
		static ModbusComposerClass *_instance;
		void command_factory();
		void attribute_factory(std::vector<Tango::Attr *> &);
		void pipe_factory();
		void write_class_property();
		void set_default_property();
		void get_class_property();
		std::string get_cvstag();
		std::string get_cvsroot();

	private:
		void device_factory(TANGO_UNUSED(const Tango::DevVarStringArray *));
		void create_static_attribute_list(std::vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,std::vector<Tango::Attr *> &);
		std::vector<std::string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(std::vector<Tango::Attr *> &att_list, std::string attname);
};

}	//	End of namespace

#endif   //	ModbusComposer_H