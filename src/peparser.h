#pragma once
#include <windows.h>


#define SIZE_OF_HEADERS(pFileData) \
    ((IMAGE_NT_HEADERS*)((unsigned char*)pFileData + ((IMAGE_DOS_HEADER*)pFileData)->e_lfanew))->OptionalHeader.SizeOfHeaders


#define SIZE_OF_IMAGE(pFileData) \
    (((IMAGE_NT_HEADERS*)((unsigned char*)(pFileData) + ((IMAGE_DOS_HEADER*)(pFileData))->e_lfanew))->OptionalHeader.SizeOfImage)


#define NUMBER_OF_SECTIONS(pFileData) \
    (((IMAGE_NT_HEADERS*)((unsigned char*)(pFileData) + ((IMAGE_DOS_HEADER*)(pFileData))->e_lfanew))->FileHeader.NumberOfSections)

#define SECTION_HEADER_AT(pFileData, index) \
    ((IMAGE_SECTION_HEADER*)((unsigned char*)IMAGE_FIRST_SECTION((IMAGE_NT_HEADERS*)((unsigned char*)pFileData + ((IMAGE_DOS_HEADER*)pFileData)->e_lfanew))) + index)

