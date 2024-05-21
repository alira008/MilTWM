#include "doublylinkedlist.h"
#include "error.h"
#include "keyboard.h"
#include "messages.h"
#include "tiling.h"
#include <ShObjIdl.h>
#include <Windows.h>
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
  IVirtualDesktopManager *i_virtual_desktop_manager = NULL;
  HWINEVENTHOOK win_event_hook = NULL;

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

  win_event_hook =
      SetWinEventHook(EVENT_MIN, EVENT_MAX, NULL, win_event_proc, 0, 0, 0);
  if (!win_event_hook) {
    DisplayError(L"Could not hook win event proc");
    goto cleanup;
  }

  TilingTileWindows();
  DisplayWindowNames(i_virtual_desktop_manager);

  MSG msg;
  DoublyLinkedList *list = doubly_linked_list_init();
  doubly_linked_list_destroy(list);

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
      DisplayWindowNames(i_virtual_desktop_manager);
      break;
    }
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

cleanup:
  if (i_virtual_desktop_manager != NULL) {
    i_virtual_desktop_manager->lpVtbl->Release(i_virtual_desktop_manager);
    CoUninitialize();
  }

  KeyboardCleanup();

  if (win_event_hook) {
    UnhookWinEvent(win_event_hook);
  }

  return exit_code;
}

void win_event_proc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd,
                    LONG idObject, LONG idChild, DWORD idEventThread,
                    DWORD dwmsEventTime) {
  WCHAR temp_buf[256] = {};
  HWND window = hwnd;
  int len_of_wchar = GetWindowTextW(window, temp_buf, 256);
  if (len_of_wchar == 0) {
    return;
  }
  if (event == EVENT_OBJECT_CREATE) {
    wprintf(L"event: object created: %s\n", temp_buf);
  } else if (event == EVENT_OBJECT_DESTROY) {
    printf("event: object destroyed\n");
  } else if (event == EVENT_OBJECT_HIDE) {
    printf("event: object hidden\n");
  } else if (event == EVENT_OBJECT_CLOAKED) {
    printf("event: object cloaked\n");
  } else if (event == EVENT_SYSTEM_MINIMIZESTART) {
    printf("event: object system minimize start\n");
  } else if (event == EVENT_OBJECT_SHOW || event == EVENT_SYSTEM_MINIMIZEEND) {
    printf("event: object show\n");
  } else if (event == EVENT_OBJECT_UNCLOAKED) {
    printf("event: object uncloaked\n");
  } else if (event == EVENT_OBJECT_FOCUS || event == EVENT_SYSTEM_FOREGROUND) {
    printf("event: object focused\n");
  } else if (event == EVENT_SYSTEM_MOVESIZESTART) {
    printf("event: object system move start\n");
  } else if (event == EVENT_SYSTEM_MOVESIZEEND) {
    printf("event: object system move end\n");
  } else if (event == EVENT_SYSTEM_CAPTURESTART ||
             event == EVENT_SYSTEM_CAPTUREEND) {
    wprintf(L"event: object mouse capture\n");
  } else if (event == EVENT_OBJECT_NAMECHANGE) {
    // wprintf(L"event: object name change\n");
  }
}
