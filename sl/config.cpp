#include "config.h"


#include <codecvt>


std::wstring Config::utf8toUnicode(std::string utf8)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(utf8);
}

std::wstring Config::multibytetoUnicode(std::string utf8)
{
	using wcmb = std::codecvt_byname < wchar_t, char, std::mbstate_t >;
	static std::wstring_convert<wcmb> conv(new wcmb(""));
	return conv.from_bytes(utf8);
}


//////////////////////////////////////////////////////////////////////////
ConfigIni::ConfigIni()
{
}

ConfigIni& ConfigIni::instance()
{
	static ConfigIni ins;
	return ins;
}

#include <boost/property_tree/ini_parser.hpp>
bool ConfigIni::init(const std::string& ini)
{
	bool ok = false;
	try {
		boost::property_tree::ptree i;
		boost::property_tree::ini_parser::read_ini(ini, i);
		_ini.swap(i); //copy on write and lock
		ok = true;
	}
	catch (...) {
	}
	return ok;
}



//////////////////////////////////////////////////////////////////////////

ConfigCml::ConfigCml()
{

}

ConfigCml& ConfigCml::instance()
{
	static ConfigCml ins;
	return ins;
}

bool ConfigCml::has(const std::string& opt) const
{
	return _cml.count(opt) > 0;
}
