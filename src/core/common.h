#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#pragma warning(disable: 4244)	// int to float
#pragma warning(disable: 4800)	// int to bool
#pragma warning(disable: 4838)  // narrowing conversion
#pragma warning(disable: 4996)  // POSIX names

#ifdef __MWERKS__
#define __STDC_LIMIT_MACROS // so we get UINT32_MAX etc
#endif

#include <stdint.h>
#include <string.h>
#include <math.h>

#ifdef __MWERKS__
#define AUDIO_MSS
#define RWLIBS // codewarrior doesn't support project level defines - so not even this is enough, but still catches most ifdefs
#endif

#if !defined RW_D3D9 && defined LIBRW
#undef WITHD3D
#undef WITHDINPUT
#endif

#if (defined WITHD3D && !defined LIBRW)
#define WITHWINDOWS
#endif

#if defined _WIN32 && defined WITHWINDOWS && !defined _INC_WINDOWS
#include <windows.h>
#endif

#ifdef WITHD3D
	#ifdef LIBRW
		#define WITH_D3D // librw includes d3d9 itself via this right now
	#else
		#ifndef USE_D3D9
		#include <d3d8.h>
		#else
		#include <d3d9.h>
		#endif
	#endif
#endif

#ifdef WITHDINPUT
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#endif

#include <rwcore.h>
#include <rpworld.h>

// gotta put this somewhere
#ifdef LIBRW
#define STREAMPOS(str) ((str)->tell())
#define STREAMFILE(str) (((rw::StreamFile*)(str))->file)
#define HIERNODEINFO(hier) ((hier)->nodeInfo)
#define HIERNODEID(hier, i) ((hier)->nodeInfo[i].id)
#define HANIMFRAME(anim, i) ((RwUInt8*)(anim)->keyframes + (i)*(anim)->interpInfo->animKeyFrameSize)
#else
#define RWHALFPIXEL	// always d3d
#define STREAMPOS(str) ((str)->Type.memory.position)
#define STREAMFILE(str) ((str)->Type.file.fpFile)
#define HIERNODEINFO(hier) ((hier)->pNodeInfo)
#define HIERNODEID(hier, i) ((hier)->pNodeInfo[i].nodeID)
#define HANIMFRAME(anim, i) ((RwUInt8*)(anim)->pFrames + (i)*(anim)->interpInfo->keyFrameSize)
#define RpHAnimStdInterpFrame RpHAnimStdKeyFrame
#endif

#ifdef RWHALFPIXEL
#define HALFPX (0.5f)
#else
#define HALFPX (0.0f)
#endif

#define rwVENDORID_ROCKSTAR 0x0253F2

#define Max(a,b) ((a) > (b) ? (a) : (b))
#define Min(a,b) ((a) < (b) ? (a) : (b))

// Use this to add const that wasn't there in the original code
#define Const const

typedef uint8_t uint8;
typedef int8_t int8;
typedef uint16_t uint16;
typedef int16_t int16;
#ifndef __MWERKS__
typedef uint32_t uint32;
typedef int32_t int32;
#else
typedef unsigned int uint32;
typedef int int32;
#endif
typedef uintptr_t uintptr;
typedef intptr_t intptr;
typedef uint64_t uint64;
typedef int64_t int64;
// hardcode ucs-2
typedef uint16_t wchar;

typedef uint8 bool8;
typedef uint16 bool16;
typedef uint32 bool32;

#if defined(_MSC_VER) || defined (__MWERKS__)
typedef ptrdiff_t ssize_t;
#endif

#ifndef nil
#define nil NULL
#endif

#include "config.h"
#include "memoryManager.h"
#include "relocatableChunk.h"

#include <rphanim.h>
#include <rpskin.h>

#ifdef __GNUC__
#define TYPEALIGN(n) __attribute__ ((aligned (n)))
#else
#ifdef _MSC_VER
#define TYPEALIGN(n) __declspec(align(n))
#else
#define TYPEALIGN(n)	// unknown compiler...ignore
#endif
#endif

#define ALIGNPTR(p) (void*)((((uintptr)(void*)p) + sizeof(void*)-1) & ~(sizeof(void*)-1))

