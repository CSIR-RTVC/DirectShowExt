cmake_minimum_required(VERSION 3.11)
project(libDirectShowExt VERSION 1.0.0 LANGUAGES CXX)

#Make sure that custom modules like FindRapidJSON are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

include(FetchContent)

FetchContent_Declare(
  BaseClasses 
  GIT_REPOSITORY https://github.com/CSIR-RTVC/baseclasses
)

##############################################
# Declare dependencies
find_package(BaseClasses 1.0.0 REQUIRED )

##############################################
# Create target and set properties
SET(PROJ_HDRS
    ./include/DirectShowExt/CodecControlInterface.h
    ./include/DirectShowExt/ComTypeDefs.h
    ./include/DirectShowExt/Crossbar.h
    ./include/DirectShowExt/CSettingsInterface.h
    ./include/DirectShowExt/CStatusInterface.h
    ./include/DirectShowExt/CustomBaseFilter.h
    ./include/DirectShowExt/CustomMediaTypes.h
    ./include/DirectShowExt/DirectShowHelper.h
    ./include/DirectShowExt/DirectShowHelper.h
    ./include/DirectShowExt/DirectShowMediaFormats.h
    ./include/DirectShowExt/FilterParameterStringConstants.h
    ./include/DirectShowExt/FilterPropertiesBase.h
    ./include/DirectShowExt/IFileInfoSourceInterface.h
    ./include/DirectShowExt/IMediaBufferDuration.h
    ./include/DirectShowExt/MultiIOBaseFilter.h
    ./include/DirectShowExt/MultiIOInputPin.h
    ./include/DirectShowExt/MultiIOOutputPin.h
    ./include/DirectShowExt/NetworkReferenceClock.h
    ./include/DirectShowExt/NotifyCodes.h
    ./include/DirectShowExt/ParameterConstants.h
    ./include/DirectShowExt/SettingsInterface.h
    ./include/DirectShowExt/StatusInterface.h
    ./include/DirectShowExt/VideoMixingBase.h
    )

SET(PROJ_SRCS
    ./src/Crossbar.cpp
    ./src/CSettingsInterface.cpp
    ./src/CStatusInterface.cpp
    ./src/CustomBaseFilter.cpp
    ./src/DirectShowHelper.cpp
    ./src/DirectShowHelper.cpp
    ./src/MultiIOBaseFilter.cpp
    ./src/MultiIOInputPin.cpp
    ./src/MultiIOOutputPin.cpp
    ./src/NetworkReferenceClock.cpp
    ./src/stdafx.cpp
    ./src/VideoMixingBase.cpp
    )

ADD_LIBRARY(DirectShowExt STATIC ${PROJ_SRCS} ${PROJ_HDRS})

#Add an alias so that library can be used inside the build tree, e.g. when testing
#add_library(DirectShowExt::DirectShowExt ALIAS DirectShowExt)

#Set target properties
target_include_directories(DirectShowExt
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(DirectShowExt PRIVATE cxx_auto_type)
target_compile_options(DirectShowExt PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

target_link_libraries(DirectShowExt
    PUBLIC
        BaseClasses::BaseClasses
    PRIVATE
)

##############################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/DirectShowExt)

install(TARGETS DirectShowExt
    EXPORT DirectShowExt-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#This is required so that the exported target has the name DirectShowExt and not DirectShowExt
# set_target_properties(DirectShowExt PROPERTIES EXPORT_NAME DirectShowExt)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT DirectShowExt-targets
  FILE
    DirectShowExtTargets.cmake
  NAMESPACE
    DirectShowExt::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/DirectShowExtConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/DirectShowExtConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/DirectShowExtConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/DirectShowExtConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/DirectShowExtConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

##############################################

export(EXPORT DirectShowExt-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/DirectShowExtTargets.cmake NAMESPACE DirectShowExt::)

#Register package in user's package registry
export(PACKAGE DirectShowExt)

#add_subdirectory(test)
