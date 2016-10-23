#include <windows.h>
#include <cstdint>

/// <summary>In this sample we provide a ReadRemoteMemory function which doesn't read memory but fills the buffer with 0-4.</summary>
BOOL __stdcall ReadRemoteMemory(HANDLE process, LPCVOID address, LPVOID buffer, SIZE_T size)
{
	auto data = static_cast<uint8_t*>(buffer);

	for (auto i = 0u; i < size; ++i)
	{
		data[i] = i % 5;
	}

	return TRUE;
}
