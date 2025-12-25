cmake_minimum_required (VERSION 3.10)

project(Diligent-Primitives CXX)

set(INTERFACE
    interface/CheckBaseStructAlignment.hpp
    interface/CommonDefinitions.h
    interface/DataBlob.h
    interface/DefineInterfaceHelperMacros.h
    interface/FlagEnum.h
    interface/FileStream.h
    interface/InterfaceID.h
    interface/MemoryAllocator.h
    interface/Object.h
    interface/ReferenceCounters.h
    interface/Primitives.h
)

# This should be an interface library. However, CMake does not show
# interface libraries in an IDE, so we have to create a static library instead
add_library(Diligent-Primitives STATIC ${INTERFACE} ${SOURCE})
set_common_target_properties(Diligent-Primitives)

target_link_libraries(Diligent-Primitives
PRIVATE
    Diligent-BuildSettings
)

target_include_directories(Diligent-Primitives
PUBLIC
    interface
)

source_group("interface" FILES ${INTERFACE})

set_target_properties(Diligent-Primitives PROPERTIES
    FOLDER DiligentCore
)

if(DILIGENT_INSTALL_CORE)
    install_core_lib(Diligent-Primitives)
endif()
