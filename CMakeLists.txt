cmake_minimum_required(VERSION 3.16)

project(BezierSurface-ComputerGraphics-2024 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB TXT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/bezierSurfaces/*.txt")
foreach(TXT_FILE ${TXT_FILES})
    get_filename_component(TXT_FILE_NAME ${TXT_FILE} NAME)
    configure_file(${TXT_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${TXT_FILE_NAME} COPYONLY)
endforeach()

file(GLOB MESHES "${CMAKE_CURRENT_SOURCE_DIR}/assets/meshes/*")
foreach(MESH ${MESHES})
    get_filename_component(MESH_NAME ${MESH} NAME)
    configure_file(${MESH} ${CMAKE_CURRENT_BINARY_DIR}/meshes/${MESH_NAME} COPYONLY)
endforeach()

file(GLOB TEXTURES "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/*.png" "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/*.jpg")
foreach(TEXTURE ${TEXTURES})
    get_filename_component(TEXTURE_NAME ${TEXTURE} NAME)
    configure_file(${TEXTURE} ${CMAKE_CURRENT_BINARY_DIR}/textures/${TEXTURE_NAME} COPYONLY)
endforeach()

file(GLOB NORMALMAPS "${CMAKE_CURRENT_SOURCE_DIR}/assets/normalMaps/*.png" "${CMAKE_CURRENT_SOURCE_DIR}/assets/normalMaps/*.jpg")
foreach(NORMALMAP ${NORMALMAPS})
    get_filename_component(NORMALMAP_NAME ${NORMALMAP} NAME)
    configure_file(${NORMALMAP} ${CMAKE_CURRENT_BINARY_DIR}/normalMaps/${NORMALMAP_NAME} COPYONLY)
endforeach()

file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp" include/*.h)
set(PROJECT_SOURCES
        ${PROJECT_SOURCES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BezierSurface-ComputerGraphics-2024
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET BezierSurface-ComputerGraphics-2024 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(BezierSurface-ComputerGraphics-2024 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(BezierSurface-ComputerGraphics-2024
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(BezierSurface-ComputerGraphics-2024 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(BezierSurface-ComputerGraphics-2024 PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.BezierSurface-ComputerGraphics-2024)
endif()
set_target_properties(BezierSurface-ComputerGraphics-2024 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS BezierSurface-ComputerGraphics-2024
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BezierSurface-ComputerGraphics-2024)
endif()
