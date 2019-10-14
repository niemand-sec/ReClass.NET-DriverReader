#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <winternl.h>

#pragma comment( lib, "ntdll.lib" )

#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)

// GIO Driver
#define IOCTL_GIO_MAPPHYSICAL	0xC3502004
#define IOCTL_GIO_UNMAPPHYSICAL 0xC3502008
#define IOCTL_GIO_MEMCPY 0xC3502808


// Kernel offsets
#define OFFSET_DIRECTORYTABLEBASE 0x028
#define OFFSET_UNIQUEPROCESSID 0x2e8
#define OFFSET_ACTIVEPROCESSLINKS 0x2f0
#define OFFSET_VIRTUALSIZE 0x338
#define OFFSET_SECTIONBASEADDRESS 0x3c0
#define OFFSET_OBJECTTABLE 0x418
#define OFFSET_IMAGEFILENAME 0x450
#define OFFSET_PRIORITYCLASS 0x45f

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


class DriverReader
{
public:
	static int getDeviceHandle(LPTSTR name);
	static bool ReadPhyMemory(uintptr_t physicalAddress, LPVOID  lpBuffer, SIZE_T  nSize, SIZE_T  *lpNumberOfBytesRead);
	static bool ReadVirtualMemory(uint64_t directoryTableBase, uintptr_t virtualAddress, LPCVOID lpBuffer, SIZE_T  nSize, SIZE_T  *lpNumberOfBytesRead);
	static uintptr_t ObtainKProcessPointer(uint64_t directoryTableBase, std::vector<uintptr_t> pKernelPointers);
	static uintptr_t GetKProcess(uintptr_t &directoryTableBase);
	static uintptr_t SearchKProcess(LPCVOID processName, uintptr_t &directoryTableBase, uintptr_t pKProcess);
	static bool ObtainKProcessInfo(uintptr_t &directoryTableBase, uintptr_t pKProcessAddress);
	//static bool VirtualQueryEx(uintptr_t &directoryTableBase, uintptr_t &infoStructure);
	static bool LeakKernelPointers(std::vector<uintptr_t> &pKernelPointers);
	static uintptr_t FindDirectoryBase();

	// Variables
	static HANDLE hDeviceDrv;
	static char targetProc[256];
	static char previousTargetProc[256];
	static uintptr_t DTBTargetProcess;
	static uintptr_t virtualSizeTargetProcess;
	static uintptr_t pBaseAddressTargetProcess;
};

