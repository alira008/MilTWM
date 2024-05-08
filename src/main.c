#include "error.h"
#include "keyboard.h"
#include "messages.h"
#include "shared_memory.h"
#include "tiling.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

// int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, PWSTR
// cmdLIine,
//                     int cmdShow) {
int main() {
  int exit_code = EXIT_FAILURE;
  HMODULE wm_dll;
  HHOOK hook_shell_process_handle;

  HANDLE currently_running_mutex =
      CreateMutexW(NULL, TRUE, L"Global\\MilTWMIsCurrentlyRunning");

  if (currently_running_mutex == NULL) {
    DisplayError(L"MilTWM failed creating currently running mutex");
    goto cleanup;

  } else if (GetLastError() == ERROR_ALREADY_EXISTS) {
    DisplayError(L"MilTWM is already running, exiting");
    goto cleanup;
  }

  SetProcessDPIAware();

  if (!KeyboardInitializeConfig()) {
    DisplayError(L"Could not setup keyboard config");
    goto cleanup;
  }

  if (!StoreDwordIntoSharedMemory(GetCurrentThreadId())) {
    DisplayError(L"Could not write thread id into shared memory");
    goto cleanup;
  }

  wm_dll = LoadLibraryW(L"wm_dll");

  if (wm_dll == NULL) {
    DisplayError(L"Could not load library wm_dll");
    goto cleanup;
  }

  FARPROC shell_process = GetProcAddress(wm_dll, "ShellProcess");

  if (shell_process == NULL) {
    DisplayError(L"Could not get proc address for ShellProcess");
    goto cleanup;
  }

  hook_shell_process_handle =
      SetWindowsHookExW(WH_SHELL, (HOOKPROC)shell_process, wm_dll, 0);

  if (hook_shell_process_handle == NULL) {
    DisplayError(L"Could not SetWindowsHookExW for shell hook");
    goto cleanup;
  }

  TilingTileWindows();
  DisplayWindowNames();

  MSG msg;

  while (GetMessage(&msg, (HWND)-1, 0, 0) != 0) {
    switch (msg.message) {
    case WM_HOTKEY:
      if (msg.wParam == QUIT_MILTWM_HOTKEY_ID) {
        exit_code = EXIT_SUCCESS;
        goto cleanup;
      }

      KeyboardHandleHotkey(msg.wParam, msg.lParam);
      break;
    case WM_WINDOW_EVENT:
      TilingTileWindows();
      DisplayWindowNames();
      break;
    }
  }

cleanup:
  if (currently_running_mutex != NULL) {
    CloseHandle(currently_running_mutex);
  }

  KeyboardCleanup();

  if (hook_shell_process_handle) {
    UnhookWindowsHookEx(hook_shell_process_handle);
  }

  if (wm_dll) {
    FreeLibrary(wm_dll);
  }

  return exit_code;
}
