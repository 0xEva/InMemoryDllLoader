#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <ntdef.h>
#include <ntstatus.h>
#include <winternl.h>
#include <wchar.h>
#include "MinHook.h"

#include "peparser.h"
#include "rawData.h"


#define FAKE_NAME L"lld.FAKE_DLL.dll"
// ValidHandle % 4 == 0
#define FAKE_HANDLE (HANDLE)(1234567)
#define FILE_OPENED 0x00000001
#define ThreadDynamicCodePolicyInfo 42


typedef enum _SECTION_INHERIT {
    ViewShare=1,
    ViewUnmap=2
} SECTION_INHERIT, *PSECTION_INHERIT;


typedef  void (NTAPI *MyNtOpenFile)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, ULONG, ULONG);
typedef  NTSTATUS (NTAPI *MyNtQueryAttributesFile)(POBJECT_ATTRIBUTES, PFILE_BASIC_INFORMATION);
typedef  NTSTATUS (NTAPI *MyNtCreateSection)(PHANDLE, ULONG, POBJECT_ATTRIBUTES, PLARGE_INTEGER, ULONG, ULONG, HANDLE);
typedef NTSTATUS (NTAPI *MyNtOpenSection)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES);
typedef NTSTATUS (NTAPI *MyNtMapViewOfSection)(HANDLE, HANDLE, PVOID, ULONG, ULONG, PLARGE_INTEGER, PULONG, SECTION_INHERIT, ULONG, ULONG);
typedef NTSTATUS (NTAPI *MyNtQueryInformationThread)(HANDLE, THREADINFOCLASS, PVOID, ULONG, PULONG);



extern MyNtOpenFile pNtOpenFile;
extern MyNtQueryAttributesFile pNtQueryAttributesFile;
extern MyNtCreateSection pNtCreateSection;
extern MyNtOpenSection pNtOpenSection;
extern MyNtMapViewOfSection pNtMapViewOfSection;
extern MyNtQueryInformationThread pNtQueryInformationThread;

extern HANDLE targetLibraryBuffer;
extern UNICODE_STRING fake_name;





NTSTATUS
NTAPI
HookedNtQueryInformationThread(
						IN HANDLE ThreadHandle,
						IN THREADINFOCLASS ThreadInformationClass,
						IN OUT PVOID ThreadInformation,
						IN ULONG ThreadInformationLength,
						OUT OPTIONAL PULONG ReturnLength
						);


NTSTATUS
NTAPI
HookedNtMapViewOfSection(
				IN HANDLE SectionHandle,
				IN HANDLE ProcessHandle,
				IN OUT PVOID *BaseAddress,
				IN ULONG ZeroBits,
				IN ULONG CommitSize,
				IN OUT PLARGE_INTEGER SectionOffset,
				IN OUT PULONG ViewSize,
				IN SECTION_INHERIT InheritDisposition,
				IN ULONG AllocationType,
				IN ULONG Protect
				);

NTSTATUS
NTAPI
HookedNtOpenSection(
			OUT PHANDLE SectionHandle,
			IN ACCESS_MASK DesiredAccess,
			IN POBJECT_ATTRIBUTES ObjectAttributes
			);

NTSTATUS
NTAPI
HookedNtCreateSection(
				OUT PHANDLE SectionHandle,
				IN ULONG DesiredAccess,
				IN POBJECT_ATTRIBUTES ObjectAttributes,
				IN PLARGE_INTEGER MaximumSize,
				IN ULONG PageAttributes,
				IN ULONG SectionAttributes,
				IN HANDLE FileHandle
				);

void
NTAPI
HookedNtOpenFile(
			OUT PHANDLE FileHandle, 
			IN ACCESS_MASK DesiredAccess, 
			IN POBJECT_ATTRIBUTES ObjectAttributes,
			OUT PIO_STATUS_BLOCK IoStatusBlock,
			IN ULONG ShareAccess,
			IN ULONG OpenOptions
			);

NTSTATUS
NTAPI
HookedNtQueryAttributesFile(
						IN POBJECT_ATTRIBUTES ObjectAttributes,
						OUT PFILE_BASIC_INFORMATION FileAttributes
						);