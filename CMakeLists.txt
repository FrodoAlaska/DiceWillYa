cmake_minimum_required(VERSION 3.27)
project(NikolaProjectTemplate) # @TODO: Change the name of your project from here

### FetchContent ###
############################################################
include(FetchContent)

# We must include these variables to turn off 
# certain outputs of external libraries. Very ugly, I know.

set(ALSOFT_EXAMPLES OFF)
set(LIBTYPE         STATIC)
set(qu3e_build_demo OFF)

# Setting the state of Nikola
set(NIKOLA_BUILD_TESTBED OFF)
set(NIKOLA_BUILD_NBR     OFF)

FetchContent_Declare(
  nikola
  GIT_REPOSITORY https://github.com/FrodoAlaska/Nikola.git 
)

FetchContent_MakeAvailable(nikola)
############################################################

### Project variables ###
############################################################
set(PROJECT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

set(PROJECT_INCLUDES 
  ${PROJECT_INCLUDE_DIR}
  ${PROJECT_LIBS_DIR}
  
  ${nikola_SOURCE_DIR}/nikola/include
  ${glfw_SOURCE_DIR}/include
  ${glm_SOURCE_DIR}
)

include(cmake/variables.cmake)
############################################################

### CMake variables ###
############################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
############################################################

### Project sources ###
############################################################
set(PROJECT_SOURCES
  # Core
  ${PROJECT_SRC_DIR}/app.cpp
  ${PROJECT_SRC_DIR}/main.cpp
)
############################################################

### Targets ###
############################################################
if(NIKOLA_DISTRIBUTE)
  set(EXE_TYPE WIN32)
else()
  set(EXE_TYPE "")
endif()

add_executable(${PROJECT_NAME} ${EXE_TYPE} ${PROJECT_SOURCES})
############################################################

### Linking ###
############################################################
target_include_directories(${PROJECT_NAME} PUBLIC BEFORE ${PROJECT_INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC nikola)
############################################################

### Compiling options ###
############################################################
target_compile_options(${PROJECT_NAME} PUBLIC ${PROJECT_BUILD_FLAGS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_BUILD_DEFINITIONS})
############################################################
