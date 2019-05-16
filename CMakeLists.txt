cmake_minimum_required(VERSION 3.5)

set(KF5_VERSION "5.58.0") # handled by release scripts
set(KF5_DEP_VERSION "5.57.0") # handled by release scripts

project(KF5QuicCharts VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM 5.57.0 NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

include(GenerateExportHeader)

include(ECMMarkAsTest)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMAddQch)
include(ECMMarkNonGuiExecutable)
include(ECMQtDeclareLoggingCategory)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

ecm_setup_version(
  PROJECT
  VARIABLE_PREFIX KF5QuickCharts
#  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/kio_version.h"
#   PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5KIOConfigVersion.cmake"
  SOVERSION 1)

option(BUILD_EXAMPLES "Build example applications" OFF)

set(REQUIRED_QT_VERSION 5.10.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Qml Quick)

add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# if(BUILD_TESTING)
#     add_subdirectory(autotests)
#     if (NOT KIOCORE_ONLY)
#         add_subdirectory(tests)
#     endif()
# endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

