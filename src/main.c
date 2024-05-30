#include "doublylinkedlist.h"
#include "error.h"
#include "keyboard.h"
#include "messages.h"
#include "tiling.h"
#include <ShObjIdl.h>
#include <Windows.h>
#include <dwmapi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void win_event_proc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd,
                    LONG idObject, LONG idChild, DWORD idEventThread,
                    DWORD dwmsEventTime);
// int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, PWSTR
// cmdLIine,
//                     int cmdShow) {
int main() {
  int exit_code = EXIT_FAILURE;
  HWINEVENTHOOK win_event_hook = NULL;

  SetProcessDPIAware();

  if (!KeyboardInitializeConfig()) {
    DisplayError(L"Could not setup keyboard config");
    goto cleanup;
  }

  win_event_hook =
      SetWinEventHook(EVENT_MIN, EVENT_MAX, NULL, win_event_proc, 0, 0, 0);
  if (!win_event_hook) {
    DisplayError(L"Could not hook win event proc");
    goto cleanup;
  }

  TilingTileWindows();
  DisplayWindowNames();

  MSG msg;

  while (GetMessage(&msg, 0, 0, 0) != 0) {
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
      DisplayWindowNames();
      break;
    }
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

cleanup:
  KeyboardCleanup();

  if (win_event_hook) {
    UnhookWinEvent(win_event_hook);
  }

  return exit_code;
}

BOOL IsWindowCloaked(HWND hwnd) {
  BOOL is_cloaked = false;

  return (SUCCEEDED(DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &is_cloaked,
                                          sizeof(is_cloaked)))) &&
         is_cloaked;
}

void win_event_proc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd,
                    LONG idObject, LONG idChild, DWORD idEventThread,
                    DWORD dwmsEventTime) {
  WCHAR window_title_buf[256] = {};
  WCHAR window_class_buf[256] = {};
  HWND window = hwnd;
  int len_of_wchar = GetWindowTextW(window, window_title_buf, 256);
  if (len_of_wchar == 0) {
    return;
  }
  if (event == EVENT_OBJECT_CREATE) {
    int len_of_win_class = RealGetWindowClassW(window, window_class_buf, 256);
    bool is_cloaked = IsWindowCloaked(window);
    if (!IsWindow(window) || len_of_win_class == 0) {
      return;
    }

    if (wcscmp(window_class_buf, L"OleMainThreadWndClass") != 0) {
      wprintf(L"event: object created: %s, addr: %p\n", window_title_buf,
              window);
    }
  } else if (event == EVENT_OBJECT_DESTROY) {
  } else if (event == EVENT_OBJECT_HIDE) {
  } else if (event == EVENT_OBJECT_CLOAKED) {
  } else if (event == EVENT_SYSTEM_MINIMIZESTART) {
  } else if (event == EVENT_OBJECT_SHOW || event == EVENT_SYSTEM_MINIMIZEEND) {
  } else if (event == EVENT_OBJECT_UNCLOAKED) {
  } else if (event == EVENT_OBJECT_FOCUS || event == EVENT_SYSTEM_FOREGROUND) {
  } else if (event == EVENT_SYSTEM_MOVESIZESTART) {
  } else if (event == EVENT_SYSTEM_MOVESIZEEND) {
  } else if (event == EVENT_SYSTEM_CAPTURESTART ||
             event == EVENT_SYSTEM_CAPTUREEND) {
  } else if (event == EVENT_OBJECT_NAMECHANGE) {
  }
}
