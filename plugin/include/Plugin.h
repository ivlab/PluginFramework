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

typedef  FrameworkPlugin Plugin;

typedef PluginAPI PluginAPIInfo;

template<typename T>
class TypedPlugin : public Plugin {
public:
	virtual ~TypedPlugin() {}

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

	virtual void initialize(const PluginInfo& info);
	virtual void addPlugin(Plugin* plugin);
	virtual bool registerPlugin(PluginAPI* api);
	virtual bool unregisterPlugin(PluginAPI* api);

private:
	std::vector<Plugin*> plugins;
};

} /* namespace MinVR */

template<typename T>
inline bool PluginFW::TypedPlugin<T>::registerPlugin(PluginFW::PluginAPI* api) {
	T* iface = api->get();
	if (iface) {
		registerPlugin(*iface, *api);
		return true;
	}

	return false;
}

template<typename T>
inline bool PluginFW::TypedPlugin<T>::unregisterPlugin(PluginFW::PluginAPI* api) {
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

inline void PluginFW::CompositePlugin::addPlugin(Plugin* plugin) {
	plugins.push_back(plugin);
}

inline bool PluginFW::CompositePlugin::registerPlugin(PluginAPI* api) {
	bool registered = false;

	for (int f = 0; f < plugins.size(); f++) {
		if (plugins[f]->registerPlugin(api)) {
			registered = true;
		}
	}

	return registered;
}

inline void PluginFW::CompositePlugin::initialize(const PluginInfo& info) {
	for (int f = 0; f < plugins.size(); f++) {
		plugins[f]->initialize(info);
	}
}

inline bool PluginFW::CompositePlugin::unregisterPlugin(PluginAPI* api) {
	bool unregistered = false;

	for (int f = 0; f < plugins.size(); f++) {
		plugins[f]->unregisterPlugin(api);
		unregistered = true;
	}

	return unregistered;
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
