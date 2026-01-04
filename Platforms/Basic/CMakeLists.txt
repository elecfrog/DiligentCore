cmake_minimum_required (VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Diligent-BasicPlatform CXX)


set(SOURCE
    src/BasicFileSystem.cpp
    src/BasicPlatformDebug.cpp
    src/BasicPlatformMisc.cpp
)

set(INTERFACE 
    interface/BasicFileSystem.hpp
    interface/BasicPlatformDebug.hpp
    interface/BasicPlatformMisc.hpp
    interface/DebugUtilities.hpp
)

set(INCLUDE
    include/SearchRecursive.inl
)

if(PLATFORM_LINUX OR PLATFORM_WIN32 OR PLATFORM_APPLE OR PLATFORM_WEB)
    list(APPEND SOURCE src/StandardFile.cpp)
    list(APPEND INTERFACE interface/StandardFile.hpp)
endif()

add_library(Diligent-BasicPlatform STATIC ${SOURCE} ${INTERFACE} ${INCLUDE})
set_common_target_properties(Diligent-BasicPlatform)

target_include_directories(Diligent-BasicPlatform 
PUBLIC
    interface
)

target_link_libraries(Diligent-BasicPlatform 
PRIVATE
    Diligent-BuildSettings 
PUBLIC
    Diligent-Primitives
)

source_group("src" FILES ${SOURCE})
source_group("interface" FILES ${INTERFACE})
source_group("include" FILES ${INCLUDE})

set_target_properties(Diligent-BasicPlatform PROPERTIES
    FOLDER DiligentCore/Platforms
)

if(DILIGENT_INSTALL_CORE)
    install_core_lib(Diligent-BasicPlatform)
    get_target_relative_dir(Diligent-BasicPlatform BASIC_PLATFORM_DIR)
    
    cmake_path(GET BASIC_PLATFORM_DIR PARENT_PATH PLATFORM_PARENT_DIR)
    
    install(DIRECTORY    "../interface"
            DESTINATION  "${CMAKE_INSTALL_INCLUDEDIR}/${PLATFORM_PARENT_DIR}"
    )
endif()
