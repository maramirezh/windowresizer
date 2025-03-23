<#
.SYNOPSIS
    Resizes a window based on its process ID.

.DESCRIPTION
    This script takes a process ID (PID) as input and resizes the window associated with that process.

.PARAMETER ProcessId
    The process ID (PID) of the window to resize. This is a required parameter.

.PARAMETER Width
	The target width of the window to resize. This is a required parameter.

.PARAMETER Height
	The target height of the window to resize. This is a required parameter.

.EXAMPLE
    .\resize_window.ps1 -ProcessId 1234 -Width 1280 -Height 720

    Resizes the window with PID 1234 to 1920x1080 at coordinates (100, 100).
#>
param(
    [int]$ProcessId,
	[int]$Width,
	[int]$Height
)

Add-Type @"
using System;
using System.Runtime.InteropServices;
public class WinAPI {
    [DllImport("user32.dll", SetLastError = true)]
    public static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, uint uFlags);
}
"@

# Get the handle of the target window (e.g., Notepad)
$process = Get-Process -Id $ProcessId -ErrorAction SilentlyContinue
if ($process) {
	Write-Host "Process with PID $ProcessId found: $($process.ProcessName)"
    $hwnd = $process.MainWindowHandle
    if ($hwnd -ne [IntPtr]::Zero) {
        # Resize and move the window (X=100, Y=100, Width=1920, Height=1080)
        [WinAPI]::SetWindowPos($hwnd, [IntPtr]::Zero, 100, 100, $Width, $Height, 0)
    } else {
        Write-Host "Window not found."
    }
} else {
    Write-Host "Process not found."
}
