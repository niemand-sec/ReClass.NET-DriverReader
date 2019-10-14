#include <cstdint>

#if (_MSC_VER >= 1915)
#define no_init_all deprecated
#endif


#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <algorithm>

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <ReClassNET_Plugin.hpp>
#include <experimental/filesystem>
#include "DriverReader.h"
namespace fs = std::experimental::filesystem;

#if (_MSC_VER >= 1915)
#define no_init_all deprecated
#endif


enum class Platform
{
	Unknown,
	X86,
	X64
};

Platform GetProcessPlatform(HANDLE process)
{
	static USHORT processorArchitecture = PROCESSOR_ARCHITECTURE_UNKNOWN;
	if (processorArchitecture == PROCESSOR_ARCHITECTURE_UNKNOWN)
	{
		SYSTEM_INFO info = {};
		GetNativeSystemInfo(&info);

		processorArchitecture = info.wProcessorArchitecture;
	}

	switch (processorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		return Platform::X86;
	case PROCESSOR_ARCHITECTURE_AMD64:
		auto isWow64 = FALSE;
		if (IsWow64Process(process, &isWow64))
		{
			return isWow64 ? Platform::X86 : Platform::X64;
		}

#ifdef RECLASSNET64
		return Platform::X64;
#else
		return Platform::X86;
#endif
	}
	return Platform::Unknown;
}

std::string getFileName(const std::string& s)
{
	char sep = '/';

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}
	return("");
}

/// <summary>Opens the remote process.</summary>
/// <param name="id">The identifier of the process returned by EnumerateProcesses.</param>
/// <param name="desiredAccess">The desired access.</param>
/// <returns>A handle to the remote process or nullptr if an error occured.</returns>
extern "C" RC_Pointer RC_CallConv OpenRemoteProcess(RC_Pointer id, ProcessAccess desiredAccess)
{
	// Open the remote process with the desired access rights and return the handle to use with the other functions.
	/*
	DWORD access = STANDARD_RIGHTS_REQUIRED | PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | SYNCHRONIZE;
	switch (desiredAccess)
	{
	case ProcessAccess::Read:
		access |= PROCESS_VM_READ;
		break;
	case ProcessAccess::Write:
		access |= PROCESS_VM_OPERATION | PROCESS_VM_WRITE;
		break;
	case ProcessAccess::Full:
		access |= PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_VM_WRITE;
		break;
	}

	const auto handle = OpenProcess(access, FALSE, static_cast<DWORD>(reinterpret_cast<size_t>(id)));

	if (handle == nullptr || handle == INVALID_HANDLE_VALUE)
	{
		return nullptr;
	}
	*/

	return id;
}

/// <summary>Queries if the process is valid.</summary>
/// <param name="handle">The process handle obtained by OpenRemoteProcess.</param>
/// <returns>True if the process is valid, false if not.</returns>
extern "C" bool RC_CallConv IsProcessValid(RC_Pointer handle)
{
	// Check if the handle is valid.
	if (handle == nullptr)
	{
		return false;
	}

	const auto retn = WaitForSingleObject(handle, 0);
	if (retn == WAIT_FAILED)
	{
		return false;
	}

	return retn == WAIT_TIMEOUT;
	/*
	if (handle == nullptr)
	{
		return false;
	}
	*/

	return true;
}

/// <summary>Closes the handle to the remote process.</summary>
/// <param name="handle">The process handle obtained by OpenRemoteProcess.</param>
extern "C" void RC_CallConv CloseRemoteProcess(RC_Pointer handle)
{
	// Close the handle to the remote process.
	
	if (handle == nullptr)
	{
		return;
	}

	CloseHandle(handle);
	
	/*
	if (handle == nullptr)
	{
		return;
	}
	*/
}


