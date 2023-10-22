#include "hook.h"
#include "rawData.h"
#include "peparser.h"

#define OK 0

MyNtOpenFile myNtOpenFile;
MyNtQueryAttributesFile myNtQueryAttributesFile;
MyNtCreateSection myNtCreateSection;
MyNtOpenSection myNtOpenSection;
MyNtMapViewOfSection myNtMapViewOfSection;
MyNtQueryInformationThread myNtQueryInformationThread;


int CreateHooks(){
    HMODULE ntdll = LoadLibrary(TEXT("ntdll.dll"));
    myNtOpenFile = (MyNtOpenFile) GetProcAddress(ntdll, "NtOpenFile");
    myNtQueryAttributesFile = (MyNtQueryAttributesFile) GetProcAddress(ntdll, "NtQueryAttributesFile");
    myNtCreateSection = (MyNtCreateSection) GetProcAddress(ntdll, "NtCreateSection");
    myNtOpenSection = (MyNtOpenSection)GetProcAddress(ntdll, "NtOpenSection");
    myNtMapViewOfSection = (MyNtMapViewOfSection)GetProcAddress(ntdll, "NtMapViewOfSection");
    myNtQueryInformationThread = (MyNtQueryInformationThread)GetProcAddress(ntdll, "NtQueryInformationThread");

    if (MH_CreateHook(myNtOpenFile, &HookedNtOpenFile, 
        (LPVOID*)(&pNtOpenFile)) != MH_OK)
    {
        return 1;
    }
    if (MH_CreateHook(myNtQueryAttributesFile, &HookedNtQueryAttributesFile, 
        (LPVOID*)(&pNtQueryAttributesFile)) != MH_OK)
    {
        return 1;
    }
    if (MH_CreateHook(myNtCreateSection, &HookedNtCreateSection, 
        (LPVOID*)(&pNtCreateSection)) != MH_OK)
    {
        return 1;
    }
    if (MH_CreateHook(myNtOpenSection, &HookedNtOpenSection, 
        (LPVOID*)(&pNtOpenSection)) != MH_OK)
    {
        return 1;
    }
    if (MH_CreateHook(myNtMapViewOfSection, &HookedNtMapViewOfSection, 
        (LPVOID*)(&pNtMapViewOfSection)) != MH_OK)
    {
        return 1;
    }
    if (MH_CreateHook(myNtQueryInformationThread, &HookedNtQueryInformationThread, 
        (LPVOID*)(&pNtQueryInformationThread)) != MH_OK)
    {
        return 1;
    }
    
    return 0;
}

int main()
{
    SIZE_T bytesWritten = 0;
    RtlInitUnicodeString(&fake_name, FAKE_NAME);
    
    targetLibraryBuffer =  VirtualAlloc(
                            NULL,
                            SIZE_OF_IMAGE(rawData),
                            MEM_COMMIT,
                            PAGE_EXECUTE_READWRITE
                            );
    if(targetLibraryBuffer == NULL){
        printf("targetLibraryBuffer is NULL %d\n", GetLastError());
        return 1;
    }
    WriteProcessMemory(
                    (HANDLE)-1,
                    targetLibraryBuffer,
                    rawData,
                    SIZE_OF_HEADERS(rawData),
                    &bytesWritten
                    );
    if(bytesWritten != SIZE_OF_HEADERS(rawData)){
        printf("WriteProcessMemory Header Failed %d %d\n", bytesWritten, SIZE_OF_HEADERS(rawData));
        return 2;
    }

    for (int index = 0; index < NUMBER_OF_SECTIONS(rawData); index++) {
        IMAGE_SECTION_HEADER* section = SECTION_HEADER_AT(rawData, index);
        //Sanity Check
        if(section == NULL){
            printf("NULL SectionHeader\n");
            return 3;
        }
        WriteProcessMemory(
            (HANDLE)-1, 
            (unsigned char*)targetLibraryBuffer + section->VirtualAddress,
            (unsigned char*)rawData + section->PointerToRawData,
            section->SizeOfRawData,
            &bytesWritten
        );
        if(bytesWritten != section->SizeOfRawData){
            printf("WriteProcessMemory Section Failed %d %d\n", bytesWritten, section->SizeOfRawData);
        }
    }

    if (MH_Initialize() != MH_OK)
    {
        return 4;
    }

    if(CreateHooks() != OK){
        return 5;
    }


    if(MH_EnableHook(MH_ALL_HOOKS) != MH_OK){
        return 6;
    }

    // BOOL DynamicCodePolicy = TRUE;
    // NtSetInformationThread(
    //                     GetCurrentThread(),
    //                     ThreadDynamicCodePolicyInfo,
    //                     &DynamicCodePolicy, 
    //                     sizeof(DynamicCodePolicy)
    //                     );

    //Everything is set up
    HMODULE hDLL = LoadLibraryW(FAKE_NAME);
    if(hDLL == NULL){
        printf("In Memory Loading Failed %d\n", GetLastError());
        return 7;
    }

	//Test our exported function
    typedef void (*MyTestExportedFunc)();
    MyTestExportedFunc myTestExportedFunc = (MyTestExportedFunc) GetProcAddress(hDLL, "TestExportedFunc");
    if(myTestExportedFunc == NULL){
        printf("GetProcAddress of TestExportedFunc failed %d\n", GetLastError());
        return 8;
    }
    myTestExportedFunc();

    if(MH_DisableHook(MH_ALL_HOOKS) != MH_OK){
        return 9;
    }


	if (MH_Uninitialize() != MH_OK)
    {
        return 10;
    }
}
