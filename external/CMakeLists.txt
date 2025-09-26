# add compile time macros for static linking and other stuff
add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DGLEW_STATIC
	-DTW_NO_DIRECT3D
	-D_CRT_SECURE_NO_WARNINGS
)



# GLFW


# Don't build the GLFW documentation, tests, or examples
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")


# run GLFW's CMake
add_subdirectory (glfw-3.1.2)

# include directories to compiler header search path
include_directories(
	glfw-3.1.2/include/
	glew-1.13.0/include/
)


# Configure OPENGL_LIBRARY for Linux or MacOS
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")



# GLEW


# identify source and headers for GLEW static library
set(GLEW_SOURCE glew-1.13.0/src/glew.c)
set(GLEW_HEADERS glew-1.13.0/include/GL/glew.h)

# create and compile GLEW static library
add_library( GLEW_1130 STATIC
	${GLEW_SOURCE}
	${GLEW_HEADERS}
)

# link GLEW static library with OPENGL_LIBRARY and user defined libraries
target_link_libraries(GLEW_1130
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)