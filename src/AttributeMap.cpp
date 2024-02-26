//=============================================================================
//
// file :        AttributeMap.cpp
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
//
// $HeadURL:  $
//
//
//
//=============================================================================

namespace ModbusComposer_ns
{
class AttributeMap;
}

#include <ModbusComposer.h>

using namespace std;

namespace ModbusComposer_ns
{

// Construct an empty attribute map
AttributeMap::AttributeMap() {
}
  
// Destruct attribute map
AttributeMap::~AttributeMap() {
  clear();
}

// Free allocated resource
void AttributeMap::clear() {
  
  for(int i=0;i<(int)map.size();i++) {
    delete map[i].ep;
  }
  map.clear();
    
}
  
// Add an attribute
void AttributeMap::add(string attName,ExpParser *ep) {
  
  ATTITEM newItem;
  newItem.attName = attName;
  newItem.ep = ep;

  // Look for insertion position
  bool found = false;
  int i=0;
  while(!found && i<(int)map.size()) {
     found = map[i].attName.compare(attName) > 0;
     if(!found) i++;
  }

  // Insert
  if(found) {
    map.insert(map.begin()+i,newItem);
  } else {
    map.push_back(newItem);
  }
    
}
  
// Get item according to attribute name
ATTITEM *AttributeMap::get(string attName) {

  bool found = false;
  int  min=0;
  int  max=map.size()-1;
  int  mid,comp;

  while(!found && min<=max) {

    mid = (min+max)/2;
    comp = map[mid].attName.compare(attName);
    if(comp>0) {
      max = mid-1;
    } else if(comp<0) {
      min = mid+1;
    } else {
      found = true;
    }

  }

  if(found) {

    return &map[mid];

  } else {

    return NULL;

  }

}

ATTITEM *AttributeMap::get(int idx) {

  return &map[idx];

}

unsigned int AttributeMap::size() {

  return map.size();

}

} // namespace ModbusComposer_ns

