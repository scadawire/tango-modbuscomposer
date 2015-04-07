//=============================================================================
//
// file :        AttributeMap.h
//
// description : Handle relation between attribute and private attribute data for a device
//
// project :     SSAColdPlate
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
// $Revision: 1.1.1.1 $
// $Date: $
//
// $HeadURL: $
//
//
//=============================================================================
#ifndef _ATTRIBUTEMAPH
#define _ATTRIBUTEMAPH

#include <string>
#include <vector>

//#include <ModbusComposer.h>

namespace ModbusComposer_ns
{

typedef struct _ATTITEM {

  std::string  attName;
  ExpParser *ep;
  
} ATTITEM;

class AttributeMap {

private:
  
    std::vector<ATTITEM> map;

public:

  // Construct an empty attribute map
  AttributeMap();

  // Destruct attribute map
  ~AttributeMap();
  
  // Free allocated resource
  void clear();
  
  // Add an attribute
  void add(std::string attName,ExpParser *ep);
  
  // Get item according to attribute name
  ATTITEM *get(std::string attName);

  // Get item at pos idx
  ATTITEM *get(int idx);

  // Return size of map
  unsigned int size();

};

} // namespace ModbusComposer_ns

#endif /* _ATTRIBUTEMAPH */
