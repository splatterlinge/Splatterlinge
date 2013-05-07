project( Ununoctium )
cmake_minimum_required( VERSION 2.8.0 )

find_package( Doxygen )
if( DOXYGEN_FOUND )
	add_custom_target( doc ${DOXYGEN_EXECUTABLE} Doxyfile )
	set_directory_properties( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "doc/html;doc/latex;" )
else( DOXYGEN_FOUND )
	message( "Doxygen not found - 'doc' can't be generated." )
endif( DOXYGEN_FOUND )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

find_package( Qt4 COMPONENTS QtCore QtGui QtOpengl REQUIRED )
find_package( OpenGL )
find_package( OpenAL )
find_package( VorbisFile )

include( ${QT_USE_FILE} )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin )

# Collect all source and header files
file( GLOB_RECURSE Ununoctium_SRCS src/*.c src/*.cpp)
file( GLOB_RECURSE Ununoctium_HDRS src/*.h src/*.hpp)

# Just in case you need additional libraries - add them here
set( Ununoctium_LIBS
	${QT_LIBRARIES}
	${OPENGL_LIBRARIES}
	${OPENAL_LIBRARY}
	${VorbisFile_LIBRARY}
)

# Additional include directories
set( Ununoctium_INCS
	${QT_INCLUDES}
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
qt4_wrap_cpp( Ununoctium_SRCS ${Ununoctium_HDRS_MOC} )

include_directories( ${Ununoctium_INCS} )
add_definitions( ${Ununoctium_DEFS} )
add_executable( Ununoctium ${Ununoctium_SRCS} )
target_link_libraries( Ununoctium ${Ununoctium_LIBS} )
