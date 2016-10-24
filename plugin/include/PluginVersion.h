/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef PLUGINVERSION_H_
#define PLUGINVERSION_H_

#include "PluginFramework.h"

extern "C"
{
	PLUGIN_API int getPluginFWVersion() {
		return PLUGIN_FW_VERSION;
	}
}

#endif /* PLUGINVERSION_H_ */