/// <summary>Enumerate all processes on the system.</summary>
/// <param name="callbackProcess">The callback for a process.</param>
extern "C" void RC_CallConv EnumerateProcesses(EnumerateProcessCallback callbackProcess)
{
	// With this trick we'll be able to print content to the console, and if we have luck we could get information printed by the game.
	AllocConsole();
	SetConsoleTitle("Debug");
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	freopen("CONIN$", "r", stdin);
	
	
	// Enumerate all processes with the current plattform (x86/x64) and call the callback.
	if (callbackProcess == nullptr)
	{
		return;
	}

	const auto handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (handle != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32W pe32 = {};
		pe32.dwSize = sizeof(PROCESSENTRY32W);
		if (Process32FirstW(handle, &pe32))
		{
			do
			{

				//const auto process = OpenRemoteProcess(reinterpret_cast<RC_Pointer>(static_cast<size_t>(pe32.th32ProcessID)), ProcessAccess::Read);

				const auto handle_limited = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION , FALSE, static_cast<DWORD>(pe32.th32ProcessID));

				if (handle_limited == nullptr || handle == INVALID_HANDLE_VALUE)
				{
					continue;
				}

				if (pe32.th32ProcessID == 0 || pe32.th32ProcessID == 4)
					continue;

				if (pe32.th32ProcessID)
				{
					const auto platform = GetProcessPlatform(handle_limited);
#ifdef RECLASSNET64
					if (platform == Platform::X64)
#else
					if (platform == Platform::X86)
#endif
					{
						EnumerateProcessData data = { };
						data.Id = pe32.th32ProcessID;
						//GetModuleFileNameExW(process, nullptr, reinterpret_cast<LPWSTR>(data.Path), PATH_MAXIMUM_LENGTH);
						
						//const auto name = fs::path(data.Path).filename().u16string();
						const auto name = fs::path(pe32.szExeFile).filename().u16string();
						const auto path = fs::path(pe32.szExeFile).u16string();
						str16cpy(data.Name, name.c_str(), std::min<size_t>(name.length(), PATH_MAXIMUM_LENGTH - 1));
						str16cpy(data.Path, path.c_str(), std::min<size_t>(path.length(), PATH_MAXIMUM_LENGTH - 1));
						//data.Path = fs::path(pe32.szExeFile).u16string();
						//str16cpy(data.Path, pe32.szExeFile, std::min<size_t>(pe32.szExeFile, PATH_MAXIMUM_LENGTH - 1));
						//std::cout << "[-] data.Id " << data.Id  << std::endl;
						//std::cout << "[-] data.Name " << name.c_str() << std::endl;
						//std::cout << "[-] data.Path " << pe32.szExeFile <<  std::endl;
						callbackProcess(&data);
					}

				}

				CloseRemoteProcess(handle_limited);

			} while (Process32NextW(handle, &pe32));
		}

		CloseHandle(handle);
	}

}


uintptr_t directoryTableBase = 0;
uintptr_t pKProcess = 0;
uintptr_t pBaseAddress = 0;

