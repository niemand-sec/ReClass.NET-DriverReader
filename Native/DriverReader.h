#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <winternl.h>

#include <ReClassNET_Plugin.hpp>
#pragma comment( lib, "ntdll.lib" )

// IMPORTANT!!! You need to set up this constant to the windows version
// you are going to compile it for.
#define WINVERSION 1607


#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)

// GIO Driver
#define IOCTL_GIO_MAPPHYSICAL	0xC3502004
#define IOCTL_GIO_UNMAPPHYSICAL 0xC3502008
#define IOCTL_GIO_MEMCPY 0xC3502808


// Kernel offsets (used for Read Write kernel memory) w10 1607
#define OFFSET_DIRECTORYTABLEBASE 0x028
#define OFFSET_VIRTUALSIZE 0x338
#define OFFSET_SECTIONBASEADDRESS 0x3c0
#define OFFSET_OBJECTTABLE 0x418
#define OFFSET_IMAGEFILENAME 0x450
#define OFFSET_PRIORITYCLASS 0x45f

// Kernel offsets (used for enumerate modules and sections of a process using kernel memory) w10 1607
#define OFFSET_EPROCESS_PEB 0x3f8
#define OFFSET_PEB_LDR 0x018
#define OFFSET_LDR_InMemoryOrderModuleList 0x20

#if (WINVERSION == 1607)
#define OFFSET_ACTIVEPROCESSLINKS 0x2f0
#define OFFSET_UNIQUEPROCESSID 0x2e8
#define OFFSET_VADROOT 0x620
#endif


#if (WINVERSION == 1703 || WINVERSION == 1709)
#define OFFSET_ACTIVEPROCESSLINKS 0x2e8
#define OFFSET_UNIQUEPROCESSID 0x2e0
#define OFFSET_VADROOT 0x628
#endif


/*
(*((ntkrnlmp!_MMVAD_SHORT *)0xffffb803e0fa73a0))                 [Type: _MMVAD_SHORT]
    [+0x000] VadNode          [Type: _RTL_BALANCED_NODE]
    [+0x000] NextVad          : 0xffffb803dfc39240 [Type: _MMVAD_SHORT *]
    [+0x018] StartingVpn      : 0x96ee040 [Type: unsigned long]
    [+0x01c] EndingVpn        : 0x96ee13f [Type: unsigned long]
	[+0x020] StartingVpnHigh  : 0x0 [Type: unsigned char]
    [+0x021] EndingVpnHigh    : 0x0 [Type: unsigned char]

*/
#define OFFSET_STARTINGVPN 0x018
#define OFFSET_ENDINGVPN 0x01c
#define OFFSET_STARTINGVPNHIGH 0x020
#define OFFSET_ENDINGVPNHIGH 0x021
#define OFFSET_MMVAD_SHORT_U 0x030


const ULONG ProtectionFlags[] = {
	PAGE_NOACCESS,
	PAGE_READONLY,
	PAGE_EXECUTE,
	PAGE_EXECUTE_READ,
	PAGE_READWRITE,
	PAGE_WRITECOPY,
	PAGE_EXECUTE_READWRITE,
	PAGE_EXECUTE_WRITECOPY,
	PAGE_NOACCESS,
	PAGE_NOCACHE | PAGE_READONLY,
	PAGE_NOCACHE | PAGE_EXECUTE,
	PAGE_NOCACHE | PAGE_EXECUTE_READ,
	PAGE_NOCACHE | PAGE_READWRITE,
	PAGE_NOCACHE | PAGE_WRITECOPY,
	PAGE_NOCACHE | PAGE_EXECUTE_READWRITE,
	PAGE_NOCACHE | PAGE_EXECUTE_WRITECOPY,
	PAGE_NOACCESS,
	PAGE_GUARD | PAGE_READONLY,
	PAGE_GUARD | PAGE_EXECUTE,
	PAGE_GUARD | PAGE_EXECUTE_READ,
	PAGE_GUARD | PAGE_READWRITE,
	PAGE_GUARD | PAGE_WRITECOPY,
	PAGE_GUARD | PAGE_EXECUTE_READWRITE,
	PAGE_GUARD | PAGE_EXECUTE_WRITECOPY,
	PAGE_NOACCESS,
	PAGE_WRITECOMBINE | PAGE_READONLY,
	PAGE_WRITECOMBINE | PAGE_EXECUTE,
	PAGE_WRITECOMBINE | PAGE_EXECUTE_READ,
	PAGE_WRITECOMBINE | PAGE_READWRITE,
	PAGE_WRITECOMBINE | PAGE_WRITECOPY,
	PAGE_WRITECOMBINE | PAGE_EXECUTE_READWRITE,
	PAGE_WRITECOMBINE | PAGE_EXECUTE_WRITECOPY,
};


