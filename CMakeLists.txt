cmake_minimum_required(VERSION 2.8.3)
project(interbeacon)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
  range_msgs
)

## Generate messages in the 'msg' folder
 add_message_files(
   FILES
   measure.msg
   activar.msg
 )


## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs  # Or other packages containing msgs
 )

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(interbeacon_test_node src/test.cpp src/cntronmod.cpp src/cntronbase.cpp)
add_dependencies(interbeacon_test_node ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(interbeacon_test_node ${catkin_LIBRARIES})

add_executable(mobile_node src/mobile.cpp src/cntronmod.cpp src/cntronbase.cpp)
add_dependencies(mobile_node ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(mobile_node
   ${catkin_LIBRARIES}
 )

add_executable(static_beacon src/static.cpp src/cntronmod.cpp src/cntronbase.cpp)
add_dependencies(static_beacon ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(static_beacon
   ${catkin_LIBRARIES}
 )

add_executable(busca_nodo src/busca_nodo.cpp src/cntronmod.cpp src/cntronbase.cpp)
add_dependencies(busca_nodo ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(busca_nodo
   ${catkin_LIBRARIES}
 )

add_executable(euroc_range src/euroc_range.cpp src/cntronmod.cpp src/cntronbase.cpp)
add_dependencies(euroc_range ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(euroc_range
   ${catkin_LIBRARIES}
 )

add_executable(euroc_oneRange src/euroc_oneRange.cpp src/cntronmod.cpp src/cntronbase.cpp)
add_dependencies(euroc_oneRange ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(euroc_oneRange
   ${catkin_LIBRARIES}
 )


