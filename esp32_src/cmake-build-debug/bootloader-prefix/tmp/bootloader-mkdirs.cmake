# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/xl/esp/esp-idf/components/bootloader/subproject"
  "/home/xl/esp/01_huizhong/cmake-build-debug/bootloader"
  "/home/xl/esp/01_huizhong/cmake-build-debug/bootloader-prefix"
  "/home/xl/esp/01_huizhong/cmake-build-debug/bootloader-prefix/tmp"
  "/home/xl/esp/01_huizhong/cmake-build-debug/bootloader-prefix/src/bootloader-stamp"
  "/home/xl/esp/01_huizhong/cmake-build-debug/bootloader-prefix/src"
  "/home/xl/esp/01_huizhong/cmake-build-debug/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/xl/esp/01_huizhong/cmake-build-debug/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/xl/esp/01_huizhong/cmake-build-debug/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
