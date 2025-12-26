cmake_minimum_required(VERSION 3.19)
project(uSPX_Robby LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets SerialPort)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(uSPX_Robby
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    CNC/cspxgraphicsview.cpp CNC/cspxgraphicsview.h
    CNC/cspxtableview.cpp CNC/cspxtableview.h
    resource.qrc
    CNC/cspxcheckboxdelegate.cpp CNC/cspxcheckboxdelegate.h   CNC/cspxgraphicsview.cpp CNC/cspxgraphicsview.h CNC/cspxhpglcommands.cpp CNC/cspxhpglcommands.h CNC/cspxrobbycontroller.cpp CNC/cspxrobbycontroller.h CNC/cspxtableview.cpp CNC/cspxtableview.h Comms/cspxMessage.cpp Comms/cspxMessage.h Containers/cspxarray.h Containers/cspxlist.h Containers/cspxmatrix.cpp Containers/cspxmatrix.h Containers/cspxparray.h Containers/cspxstack.h Containers/cspxstring.cpp Containers/cspxstring.h Containers/cspxTMatrix.h Containers/cspxTVector.cpp Containers/cspxTVector.h Containers/cspxtypes.h Containers/cspxutility.cpp Containers/cspxutility.h Containers/cspxvector.cpp Containers/cspxvector.h
    Comms/cspxCommunication.cpp Comms/cspxcommunication.h

)

target_link_libraries(uSPX_Robby
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::SerialPort
)