// PDP-10 like byte functions
#define MASK(p, s) (((1<<(s))-1) << (p))
inline uint32 dpb(uint32 b, uint32 p, uint32 s, uint32 w)
{
	uint32 m = MASK(p,s);
	return (w & ~m) | ((b<<p) & m);
}
inline uint32 ldb(uint32 p, uint32 s, uint32 w)
{
	return w>>p & (1<<s)-1;
}

#include "skeleton.h"
#include "Draw.h"

#if defined(PROPER_SCALING)
	#ifdef FORCE_PC_SCALING
		#define DEFAULT_SCREEN_WIDTH  (640)
		#define DEFAULT_SCREEN_HEIGHT (448)
	#else
		#define DEFAULT_SCREEN_WIDTH  (640)
		#define DEFAULT_SCREEN_HEIGHT (480)
	#endif
#elif defined(GTA_PS2)
		#define DEFAULT_SCREEN_WIDTH  (640)
		#define DEFAULT_SCREEN_HEIGHT (480)
#else //elif defined(GTA_PC)
		#define DEFAULT_SCREEN_WIDTH  (640)
		#define DEFAULT_SCREEN_HEIGHT (448)
#endif

#define DEFAULT_ASPECT_RATIO (4.0f/3.0f)
#define DEFAULT_VIEWWINDOW (0.7f)

// game uses maximumWidth/Height, but this probably won't work
// with RW windowed mode
#ifdef GTA_PS2
	#ifdef GTA_PAL
		#define SCREEN_WIDTH  ((float)640)
		#define SCREEN_HEIGHT ((float)512)
	#else
		#define SCREEN_WIDTH  ((float)640)
		#define SCREEN_HEIGHT ((float)448)
	#endif
#else
#define SCREEN_WIDTH  ((float)RsGlobal.width)
#define SCREEN_HEIGHT ((float)RsGlobal.height)
#endif

#define SCREEN_HEIGHT_PAL ((float)512)
#define SCREEN_HEIGHT_NTSC ((float)448)

#define SCREEN_ASPECT_RATIO (CDraw::GetAspectRatio())
#define SCREEN_VIEWWINDOW (Tan(DEGTORAD(CDraw::GetScaledFOV() * 0.5f)))

// This scales from PS2 pixel coordinates to the real resolution
#define SCREEN_STRETCH_X(a)   ((a) * (float) SCREEN_WIDTH / DEFAULT_SCREEN_WIDTH)
#define SCREEN_STRETCH_Y(a)   ((a) * (float) SCREEN_HEIGHT / DEFAULT_SCREEN_HEIGHT)
#define SCREEN_STRETCH_FROM_RIGHT(a)  (SCREEN_WIDTH - SCREEN_STRETCH_X(a))
#define SCREEN_STRETCH_FROM_BOTTOM(a) (SCREEN_HEIGHT - SCREEN_STRETCH_Y(a))

// This scales from PS2 pixel coordinates while optionally maintaining the aspect ratio
#define SCREEN_SCALE_X(a) SCREEN_SCALE_AR(SCREEN_STRETCH_X(a))
#define SCREEN_SCALE_Y(a) SCREEN_STRETCH_Y(a)
#define SCREEN_SCALE_FROM_RIGHT(a) (SCREEN_WIDTH - SCREEN_SCALE_X(a))
#define SCREEN_SCALE_FROM_BOTTOM(a) (SCREEN_HEIGHT - SCREEN_SCALE_Y(a))

#ifdef ASPECT_RATIO_SCALE
#define SCREEN_SCALE_AR(a) ((a) * DEFAULT_ASPECT_RATIO / SCREEN_ASPECT_RATIO)
#define SCALE_AND_CENTER_X(x) ((SCREEN_WIDTH == DEFAULT_SCREEN_WIDTH) ? (x) : (SCREEN_WIDTH - SCREEN_SCALE_X(DEFAULT_SCREEN_WIDTH)) / 2 + SCREEN_SCALE_X((x)))
#ifdef PROPER_SCALING
	#ifndef FORCE_PC_SCALING			
		#undef SCREEN_SCALE_Y	
		#define SCREEN_SCALE_Y(a) CDraw::ScaleY(SCREEN_STRETCH_Y(a))	
	#endif
#endif
#else
#define SCREEN_SCALE_AR(a) (a)
#define SCALE_AND_CENTER_X(x) SCREEN_STRETCH_X(x)
#endif

