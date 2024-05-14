#include "tiling.h"
#include "error.h"
#include <ShObjIdl_core.h>
#include <stdio.h>
#include <wchar.h>

#define MAX_MANAGED_WINDOWS 256
HWND g_focused_window = 0;
HWND g_managed_windows[MAX_MANAGED_WINDOWS];
int g_number_of_managed_windows = 0;
int g_current_focused_window_index = 0;

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM l_param) {
  if (g_number_of_managed_windows >= MAX_MANAGED_WINDOWS) {
    return FALSE;
  }

  if (!IsWindowVisible(hwnd) || IsHungAppWindow(hwnd)) {
    return TRUE;
  }

  WINDOWINFO window_info = {.cbSize = sizeof(WINDOWINFO)};
  if (!GetWindowInfo(hwnd, &window_info)) {
    return TRUE;
  }

  if (window_info.dwStyle & WS_POPUP) {
    return TRUE;
  }

  if (!(window_info.dwExStyle & WS_ICONIC)) {
    return TRUE;
  }

  if (GetWindowTextLengthW(hwnd) == 0) {
    return TRUE;
  }

  RECT client_rect;
  if (!GetClientRect(hwnd, &client_rect)) {
    return TRUE;
  }

  // skip small windows
  if (client_rect.right < 100 || client_rect.bottom < 100) {
    return TRUE;
  }

  g_managed_windows[g_number_of_managed_windows] = hwnd;
  g_number_of_managed_windows++;
  return TRUE;
}

void TilingTileWindows() {
  g_number_of_managed_windows = 0;

  if (g_focused_window == NULL) {
    EnumChildWindows(GetDesktopWindow(), EnumChildProc, 0);
  } else {
    g_managed_windows[g_number_of_managed_windows] = g_focused_window;
    g_number_of_managed_windows++;
  }

  if (g_number_of_managed_windows == 0) {
    return;
  }
}

void TilingToggleFocusedWindow(HWND hwnd) {
  if (g_focused_window != NULL) {
    g_focused_window = NULL;
  } else {
    g_focused_window = hwnd;
  }

  TilingTileWindows();
}

void TilingFocusNextWindow(bool back) {

  if (g_focused_window != NULL) {
    TilingToggleFocusedWindow(NULL);
  }

  g_current_focused_window_index += back ? -1 : 1;

  if (g_current_focused_window_index < 0) {
    g_current_focused_window_index = g_number_of_managed_windows - 1;
  } else if (g_current_focused_window_index >= g_number_of_managed_windows) {
    g_current_focused_window_index = 0;
  }

  SwitchToThisWindow(g_managed_windows[g_current_focused_window_index], FALSE);
}

void DisplayWindowNames(IVirtualDesktopManager *i_virtual_desktop_manager) {
  printf("number of managed windows: %d\n", g_number_of_managed_windows);
  for (int i = 0; i < g_number_of_managed_windows; ++i) {
    WCHAR temp_buf[256] = {};
    HWND window = g_managed_windows[i];
    int len_of_wchar = GetWindowTextW(window, temp_buf, 256);
    if (len_of_wchar == 0) {
      continue;
    }

    // GUID desktopId = {0};
    // if (FAILED(i_virtual_desktop_manager->lpVtbl->GetWindowDesktopId(
    //         i_virtual_desktop_manager, window, &desktopId))) {
    //   wprintf(L"failed to call get window desktop id\n");
    //   continue;
    // }
    // wprintf(L"guid of desktop: %lu-%hu-%hu-", desktopId.Data1, desktopId.Data2,
    //         desktopId.Data3);
    // for (int i = 0; i < 8; ++i) {
    //   wprintf(L"%X", desktopId.Data4[i]);
    // }
    // wprintf(L"\n");

    BOOL is_on_current_desktop = false;
    if (FAILED(
            i_virtual_desktop_manager->lpVtbl->IsWindowOnCurrentVirtualDesktop(
                i_virtual_desktop_manager, window, &is_on_current_desktop))) {
      wprintf(L"failed to call is window on current virtual desktop\n");
      wprintf(L"handle: %s", temp_buf);
      continue;
    }
    if (is_on_current_desktop) {
      wprintf(L"handle: %s is on current desktop\n", temp_buf);
    } else {
      // wprintf(L"handle: %s\n", temp_buf);
    }
  }
  printf("end of displaying window names\n");
}
