message ("Source build.")

file (GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

include_directories ("${PROJECT_SOURCE_DIR}/Source")
add_executable (viewer ${SOURCES})

find_package (OpenGL REQUIRED)
find_package (GLUT REQUIRED)
include_directories (${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

target_link_libraries (viewer ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
