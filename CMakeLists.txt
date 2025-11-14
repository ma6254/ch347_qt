cmake_minimum_required(VERSION 3.16)

project(ch347_qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Svg)

add_subdirectory("vendor/yaml-cpp")

set(TS_FILES assets/zh_CN.ts)

set(QM_FILES assets/main.qrc)

include_directories(
    vendor/yaml-cpp/include
    src
    src/main_window
    src/views
    src/components/theme
    src/components/config
    src/components/utils
)

file(
    GLOB
    HEADER_FILES
    src/*.h
    src/main_window/*.h
    src/views/*.h
    src/components/theme/*.h
    src/components/config/*.h
    src/components/utils/*.h
)

file(
    GLOB
    SRC_FILES
    src/*.cpp
    src/main_window/*.cpp
    src/views/*.cpp
    src/components/theme/*.cpp
    src/components/config/*.cpp
    src/components/utils/*.cpp
)

file(
    GLOB
    UI_FILES
    src/main_window/*.ui
    src/views/*.ui
)

execute_process(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT
)

add_definitions(-DGIT_COMMIT=\"${GIT_COMMIT}\")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ch347_qt
        MANUAL_FINALIZATION
        ${SRC_FILES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ch347_qt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(ch347_qt SHARED
            ${SRC_FILES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ch347_qt
            ${SRC_FILES}
            ${HEADER_FILES}
            ${UI_FILES}
            ${QM_FILES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(
    ch347_qt
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Svg
    yaml-cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ch347_qt)
endif()
set_target_properties(ch347_qt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ch347_qt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ch347_qt)
endif()
