cmake_minimum_required(VERSION 3.5)

set(KF5_VERSION "5.73.0") # handled by release scripts
set(KF5_DEP_VERSION "5.73.0") # handled by release scripts

project(KQuickCharts VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM 5.73.0 NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

include(GenerateExportHeader)

include(ECMAddTests)
include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_EXAMPLES "Build example applications" OFF)

set(REQUIRED_QT_VERSION 5.12.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Qml Quick QuickControls2)

add_subdirectory(controls)
add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

ecm_setup_version(
    PROJECT
    VARIABLE_PREFIX KF5QuickCharts
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/quickcharts_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5QuickChartsConfigVersion.cmake"
    SOVERSION 1
)

configure_package_config_file(
    "KF5QuickChartsConfig.cmake.in"
    "KF5QuickChartsConfig.cmake"
    INSTALL_DESTINATION ${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5QuickCharts
    PATH_VARS KF5_INCLUDE_INSTALL_DIR CMAKE_INSTALL_PREFIX
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5QuickChartsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5QuickChartsConfigVersion.cmake"
    DESTINATION ${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5QuickCharts
    COMPONENT Devel
)

install(EXPORT KF5QuickChartsTargets
    DESTINATION ${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5QuickCharts
    FILE KF5QuickChartsTargets.cmake
    NAMESPACE KF5::
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
