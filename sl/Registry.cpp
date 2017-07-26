#include "Registry.h"


#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <stack>

#include <windows.h>
//////////////////////////////////////////////////////////////////////////

/*******************************************************************************
** Some convenience functions
*/

static bool StartWith(const std::wstring& org, const std::wstring& start)
{
	if (start.length() > org.length())
		return false;

	return 0 == _wcsnicmp(org.c_str(), start.c_str(), start.length());
}

static bool StartWith(const std::wstring& org, wchar_t start)
{
	if (org.empty())return false;
	return 0 == _wcsnicmp(org.c_str(), &start, 1);
}

static const REGSAM registryPermissions = KEY_READ | KEY_WRITE;

enum ChildSpec { AllKeys, ChildKeys, ChildGroups };

static std::wstring keyPath(const std::wstring &rKey)
{
	auto idx = rKey.find_last_of(L'\\');
	if (idx == -1)
		return std::wstring();
	return rKey.substr(0, idx);
}

static std::wstring keyName(const std::wstring &rKey)
{

	auto idx = rKey.find_last_of(L'\\');
	std::wstring res;
	if (idx == -1)
		res = rKey;
	else
		res = rKey.substr(idx + 1);

	if (res == std::wstring(L"Default") || res == std::wstring(L"."))
		res = std::wstring();

	return res;
}

static std::wstring escapedKey(std::wstring uKey)
{
	for (auto& c : uKey) {
		if (c == L'\\')
			c = L'/';
		else if (c == L'/')
			c = L'\\';
	}
	return uKey;
}

static std::wstring unescapedKey(std::wstring rKey)
{
	return escapedKey(rKey);
}

typedef std::map<std::wstring, std::wstring> NameSet;

static StringList keys(const NameSet& ns){
	StringList o;
	for (auto& itr : ns) {
		o.push_back(itr.first);
	}
	return std::move(o);
}


static void mergeKeySets(NameSet *dest, const NameSet &src)
{
	for (auto& itr : src) {
		dest->insert(std::pair<std::wstring, std::wstring>(itr.first, std::wstring()));
	}
}

static void mergeKeySets(NameSet *dest, const StringList &src)
{
	for (auto& itr : src) {
		dest->insert(std::pair<std::wstring, std::wstring>(itr, std::wstring()));
	}
}


/*******************************************************************************
** Wrappers for the insane windows registry API
*/

static std::wstring errorCodeToString(DWORD errorCode)
{
	wchar_t *data = 0;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode, 0, data, 0, 0);

	if (!data)
		return std::wstring();

	std::wstring result(data);
	
	LocalFree(data);

	if (result.at(result.length() - 1) == L'\n')
		result = result.substr(0, result.length() - 1);

	return result;
}

// Open a key with the specified perms
static HKEY openKey(HKEY parentHandle, REGSAM perms, const std::wstring &rSubKey)
{
	HKEY resultHandle = 0;
	LONG res = RegOpenKeyEx(parentHandle, reinterpret_cast<const wchar_t *>(rSubKey.c_str()),
		0, perms, &resultHandle);

	if (res == ERROR_SUCCESS)
		return resultHandle;

	return 0;
}


// Open a key with the specified perms, create it if it does not exist
static HKEY createOrOpenKey(HKEY parentHandle, REGSAM perms, const std::wstring &rSubKey)
{
	// try to open it
	HKEY resultHandle = openKey(parentHandle, perms, rSubKey);
	if (resultHandle != 0)
		return resultHandle;

	// try to create it
	LONG res = RegCreateKeyEx(parentHandle, reinterpret_cast<const wchar_t *>(rSubKey.c_str()), 0, 0,
		REG_OPTION_NON_VOLATILE, perms, 0, &resultHandle, 0);

	if (res == ERROR_SUCCESS)
		return resultHandle;

	//qWarning("QSettings: Failed to create subkey \"%s\": %s",
	//        rSubKey.toLatin1().data(), errorCodeToString(res).toLatin1().data());

	return 0;
}