// these are temp marcos while we don't implement all PSP UI coordinates
#define PSP_DEFAULT_SCREEN_WIDTH  (480)
#define PSP_DEFAULT_SCREEN_HEIGHT (272)

#define PSP_SCALE_TO_PS2_X(a) ((float)a * ((float)DEFAULT_SCREEN_WIDTH / (float)PSP_DEFAULT_SCREEN_WIDTH))
#define PSP_SCALE_TO_PS2_Y(a) ((float)a * ((float)DEFAULT_SCREEN_HEIGHT / (float)PSP_DEFAULT_SCREEN_HEIGHT))

#define PSP_SCREEN_SCALE_X(a) ((a) * (float) SCREEN_WIDTH / PSP_DEFAULT_SCREEN_WIDTH)
#define PSP_SCREEN_SCALE_Y(a) ((a) * (float) SCREEN_WIDTH / PSP_DEFAULT_SCREEN_WIDTH)
#define PSP_SCREEN_SCALE_FROM_RIGHT(a) (SCREEN_WIDTH - PSP_SCREEN_SCALE_X(a))
#define PSP_SCREEN_SCALE_FROM_BOTTOM(a) (SCREEN_HEIGHT - PSP_SCREEN_SCALE_Y(a))

#include "maths.h"
#include "Vector.h"
#include "Vector2D.h"
#include "Matrix.h"
#include "Rect.h"

class CRGBA
{
public:
	union
	{
	    uint32 color32;
		struct { uint8 r, g, b, a; };
		struct { uint8 red, green, blue, alpha; };
#ifdef RWCORE_H
		struct  { RwRGBA rwRGBA; };
#endif
	};
	
	CRGBA(void) { }
	CRGBA(uint8 r, uint8 g, uint8 b, uint8 a) : r(r), g(g), b(b), a(a) { }
	
	bool operator ==(const CRGBA &right)
	{
		return this->r == right.r && this->g == right.g && this->b == right.b && this->a == right.a;
	}
	
	bool operator !=(const CRGBA &right)
	{
		return !(*this == right);
	}
	
	CRGBA &operator =(const CRGBA &right)
	{
		this->r = right.r;
		this->g = right.g;
		this->b = right.b;
		this->a = right.a;
		return *this;
	}
#ifdef RWCORE_H
	operator RwRGBA &(void) {
		return rwRGBA;
	}
	
	operator RwRGBA *(void) {
		return &rwRGBA;
	}
	
	operator RwRGBA (void) const {
		return rwRGBA;
	}

	CRGBA &operator =(const RwRGBA &right)
	{
		this->r = right.red;
		this->g = right.green;
		this->b = right.blue;
		this->a = right.alpha;
		return *this;
	}
#endif
};

#if (defined(_MSC_VER))
extern int strcasecmp(const char *str1, const char *str2);
extern int strncasecmp(const char *str1, const char *str2, size_t len);
#endif

extern wchar *AllocUnicode(const char*src);

#define clamp(v, low, high) ((v)<(low) ? (low) : (v)>(high) ? (high) : (v))

#define clamp2(v, center, radius) ((v) < (center) ? Max(v, center - radius) : Min(v, center + radius))

inline float sq(float x) { return x*x; }
#define SQR(x) ((x) * (x))

#ifdef __MWERKS__
#define M_E        2.71828182845904523536   // e
#define M_LOG2E    1.44269504088896340736   // log2(e)
#define M_LOG10E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880   // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)
#endif

#define PI (float)M_PI
#define TWOPI (PI*2)
#define HALFPI (PI/2)
#define DEGTORAD(x) ((x) * PI / 180.0f)
#define RADTODEG(x) ((x) * 180.0f / PI)

#ifdef USE_PS2_RAND
#define MYRAND_MAX		65535
#else
#define MYRAND_MAX		32767
#endif

int myrand(void);
void mysrand(unsigned int seed);

void re3_debug(const char *format, ...);
void re3_trace(const char *filename, unsigned int lineno, const char *func, const char *format, ...);
void re3_assert(const char *expr, const char *filename, unsigned int lineno, const char *func);
void re3_usererror(const char *format, ...);

#define DEBUGBREAK() __debugbreak();

