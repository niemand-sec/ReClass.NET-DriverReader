#include <windows.h>
#include <cstdint>

#include <ReClassNET_Plugin.hpp>

/// <summary>Enumerate all processes on the system.</summary>
/// <param name="callbackProcess">The callback for a process.</param>
void __stdcall EnumerateProcesses(EnumerateProcessCallback callbackProcess)
{
	// Enumerate all processes with the current plattform (x86/x64) and call the callback.
}

/// <summary>Enumerate all sections and modules of the remote process.</summary>
/// <param name="process">The process handle obtained by OpenRemoteProcess.</param>
/// <param name="callbackSection">The callback for a section.</param>
/// <param name="callbackModule">The callback for a module.</param>
void __stdcall EnumerateRemoteSectionsAndModules(RC_Pointer handle, EnumerateRemoteSectionsCallback callbackSection, EnumerateRemoteModulesCallback callbackModule)
{
	// Enumerate all sections and modules of the remote process and call the callback for them.
}

/// <summary>Opens the remote process.</summary>
/// <param name="id">The identifier of the process returned by EnumerateProcesses.</param>
/// <param name="desiredAccess">The desired access.</param>
/// <returns>A handle to the remote process or nullptr if an error occured.</returns>
RC_Pointer __stdcall OpenRemoteProcess(RC_Pointer id, ProcessAccess desiredAccess)
{
	// Open the remote process with the desired access rights and return the handle to use with the other functions.

	return nullptr;
}

/// <summary>Queries if the process is valid.</summary>
/// <param name="handle">The process handle obtained by OpenRemoteProcess.</param>
/// <returns>True if the process is valid, false if not.</returns>
bool __stdcall IsProcessValid(RC_Pointer handle)
{
	// Check if the handle is valid.

	return false;
}

/// <summary>Closes the handle to the remote process.</summary>
/// <param name="handle">The process handle obtained by OpenRemoteProcess.</param>
void __stdcall CloseRemoteProcess(RC_Pointer handle)
{
	// Close the handle to the remote process.
}

/// <summary>Reads memory of the remote process.</summary>
/// <param name="handle">The process handle obtained by OpenRemoteProcess.</param>
/// <param name="address">The address to read from.</param>
/// <param name="buffer">The buffer to read into.</param>
/// <param name="offset">The offset into the buffer.</param>
/// <param name="size">The number of bytes to read.</param>
/// <returns>True if it succeeds, false if it fails.</returns>
bool __stdcall ReadRemoteMemory(RC_Pointer handle, RC_Pointer address, RC_Pointer buffer, int offset, int size)
{
	// Read the memory of the remote process into the buffer.

	return false;
}

/// <summary>Writes memory to the remote process.</summary>
/// <param name="process">The process handle obtained by OpenRemoteProcess.</param>
/// <param name="address">The address to write to.</param>
/// <param name="buffer">The buffer to write.</param>
/// <param name="offset">The offset into the buffer.</param>
/// <param name="size">The number of bytes to write.</param>
/// <returns>True if it succeeds, false if it fails.</returns>
bool __stdcall WriteRemoteMemory(RC_Pointer handle, RC_Pointer address, RC_Pointer buffer, int offset, int size)
{
	// Write the buffer into the memory of the remote process.

	return false;
}

/// <summary>Control the remote process (Pause, Resume, Terminate).</summary>
/// <param name="handle">The process handle obtained by OpenRemoteProcess.</param>
/// <param name="action">The action to perform.</param>
void __stdcall ControlRemoteProcess(RC_Pointer handle, ControlRemoteProcessAction action)
{
	// Perform the desired action on the remote process.
}

/// <summary>Attach a debugger to the process.</summary>
/// <param name="id">The identifier of the process returned by EnumerateProcesses.</param>
/// <returns>True if it succeeds, false if it fails.</returns>
bool __stdcall AttachDebuggerToProcess(RC_Pointer id)
{
	// Attach a debugger to the remote process.

	return false;
}

/// <summary>Detach a debugger from the remote process.</summary>
/// <param name="id">The identifier of the process returned by EnumerateProcesses.</param>
void __stdcall DetachDebuggerFromProcess(RC_Pointer id)
{
	// Detach the debugger.
}

/// <summary>Wait for a debug event within the given timeout.</summary>
/// <param name="evt">[out] The occured debug event.</param>
/// <param name="timeoutInMilliseconds">The timeout in milliseconds.</param>
/// <returns>True if an event occured within the given timeout, false if not.</returns>
bool __stdcall AwaitDebugEvent(DebugEvent* evt, int timeoutInMilliseconds)
{
	// Wait for a debug event.
	
	return false;
}

/// <summary>Handles the debug event described by evt.</summary>
/// <param name="evt">[in] The (modified) event returned by AwaitDebugEvent.</param>
void __stdcall HandleDebugEvent(DebugEvent* evt)
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
bool __stdcall SetHardwareBreakpoint(RC_Pointer id, RC_Pointer address, HardwareBreakpointRegister reg, HardwareBreakpointTrigger type, HardwareBreakpointSize size, bool set)
{
	// Set a hardware breakpoint with the given parameters.

	return false;
}
