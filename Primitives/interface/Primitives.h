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