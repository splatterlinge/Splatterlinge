project( Ununoctium )
cmake_minimum_required( VERSION 2.8.0 )

find_package( Qt4 COMPONENTS QtCore QtGui QtOpengl REQUIRED )
find_package( OpenGL )

include( ${QT_USE_FILE} )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin )

# Collect all source and header files
file( GLOB_RECURSE Ununoctium_SRCS src/*.c src/*.cpp)
file( GLOB_RECURSE Ununoctium_HDRS src/*.h)

# Just in case you need additional libraries - add them here
set( Ununoctium_LIBS
	${QT_LIBRARIES}
	${OPENGL_LIBRARIES}
)

# Additional include directories
set( Ununoctium_INCS
	${QT_INCLUDES}
)

# Additional compiler definitions
set( Ununoctium_DEFS
	${QT_DEFINITIONS}
)

set( Ununoctium_HDRS_MOC )
foreach( header ${Ununoctium_HDRS})
	file( STRINGS "${header}" lines REGEX "Q_OBJECT" )
	if( lines )
		list( APPEND Ununoctium_HDRS_MOC "${header}" )
	endif()
endforeach()
qt4_wrap_cpp( Ununoctium_SRCS ${Ununoctium_HDRS_MOC} )

include_directories( ${Ununoctium_INCS} )
add_definitions( ${Ununoctium_DEFS} )
add_executable( Ununoctium ${Ununoctium_SRCS} )
target_link_libraries( Ununoctium ${Ununoctium_LIBS} )
