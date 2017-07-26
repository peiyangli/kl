#pragma once



class Flag
{
	int i;
public:
	inline Flag(long ai) : i(int(ai)) {}
	inline Flag(unsigned long ai) : i(int(long(ai))) {}
	inline Flag(int ai) : i(ai) {}
	inline Flag(unsigned ai) : i(int(ai)) {}
	inline Flag(short ai) : i(int(ai)) {}
	inline operator int() const { return i; }
	inline operator unsigned() const { return unsigned(i); }
};

template<bool c, typename Then, typename Else> class IF_ { };
template<typename Then, typename Else>
class IF_<true, Then, Else> { public: typedef Then reType; };
template<typename Then, typename Else>
class IF_<false, Then, Else> { public: typedef Else reType; };

template<typename Enum>
class Flags
{
	static_assert((sizeof(Enum) <= sizeof(int)),
		"Flags uses an int as storage, so an enum with underlying "
		"long long will overflow.");
	struct Private;
	typedef int(Private::*Zero);
public:
	typedef typename IF_<
		is_unsigned<Enum>::value,
		unsigned int,
		signed int
	>::type Int;

	typedef Enum enum_type;
	// compiler-generated copy/move ctor/assignment operators are fine!
	inline Flags(Enum f) : i(Int(f)) {}
	inline Flags(Zero = 0) : i(0) {}
	inline Flags(Flag f) : i(f) {}

	inline Flags &operator&=(int mask) { i &= mask; return *this; }
	inline Flags &operator&=(uint mask) { i &= mask; return *this; }
	inline Flags &operator&=(Enum mask) { i &= Int(mask); return *this; }
	inline Flags &operator|=(QFlags f) { i |= f.i; return *this; }
	inline Flags &operator|=(Enum f) { i |= Int(f); return *this; }
	inline Flags &operator^=(QFlags f) { i ^= f.i; return *this; }
	inline Flags &operator^=(Enum f) { i ^= Int(f); return *this; }

	inline operator Int() const { return i; }

	inline Flags operator|(Flags f) const { return Flags(Flag(i | f.i)); }
	inline Flags operator|(Enum f) const { return Flags(Flag(i | Int(f))); }
	inline Flags operator^(Flags f) const { return Flags(Flag(i ^ f.i)); }
	inline Flags operator^(Enum f) const { return Flags(Flag(i ^ Int(f))); }
	inline Flags operator&(int mask) const { return Flags(Flag(i & mask)); }
	inline Flags operator&(uint mask) const { return Flags(Flag(i & mask)); }
	inline Flags operator&(Enum f) const { return Flags(Flag(i & Int(f))); }
	inline Flags operator~() const { return Flags(Flag(~i)); }

	inline bool operator!() const { return !i; }
	inline operator bool() const { return i; }

	inline bool testFlag(Enum f) const { return (i & Int(f)) == Int(f) && (Int(f) != 0 || i == Int(f)); }
private:
	Int i;
};

#define DECLARE_FLAGS(EFlags, Enum)\
typedef Flags<Enum> EFlags;


/*/////////////////////////////////////////////////////////////////////////
e.g.
class LocalServer{
public:
	enum SocketOption {
		NoOptions = 0x0,
		UserAccessOption = 0x01,
		GroupAccessOption = 0x2,
		OtherAccessOption = 0x4,
		WorldAccessOption = 0x7
	};

	DECLARE_FLAGS(SocketOptions, SocketOption)
};
/////////////////////////////////////////////////////////////////////////*/