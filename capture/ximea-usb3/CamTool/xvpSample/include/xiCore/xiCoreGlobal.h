#ifndef XICORE_GLOBAL_H
#define XICORE_GLOBAL_H

#include <QtCore/qglobal.h>


#if defined(XICORE_LIBRARY)
#  define XICORE_API Q_DECL_EXPORT
#else
#  define XICORE_API Q_DECL_IMPORT
#endif

typedef void* XICORE_HANDLE;    //!< Used for camera handles coming from xiApi
typedef void* TxMemPoolHandle;  //!< Reference to a memory pool
typedef int TxVecObjID;         //!< Use this id when storing the reference to the object, not the pointer (pointer is unsafe). Zero value means "no object".
typedef int TxBinaryLayerID;    //!< A handle to CxBinaryLayer.
typedef quint32 TxFourCC;       //!< Indentifier of video codec ("four character code")

#define XI_OFFSETOF(s, m) ((size_t)&reinterpret_cast<const volatile char&>((((s *)0)->m)))

#define XI_FOURCC(ch0,ch1,ch2,ch3) ((quint32)(quint8)(ch0) | ((quint32)(quint8)(ch1) << 8) | ((quint32)(quint8)(ch2) << 16) | ((quint32)(quint8)(ch3) << 24))

//Memory leaks detection.
//Only for VC++ compilers. See the xiCore.pro, where XICORE_MEMLEAKS is defined.
#ifdef XICORE_MEMLEAKS
#include <vld.h>
#endif

#endif // XICORE_GLOBAL_H
