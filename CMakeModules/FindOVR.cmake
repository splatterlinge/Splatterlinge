# - Find the Oculus Rift SDK
# This module defines the following variables:
#  OVR_INCLUDES - include directories for OVR
#  OVR_LIBRARIES - libraries to link against OVR
#  OVR_FOUND - true if OVR has been found and can be used
#  OBR_DEFINITIONS - defines OVR_ENABLED

if(NOT OVR_SDK_ROOT)
	set(OVR_SDK_ROOT "/opt/OculusSDK")
	message(STATUS "Variable OVR_SDK_ROOT not set - using default OculusSDK directory: " ${OVR_SDK_ROOT} )
endif(NOT OVR_SDK_ROOT)

find_path(OVR_INCLUDE_DIR OVR.h
	NO_DEFAULT_PATH CMAKE_FIND_ROOT_PATH_BOTH
	PATHS "${OVR_SDK_ROOT}/LibOVR/Include")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	if(NOT WIN32)
		set(OVR_LIBRARY_PATHS "${OVR_SDK_ROOT}/LibOVR/Lib/Linux/Release/x86_64")
	else(NOT WIN32)
		if(MINGW)
			set(OVR_LIBRARY_PATHS "${OVR_SDK_ROOT}/LibOVR/Lib/MinGW/Release/w64")
		else(MINGW)
			set(OVR_LIBRARY_PATHS "${OVR_SDK_ROOT}/LibOVR/Lib/x64")
		endif(MINGW)
	endif(NOT WIN32)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
	if(NOT WIN32)
		set(OVR_LIBRARY_PATHS "${OVR_SDK_ROOT}/LibOVR/Lib/Linux/Release/i386")
	else(NOT WIN32)
		if(MINGW)
			set(OVR_LIBRARY_PATHS "${OVR_SDK_ROOT}/LibOVR/Lib/MinGW/Release/w32")
		else(MINGW)
			set(OVR_LIBRARY_PATHS "${OVR_SDK_ROOT}/LibOVR/Lib/Win32")
		endif(MINGW)
	endif(NOT WIN32)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

find_library(OVR_LIBRARY NAMES OVR ovr libOVR libovr
	NO_DEFAULT_PATH CMAKE_FIND_ROOT_PATH_BOTH
	PATHS ${OVR_LIBRARY_PATHS})

set(OVR_INCLUDES ${OVR_INCLUDE_DIR})
set(OVR_LIBRARIES ${OVR_LIBRARY} )
set(OVR_DEFINITIONS "-DOVR_ENABLED")

find_package( Threads REQUIRED )
set(OVR_LIBRARIES ${OVR_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
if(NOT WIN32)
	find_package( X11 REQUIRED )
	find_package( Xinerama REQUIRED )
	find_package( UDev REQUIRED )
	set( OVR_INCLUDES ${OVR_INCLUDES} ${XINERAMA_INCLUDE_DIR} ${X11_INCLUDE_DIR} )
	set( OVR_LIBRARIES ${OVR_LIBRARIES} ${XINERAMA_LIBRARIES} ${UDEV_LIBRARIES} ${X11_LIBRARIES} )
endif(NOT WIN32)

find_package_handle_standard_args(OVR REQUIRED_VARS OVR_INCLUDE_DIR OVR_LIBRARY)

mark_as_advanced(OVR_INCLUDE_DIR OVR_LIBRARY)
