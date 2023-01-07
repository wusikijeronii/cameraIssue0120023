
project(cameratest)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")
add_definitions(-DSDL_MAIN_HANDLED)

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}  main.cpp camera.cpp shader.cpp gl_core_4_5.c)


target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${SDL2_INCLUDE_DIR}>
	$<BUILD_INTERFACE:${OPENGL_INCLUDE_DIR}>
	$<BUILD_INTERFACE:${GLM_INCLUDE_DIRS}>)

include_directories("external/glm")
    
    
target_link_libraries(${PROJECT_NAME}
    PUBLIC opengl32
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/SDL2.dll"
    PUBLIC ${OPENGL_LIBRARIES}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)