// Switch to enable development messages.
#if 1 
#define DEV(f, ...)
#else
#define DEV(f, ...)   re3_debug("[DEV]: " f, ## __VA_ARGS__)
#endif

#ifdef __MWERKS__
void debug(char *f, ...);
void Error(char *f, ...);
__inline__ void TRACE(char *f, ...) { } // this is re3 only, and so the function needs to be inline - this way no call actually gets placed
// USERERROR only gets used in oal builds ... once
#else
#define debug(f, ...) re3_debug("[DBG]: " f, ## __VA_ARGS__)
#define Error(f, ...) re3_debug("[ERROR]: " f, ## __VA_ARGS__)
#ifndef MASTER
#define TRACE(f, ...) re3_trace(__FILE__, __LINE__, __FUNCTION__, f, ## __VA_ARGS__)
#define USERERROR(f, ...) re3_usererror(f, ## __VA_ARGS__)
#else
#define TRACE(f, ...)
#define USERERROR(f, ...)
#endif
#endif

#ifndef MASTER
#define assert(_Expression) (void)( (!!(_Expression)) || (re3_assert(#_Expression, __FILE__, __LINE__, __FUNCTION__), 0) )
#else
#define assert(_Expression) (_Expression)
#endif
#define ASSERT assert

#ifdef __MWERKS__
#define static_assert(bool_constexpr, message)
#endif

#define _TODO(x)
#define _TODOCONST(x) (x)

#ifdef CHECK_STRUCT_SIZES 
#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)
#else
#define VALIDATE_SIZE(struc, size)
#endif
#define VALIDATE_OFFSET(struc, member, offset) static_assert(offsetof(struc, member) == offset, "The offset of " #member " in " #struc " is not " #offset "...")

#define PERCENT(x, p)                    ((float(x) * (float(p) / 100.0f)))
#define ARRAY_SIZE(array)                (sizeof(array) / sizeof(array[0]))
#define BIT(num)                         (1<<(num))

#define ABS(a)  (((a) < 0) ? (-(a)) : (a))
#define norm(value, min, max) (((value) < (min)) ? 0 : (((value) > (max)) ? 1 : (((value) - (min)) / ((max) - (min)))))
#define lerp(norm, min, max) ( (norm) * ((max) - (min)) + (min) )

#define STRINGIFY(x)                    #x
#define STR(x)                          STRINGIFY(x)
#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)

