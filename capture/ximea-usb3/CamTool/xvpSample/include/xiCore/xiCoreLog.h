#ifndef XICORELOG_H
#define XICORELOG_H


//TODO This is only a temporary logger

#include <QDebug>
#include <QThread>

//Uncomment to enable logging
//#define XICORE_ENABLE_LOG

//Uncomment to enable memory pool allocation logging
//#define XICORE_LOG_MEMPOOL_ALLOCATIONS

//Uncomment to enable memory pool allocation logging
//#define XICORE_API_ENABLE_LOG

//Uncomment to enable profiling log
//#define XICORE_ENABLE_PROFILE_LOG

#ifdef __BASE_FILE__
#define __XICORE_FILE__ __BASE_FILE__
#else
#define __XICORE_FILE__ __FILE__
#endif

#define _LOG_MSG(label, msg) QThread::currentThreadId() << __XICORE_FILE__ << "L" << __LINE__ << ":" << label << msg


#ifdef XICORE_ENABLE_LOG

#define LOG_TRACE(msg) qDebug() << _LOG_MSG("", msg)
#define LOG_WARN(msg) qDebug() << _LOG_MSG("", msg)
#define LOG_ERR(msg) qCritical() << _LOG_MSG("", msg)
#define LOG_FATAL(msg) qFatal() << _LOG_MSG("", msg)
#define LOG_INFO(msg) qCritical() << msg

#else

#define LOG_TRACE(msg)
#define LOG_WARN(msg)
#define LOG_ERR(msg) qCritical() << _LOG_MSG("", msg)
#define LOG_FATAL(msg)
#define LOG_INFO(msg) qCritical() << msg

#endif//XICORE_ENABLE_LOG

#define INVALID_PARAM(func, param) #func ": Invalid parameter '" #param "'"
#define ERR_MSG(func, msg) QString(#func ": %1").arg(msg)


#ifdef XICORE_LOG_MEMPOOL_ALLOCATIONS
#define LOG_MP_TRACE(msg) qDebug() << _LOG_MSG("MP: ", msg)
#else
#define LOG_MP_TRACE(msg)
#endif

#ifdef XICORE_API_ENABLE_LOG
#define LOG_API_TRACE(msg) qDebug() << _LOG_MSG("API: ", msg)
#else
#define LOG_API_TRACE(msg)
#endif

#ifdef XICORE_ENABLE_PROFILE_LOG
#define LOG_PROF(msg) qDebug() << _LOG_MSG("PROFILE: ", msg)
#else
#define LOG_PROF(msg)
#endif

#endif // XICORELOG_H
