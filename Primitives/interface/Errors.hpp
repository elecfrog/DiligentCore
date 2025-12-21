#pragma once

#include <stdexcept>
#include <string>
#include <iostream>
#include <cstdint>

#include "DebugOutput.h"
#include "FormatString.hpp"

namespace Diligent
{

template <bool>
void ThrowIf(std::string&&)
{
}

template <>
inline void ThrowIf<true>(std::string&& msg)
{
    throw std::runtime_error(std::move(msg));
}

template <bool bThrowException, typename... ArgsType>
void LogError(bool IsFatal, const char* Function, const char* FullFilePath, int Line, const ArgsType&... Args)
{
    std::string FileName(FullFilePath);

    size_t LastSlashPos = FileName.find_last_of("/\\");
    if (LastSlashPos != std::string::npos)
        FileName.erase(0, LastSlashPos + 1);
    std::string Msg = FormatString(Args...);
    if (DebugMessageCallback != nullptr)
    {
        DebugMessageCallback(IsFatal ? DEBUG_MESSAGE_SEVERITY_FATAL_ERROR : DEBUG_MESSAGE_SEVERITY_ERROR, Msg.c_str(), Function, FileName.c_str(), Line);
    }
    else
    {
        // No callback set - output to cerr
        std::cerr << "Diligent Engine: " << (IsFatal ? "Fatal Error" : "Error") << " in " << Function << "() (" << FileName << ", " << Line << "): " << Msg << '\n';
    }
    ThrowIf<bThrowException>(std::move(Msg));
}

} // namespace Diligent



#define DG_LOG_ERROR(...)                                                                                 \
    do                                                                                                 \
    {                                                                                                  \
        Diligent::LogError<false>(/*IsFatal=*/false, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (false)


#define LOG_FATAL_ERROR(...)                                                                          \
    do                                                                                                \
    {                                                                                                 \
        Diligent::LogError<false>(/*IsFatal=*/true, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (false)

#define LOG_ERROR_ONCE(...)             \
    do                                  \
    {                                   \
        static bool IsFirstTime = true; \
        if (IsFirstTime)                \
        {                               \
            DG_LOG_ERROR(##__VA_ARGS__);   \
            IsFirstTime = false;        \
        }                               \
    } while (false)


#define LOG_ERROR_AND_THROW(...)                                                                      \
    do                                                                                                \
    {                                                                                                 \
        Diligent::LogError<true>(/*IsFatal=*/false, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (false)

#define LOG_FATAL_ERROR_AND_THROW(...)                                                               \
    do                                                                                               \
    {                                                                                                \
        Diligent::LogError<true>(/*IsFatal=*/true, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (false)


#define LOG_DEBUG_MESSAGE(Severity, ...)                                                                                            \
    do                                                                                                                              \
    {                                                                                                                               \
        std::string _msg = Diligent::FormatString(__VA_ARGS__);                                                                     \
        if (Diligent::DebugMessageCallback != nullptr) Diligent::DebugMessageCallback(Severity, _msg.c_str(), nullptr, nullptr, 0); \
    } while (false)

#define LOG_FATAL_ERROR_MESSAGE(...) LOG_DEBUG_MESSAGE(Diligent::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR, ##__VA_ARGS__)
#define LOG_ERROR_MESSAGE(...)       LOG_DEBUG_MESSAGE(Diligent::DEBUG_MESSAGE_SEVERITY_ERROR, ##__VA_ARGS__)
#define LOG_WARNING_MESSAGE(...)     LOG_DEBUG_MESSAGE(Diligent::DEBUG_MESSAGE_SEVERITY_WARNING, ##__VA_ARGS__)
#define LOG_INFO_MESSAGE(...)        LOG_DEBUG_MESSAGE(Diligent::DEBUG_MESSAGE_SEVERITY_INFO, ##__VA_ARGS__)

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

#define LOG_DEBUG_MESSAGE_ONCE(Severity, ...)           \
    do                                                  \
    {                                                   \
        static bool IsFirstTime = true;                 \
        if (IsFirstTime)                                \
        {                                               \
            LOG_DEBUG_MESSAGE(Severity, ##__VA_ARGS__); \
            IsFirstTime = false;                        \
        }                                               \
    } while (false)

#define LOG_FATAL_ERROR_MESSAGE_ONCE(...) LOG_DEBUG_MESSAGE_ONCE(Diligent::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR, ##__VA_ARGS__)
#define LOG_ERROR_MESSAGE_ONCE(...)       LOG_DEBUG_MESSAGE_ONCE(Diligent::DEBUG_MESSAGE_SEVERITY_ERROR, ##__VA_ARGS__)
#define LOG_WARNING_MESSAGE_ONCE(...)     LOG_DEBUG_MESSAGE_ONCE(Diligent::DEBUG_MESSAGE_SEVERITY_WARNING, ##__VA_ARGS__)
#define LOG_INFO_MESSAGE_ONCE(...)        LOG_DEBUG_MESSAGE_ONCE(Diligent::DEBUG_MESSAGE_SEVERITY_INFO, ##__VA_ARGS__)


#define CHECK_EXPR(Expr, Severity, ...)                 \
    do                                                  \
    {                                                   \
        if (!(Expr))                                    \
        {                                               \
            LOG_DEBUG_MESSAGE(Severity, ##__VA_ARGS__); \
        }                                               \
    } while (false)

#define CHECK_FATAL_ERR(Expr, ...) CHECK_EXPR(Expr, Diligent::DEBUG_MESSAGE_SEVERITY_FATAL_ERROR, ##__VA_ARGS__)
#define CHECK_ERR(Expr, ...)       CHECK_EXPR(Expr, Diligent::DEBUG_MESSAGE_SEVERITY_ERROR, ##__VA_ARGS__)
#define CHECK_WARN(Expr, ...)      CHECK_EXPR(Expr, Diligent::DEBUG_MESSAGE_SEVERITY_WARNING, ##__VA_ARGS__)
#define CHECK_INFO(Expr, ...)      CHECK_EXPR(Expr, Diligent::DEBUG_MESSAGE_SEVERITY_INFO, ##__VA_ARGS__)

#define CHECK_THROW(Expr, ...)                  \
    do                                          \
    {                                           \
        if (!(Expr))                            \
        {                                       \
            LOG_ERROR_AND_THROW(##__VA_ARGS__); \
        }                                       \
    } while (false)


#define ASSERT_SIZEOF(Struct, Size, ...) static_assert(sizeof(Struct) == Size, "sizeof(" #Struct ") is expected to be " #Size ". " __VA_ARGS__)

#if UINTPTR_MAX == UINT64_MAX
#    define ASSERT_SIZEOF64 ASSERT_SIZEOF
#else
#    define ASSERT_SIZEOF64(...)
#endif
