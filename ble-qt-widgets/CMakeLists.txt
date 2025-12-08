
cmake_minimum_required(VERSION 3.21)
project(BleUartQtWidgets LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets Bluetooth)
qt_standard_project_setup()

qt_add_executable(ble_uart_qt_widgets
    main.cpp
    MainWindow.h
    MainWindow.cpp
    blecontroller.h blecontroller.cpp
)

target_link_libraries(ble_uart_qt_widgets
    PRIVATE Qt6::Widgets Qt6::Bluetooth
)
