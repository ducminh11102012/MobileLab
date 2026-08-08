cmake_minimum_required(VERSION 3.20)
project(MobileLabAndroid VERSION 0.3.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
qt_add_executable(mobilelab-android
 src/main.cpp src/MainWindow.cpp src/MainWindow.h
 src/core/AndroidRuntime.cpp src/core/AndroidRuntime.h
 src/core/ResourceScheduler.cpp src/core/ResourceScheduler.h
 src/core/ApiServer.cpp src/core/ApiServer.h
 src/core/ArmVirtualMachine.cpp src/core/ArmVirtualMachine.h
)
target_link_libraries(mobilelab-android PRIVATE Qt6::Widgets Qt6::Network)
target_compile_definitions(mobilelab-android PRIVATE MOBILELAB_ANDROID_UI=1 MOBILELAB_ARM_RUNTIME=1)
install(TARGETS mobilelab-android RUNTIME DESTINATION bin)