/// <summary>Enumerate all sections and modules of the remote process.</summary>
/// <param name="process">The process handle obtained by OpenRemoteProcess.</param>
/// <param name="callbackSection">The callback for a section.</param>
/// <param name="callbackModule">The callback for a module.</param>
void RC_CallConv EnumerateRemoteSectionsAndModules(RC_Pointer id, EnumerateRemoteSectionsCallback callbackSection, EnumerateRemoteModulesCallback callbackModule)
{
	 std::cout << "[.] EnumerateRemoteSectionsAndModules " << id << std::endl;
	// Enumerate all sections and modules of the remote process and call the callback for them.
	
	if (callbackSection == nullptr && callbackModule == nullptr && !DriverReader::DTBTargetProcess)
	{
		std::cout << "[-] EnumerateRemoteSectionsAndModules failed" << std::endl;
		return;
	}

	std::vector<EnumerateRemoteSectionData> sections;

	MEMORY_BASIC_INFORMATION memInfo = { };
	memInfo.RegionSize = 0x1000;
	size_t address = 0;

	const auto handle_limited = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, reinterpret_cast<DWORD>(id));


	while (VirtualQueryEx(handle_limited, reinterpret_cast<LPCVOID>(address), &memInfo, sizeof(MEMORY_BASIC_INFORMATION)) != 0 && address + memInfo.RegionSize > address)
	{
		if (memInfo.State == MEM_COMMIT)
		{
			EnumerateRemoteSectionData section = {};
			section.BaseAddress = memInfo.BaseAddress;
			section.Size = memInfo.RegionSize;

			//std::cout << "[-] section.BaseAddress failed" << section.BaseAddress << std::endl;

			section.Protection = SectionProtection::NoAccess;
			if ((memInfo.Protect & PAGE_EXECUTE) == PAGE_EXECUTE) section.Protection |= SectionProtection::Execute;
			if ((memInfo.Protect & PAGE_EXECUTE_READ) == PAGE_EXECUTE_READ) section.Protection |= SectionProtection::Execute | SectionProtection::Read;
			if ((memInfo.Protect & PAGE_EXECUTE_READWRITE) == PAGE_EXECUTE_READWRITE) section.Protection |= SectionProtection::Execute | SectionProtection::Read | SectionProtection::Write;
			if ((memInfo.Protect & PAGE_EXECUTE_WRITECOPY) == PAGE_EXECUTE_WRITECOPY) section.Protection |= SectionProtection::Execute | SectionProtection::Read | SectionProtection::CopyOnWrite;
			if ((memInfo.Protect & PAGE_READONLY) == PAGE_READONLY) section.Protection |= SectionProtection::Read;
			if ((memInfo.Protect & PAGE_READWRITE) == PAGE_READWRITE) section.Protection |= SectionProtection::Read | SectionProtection::Write;
			if ((memInfo.Protect & PAGE_WRITECOPY) == PAGE_WRITECOPY) section.Protection |= SectionProtection::Read | SectionProtection::CopyOnWrite;
			if ((memInfo.Protect & PAGE_GUARD) == PAGE_GUARD) section.Protection |= SectionProtection::Guard;

			switch (memInfo.Type)
			{
			case MEM_IMAGE:
				section.Type = SectionType::Image;
				break;
			case MEM_MAPPED:
				section.Type = SectionType::Mapped;
				break;
			case MEM_PRIVATE:
				section.Type = SectionType::Private;
				break;
			}

			section.Category = section.Type == SectionType::Private ? SectionCategory::HEAP : SectionCategory::Unknown;

			sections.push_back(std::move(section));
		}
		address = reinterpret_cast<size_t>(memInfo.BaseAddress) + memInfo.RegionSize;
	}
	CloseRemoteProcess(handle_limited);

	const auto handle2 = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, reinterpret_cast<DWORD>(id));
	if (handle2 != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32W me32 = {};
		me32.dwSize = sizeof(MODULEENTRY32W);
		if (Module32FirstW(handle2, &me32))
		{
			do
			{
				if (callbackModule != nullptr)
				{
					EnumerateRemoteModuleData data = {};
					data.BaseAddress = me32.modBaseAddr;
					data.Size = me32.modBaseSize;
					std::memcpy(data.Path, me32.szExePath, PATH_MAXIMUM_LENGTH * sizeof(RC_UnicodeChar));

					callbackModule(&data);
				}

				if (callbackSection != nullptr)
				{
					auto it = std::lower_bound(std::begin(sections), std::end(sections), static_cast<LPVOID>(me32.modBaseAddr), [&sections](const auto& lhs, const LPVOID& rhs)
					{
						return lhs.BaseAddress < rhs;
					});

					IMAGE_DOS_HEADER DosHdr = {};
					IMAGE_NT_HEADERS NtHdr = {};

					//ReadRemoteMemory(handle, me32.modBaseAddr, &DosHdr, 0, sizeof(IMAGE_DOS_HEADER));
					//ReadRemoteMemory(handle, me32.modBaseAddr + DosHdr.e_lfanew, &NtHdr, 0, sizeof(IMAGE_NT_HEADERS));

					DriverReader::ReadVirtualMemory(directoryTableBase, reinterpret_cast<uintptr_t>(me32.modBaseAddr), &DosHdr, sizeof(IMAGE_DOS_HEADER), NULL);
					DriverReader::ReadVirtualMemory(directoryTableBase, reinterpret_cast<uintptr_t>(me32.modBaseAddr + DosHdr.e_lfanew), &NtHdr, sizeof(IMAGE_NT_HEADERS), NULL);

					std::vector<IMAGE_SECTION_HEADER> sectionHeaders(NtHdr.FileHeader.NumberOfSections);
					//ReadRemoteMemory(handle, me32.modBaseAddr + DosHdr.e_lfanew + sizeof(IMAGE_NT_HEADERS), sectionHeaders.data(), 0, NtHdr.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER));
					DriverReader::ReadVirtualMemory(directoryTableBase, reinterpret_cast<uintptr_t>(me32.modBaseAddr + DosHdr.e_lfanew + sizeof(IMAGE_NT_HEADERS)), sectionHeaders.data(), NtHdr.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER), NULL);

					for (auto i = 0; i < NtHdr.FileHeader.NumberOfSections; ++i)
					{
						auto&& sectionHeader = sectionHeaders[i];

						const auto sectionAddress = reinterpret_cast<size_t>(me32.modBaseAddr) + sectionHeader.VirtualAddress;
						for (auto j = it; j != std::end(sections); ++j)
						{
							if (sectionAddress >= reinterpret_cast<size_t>(j->BaseAddress) && sectionAddress < reinterpret_cast<size_t>(j->BaseAddress) + static_cast<size_t>(j->Size))
							{
								// Copy the name because it is not null padded.
								char buffer[IMAGE_SIZEOF_SHORT_NAME + 1] = { 0 };
								std::memcpy(buffer, sectionHeader.Name, IMAGE_SIZEOF_SHORT_NAME);

								if (std::strcmp(buffer, ".text") == 0 || std::strcmp(buffer, "code") == 0)
								{
									j->Category = SectionCategory::CODE;
								}
								else if (std::strcmp(buffer, ".data") == 0 || std::strcmp(buffer, "data") == 0 || std::strcmp(buffer, ".rdata") == 0 || std::strcmp(buffer, ".idata") == 0)
								{
									j->Category = SectionCategory::DATA;
								}

								MultiByteToUnicode(buffer, j->Name, IMAGE_SIZEOF_SHORT_NAME);
								std::memcpy(j->ModulePath, me32.szExePath, PATH_MAXIMUM_LENGTH * sizeof(RC_UnicodeChar));

								break;
							}
						}

					}
				}
			} while (Module32NextW(handle2, &me32));
		}

		CloseHandle(handle2);

		if (callbackSection != nullptr)
		{
			for (auto&& section : sections)
			{
				callbackSection(&section);
			}
		}
	}

	
}