// Structure of MAP
typedef struct _READ_REQUEST {
	DWORD InterfaceType;
	DWORD Bus;
	ULONG64 PhysicalAddress;
	DWORD IOSpace;
	DWORD size;
} READ_REQUEST;

typedef struct _WRITE_REQUEST {
	DWORDLONG address;
	DWORD length;
	DWORDLONG buffer;
} WRITE_REQUEST;

typedef struct _MEMCPY_REQUEST {
	ULONG64 dest;
	ULONG64 src;
	DWORD size;
} MEMCPY_REQUEST;

struct SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX			// Size => 28
{
	PVOID Object;									// Size => 4 Offset =>0
	ULONG UniqueProcessId;							// Size => 4 Offset =>4
	ULONG HandleValue;								// Size => 4 Offset =>8
	ULONG GrantedAccess;							// Size => 4 Offset =>12
	USHORT CreatorBackTraceIndex;					// Size => 2 Offset =>16
	USHORT ObjectTypeIndex;							// Size => 2 Offset =>18
	ULONG HandleAttributes;							// Size => 4 Offset =>20
	ULONG Reserved;									// Size => 4 Offset =>24
};

struct SYSTEM_HANDLE_INFORMATION_EX					// Size => 36
{
	ULONG NumberOfHandles;							// Size => 4 Offset => 0
	ULONG Reserved;									// Size => 4 Offset => 4
	SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX Handles[1];	// Size => 36 Offset => 8
};


typedef struct _LDR_MODULE
{
	LIST_ENTRY      InLoadOrderModuleList;
	LIST_ENTRY      InMemoryOrderModuleList;
	LIST_ENTRY      InInitializationOrderModuleList;
	PVOID           BaseAddress;
	PVOID           EntryPoint;
	ULONG           SizeOfImage;
	UNICODE_STRING  FullDllName;
	UNICODE_STRING  BaseDllName;
	ULONG           Flags;
	SHORT           LoadCount;
	SHORT           TlsIndex;
	LIST_ENTRY      HashTableEntry;
	ULONG           TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;


class DriverReader
{
public:
	static int getDeviceHandle(LPTSTR name);
	static bool ReadPhyMemory(uintptr_t physicalAddress, LPVOID  lpBuffer, SIZE_T  nSize, SIZE_T  *lpNumberOfBytesRead);
	static bool WriteVirtualMemory(uint64_t directoryTableBase, uintptr_t virtualAddress, LPVOID  lpBuffer, SIZE_T  nSize, SIZE_T  *lpNumberOfBytesWritten);
	static bool ReadVirtualMemory(uint64_t directoryTableBase, uintptr_t virtualAddress, LPCVOID lpBuffer, SIZE_T  nSize, SIZE_T  *lpNumberOfBytesRead);
	static uintptr_t ObtainKProcessPointer(uint64_t directoryTableBase, std::vector<uintptr_t> pKernelPointers);
	static uintptr_t GetKProcess(uintptr_t &directoryTableBase);
	static uintptr_t SearchKProcess(LPCVOID processName, uintptr_t &directoryTableBase, uintptr_t pKProcess);
	static bool ObtainKProcessInfo(uintptr_t &directoryTableBase, uintptr_t pKProcessAddress);
	//static bool VirtualQueryEx(uintptr_t &directoryTableBase, uintptr_t &infoStructure);
	static bool LeakKernelPointers(std::vector<uintptr_t> &pKernelPointers);
	static uintptr_t FindDirectoryBase();
	static void WalkVadADLTree(uintptr_t directoryTableBase, uintptr_t start);
	static void EnumRing3ProcessModules(uintptr_t directoryTableBase);


	// Variables
	static HANDLE hDeviceDrv;
	static char targetProc[256];
	static char previousTargetProc[256];
	static uintptr_t DTBTargetProcess;
	static uintptr_t virtualSizeTargetProcess;
	static uintptr_t pBaseAddressTargetProcess;
	static uintptr_t pVadRootTargetProcess;
	static uintptr_t pPEBTargetProcess;
	static std::vector<EnumerateRemoteSectionData> sections;
	static std::vector<EnumerateRemoteModuleData> modules;

};

