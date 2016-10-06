/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef PLUGINAPI_H_
#define PLUGINAPI_H_

#include <string>
#include <typeinfo>
#include <iostream>

namespace PluginFW {

class PluginAPI {
public:
	virtual ~PluginAPI() {}

	virtual std::string getName() const = 0;
	virtual int getMinVersion() const = 0;
	virtual const std::type_info& getType() = 0;

	PluginAPI& get();

	template<typename API>
	PluginAPI& getInterface();

	template<typename T>
	operator T*();

protected:
	virtual const std::type_info& getInternalType() = 0;
};

template<typename T>
class SpecificPluginAPI : public PluginAPI {
public:
	SpecificPluginAPI(T* api);

	T* getApi();

protected:
	const std::type_info& getInternalType();

private:
	T* api;
};

template<typename T, int VERSION, int MIN_VERSION>
class VersionedPluginAPI : public SpecificPluginAPI<T> {
public:
	VersionedPluginAPI(const std::string& name, T* api);
	virtual ~VersionedPluginAPI();

	std::string getName() const;
	static int getVersion();
	int getMinVersion() const;
	const std::type_info& getType();

private:
	std::string apiName;
};

class BlankPluginType {};
typedef PluginFW::VersionedPluginAPI<BlankPluginType, 0, 0> BlankPluginAPI;

template<typename T>
class PluginAPISpecification {
public:
	typedef VersionedPluginAPI<T, 0, 0> API;
	static std::string getName() { return "DANGEROUS: NOT NAMED, NOT VERSIONED (please provide API specification)"; }
};

} /* namespace PluginFW */

template<typename T, int VERSION, int MIN_VERSION>
inline PluginFW::VersionedPluginAPI<T, VERSION, MIN_VERSION>::VersionedPluginAPI(const std::string& name, T* api) : SpecificPluginAPI<T>(api), apiName(name) {
}

template<typename T, int VERSION, int MIN_VERSION>
inline PluginFW::VersionedPluginAPI<T, VERSION, MIN_VERSION>::~VersionedPluginAPI() {
}

template<typename T, int VERSION, int MIN_VERSION>
inline std::string PluginFW::VersionedPluginAPI<T, VERSION, MIN_VERSION>::getName() const {
	return apiName;
}

template<typename T, int VERSION, int MIN_VERSION>
inline const std::type_info& PluginFW::VersionedPluginAPI<T, VERSION, MIN_VERSION>::getType() {
	return typeid(VersionedPluginAPI<T, VERSION, MIN_VERSION>);
}

template<typename T, int VERSION, int MIN_VERSION>
inline int PluginFW::VersionedPluginAPI<T, VERSION, MIN_VERSION>::getVersion() {
	return VERSION;
}

template<typename T, int VERSION, int MIN_VERSION>
inline int PluginFW::VersionedPluginAPI<T, VERSION, MIN_VERSION>::getMinVersion() const {
	return MIN_VERSION;
}

template<typename T>
inline PluginFW::SpecificPluginAPI<T>::SpecificPluginAPI(T* api) : api(api) {
}

template<typename T>
inline T* PluginFW::SpecificPluginAPI<T>::getApi() {
	return api;
}

template<typename API>
inline PluginFW::PluginAPI& PluginFW::PluginAPI::getInterface() {
	static BlankPluginAPI iface = BlankPluginAPI("", NULL);

	if (this->getMinVersion() > API::getVersion())
	{
		std::cout << "Cannot use plugin for interface " << getName() << " as the minimum version is " << this->getMinVersion() << " but found " << API::getVersion() << std::endl;
		return iface;
	}

	return *this;
}

template<typename T>
inline PluginFW::PluginAPI::operator T*() {
	PluginAPI* api = &(getInterface<typename PluginAPISpecification<T>::API>());

	if (api->getInternalType() != typeid(T)) {
		return NULL;
	}

	SpecificPluginAPI<T>* specificPluginAPI = static_cast<SpecificPluginAPI<T>*>(api);
	return specificPluginAPI->getApi();
}

template<typename T>
inline const std::type_info& PluginFW::SpecificPluginAPI<T>::getInternalType() {
	return typeid(T);
}

inline PluginFW::PluginAPI& PluginFW::PluginAPI::get() {
	return *this;
}

#define addPluginAPISpecification(name, T, version, minVersion) \
namespace PluginFW { \
	template<> class PluginAPISpecification<T> { \
	public: \
	typedef PluginFW::VersionedPluginAPI<T, version, minVersion> API; \
	static std::string getName() { return name; } \
}; \
}

#endif /* PLUGINAPI_H_ */