/// <summary>Reads memory of the remote process.</summary>
/// <param name="handle">The process handle obtained by OpenRemoteProcess.</param>
/// <param name="address">The address to read from.</param>
/// <param name="buffer">The buffer to read into.</param>
/// <param name="offset">The offset into the buffer.</param>
/// <param name="size">The number of bytes to read.</param>
/// <returns>True if it succeeds, false if it fails.</returns>
extern "C" bool RC_CallConv ReadRemoteMemory(RC_Pointer id, RC_Pointer address, RC_Pointer buffer, int offset, int size)
{
	// Read the memory of the remote process into the buffer.	
	if (id)
	{
		//TCHAR Buffer[MAX_PATH];
		//std::cout << "\t[.] handle " << handle << std::endl;
		//std::cout << "\t[.] targetProc " << DriverReader::targetProc << std::endl;
		const auto handle_limited = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION , FALSE, reinterpret_cast<DWORD>(id));

		if (handle_limited == nullptr)
		{
			return false;
		}

		if (GetProcessImageFileNameA(handle_limited, DriverReader::targetProc, sizeof(DriverReader::targetProc)))
		{
			//std::cout << "\t[.] targetProc " << DriverReader::targetProc << std::endl;
			strcpy(DriverReader::targetProc,getFileName(DriverReader::targetProc).c_str());
			//std::cout << "\t[.] targetProc " << DriverReader::targetProc << std::endl;
		}
		else
		{
			//std::cout << "\t[.] targetProc failed: 0x" << std::hex << GetLastError() << std::endl;
		}
		CloseHandle(handle_limited);
	}


	if (strcmp(DriverReader::targetProc, DriverReader::previousTargetProc) != 0)
	{
		if (DriverReader::getDeviceHandle("\\\\.\\GIO"))
		{
			std::cout << "[-] Driver not loaded" << std::endl;
			return false;
		}
		strcpy(DriverReader::previousTargetProc, DriverReader::targetProc);
		
		pKProcess = DriverReader::GetKProcess(directoryTableBase);

		pBaseAddress = DriverReader::SearchKProcess(DriverReader::targetProc, directoryTableBase, pKProcess);

		if (!DriverReader::ObtainKProcessInfo(directoryTableBase, pBaseAddress))
		{
			std::cout << "[-] ObtainKProcessInfo failed" << std::endl;
		}

	}

	//std::cout << "[+] Reading ########################################" << std::endl;
	//std::cout << "[+] directoryTableBase" << directoryTableBase << std::endl;
	//std::cout << "[+] pKProcess" << pKProcess << std::endl;
	//std::cout << "[+] pBaseAddress" << pBaseAddress << std::endl;
	std::cout << "[+] address" << address << std::endl;
	std::cout << "[+] size" << size << std::endl;
	
	buffer = reinterpret_cast<RC_Pointer>(reinterpret_cast<uintptr_t>(buffer) + offset);
	//std::cout << "[+] buffer" << buffer << std::endl;
	//std::cout << "[+] buffer" << DriverReader::DTBTargetProcess << std::endl;

	SIZE_T numberOfBytesRead;
	if (DriverReader::ReadVirtualMemory(DriverReader::DTBTargetProcess, reinterpret_cast<uintptr_t>(address), buffer, size, &numberOfBytesRead))
	{
		return true;
	}

	return false;
}

