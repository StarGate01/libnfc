# - Try to find the PC/SC smart card  library
# Once done this will define
#
#  PCSC_FOUND - system has the PC/SC library
#  PCSC_INCLUDE_DIRS - the PC/SC include directory
#  PCSC_LIBRARIES - The libraries needed to use PC/SC
#
# Author: F. Kooman <fkooman@tuxed.net>
# Version: 20101019
#

FIND_PACKAGE (PkgConfig)
IF(PKG_CONFIG_FOUND)
    # Will find PC/SC library on Linux/BSDs using PkgConfig
    PKG_CHECK_MODULES(NCI libnfc-nci)
#   PKG_CHECK_MODULES(PCSC QUIET libpcsclite)   # IF CMake >= 2.8.2?
ENDIF(PKG_CONFIG_FOUND)

IF(NOT NCI_FOUND)
   FIND_PATH(NCI_INCLUDE_DIRS linux_nfc_api.h)
   FIND_LIBRARY(NCI_LIBRARIES NAMES NCI libnfc_nci_linux)
ENDIF(NOT NCI_FOUND)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NCI DEFAULT_MSG
  NCI_LIBRARIES
  NCI_INCLUDE_DIRS
)
MARK_AS_ADVANCED(NCI_INCLUDE_DIRS NCI_LIBRARIES)
