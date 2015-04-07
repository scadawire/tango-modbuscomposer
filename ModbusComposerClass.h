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

#include <tango.h>
#include <ModbusComposer.h>
#include <ExpParser.h>


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

     VALUE r;
     _ep->EvaluateRead(&r);
     
     switch(get_type()) {
       
	 case Tango::DEV_BOOLEAN:
	 {
	   Tango::DevBoolean *_retBoolean = new Tango::DevBoolean;
	   *_retBoolean = _ep->GetBoolResult(r);
	   attr.set_value(_retBoolean,1,0,true);
         }
	 break;
	 
	 case Tango::DEV_SHORT:
         {
	   Tango::DevShort *_retShort = new Tango::DevShort;
	   *_retShort = (Tango::DevShort)(r.value[0]+0.5);
	   attr.set_value(_retShort,1,0,true);
         }
	 break;
	 
	 case Tango::DEV_DOUBLE:
         {
	   Tango::DevDouble *_retDouble = new Tango::DevDouble;
	   *_retDouble = r.value[0];
	   attr.set_value(_retDouble,1,0,true);
         }
	 break;
	 
	 case Tango::DEV_LONG:
         {
	   Tango::DevLong *_retLong = new Tango::DevLong;
	   *_retLong = (Tango::DevLong)(r.value[0]+0.5);
	   attr.set_value(_retLong,1,0,true);
         }
	 break;
	        
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

/**
 *	The ModbusComposerClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  ModbusComposerClass : public Tango::DeviceClass
#else
class ModbusComposerClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(ModbusComposerClass::Additionnal DServer data members) ENABLED START -----*/
	
	
	/*----- PROTECTED REGION END -----*/	//	ModbusComposerClass::Additionnal DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
	
		//	Method prototypes
		static ModbusComposerClass *init(const char *);
		static ModbusComposerClass *instance();
		~ModbusComposerClass();
		Tango::DbDatum	get_class_property(string &);
		Tango::DbDatum	get_default_device_property(string &);
		Tango::DbDatum	get_default_class_property(string &);
	
	protected:
		ModbusComposerClass(string &);
		static ModbusComposerClass *_instance;
		void command_factory();
		void attribute_factory(vector<Tango::Attr *> &);
		void pipe_factory();
		void write_class_property();
		void set_default_property();
		void get_class_property();
		string get_cvstag();
		string get_cvsroot();
	
	private:
		void device_factory(const Tango::DevVarStringArray *);
		void create_static_attribute_list(vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,vector<Tango::Attr *> &);
		vector<string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname);
};

}	//	End of namespace

#endif   //	ModbusComposer_H
