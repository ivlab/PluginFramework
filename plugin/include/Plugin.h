/* ================================================================================

This file is part of the PluginFramework Open Source Project.

File: extend/Plugin.h

Original Author(s) of this File:
	Dan Orban, 2015, University of Minnesota

Author(s) of Significant Updates/Modifications to the File:
	...

-----------------------------------------------------------------------------------
Copyright (c) 2015 Regents of the University of Minnesota
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* The name of the University of Minnesota, nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
================================================================================ */

#ifndef PLUGIN_H_
#define PLUGIN_H_

#include "PluginFramework.h"
#include <memory>
#include <vector>

namespace PluginFW {

class Plugin : public FrameworkPlugin {
};

typedef PluginAPI PluginAPIInfo;

template<typename T>
class SpecificPlugin : public Plugin {
public:
	virtual ~SpecificPlugin() {}

	bool registerPlugin(PluginAPI* api);
	bool unregisterPlugin(PluginAPI* api);

protected:
	virtual void registerPlugin(T& api, PluginAPIInfo& apiInfo) = 0;
	virtual void unregisterPlugin(T& api, PluginAPIInfo& apiInfo) = 0;
};

class CompositePlugin : public Plugin {
public:
	CompositePlugin(const std::vector<Plugin*> plugins);
	virtual ~CompositePlugin();

	bool registerPlugin(PluginAPI* api);
	bool unregisterPlugin(PluginAPI* api);

private:
	std::vector<Plugin*> plugins;
};

} /* namespace MinVR */


extern "C"
{
	PLUGIN_API int getPluginFWVersion() {
		return PLUGIN_FW_VERSION;
	}
}

template<typename T>
inline bool PluginFW::SpecificPlugin<T>::registerPlugin(PluginFW::PluginAPI* api) {
	T* iface = api->get();
	if (iface) {
		registerPlugin(*iface, *api);
		return true;
	}

	return false;
}

template<typename T>
inline bool PluginFW::SpecificPlugin<T>::unregisterPlugin(PluginFW::PluginAPI* api) {
	T* iface = api->get();
	if (iface) {
		unregisterPlugin(*iface, *api);
		return true;
	}

	return false;
}

inline PluginFW::CompositePlugin::CompositePlugin(
		const std::vector<Plugin*> plugins) : plugins(plugins) {
}

inline PluginFW::CompositePlugin::~CompositePlugin() {
	for (int f = 0; f < plugins.size(); f++) {
		delete plugins[f];
	}
}

inline bool PluginFW::CompositePlugin::registerPlugin(PluginAPI* api) {
	for (int f = 0; f < plugins.size(); f++) {
		plugins[f]->registerPlugin(api);
	}
}

inline bool PluginFW::CompositePlugin::unregisterPlugin(PluginAPI* api) {
	for (int f = 0; f < plugins.size(); f++) {
		plugins[f]->unregisterPlugin(api);
	}
}

/*

extern "C"
{
	minVR::PluginRef loadPlugin() {
		return new minVR::Plugin();
	}
}
*/

#endif /* PLUGIN_H_ */
