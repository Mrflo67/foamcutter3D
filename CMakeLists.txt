# CMakeList.txt : projet CMake pour ProjetCmake, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)




set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})


#includes 
find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)




add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Include/Header/)


####Glew

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Include/Glew/)






ADD_DEFINITIONS(-DGLEW_STATIC)
#executable config


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(REMOVE 
	"src/Include/Cpp/main.cpp"	
)

file(GLOB_RECURSE
	source_files
	src/*
)

add_executable (ProjetCmake ${source_files})

#linking

target_link_libraries(ProjetCmake glfw ${GLFW_LIBRARIES})
target_link_libraries(ProjetCmake ${OPENGL_gl_LIBRARY})


# TODO: Ajoutez des tests et installez des cibles si nécessaire.
