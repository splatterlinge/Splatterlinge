project( Ununoctium )
cmake_minimum_required( VERSION 2.8.0 )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )
set( QT_VERSION "4.8.0" CACHE STRING "QT Version" )

message( STATUS ${QT_VERSION} )
if( QT_VERSION LESS "5.0" )
    find_package( Qt4 COMPONENTS QtCore QtGui QtOpengl REQUIRED )
    include( ${QT_USE_FILE} )
else( QT_VERSION LESS "5.0" )
    find_package( Qt5Core REQUIRED )
endif( QT_VERSION LESS "5.0" )
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
file( GLOB_RECURSE Ununoctium_SRCS src/*.c src/*.cpp)
file( GLOB_RECURSE Ununoctium_HDRS src/*.h src/*.hpp)

# Just in case you need additional libraries - add them here
set( Ununoctium_LIBS
	${QT_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARY}
	${OPENAL_LIBRARY}
	${VorbisFile_LIBRARY}
)

# Additional include directories
set( Ununoctium_INCS
	${QT_INCLUDES}
	${GLEW_INCLUDE_DIR}
	${OPENAL_INCLUDE_DIR}
	./src/
)

# Additional compiler definitions
set( Ununoctium_DEFS
	${QT_DEFINITIONS}
	-Wall
)

set( Ununoctium_HDRS_MOC )
foreach( header ${Ununoctium_HDRS})
	file( STRINGS "${header}" lines REGEX "Q_OBJECT" )
	if( lines )
		list( APPEND Ununoctium_HDRS_MOC "${header}" )
	endif()
endforeach()

if( QT_VERSION LESS "5.0" )
    qt4_wrap_cpp( Ununoctium_SRCS ${Ununoctium_HDRS_MOC} )
else( QT_VERSION LESS "5.0" )
    qt5_wrap_cpp( Ununoctium_SRCS ${Ununoctium_HDRS_MOC} )
endif( QT_VERSION LESS "5.0" )

include_directories( ${Ununoctium_INCS} )
add_definitions( ${Ununoctium_DEFS} )
add_executable( Ununoctium ${Ununoctium_SRCS} )
target_link_libraries( Ununoctium ${Ununoctium_LIBS} )
if( NOT QT_VERSION LESS "5.0" )
    qt5_use_modules( Ununoctium Core Gui Widgets OpenGL )
endif( NOT QT_VERSION LESS "5.0" )
