#pragma once

#ifdef _DL_DLL_
#define STAPI __declspec(dllexport)
#else
#define STAPI __declspec(dllimport)
#endif

//#ifdef __cplusplus
//#endif

#include <guiddef.h>
#include <windows.h>


//do not modify
struct IBaseObject;
/// \fn DWORD __stdcall GetClassObjectFunc(__out ILocalAccelerate ** ppLocalAccImp, __in const GUID& InferfaceID)
/// \brief DLL导出接口，用于获取主要操作对象
/// \param[out] ppLocalAccImp 用于接收\ref ILocalAccelerate 的一个子类实现实例
/// \param[in] InferfaceID 接口GUID， 通常用来区分对象实现以及版本
/// \return 返回系统错误(System Error Code)。 0表示成功
/// \warning 需要调用\ref ReleaseClassObject 进行释放
//extern "C" 
STAPI DWORD __stdcall STGetObject(__out IBaseObject ** ppObj, __in const GUID& InferfaceID);


/// \fn void __stdcall ReleaseClassObject(__in ILocalAccelerate * pLocalAccImp)
/// \brief DLL导出接口，用于释放GetClassObjectFunc， 或者GetSubInterface 获取的对象实例。
/// 
///		从dll获取的所有接口， 没有特殊说明的情况下都应该调用此接口进行释放
/// \param[in] pLocalAccImp \ref ILocalAccelerate 的一个子实现实例， 传入进行释放
/// \return 无返回值
//extern "C" 
STAPI void __stdcall STReleaseObject(__in IBaseObject * ppObj);


/// \brief GetClassObjectFunc的类型定义， 一般用于动态加载dll。 \see GetClassObjectFunc
//extern "C" 
typedef DWORD(__stdcall *STGetObjectFuncProc)(__out IBaseObject ** ppObj, __in const GUID& InferfaceID);

/// \brief ReleaseClassObject的类型定义， 一般用于动态加载dll。 \see ReleaseClassObject
//extern "C" 
typedef void(__stdcall *STReleaseObjectProc)(__in IBaseObject * ppObj);

/// \brief 导出的基类对象接口
/// 所有导出的接口实现都应该继承此接口
struct IBaseObject{
	virtual ~IBaseObject();
};