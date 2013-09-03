# - Find the Oculus Rift SDK
# This module defines the following variables:
#  OVR_INCLUDES - include directories for OVR
#  OVR_LIBRARIES - libraries to link against OVR
#  OVR_FOUND - true if OVR has been found and can be used
#  OBR_DEFINITIONS - defines OVR_ENABLED

if(NOT OVR_SDK_ROOT)
set(OVR_SDK_ROOT "/opt/OculusSDK")
endif(NOT OVR_SDK_ROOT)

find_path(OVR_INCLUDE_DIR OVR.h PATHS
	"${OVR_SDK_ROOT}/LibOVR/Include")

find_library(OVR_LIBRARY NAMES OVR ovr PATHS
	"${OVR_SDK_ROOT}/LibOVR/Lib/Linux/Release/x86_64"
	"${OVR_SDK_ROOT}/LibOVR/Lib/Linux/Release/i386" )

find_package( Threads REQUIRED )
find_package( Xinerama REQUIRED )
find_package( UDev REQUIRED )

set(OVR_INCLUDES ${OVR_INCLUDE_DIR} ${XINERAMA_INCLUDE_DIR})
set(OVR_LIBRARIES ${OVR_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${XINERAMA_LIBRARIES} ${UDEV_LIBRARIES})
set(OVR_DEFINITIONS "-DOVR_ENABLED")

find_package_handle_standard_args(OVR REQUIRED_VARS OVR_INCLUDE_DIR OVR_LIBRARY)

mark_as_advanced(OVR_INCLUDE_DIR OVR_LIBRARY)
