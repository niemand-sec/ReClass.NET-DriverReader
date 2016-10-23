#include <windows.h>
#include <cstdint>

enum class RequestFunction
{
	IsProcessValid,
	OpenRemoteProcess,
	CloseRemoteProcess,
	ReadRemoteMemory,
	WriteRemoteMemory,
	EnumerateProcesses,
	EnumerateRemoteSectionsAndModules,
	DisassembleRemoteCode,
	ControlRemoteProcess
};

typedef LPVOID(__stdcall *RequestFunctionPtrCallback)(RequestFunction request);
RequestFunctionPtrCallback requestFunction;

/// <summary>This method gets called when ReClass.NET loads the plugin.</summary>
VOID __stdcall Initialize(RequestFunctionPtrCallback requestCallback)
{
	requestFunction = requestCallback;

	// Use requestFunction to get a function pointer from ReClass.NET to call the desired function.
	//auto isProcessValid = static_cast<BOOL(__stdcall*)(HANDLE process)>(requestFunction(RequestFunction::IsProcessValid));
	//if (isProcessValid(handle)) { ... }
}

/// <summary>This function doesn't read memory but fills the buffer with 0-4.</summary>
BOOL __stdcall ReadRemoteMemory(HANDLE process, LPCVOID address, LPVOID buffer, SIZE_T size)
{
	auto data = static_cast<uint8_t*>(buffer);

	for (auto i = 0u; i < size; ++i)
	{
		data[i] = i % 5;
	}

	return TRUE;
}
