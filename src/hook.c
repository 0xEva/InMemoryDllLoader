#include "hook.h"


MyNtOpenFile pNtOpenFile = NULL;
MyNtQueryAttributesFile pNtQueryAttributesFile = NULL;
MyNtCreateSection pNtCreateSection = NULL;
MyNtOpenSection pNtOpenSection = NULL;
MyNtMapViewOfSection pNtMapViewOfSection = NULL;
MyNtQueryInformationThread pNtQueryInformationThread = NULL;

HANDLE targetLibraryBuffer = INVALID_HANDLE_VALUE;
UNICODE_STRING fake_name = {0};



BOOL UnicodeStringsEqual(const UNICODE_STRING* str1, const UNICODE_STRING* str2)
{
    if (str1->Length != str2->Length) {
        return FALSE;
    }

    return wcsncmp(str1->Buffer, str2->Buffer, str1->Length / sizeof(WCHAR)) == 0;
}

NTSTATUS
NTAPI
HookedNtQueryInformationThread(
					IN HANDLE ThreadHandle,
					IN THREADINFOCLASS ThreadInformationClass,
					IN OUT PVOID ThreadInformation,
					IN ULONG ThreadInformationLength,
					OUT OPTIONAL PULONG ReturnLength
					)
{
	if(ThreadInformationClass == ThreadDynamicCodePolicyInfo){
	printf("HookedNtQueryInformationThread Found ThreadDynamicCodePolicyInfo\n");
		*(DWORD*)ThreadInformation = 1;
		if(ReturnLength){
			*ReturnLength = ThreadInformationLength;
		}
		return STATUS_SUCCESS;
	}else{
		return pNtQueryInformationThread(ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength, ReturnLength);
	}

}


NTSTATUS
NTAPI
HookedNtOpenSection(
			OUT PHANDLE SectionHandle,
			IN ACCESS_MASK DesiredAccess,
			IN POBJECT_ATTRIBUTES ObjectAttributes
			)
{
	if(UnicodeStringsEqual(ObjectAttributes->ObjectName, &fake_name)){
		printf("HookedNtOpenSection Found FAKE_NAME\n");
		*SectionHandle = FAKE_HANDLE;
		return STATUS_SUCCESS;
	}else{
		return pNtOpenSection(SectionHandle, DesiredAccess, ObjectAttributes);
	}
}


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
				)
{
	if(SectionHandle == FAKE_HANDLE){
		printf("HookedNtMapViewOfSection Found FAKE_HANDLE\n");
		*BaseAddress = targetLibraryBuffer;
		*ViewSize = SIZE_OF_IMAGE(rawData);
		if(SectionOffset){
			printf("SectionOffset Non-NULL\n");
		}
		return STATUS_IMAGE_NOT_AT_BASE;
	}else{
		return pNtMapViewOfSection(SectionHandle, ProcessHandle, BaseAddress, 
		ZeroBits, CommitSize, SectionOffset, ViewSize, InheritDisposition, AllocationType, Protect);
	}
}


NTSTATUS
NTAPI
HookedNtQueryAttributesFile(
							IN POBJECT_ATTRIBUTES ObjectAttributes,
							OUT PFILE_BASIC_INFORMATION FileAttributes
							)
{
	if(UnicodeStringsEqual(ObjectAttributes->ObjectName, &fake_name)){
		printf("HookedNtQueryAttributesFile Found FAKE_NAME\n"); 
		LARGE_INTEGER zero = {0};
		FileAttributes->CreationTime = zero;
		FileAttributes->LastAccessTime = zero;
		FileAttributes->ChangeTime = zero;
		FileAttributes->FileAttributes = FILE_ATTRIBUTE_NORMAL;
		return STATUS_SUCCESS;
	}else{
		return pNtQueryAttributesFile(ObjectAttributes, FileAttributes);
	}
}



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
					)
{
	if(FileHandle == FAKE_HANDLE){
		printf("HookedNtCreateSection Found FAKE_HANDLE\n");
		*SectionHandle = targetLibraryBuffer;
		return STATUS_SUCCESS;
	}else{
		return pNtCreateSection(SectionHandle, DesiredAccess, ObjectAttributes, 
		MaximumSize, PageAttributes, SectionAttributes, FileHandle);
	}

}



void
NTAPI
HookedNtOpenFile(
			OUT PHANDLE FileHandle, 
			IN ACCESS_MASK DesiredAccess, 
			IN POBJECT_ATTRIBUTES ObjectAttributes,
			OUT PIO_STATUS_BLOCK IoStatusBlock,
			IN ULONG ShareAccess,
			IN ULONG OpenOptions
			)
{
	if(UnicodeStringsEqual(ObjectAttributes->ObjectName, &fake_name)){
		printf("HookedNtOpenFile Found FAKE_NAME\n");
		*FileHandle = FAKE_HANDLE;
		IoStatusBlock->Status = STATUS_SUCCESS;
		IoStatusBlock->Information = FILE_OPENED;
	}else{
		pNtOpenFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions);
	}

}