/// <summary>Writes memory to the remote process.</summary>
/// <param name="process">The process handle obtained by OpenRemoteProcess.</param>
/// <param name="address">The address to write to.</param>
/// <param name="buffer">The buffer to write.</param>
/// <param name="offset">The offset into the buffer.</param>
/// <param name="size">The number of bytes to write.</param>
/// <returns>True if it succeeds, false if it fails.</returns>
extern "C" bool RC_CallConv WriteRemoteMemory(RC_Pointer handle, RC_Pointer address, RC_Pointer buffer, int offset, int size)
{
	// Write the buffer into the memory of the remote process.

	return false;
}

/// <summary>Control the remote process (Pause, Resume, Terminate).</summary>
/// <param name="handle">The process handle obtained by OpenRemoteProcess.</param>
/// <param name="action">The action to perform.</param>
extern "C" void RC_CallConv ControlRemoteProcess(RC_Pointer handle, ControlRemoteProcessAction action)
{
	// Perform the desired action on the remote process.
}

/// <summary>Attach a debugger to the process.</summary>
/// <param name="id">The identifier of the process returned by EnumerateProcesses.</param>
/// <returns>True if it succeeds, false if it fails.</returns>
extern "C" bool RC_CallConv AttachDebuggerToProcess(RC_Pointer id)
{
	// Attach a debugger to the remote process.

	return false;
}

/// <summary>Detach a debugger from the remote process.</summary>
/// <param name="id">The identifier of the process returned by EnumerateProcesses.</param>
extern "C" void RC_CallConv DetachDebuggerFromProcess(RC_Pointer id)
{
	// Detach the debugger.
}

/// <summary>Wait for a debug event within the given timeout.</summary>
/// <param name="evt">[out] The occured debug event.</param>
/// <param name="timeoutInMilliseconds">The timeout in milliseconds.</param>
/// <returns>True if an event occured within the given timeout, false if not.</returns>
extern "C" bool RC_CallConv AwaitDebugEvent(DebugEvent* evt, int timeoutInMilliseconds)
{
	// Wait for a debug event.

	return false;
}

/// <summary>Handles the debug event described by evt.</summary>
/// <param name="evt">[in] The (modified) event returned by AwaitDebugEvent.</param>
extern "C" void RC_CallConv HandleDebugEvent(DebugEvent* evt)
{
	// Handle the debug event.
}

/// <summary>Sets a hardware breakpoint.</summary>
/// <param name="processId">The identifier of the process returned by EnumerateProcesses.</param>
/// <param name="address">The address of the breakpoint.</param>
/// <param name="reg">The register to use.</param>
/// <param name="type">The type of the breakpoint.</param>
/// <param name="size">The size of the breakpoint.</param>
/// <param name="set">True to set the breakpoint, false to remove it.</param>
/// <returns>True if it succeeds, false if it fails.</returns>
extern "C" bool RC_CallConv SetHardwareBreakpoint(RC_Pointer id, RC_Pointer address, HardwareBreakpointRegister reg, HardwareBreakpointTrigger type, HardwareBreakpointSize size, bool set)
{
	// Set a hardware breakpoint with the given parameters.

	return false;
}
