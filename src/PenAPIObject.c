//
// WacomWebPlugin - An implementation of the Wacom Web API on Linux.
// Copyright (C) 2013  Zane Ashby <zane.a@demonastery.org>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

//
// PenAPIObject
//

//
// Headers.
//

#include "PenAPIObject.h"
#include "XInput.h"


//
// PenAPI functions.
//

// Allocate copy of object.
NPObject* PenAPIAllocate(NPP instance, NPClass *aClass)
{
  PenAPIObject *obj = (PenAPIObject*)g_netscapeFuncs->memalloc(sizeof(PenAPIObject));

  if (!obj)
    return NULL;

  memset(obj, 0, sizeof(PenAPIObject));

  obj->instance = instance;

  return (NPObject*)obj;
}

// Deallocate object.
void PenAPIDeallocate(NPObject *obj)
{
  g_netscapeFuncs->memfree(obj);
}

// Check if our PenAPI API handles a property.
// This is where the majority of the Wacom API is defined.
bool PenAPIHasProperty(NPObject *obj, NPIdentifier name)
{
  if (IS_IDENTIFIER("isEraser")           || 
      IS_IDENTIFIER("isWacom")            || 
      IS_IDENTIFIER("pointerType")        || 
      IS_IDENTIFIER("posX")               || 
      IS_IDENTIFIER("posY")               || 
      IS_IDENTIFIER("pressure")           || 
      IS_IDENTIFIER("rotationDeg")        || 
      IS_IDENTIFIER("rotationRad")        || 
      IS_IDENTIFIER("sysX")               || 
      IS_IDENTIFIER("sysY")               || 
      IS_IDENTIFIER("tabX")               || 
      IS_IDENTIFIER("tabY")               || 
      IS_IDENTIFIER("tabletModel")        || 
      IS_IDENTIFIER("tabletModelID")      || 
      IS_IDENTIFIER("tangentialPressure") || 
      IS_IDENTIFIER("tiltX")              || 
      IS_IDENTIFIER("tiltY")              || 
      IS_IDENTIFIER("version"))
    return true;

  return false;
}

// Provide the appropriate value to JS.
// Most of this will be passed through to an XInput object.
bool PenAPIGetProperty(NPObject *obj, NPIdentifier name, NPVariant *result)
{
  // Get values from wrapper.
  xinput_values_t *values = xinput_getValues();

  XINPUT(isEraser, BOOLEAN);
  XINPUT(isWacom, BOOLEAN);
  XINPUT(pointerType, INT32);
  XINPUT(posX, INT32);
  XINPUT(posY, INT32);
  XINPUT(pressure, DOUBLE);
  XINPUT(rotationDeg, DOUBLE);
  XINPUT(rotationRad, DOUBLE);
  XINPUT(sysX, DOUBLE);
  XINPUT(sysY, DOUBLE);
  XINPUT(tabX, INT32);
  XINPUT(tabY, INT32);
  XINPUT_STRING(tabletModel);
  XINPUT_STRING(tabletModelID);
  XINPUT(tangentialPressure, DOUBLE);
  XINPUT(tiltX, DOUBLE);
  XINPUT(tiltY, DOUBLE);
  
  if (IS_IDENTIFIER("version")) {
    // The PLUGIN_VERSION as a decimal.
    INT32_TO_NPVARIANT(PLUGIN_VERSIONL, *result);
    return true;
  }

  return false;
}

// No methods in this API for now so we just return false.
// Although the API has a "SetFocus" method that should be implemented one day.
bool PenAPIHasMethod(NPObject *obj, NPIdentifier name) {
  return false;
}


//
// Initialize PenAPIClass.
// Try to only define what is strictly required.
//

NPClass PenAPIClass = {
  .structVersion = NP_CLASS_STRUCT_VERSION,
  .allocate = PenAPIAllocate,
  .deallocate = PenAPIDeallocate,
  .hasProperty = PenAPIHasProperty,
  .getProperty = PenAPIGetProperty,
  .invalidate = 0,
  .hasMethod = PenAPIHasMethod, // Needed on Firefox
  .invoke = 0,
  .invokeDefault = 0,
  .setProperty = 0,
  .removeProperty = 0,
  .enumerate = 0,
  .construct = 0,
};
