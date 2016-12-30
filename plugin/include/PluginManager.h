/* ================================================================================

This file is part of the PluginFramework Open Source Project.

File: extend/PluginManager.h

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

#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include <string>
#include <vector>
#include "SharedLibrary.h"
#include "PluginAPI.h"
#include "PluginFramework.h"

namespace PluginFW {

class PluginManager {
public:
	PluginManager();
	PluginManager(std::vector<PluginAPI*> pluginAPIs);
	virtual ~PluginManager();

	void loadPlugin(const std::string& filePath);
	void loadPlugin(const std::string& filePath, const std::string& name);
	void setUserData(void* userData);

	template<typename T>
	void addInterface(T* api);

protected:
	void addInterfaceInternal(PluginAPI* api);

private:
	std::vector<PluginAPI*> _pluginAPIs;
	std::vector<FrameworkPlugin*> _plugins;
	std::vector<SharedLibrary*> _libraries;
	void* userData;
};

} /* namespace extend */

template<typename T>
void PluginFW::PluginManager::addInterface(T* api) {
	addInterfaceInternal(new typename PluginAPISpecification<T>::API(PluginAPISpecification<T>::getName(), api));
}

#endif /* PLUGINMANAGER_H_ */
