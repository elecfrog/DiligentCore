// Primitives.h - Simplified compatibility header for Diligent Engine primitives
// This header only provides essential Diligent-specific definitions, everything else uses spw log system directly

#pragma once

#include <log_system/log_system.hpp>
#include <sstream>
#include <iomanip>

// Forward declarations for Diligent namespace compatibility
namespace Diligent
{
    // Global callback - declare but don't define (let platforms provide their own)
    extern spw::DebugMessageCallbackType DebugMessageCallback;
    
    // Set callback function
    inline void SetDebugMessageCallback(spw::DebugMessageCallbackType callback)
    {
        DebugMessageCallback = callback;
        spw::LogSystem::Instance().SetDebugMessageCallback(callback);
    }
}

// Special Diligent version macros - these are the only ones that MUST be defined
// Debug versions (when DILIGENT_DEBUG is defined)
#ifdef DILIGENT_DEBUG
#    define LOG_DBG_FATAL_ERROR_MESSAGE LOG_FATAL_ERROR_MESSAGE
#    define LOG_DBG_ERROR_MESSAGE       LOG_ERROR_MESSAGE
#    define LOG_DBG_WARNING_MESSAGE     LOG_WARNING_MESSAGE
#    define LOG_DBG_INFO_MESSAGE        LOG_INFO_MESSAGE
#else
#    define LOG_DBG_FATAL_ERROR_MESSAGE(...)
#    define LOG_DBG_ERROR_MESSAGE(...)
#    define LOG_DBG_WARNING_MESSAGE(...)
#    define LOG_DBG_INFO_MESSAGE(...)
#endif

// Development versions (when DILIGENT_DEVELOPMENT is defined)
#ifdef DILIGENT_DEVELOPMENT
#    define LOG_DVP_FATAL_ERROR_MESSAGE LOG_FATAL_ERROR_MESSAGE
#    define LOG_DVP_ERROR_MESSAGE       LOG_ERROR_MESSAGE
#    define LOG_DVP_WARNING_MESSAGE     LOG_WARNING_MESSAGE
#    define LOG_DVP_INFO_MESSAGE        LOG_INFO_MESSAGE
#else
#    define LOG_DVP_FATAL_ERROR_MESSAGE(...)
#    define LOG_DVP_ERROR_MESSAGE(...)
#    define LOG_DVP_WARNING_MESSAGE(...)
#    define LOG_DVP_INFO_MESSAGE(...)
#endif

// Check macros