#ifdef DEBUGMENU
// Tweaking stuff for debugmenu
#define TWEAKPATH                                   ___tw___TWEAKPATH
#define SETTWEAKPATH(path)	                        static const char *___tw___TWEAKPATH = path;
#define TWEAKFUNC(v)                                static CTweakFunc   CONCAT(___tw___tweak, __COUNTER__)(&v, STR(v), TWEAKPATH);
#define TWEAKFUNCN(v, name)                         static CTweakFunc   CONCAT(___tw___tweak, __COUNTER__)(&v, name, TWEAKPATH);
#define TWEAKBOOL(v)                                static CTweakBool   CONCAT(___tw___tweak, __COUNTER__)(&v, STR(v), TWEAKPATH);
#define TWEAKBOOLN(v, name)                         static CTweakBool   CONCAT(___tw___tweak, __COUNTER__)(&v, name, TWEAKPATH);
#define TWEAKINT32(v, lower, upper, step)           static CTweakInt32  CONCAT(___tw___tweak, __COUNTER__)(&v, STR(v), lower, upper, step, TWEAKPATH);
#define TWEAKINT32N(v, lower, upper, step, name)    static CTweakInt32  CONCAT(___tw___tweak, __COUNTER__)(&v, name, lower, upper, step, TWEAKPATH);
#define TWEAKUINT32(v, lower, upper, step)          static CTweakUInt32 CONCAT(___tw___tweak, __COUNTER__)(&v, STR(v), lower, upper, step, TWEAKPATH);
#define TWEAKUINT32N(v, lower, upper, step, name)   static CTweakUInt32 CONCAT(___tw___tweak, __COUNTER__)(&v, name, lower, upper, step, TWEAKPATH);
#define TWEAKINT16(v, lower, upper, step)           static CTweakInt16  CONCAT(___tw___tweak, __COUNTER__)(&v, STR(v), lower, upper, step, TWEAKPATH);
#define TWEAKINT16N(v, lower, upper, step, name)    static CTweakInt16  CONCAT(___tw___tweak, __COUNTER__)(&v, name, lower, upper, step, TWEAKPATH);
#define TWEAKUINT16(v, lower, upper, step)          static CTweakUInt16 CONCAT(___tw___tweak, __COUNTER__)(&v, STR(v), lower, upper, step, TWEAKPATH);
#define TWEAKUINT16N(v, lower, upper, step, name)   static CTweakUInt16 CONCAT(___tw___tweak, __COUNTER__)(&v, name, lower, upper, step, TWEAKPATH);
#define TWEAKINT8(v, lower, upper, step)            static CTweakInt8   CONCAT(___tw___tweak, __COUNTER__)(&v, STR(v), lower, upper, step, TWEAKPATH);
#define TWEAKINT8N(v, lower, upper, step, name)     static CTweakInt8   CONCAT(___tw___tweak, __COUNTER__)(&v, name, lower, upper, step, TWEAKPATH);
#define TWEAKUINT8(v, lower, upper, step)           static CTweakUInt8  CONCAT(___tw___tweak, __COUNTER__)(&v, STR(v), lower, upper, step, TWEAKPATH);
#define TWEAKUINT8N(v, lower, upper, step, name)    static CTweakUInt8  CONCAT(___tw___tweak, __COUNTER__)(&v, name, lower, upper, step, TWEAKPATH);
#define TWEAKFLOAT(v, lower, upper, step)           static CTweakFloat  CONCAT(___tw___tweak, __COUNTER__)(&v, STR(v), lower, upper, step, TWEAKPATH);
#define TWEAKFLOATN(v, lower, upper, step, name)    static CTweakFloat  CONCAT(___tw___tweak, __COUNTER__)(&v, name, lower, upper, step, TWEAKPATH);
#define TWEAKSWITCH(v, lower, upper, str, f)        static CTweakSwitch CONCAT(___tw___tweak, __COUNTER__)(&v, STR(v), lower, upper, str, f, TWEAKPATH);
#define TWEAKSWITCHN(v, lower, upper, str, f, name) static CTweakSwitch CONCAT(___tw___tweak, __COUNTER__)(&v, name, lower, upper, str, f, TWEAKPATH);

// interface
class CTweakVar
{
public:
	virtual void AddDBG(const char *path) = 0;
};

class CTweakVars
{
public:
	static void Add(CTweakVar *var);
	static void AddDBG(const char *path);
};

class CTweakFunc : public CTweakVar
{
	const char *m_pPath, *m_pVarName;
	void (*m_pFunc)();
public:
	CTweakFunc(void (*pFunc)(), const char *strName, const char *strPath) :
		m_pPath(strPath), m_pVarName(strName), m_pFunc(pFunc)
	{
		CTweakVars::Add(this);
	}
	
	void AddDBG(const char *path);
};

class CTweakBool : public CTweakVar
{
	const char *m_pPath, *m_pVarName;
	bool *m_pBoolVar;
public:
	CTweakBool(bool *pBool, const char *strName, const char *strPath) :
		m_pPath(strPath), m_pVarName(strName), m_pBoolVar(pBool)
	{
		CTweakVars::Add(this);
	}
	
	void AddDBG(const char *path);
};

class CTweakSwitch : public CTweakVar
{
	const char *m_pPath, *m_pVarName;
	void *m_pIntVar;
	int32 m_nMin, m_nMax;
	const char **m_aStr;
	void (*m_pFunc)();
public:
	CTweakSwitch(void *pInt, const char *strName, int32 nMin, int32 nMax, const char **aStr,
	             void (*pFunc)(), const char *strPath)
	    : m_pPath(strPath), m_pVarName(strName), m_pIntVar(pInt), m_nMin(nMin), m_nMax(nMax),
	      m_aStr(aStr)
	{
		CTweakVars::Add(this);
	}

	void AddDBG(const char *path);
};

