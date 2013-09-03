project( Splatterlinge )
cmake_minimum_required( VERSION 2.8.0 )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )

set( USE_QT5 FALSE CACHE BOOL "Switches between Qt5 and Qt4 - defaults to FALSE (Qt4)" )
if( USE_QT5 )
	message( STATUS "Using Qt5" )
	find_package( Qt5Core REQUIRED )
else( USE_QT5 )
	message( STATUS "Using Qt4" )
	find_package( Qt4 COMPONENTS QtCore QtGui QtOpengl REQUIRED )
	include( ${QT_USE_FILE} )
endif( USE_QT5 )

set( USE_OVR FALSE CACHE BOOL "Enables Oculus Rift support" )
set( OVR_SDK_ROOT CACHE STRING "/opt/OculusSDK" )
if( USE_OVR )
	message( STATUS "Oculus Rift enabled" )
	find_package( OVR REQUIRED )
endif( USE_OVR )

find_package( OpenGL REQUIRED )
find_package( GLEW REQUIRED )
find_package( OpenAL REQUIRED )
find_package( VorbisFile REQUIRED )

find_package( Doxygen )
if( DOXYGEN_FOUND )
	add_custom_target( doc ${DOXYGEN_EXECUTABLE} Doxyfile )
	set_directory_properties( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "doc/html;doc/latex;" )
else( DOXYGEN_FOUND )
	message( "Doxygen not found - 'doc' can't be generated." )
endif( DOXYGEN_FOUND )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin )

# Collect all source and header files
file( GLOB_RECURSE Splatterlinge_SRCS src/*.c src/*.cpp)
file( GLOB_RECURSE Splatterlinge_HDRS src/*.h src/*.hpp)

# Just in case you need additional libraries - add them here
set( Splatterlinge_LIBS
	${QT_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARY}
	${OPENAL_LIBRARY}
	${VorbisFile_LIBRARY}
	${OVR_LIBRARIES}
)

# Additional include directories
set( Splatterlinge_INCS
	${QT_INCLUDES}
	${GLEW_INCLUDE_DIR}
	${OPENAL_INCLUDE_DIR}
	${OPENAL_INCLUDE_DIR}
	${OVR_INCLUDES}
	./src/
)

# Additional compiler definitions
set( Splatterlinge_DEFS
	${QT_DEFINITIONS}
	${OVR_DEFINITIONS}
	-Wall
)

# Create a list of header files containing classes that
#  require Qt's Meta Object Compiler (all headers containing Q_OBJECT)
set( Splatterlinge_HDRS_MOC )
foreach( header ${Splatterlinge_HDRS} )
	file( STRINGS "${header}" lines REGEX "Q_OBJECT" )
	if( lines )
		list( APPEND Splatterlinge_HDRS_MOC "${header}" )
	endif()
endforeach()
# Run the MOC
if( USE_QT5 )
	qt5_wrap_cpp( Splatterlinge_SRCS ${Splatterlinge_HDRS_MOC} )
else( USE_QT5 )
	qt4_wrap_cpp( Splatterlinge_SRCS ${Splatterlinge_HDRS_MOC} )
endif( USE_QT5 )

include_directories( ${Splatterlinge_INCS} )
add_definitions( ${Splatterlinge_DEFS} )
add_executable( Splatterlinge ${Splatterlinge_SRCS} )
target_link_libraries( Splatterlinge ${Splatterlinge_LIBS} )

if( USE_QT5 )
	qt5_use_modules( Splatterlinge Core Gui Widgets OpenGL )
endif( USE_QT5 )
