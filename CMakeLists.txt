cmake_minimum_required(VERSION 3.17)
project(myproject)

set(CMAKE_CXX_STANDARD 14)



file(GLOB SOURCES
        Config.h						//*号对应具体的文件名
        Config.cpp
        Constant.h
        CRFModel.cpp
        CRFModel.h
        DataSet.cpp
        DataSet.h
        example.txt
        FactorGraph.cpp
        FactorGraph.h
        Transmitter.cpp
        Transmitter.h
        Util.cpp
        Util.h
        )

add_executable(myproject main.cpp ${SOURCE_FILES} ${SOURCES})

