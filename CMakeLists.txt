cmake_minimum_required(VERSION 3.20)
project(MIA_P1_201900629)

set(CMAKE_CXX_STANDARD 14)

add_executable(MIA_P1_201900629 
		main.cpp
		parser.cpp
		parser.h
		scanner.cpp
		scanner.h mbr.h partition.h mkdisk.cpp mkdisk.h rmdisk.h rmdisk.cpp fdisk.h fdisk.cpp ebr.h aux_structs.h mount.cpp mount.h)