// Open or create a key in read-write mode if possible, otherwise read-only
static HKEY createOrOpenKey(HKEY parentHandle, const std::wstring &rSubKey, bool *readOnly, REGSAM more)
{
	// try to open or create it read/write
	HKEY resultHandle = createOrOpenKey(parentHandle, registryPermissions | more, rSubKey);
	if (resultHandle != 0) {
		if (readOnly != 0)
			*readOnly = false;
		return resultHandle;
	}

	// try to open or create it read/only
	resultHandle = createOrOpenKey(parentHandle, KEY_READ | more, rSubKey);
	if (resultHandle != 0) {
		if (readOnly != 0)
			*readOnly = true;
		return resultHandle;
	}
	return 0;
}


static StringList childKeysOrGroups(HKEY parentHandle, ChildSpec spec)
{
	StringList result;
	DWORD numKeys;
	DWORD maxKeySize;
	DWORD numSubgroups;
	DWORD maxSubgroupSize;

	// Find the number of keys and subgroups, as well as the max of their lengths.
	LONG res = RegQueryInfoKey(parentHandle, 0, 0, 0, &numSubgroups, &maxSubgroupSize, 0,
		&numKeys, &maxKeySize, 0, 0, 0);

	if (res != ERROR_SUCCESS) {
		//qWarning("QSettings: RegQueryInfoKey() failed: %s", errorCodeToString(res).toLatin1().data());
		return result;
	}

	++maxSubgroupSize;
	++maxKeySize;

	int n;
	int m;
	if (spec == ChildKeys) {
		n = numKeys;
		m = maxKeySize;
	}
	else {
		n = numSubgroups;
		m = maxSubgroupSize;
	}

	/* The size does not include the terminating null character. */
	++m;

	// Get the list
	DWORD bufsize = m * sizeof(wchar_t);
	std::unique_ptr<byte[]> buff(new byte[bufsize]);
	for (int i = 0; i < n; ++i) {
		std::wstring item;
		DWORD l = bufsize / sizeof(wchar_t);
		if (spec == ChildKeys) {
			res = RegEnumValue(parentHandle, i, reinterpret_cast<wchar_t *>(buff.get()), &l, 0, 0, 0, 0);
		}
		else {
			res = RegEnumKeyEx(parentHandle, i, reinterpret_cast<wchar_t *>(buff.get()), &l, 0, 0, 0, 0);
		}
		if (res == ERROR_SUCCESS)
			item = std::wstring((wchar_t*)buff.get(), l);

		if (res != ERROR_SUCCESS) {
			//qWarning("QSettings: RegEnumValue failed: %s", errorCodeToString(res).toLatin1().data());
			continue;
		}
		if (item.empty())
			item = L".";

		result.push_back(item);
	}
	return result;
}



static void allKeys(HKEY parentHandle, const std::wstring &rSubKey, NameSet *result, REGSAM more)
{
	HKEY handle = openKey(parentHandle, KEY_READ | more, rSubKey);
	if (handle == 0)
		return;

	StringList childKeys = childKeysOrGroups(handle, ChildKeys);
	StringList childGroups = childKeysOrGroups(handle, ChildGroups);
	RegCloseKey(handle);

	for (size_t i = 0; i < childKeys.size(); ++i) {
		std::wstring s = rSubKey;
		if (!s.empty())
			s += L'\\';
		s += childKeys.at(i);
		result->insert(std::pair<std::wstring, std::wstring>(s, std::wstring()));
	}

	for (size_t i = 0; i < childGroups.size(); ++i) {
		std::wstring s = rSubKey;
		if (!s.empty())
			s += L'\\';
		s += childGroups.at(i);
		allKeys(parentHandle, s, result, more);
	}
}