#define _TWEEKCLASS(name, type)                                                                    \
	class name : public CTweakVar                                                              \
	{                                                                                          \
	public:                                                                                    \
		const char *m_pPath, *m_pVarName;                                                  \
		type *m_pIntVar, m_nLoawerBound, m_nUpperBound, m_nStep;                           \
                                                                                                   \
		name(type *pInt, const char *strName, type nLower, type nUpper, type nStep,        \
		     const char *strPath)                                                          \
		    : m_pPath(strPath), m_pVarName(strName), m_pIntVar(pInt),                      \
		      m_nLoawerBound(nLower), m_nUpperBound(nUpper), m_nStep(nStep)                \
                                                                                                   \
		{                                                                                  \
			CTweakVars::Add(this);                                                     \
		}                                                                                  \
                                                                                                   \
		void AddDBG(const char *path);                                                     \
	};

_TWEEKCLASS(CTweakInt8, int8);
_TWEEKCLASS(CTweakUInt8, uint8);
_TWEEKCLASS(CTweakInt16, int16);
_TWEEKCLASS(CTweakUInt16, uint16);
_TWEEKCLASS(CTweakInt32, int32);
_TWEEKCLASS(CTweakUInt32, uint32);
_TWEEKCLASS(CTweakFloat, float);

#undef _TWEEKCLASS
#endif

#ifdef VALIDATE_SAVE_SIZE
extern int32 _saveBufCount;
#define INITSAVEBUF _saveBufCount = 0;
#define VALIDATESAVEBUF(b) assert(_saveBufCount == b);
#else
#define INITSAVEBUF
#define VALIDATESAVEBUF(b)
#endif

inline void SkipSaveBuf(uint8 *&buf, int32 skip)
{
	buf += skip;
#ifdef VALIDATE_SAVE_SIZE
	_saveBufCount += skip;
#endif
}

inline void SkipSaveBuf(uint8*& buf, uint32 &length, int32 skip)
{
	buf += skip;
	length += skip;
#ifdef VALIDATE_SAVE_SIZE
	_saveBufCount += skip;
#endif
}

template<typename T>
inline const T ReadSaveBuf(uint8 *&buf)
{
	T &value = *(T*)buf;
	SkipSaveBuf(buf, sizeof(T));
	return value;
}

template<typename T>
inline const T ReadSaveBuf(uint8 *&buf, uint32 &length)
{
	T &value = *(T*)buf;
	SkipSaveBuf(buf, length, sizeof(T));
	return value;
}

template<typename T>
inline T *WriteSaveBuf(uint8 *&buf, const T &value)
{
	T *p = (T*)buf;
	*p = value;
	SkipSaveBuf(buf, sizeof(T));
	return p;
}

template<typename T>
inline T *WriteSaveBuf(uint8 *&buf, uint32 &length, const T &value)
{
	T *p = (T*)buf;
	*p = value;
	SkipSaveBuf(buf, length, sizeof(T));
	return p;
}


#define SAVE_HEADER_SIZE (4*sizeof(char)+sizeof(uint32))

#define WriteSaveHeader(buf,a,b,c,d,size) \
	WriteSaveBuf(buf, a);\
	WriteSaveBuf(buf, b);\
	WriteSaveBuf(buf, c);\
	WriteSaveBuf(buf, d);\
	WriteSaveBuf<uint32>(buf, size);

#define WriteSaveHeaderWithLength(buf,len,a,b,c,d,size) \
	WriteSaveBuf(buf, len, a);\
	WriteSaveBuf(buf, len, b);\
	WriteSaveBuf(buf, len, c);\
	WriteSaveBuf(buf, len, d);\
	WriteSaveBuf<uint32>(buf, len, size);

#define CheckSaveHeader(buf,a,b,c,d,size)\
	assert(ReadSaveBuf<char>(buf) == a);\
	assert(ReadSaveBuf<char>(buf) == b);\
	assert(ReadSaveBuf<char>(buf) == c);\
	assert(ReadSaveBuf<char>(buf) == d);\
	assert(ReadSaveBuf<uint32>(buf) == size);

#define CheckSaveHeaderWithLength(buf,len,a,b,c,d,size)\
	assert(ReadSaveBuf<char>(buf,len) == a);\
	assert(ReadSaveBuf<char>(buf,len) == b);\
	assert(ReadSaveBuf<char>(buf,len) == c);\
	assert(ReadSaveBuf<char>(buf,len) == d);\
	assert(ReadSaveBuf<uint32>(buf,len) == size);


void cprintf(char*, ...);
