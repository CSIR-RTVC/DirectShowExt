# CMakeLists.txt in DirectShowExt dir
# Adds a library called DirectShowExt
find_package(baseclasses 1.0 REQUIRED)

SET (DSE_INCLUDE_DIRS 
)

SET (DSE_INCLUDE_DEPENDENCIES
	${libDirectShowExt_SOURCE_DIR}/include/
	${libDirectShowExt_SOURCE_DIR}/include/DirectShowExt
	${PROJECT_SOURCE_DIR}/externals/DShowBaseClasses/baseclasses
)

# Lib directories
LINK_DIRECTORIES(
    $ENV{MSSDK}/Lib
)

SET(PROJ_HDRS
    ../include/DirectShowExt/CodecControlInterface.h
    ../include/DirectShowExt/ComTypeDefs.h
    ../include/DirectShowExt/Crossbar.h
    ../include/DirectShowExt/CSettingsInterface.h
    ../include/DirectShowExt/CStatusInterface.h
    ../include/DirectShowExt/CustomBaseFilter.h
    ../include/DirectShowExt/CustomMediaTypes.h
    ../include/DirectShowExt/DirectShowHelper.h
    ../include/DirectShowExt/DirectShowHelper.h
    ../include/DirectShowExt/DirectShowMediaFormats.h
    ../include/DirectShowExt/FilterParameterStringConstants.h
    ../include/DirectShowExt/FilterPropertiesBase.h
    ../include/DirectShowExt/IFileInfoSourceInterface.h
    ../include/DirectShowExt/IMediaBufferDuration.h
    ../include/DirectShowExt/MultiIOBaseFilter.h
    ../include/DirectShowExt/MultiIOInputPin.h
    ../include/DirectShowExt/MultiIOOutputPin.h
    ../include/DirectShowExt/NetworkReferenceClock.h
    ../include/DirectShowExt/NotifyCodes.h
    ../include/DirectShowExt/ParameterConstants.h
    ../include/DirectShowExt/SettingsInterface.h
    ../include/DirectShowExt/StatusInterface.h
    stdafx.h
    ../include/DirectShowExt/VideoMixingBase.h
    )

SET(PROJ_SRCS
    Crossbar.cpp
    CSettingsInterface.cpp
    CStatusInterface.cpp
    CustomBaseFilter.cpp
    DirectShowHelper.cpp
    DirectShowHelper.cpp
    MultiIOBaseFilter.cpp
    MultiIOInputPin.cpp
    MultiIOOutputPin.cpp
    NetworkReferenceClock.cpp
    stdafx.cpp
    VideoMixingBase.cpp
    )

ADD_LIBRARY(DirectShowExt STATIC ${PROJ_SRCS} ${PROJ_HDRS})
#export(EXPORT DirectShowExt FILE DirectShowExtTargets.cmake NAMESPACE DirectShowExt::)
#install(EXPORT DirectShowExt FILE DirectShowExtTargets.cmake COMPONENT DirectShowExt NAMESPACE DirectShowExt:: DESTINATION cmake CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES})
install(TARGETS DirectShowExt
    EXPORT DirectShowExtConfig
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
export(TARGETS
        DirectShowExt
    NAMESPACE DirectShowExt::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/DirectShowExtConfig.cmake"
)
install(EXPORT DirectShowExtConfig
  FILE
    DirectShowExtConfig.cmake
  NAMESPACE
    DirectShowExt::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/DirectShowExt
)

target_include_directories(DirectShowExt
    PUBLIC 
        $<INSTALL_INTERFACE:${DSE_INCLUDE_DIRS}>    
        $<BUILD_INTERFACE:${DSE_INCLUDE_DIRS}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
		${DSE_INCLUDE_DEPENDENCIES}
)
#target_compile_options(DirectShowExt PRIVATE -Werror)
target_compile_features(DirectShowExt PRIVATE cxx_std_11)

# adding precompiled header support on windows
if (MSVC)
    set_target_properties(DirectShowExt PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
    set_source_files_properties(stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif (MSVC)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    SET_TARGET_PROPERTIES(DirectShowExt PROPERTIES DEBUG_POSTFIX "D")
    message("DirectShowExt Debug Build")
else (CMAKE_BUILD_TYPE STREQUAL Debug)
    SET_TARGET_PROPERTIES(DirectShowExt PROPERTIES DEBUG_POSTFIX "")
    message("DirectShowExt Release Build")
endif (CMAKE_BUILD_TYPE STREQUAL Debug)
