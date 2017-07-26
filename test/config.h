#pragma once

#include <boost/property_tree/ptree.hpp>

class Config{
public:
	static std::wstring utf8toUnicode(std::string);
	static std::wstring multibytetoUnicode(std::string);
};

//////////////////////////////////////////////////////////////////////////
//utf8
class ConfigIni{

	boost::property_tree::ptree _ini;

	ConfigIni();
public:
	static ConfigIni& instance();

	bool init(const std::string& ini);

	template<typename T>
	T get(const std::string&, const T&);
	template<>
	std::wstring get<std::wstring>(const std::string&, const std::wstring&);
};

template<typename T>
T ConfigIni::get(const std::string& path, const T& def)
{
	try {
		auto out = _ini.get<T>(path);
		return out;
	}
	catch (...) {
	}
	return def;
}

template<>
std::wstring ConfigIni::get<std::wstring>(const std::string& path, const std::wstring& def){
	try {
		auto out = _ini.get<std::string>(path);
		return Config::utf8toUnicode(out);
	}
	catch (...) {
	}
	return def;
}

//////////////////////////////////////////////////////////////////////////
#include <boost/program_options.hpp>
//char
class ConfigCml{
	boost::program_options::variables_map _cml;

	ConfigCml();
public:
	static ConfigCml& instance();

	template<typename CharT>
	bool init(int argc, CharT **argv, const boost::program_options::options_description& opt);

	bool has(const std::string&)const;

	template<typename T>
	T get(const std::string& key, const T& def);
	template<>
	std::wstring get<std::wstring>(const std::string& key, const std::wstring& def);
};

template<typename CharT>
bool ConfigCml::init(int argc, CharT **argv, const boost::program_options::options_description& opt)
{
	bool ok = false;

	try{
		boost::program_options::store(boost::program_options::basic_command_line_parser<CharT>(argc, argv).options(opt).allow_unregistered().run(), _cml);
		boost::program_options::notify(_cml);
		ok = true;
	}
	catch (boost::program_options::invalid_command_line_syntax&){
	}
	catch (boost::bad_lexical_cast&){
	}
	catch (...){
	}
	return ok;
}

template<typename T>
T ConfigCml::get(const std::string& key, const T& def)
{
	using namespace boost::program_options;

	try{
		if (_cml.count(key)){
			auto v = _cml[key].as<T>();
			return v;
		}
	}
	catch (boost::bad_lexical_cast&){
	}
	catch (...){
	}

	return def;
}

template<>
std::wstring ConfigCml::get<std::wstring>(const std::string& key, const std::wstring& def)
{
	using namespace boost::program_options;

	try{
		if (_cml.count(key)){
			auto v = _cml[key].as<std::string>();
			return Config::multibytetoUnicode(v);
		}
	}
	catch (boost::bad_lexical_cast&){
	}
	catch (...){
	}

	return def;
}


//////////////////////////////////////////////////////////////////////////
//e.g.
// --help --memory="Ã»ÓÐÄÚ´æ" -d -w74835 -v"2.0.0"
// #include "config.h"
// int main(int argc, char* argv[])
// {
// 	ConfigIni& conf = ConfigIni::instance();
// 	conf.init("config.ini");
// 	auto sc = conf.get<std::string>("test.hello", "");
// 	auto wsc = conf.get<std::wstring>("test.nihao", L"");
// 	auto nc = conf.get("test.number", 0);
// 
// 	ConfigCml& cml = ConfigCml::instance();
// 	boost::program_options::options_description od("for test only!!!");
// 	od.add_options()
// 		("help,h", "help for command line")
// 		("debug,d", "Debug mode")
// 		("memory,m", boost::program_options::value<std::string>(), "")
// 		("start,s", "start repair as startup")
// 		("version,v", boost::program_options::value<std::string>()->default_value("1.0.0"), "Version of PC doctor")
// 		("qss,q", boost::program_options::value<std::string>(), "qss for skin")
// 		("hwnd,w", boost::program_options::value<int>(), "Handle of parent window")
// 		("ppid,p", boost::program_options::value<int64_t>(), "Parent process PID");
// 	cml.init(argc, argv, od);
// 
// 	auto scml = cml.get<std::wstring>("memory", L"none");
// 
// 	return 0;
// }
//////////////////////////////////////////////////////////////////////////