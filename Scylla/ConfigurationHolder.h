#pragma once

#include <windows.h>
#include <map>
#include "Configuration.h"

enum ConfigOption
{
	USE_PE_HEADER_FROM_DISK,
	DEBUG_PRIVILEGE,
	CREATE_BACKUP,
	DLL_INJECTION_AUTO_UNLOAD,
	IAT_SECTION_NAME,
	UPDATE_HEADER_CHECKSUM,
	REMOVE_DOS_HEADER_STUB,
	IAT_FIX_AND_OEP_FIX,
	SUSPEND_PROCESS_FOR_DUMPING,
	OriginalFirstThunk_SUPPORT,
	USE_ADVANCED_IAT_SEARCH,
	SCAN_DIRECT_IMPORTS,
	FIX_DIRECT_IMPORTS_NORMAL,
	FIX_DIRECT_IMPORTS_UNIVERSAL,
	CREATE_NEW_IAT_IN_SECTION,
    DONT_CREATE_NEW_SECTION,
    APIS_ALWAYS_FROM_DISK
};

class ConfigurationHolder
{
public:

	ConfigurationHolder(const WCHAR* fileName);

	bool loadConfiguration();
	bool saveConfiguration() const;

	Configuration& operator[](ConfigOption option);
	const Configuration& operator[](ConfigOption option) const;

private:

	static const WCHAR CONFIG_FILE_SECTION_NAME[];

	WCHAR configPath[MAX_PATH];
	std::map<ConfigOption, Configuration> config;

	bool buildConfigFilePath(const WCHAR* fileName);

	bool readStringFromConfigFile(Configuration & configObject);
	bool readBooleanFromConfigFile(Configuration & configObject);
	bool readNumericFromConfigFile(Configuration & configObject, int nBase);

	bool saveStringToConfigFile(const Configuration & configObject) const;
	bool saveBooleanToConfigFile(const Configuration & configObject) const;
	bool saveNumericToConfigFile(const Configuration & configObject, int nBase) const;

	bool loadConfig(Configuration & configObject);
	bool saveConfig(const Configuration & configObject) const;
};
