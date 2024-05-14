#include "error.h"
#include "keyboard.h"
#include "messages.h"
#include "shared_memory.h"
#include "tiling.h"
#include <ShObjIdl.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

// int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, PWSTR
// cmdLIine,
//                     int cmdShow) {
int main() {
  int exit_code = EXIT_FAILURE;
  HMODULE wm_dll;
  HHOOK hook_shell_process_handle, hook_window_pos_changed_proc_handle;
  IVirtualDesktopManager *i_virtual_desktop_manager = NULL;

  HANDLE currently_running_mutex =
      CreateMutexW(NULL, TRUE, L"Global\\MilTWMIsCurrentlyRunning");

  if (currently_running_mutex == NULL) {
    DisplayError(L"MilTWM failed creating currently running mutex");
    goto cleanup;

  } else if (GetLastError() == ERROR_ALREADY_EXISTS) {
    DisplayError(L"MilTWM is already running, exiting");
    goto cleanup;
  }

  if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
    DisplayError(L"Initation COM for Virtual desktop manager failed");
    goto cleanup;
  }

  // initiate com of virtual desktop manager
  if (FAILED(CoCreateInstance(&CLSID_VirtualDesktopManager, NULL,
                              CLSCTX_INPROC_SERVER, &IID_IVirtualDesktopManager,
                              (void **)&i_virtual_desktop_manager))) {
    DisplayError(L"Initation COM for Virtual desktop manager failed");
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

  FARPROC window_pos_changed_proc =
      GetProcAddress(wm_dll, "WindowPosChangedProc");

  if (window_pos_changed_proc == NULL) {
    DisplayError(L"Could not get proc address for WindowPosChangedProc");
    goto cleanup;
  }

  hook_window_pos_changed_proc_handle = SetWindowsHookExW(
      WH_CALLWNDPROC, (HOOKPROC)window_pos_changed_proc, wm_dll, 0);

  if (hook_window_pos_changed_proc_handle == NULL) {
    DisplayError(L"Could not SetWindowsHookExW for window pos changed hook");
    goto cleanup;
  }
  TilingTileWindows();
  DisplayWindowNames(i_virtual_desktop_manager);

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
      printf("window event\n");
      TilingTileWindows();
      DisplayWindowNames(i_virtual_desktop_manager);
      break;
    case WM_SIZE:
      if (msg.wParam == SIZE_MAXIMIZED || msg.wParam == SIZE_MINIMIZED ||
          msg.wParam == SIZE_RESTORED) {
        printf("size of window changed\n");
      }

      break;
    case WM_MOVE:
      printf("position of window changed\n");
      break;
    }
  }

cleanup:
  if (i_virtual_desktop_manager != NULL) {
    i_virtual_desktop_manager->lpVtbl->Release(i_virtual_desktop_manager);
    CoUninitialize();
  }
  if (currently_running_mutex != NULL) {
    CloseHandle(currently_running_mutex);
  }

  KeyboardCleanup();

  if (hook_shell_process_handle) {
    UnhookWindowsHookEx(hook_shell_process_handle);
  }

  if (hook_window_pos_changed_proc_handle) {
    UnhookWindowsHookEx(hook_window_pos_changed_proc_handle);
  }

  if (wm_dll) {
    printf("freeing library");
    FreeLibrary(wm_dll);
  }

  return exit_code;
}