static void deleteChildGroups(HKEY parentHandle, REGSAM more)
{
	StringList childGroups = childKeysOrGroups(parentHandle, ChildGroups);

	for (size_t i = 0; i < childGroups.size(); ++i) {
		std::wstring group = childGroups.at(i);

		// delete subgroups in group
		HKEY childGroupHandle = openKey(parentHandle, registryPermissions | more, group);
		if (childGroupHandle == 0)
			continue;
		deleteChildGroups(childGroupHandle, more);
		RegCloseKey(childGroupHandle);

		// delete group itself
		LONG res = RegDeleteKey(parentHandle, reinterpret_cast<const wchar_t *>(group.c_str()));
		if (res != ERROR_SUCCESS) {
			//qWarning("QSettings: RegDeleteKey failed on subkey \"%s\": %s", group.toLatin1().data(), errorCodeToString(res).toLatin1().data());
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////


static StringList splitArgs(const std::wstring &s, size_t idx)
{
	size_t l = s.length();
// 	ASSERT(l > 0);
// 	ASSERT(s.at(idx) == L'(');
// 	ASSERT(s.at(l - 1) == L')');

	StringList result;
	std::wstring item;

	for (++idx; idx < l; ++idx) {
		auto c = s.at(idx);
		if (c == L')') {
			//ASSERT(idx == l - 1);
			result.push_back(item);
		}
		else if (c == L' ') {
			result.push_back(item);
			item.clear();
		}
		else {
			item.push_back(c);
		}
	}

	return result;
}


static std::wstring variantToString(const RegVariant &v)
{
	if (v.Is<std::wstring>()){
		return v.Get<std::wstring>();
	}
	if (v.Is<int>()){
		auto value = v.Get<int>();
		std::wstringstream ss;
		ss << value;
		return ss.str();
	}
	return L"";
}



static RegVariant stringToVariant(const std::wstring &s)
{
	return RegVariant(std::wstring(s));
}



/*******************************************************************************
** class RegistryKey
*/
class RegistryKey
{
public:
	RegistryKey(HKEY parent_handle = 0, const std::wstring &key = std::wstring(), bool read_only = true, bool wow64 = false);
	std::wstring key() const;
	HKEY handle() const;
	HKEY parentHandle() const;
	bool readOnly() const;
	void close();
private:
	HKEY m_parent_handle;
	mutable HKEY m_handle;
	std::wstring m_key;
	mutable bool m_read_only;
	REGSAM	_WOW64;
};



RegistryKey::RegistryKey(HKEY parent_handle, const std::wstring &key, bool read_only, bool wow64)
{
	_WOW64 = wow64 ? KEY_WOW64_64KEY : 0;
	m_parent_handle = parent_handle;
	m_handle = 0;
	m_read_only = read_only;
	m_key = key;
}

std::wstring RegistryKey::key() const
{
	return m_key;
}

HKEY RegistryKey::handle() const
{
	if (m_handle != 0)
		return m_handle;

	if (m_read_only)
		m_handle = openKey(m_parent_handle, KEY_READ | _WOW64, m_key);
	else
		m_handle = createOrOpenKey(m_parent_handle, m_key, &m_read_only, _WOW64);

	return m_handle;
}

HKEY RegistryKey::parentHandle() const
{
	return m_parent_handle;
}

bool RegistryKey::readOnly() const
{
	return m_read_only;
}

void RegistryKey::close()
{
	if (m_handle != 0)
		RegCloseKey(m_handle);
	m_handle = 0;
}

typedef std::vector<RegistryKey> RegistryKeyList;


class QSettingsGroup
{
public:
	inline QSettingsGroup()
		: num(-1), maxNum(-1) {}
	inline QSettingsGroup(const std::wstring &s)
		: str(s), num(-1), maxNum(-1) {}
	inline QSettingsGroup(const std::wstring &s, bool guessArraySize)
		: str(s), num(0), maxNum(guessArraySize ? 0 : -1) {}

	inline std::wstring name() const { return str; }
	inline std::wstring toString() const;
	inline bool isArray() const { return num != -1; }
	inline int arraySizeGuess() const { return maxNum; }
	inline void setArrayIndex(int i)
	{
		num = i + 1; if (maxNum != -1 && num > maxNum) maxNum = num;
	}

	static std::wstring number(int num){
		std::wstringstream ss;
		ss << num;
		return ss.str();
	}

	std::wstring str;
	int num;
	int maxNum;
};

inline std::wstring QSettingsGroup::toString() const
{
	std::wstring result;
	result = str;
	if (num > 0) {
		result += L'/';
		result += number(num);
	}
	return result;
}



/*******************************************************************************
** class RegistryPrivate
*/

class RegistryPrivate
{
	friend class Registry;
public:
	RegistryPrivate(std::wstring rKey, bool wow = false);
	~RegistryPrivate();

	void remove(const std::wstring &uKey);
	void set(const std::wstring &uKey, const RegVariant &value, bool expandSZ = false);
	//void set(const QString &uKey, const QVariant &value, std::function<unsigned long(unsigned long)> typefun);
	bool get(const std::wstring &uKey, RegVariant *value) const;
	StringList children(const std::wstring &uKey, ChildSpec spec) const;
	void clear();
	void sync();
	void flush();
	bool isWritable() const;
	HKEY writeHandle() const;
	bool readKey(HKEY parentHandle, const std::wstring &rSubKey, RegVariant *value) const;
	std::wstring fileName() const;

	std::wstring actualKey(const std::wstring &key) const;

	Registry::Status status()const{
		return _s;
	}


	static std::wstring normalizedKey(const std::wstring &key);
private:
	void setStatus(Registry::Status s){
		_s = s;
	}

private:
	RegistryKeyList regList; // list of registry locations to search for keys
	bool deleteWriteHandleOnExit;

	bool fallbacks;

	std::stack<QSettingsGroup> groupStack;
	std::wstring groupPrefix;

	Registry::Status _s;
	REGSAM	_WOW64;
};



RegistryPrivate::RegistryPrivate(std::wstring rPath, bool wow)
{
	_WOW64 = wow ? KEY_WOW64_64KEY : 0;
	deleteWriteHandleOnExit = false;

	if (StartWith(rPath, L'\\'))
		rPath = rPath.substr(1);

	int keyLength;
	HKEY keyName;

	if (StartWith(rPath, L"HKEY_CURRENT_USER")) {
		keyLength = 17;
		keyName = HKEY_CURRENT_USER;
	}
	else if (StartWith(rPath, L"HKCU")) {
		keyLength = 4;
		keyName = HKEY_CURRENT_USER;
	}
	else if (StartWith(rPath, L"HKEY_LOCAL_MACHINE")) {
		keyLength = 18;
		keyName = HKEY_LOCAL_MACHINE;
	}
	else if (StartWith(rPath, L"HKLM")) {
		keyLength = 4;
		keyName = HKEY_LOCAL_MACHINE;
	}
	else if (StartWith(rPath, L"HKEY_CLASSES_ROOT")) {
		keyLength = 17;
		keyName = HKEY_CLASSES_ROOT;
	}
	else if (StartWith(rPath, L"HKCR")) {
		keyLength = 4;
		keyName = HKEY_CLASSES_ROOT;
	}
	else if (StartWith(rPath, L"HKEY_USERS")) {
		keyLength = 10;
		keyName = HKEY_USERS;
	}
	else if (StartWith(rPath, L"HKU")) {
		keyLength = 3;
		keyName = HKEY_USERS;
	}
	else {
		return;
	}

	if (rPath.length() == keyLength)
		regList.push_back(RegistryKey(keyName, std::wstring(), false, wow));
	else if (rPath[keyLength] == L'\\')
		regList.push_back(RegistryKey(keyName, rPath.substr(keyLength + 1), false, wow));
}

std::wstring RegistryPrivate::actualKey(const std::wstring &key) const
{
	std::wstring n = normalizedKey(key);
	//Q_ASSERT_X(!n.isEmpty(), "QSettings", "empty key");
	n = groupPrefix + n;
	return n;
}

std::wstring RegistryPrivate::normalizedKey(const std::wstring &key)
{
	std::wstring result = key;

	size_t i = 0;
	while (i < result.size()) {
		while (result.at(i) == L'/') {
			result.erase(i);
			if (i == result.size())
				goto after_loop;
		}
		while (result.at(i) != L'/') {
			++i;
			if (i == result.size())
				return result;
		}
		++i; // leave the slash alone
	}

after_loop:
	if (!result.empty())
		result = result.substr(0, i - 1);
	return result;
}



bool RegistryPrivate::readKey(HKEY parentHandle, const std::wstring &rSubKey, RegVariant *value) const
{
	std::wstring rSubkeyName = keyName(rSubKey);
	std::wstring rSubkeyPath = keyPath(rSubKey);

	// open a handle on the subkey
	HKEY handle = openKey(parentHandle, KEY_READ | _WOW64, rSubkeyPath);
	if (handle == 0)
		return false;

	// get the size and type of the value
	DWORD dataType;
	DWORD dataSize;
	LONG res = RegQueryValueEx(handle, reinterpret_cast<const wchar_t *>(rSubkeyName.c_str()), 0, &dataType, 0, &dataSize);
	if (res != ERROR_SUCCESS) {
		RegCloseKey(handle);
		return false;
	}

	// get the value
	std::unique_ptr<byte[]> data(new byte[dataSize]);
	res = RegQueryValueEx(handle, reinterpret_cast<const wchar_t *>(rSubkeyName.c_str()), 0, 0,
		reinterpret_cast<unsigned char*>(data.get()), &dataSize);
	if (res != ERROR_SUCCESS) {
		RegCloseKey(handle);
		return false;
	}

	switch (dataType) {
	case REG_EXPAND_SZ:
	case REG_SZ: {
		std::wstring s;
		if (dataSize) {
			s = ((const wchar_t *)data.get());
		}
		if (value != 0)
			*value = RegVariant(s);
		break;
	}

	case REG_MULTI_SZ: {
		StringList l;
		if (dataSize) {
			size_t i = 0;
			for (;;) {
				std::wstring s = ((const wchar_t *)data.get() + i);
				i += s.length() + 1;

				if (s.empty())
					break;
				l.push_back(s);
			}
		}
		if (value != 0)
			*value = RegVariant(l);
		break;
	}
	case REG_NONE:
	case REG_BINARY: {
		ByteArray s;
		if (dataSize) {
			s = ByteArray((char*)data.get(), dataSize);
		}
		if (value != 0)
			*value = s;
		break;
	}

	case REG_DWORD_BIG_ENDIAN:
	case REG_DWORD: {
		//Q_ASSERT(data.size() == sizeof(int));
		int i;
		memcpy((char*)&i, data.get(), sizeof(int));
		if (value != 0)
			*value = i;
		break;
	}
	case REG_QWORD: {
		//Q_ASSERT(data.size() == sizeof(qint64));
		unsigned long long i;
		memcpy((char*)&i, data.get(), sizeof(unsigned long long));
		if (value != 0)
			*value = i;
		break;
	}

	default:
		//qWarning("QSettings: Unknown data %d type in Windows registry", static_cast<int>(dataType));
		if (value != 0)
			*value = RegVariant();
		break;
	}

	RegCloseKey(handle);
	return true;
}

HKEY RegistryPrivate::writeHandle() const
{
	if (regList.empty())
		return 0;
	const RegistryKey &key = regList.at(0);
	if (key.handle() == 0 || key.readOnly())
		return 0;
	return key.handle();
}


RegistryPrivate::~RegistryPrivate()
{
	if (deleteWriteHandleOnExit && writeHandle() != 0) {

		std::wstring emptyKey;
		DWORD res = RegDeleteKey(writeHandle(), reinterpret_cast<const wchar_t *>(emptyKey.c_str()));
		if (res != ERROR_SUCCESS) {
			//qWarning("QSettings: Failed to delete key \"%s\": %s", regList.at(0).key().toLatin1().data(), errorCodeToString(res).toLatin1().data());
		}

	}

	for (size_t i = 0; i < regList.size(); ++i)
		regList[i].close();
}


void RegistryPrivate::remove(const std::wstring &uKey)
{
	if (writeHandle() == 0) {
		setStatus(Registry::AccessError);
		return;
	}

	std::wstring rKey = escapedKey(uKey);

	// try to delete value bar in key foo
	LONG res;
	HKEY handle = openKey(writeHandle(), registryPermissions | _WOW64, keyPath(rKey));
	if (handle != 0) {
		res = RegDeleteValue(handle, reinterpret_cast<const wchar_t *>(keyName(rKey).c_str()));
		RegCloseKey(handle);
	}

	// try to delete key foo/bar and all subkeys
	handle = openKey(writeHandle(), registryPermissions | _WOW64, rKey);
	if (handle != 0) {
		deleteChildGroups(handle, _WOW64);

		if (rKey.empty()) {
			StringList childKeys = childKeysOrGroups(handle, ChildKeys);

			for (size_t i = 0; i < childKeys.size(); ++i) {
				std::wstring group = childKeys.at(i);

				LONG res = RegDeleteValue(handle, reinterpret_cast<const wchar_t *>(group.c_str()));
				if (res != ERROR_SUCCESS) {
					//qWarning("QSettings: RegDeleteValue failed on subkey \"%s\": %s", group.toLatin1().data(), errorCodeToString(res).toLatin1().data());
				}
			}
		}
		else {

			res = RegDeleteKey(writeHandle(), reinterpret_cast<const wchar_t *>(rKey.c_str()));

			if (res != ERROR_SUCCESS) {
				//qWarning("QSettings: RegDeleteKey failed on key \"%s\": %s", rKey.toLatin1().data(), errorCodeToString(res).toLatin1().data());
			}
		}
		RegCloseKey(handle);
	}
}

static bool stringContainsNullChar(const std::wstring &s)
{
	for (auto& c : s) {
		if (c == 0)
			return true;
	}
	return false;
}



void RegistryPrivate::set(const std::wstring &uKey, const RegVariant &value, bool expand)
{
	if (writeHandle() == 0) {
		setStatus(Registry::AccessError);
		return;
	}

	std::wstring rKey = escapedKey(uKey);

	HKEY handle = createOrOpenKey(writeHandle(), registryPermissions | _WOW64, keyPath(rKey));
	if (handle == 0) {
		setStatus(Registry::AccessError);
		return;
	}

	DWORD type;
	//QByteArray regValueBuff;
	ByteArray regValueBuff;
	do 
	{
		if (value.Is<std::wstring>()){
			type = expand ? REG_EXPAND_SZ : REG_SZ;
			std::wstring i = value.Get<std::wstring>();
			regValueBuff = std::move(ByteArray((const char*)i.c_str(), (i.length() + 1) * 2));
			break;
		}

		if (value.Is<int>()){
			type = REG_DWORD;
			int i = value.Get<int>();
			regValueBuff = std::move(ByteArray((const char*)&i, sizeof(int)));
			break;
		}

		if (value.Is<unsigned long long>()){
			type = REG_QWORD;
			unsigned long long i = value.Get<unsigned long long>();
			regValueBuff = std::move(ByteArray((const char*)&i, sizeof(unsigned long long)));
			break;
		}

		if (value.Is<StringList>()){
			// If none of the elements contains '\0', we can use REG_MULTI_SZ, the
			// native registry string list type. Otherwise we use REG_BINARY.
			type = REG_MULTI_SZ;
			StringList l = value.Get<StringList>();// variantListToStringList(value.toList());

			StringList::const_iterator it = l.cbegin();
			for (; it != l.cend(); ++it) {
				const std::wstring &s = *it;
				regValueBuff += std::move(ByteArray((const char*)s.c_str(), (s.length() + 1) * 2));
			}

			regValueBuff.resize(regValueBuff.size() + 2);
			break;
		}

		if (value.Is<ByteArray>()){
			type = REG_BINARY;
			regValueBuff = value.Get<ByteArray>();
			break;
		}

	} while (false);

	// set the value
	LONG res = RegSetValueEx(handle, reinterpret_cast<const wchar_t *>(keyName(rKey).c_str()), 0, type,
		reinterpret_cast<const unsigned char*>(regValueBuff.data()),
		regValueBuff.size());

	if (res == ERROR_SUCCESS) {
		deleteWriteHandleOnExit = false;
	}
	else {
		//qWarning("QSettings: failed to set subkey \"%s\": %s", rKey.toLatin1().data(), errorCodeToString(res).toLatin1().data());
		setStatus(Registry::AccessError);
	}

	RegCloseKey(handle);
}

bool RegistryPrivate::get(const std::wstring &uKey, RegVariant *value) const
{
	std::wstring rKey = escapedKey(uKey);

	for (size_t i = 0; i < regList.size(); ++i) {
		HKEY handle = regList.at(i).handle();
		if (handle != 0 && readKey(handle, rKey, value))
			return true;

		if (!fallbacks)
			return false;
	}

	return false;
}

StringList RegistryPrivate::children(const std::wstring &uKey, ChildSpec spec) const
{
	NameSet result;
	std::wstring rKey = escapedKey(uKey);

	for (size_t i = 0; i < regList.size(); ++i) {
		HKEY parent_handle = regList.at(i).handle();
		if (parent_handle == 0)
			continue;
		HKEY handle = openKey(parent_handle, KEY_READ | _WOW64, rKey);
		if (handle == 0)
			continue;

		if (spec == AllKeys) {
			NameSet keys;
			allKeys(handle, L"", &keys, _WOW64);
			mergeKeySets(&result, keys);
		}
		else { // ChildGroups or ChildKeys
			StringList names = childKeysOrGroups(handle, spec);
			mergeKeySets(&result, names);
		}

		RegCloseKey(handle);

		if (!fallbacks)
			return keys(result);
	}

	return keys(result);
}


void RegistryPrivate::clear()
{
	remove(std::wstring());
	deleteWriteHandleOnExit = true;
}

void RegistryPrivate::sync()
{
	RegFlushKey(writeHandle());
}

void RegistryPrivate::flush()
{
	// Windows does this for us.
}

std::wstring RegistryPrivate::fileName() const
{
	if (regList.empty())
		return std::wstring();

	const RegistryKey &key = regList.at(0);
	std::wstring result;
	if (key.parentHandle() == HKEY_CURRENT_USER)
		result = L"\\HKEY_CURRENT_USER\\";
	else
		result = L"\\HKEY_LOCAL_MACHINE\\";

	return result + regList.at(0).key();
}

bool RegistryPrivate::isWritable() const
{
	return writeHandle() != 0;
}

//////////////////////////////////////////////////////////////////////////

Registry::Registry(std::wstring rPath, bool wow /*= false*/)
: d(new RegistryPrivate(rPath, wow))
{

}


Registry::~Registry()
{
}

StringList Registry::allKeys() const
{
	return d->children(d->groupPrefix, AllKeys);
}

StringList Registry::childKeys() const
{
	return d->children(d->groupPrefix, ChildKeys);
}

StringList Registry::childGroups() const
{
	return d->children(d->groupPrefix, ChildGroups);
}

bool Registry::isWritable() const
{
	return d->isWritable();
}

void Registry::setValue(const std::wstring &key, const RegVariant &value, bool exSz /*= false*/)
{
	std::wstring k = d->actualKey(key);
	d->set(k, value, exSz);
}

RegVariant Registry::value(const std::wstring &key, const RegVariant &defaultValue /*= RegVariant()*/) const
{
	RegVariant result = defaultValue;
	std::wstring k = d->actualKey(key);
	d->get(k, &result);
	return result;
}

std::wstring Registry::group() const
{
	return d->groupPrefix.substr(0, d->groupPrefix.size() - 1);
}

void Registry::remove(const std::wstring &key)
{
	std::wstring theKey = d->normalizedKey(key);
	if (theKey.empty())
		theKey = group();
	else
		theKey = d->groupPrefix + theKey;
	if (theKey.empty()) {
		d->clear();
	}
	else {
		d->remove(theKey);
	}
}

bool Registry::contains(const std::wstring &key) const
{
	std::wstring k = d->actualKey(key);
	return d->get(k, 0);
}

Registry::Status Registry::status() const
{
	return d->_s;
}


