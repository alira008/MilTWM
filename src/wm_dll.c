#include "error.h"
#include "messages.h"
#include "shared_memory.h"
#include <Windows.h>

DWORD wm_main_thread_id = 0;

__declspec(dllexport) LRESULT CALLBACK ShellProcess(int code, WPARAM w_param,
                                                    LPARAM l_param) {
  if (code == HSHELL_WINDOWCREATED || code == HSHELL_WINDOWDESTROYED) {
    PostThreadMessageW(wm_main_thread_id, WM_WINDOW_EVENT, 0, 0);
  }

  return CallNextHookEx(NULL, code, w_param, l_param);
}

BOOL APIENTRY DllMain(HINSTANCE h_module, DWORD ul_reason_for_call,
                      LPVOID lp_reserved) {
  if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
    if (!GetDwordFromSharedMemory(&wm_main_thread_id)) {
      DisplayError(L"Error getting thread id from shared memory");
    }
  }

  return TRUE;
}
