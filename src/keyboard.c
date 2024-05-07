#include "keyboard.h"
#include "error.h"
#include "tiling.h"
#include <Windows.h>
#include <winuser.h>

void BindKeyboardHotkey(int id, UINT modifier, UINT keycode) {
  if (!RegisterHotKey(NULL, id, modifier | MOD_NOREPEAT, keycode)) {
    DisplayError(L"Failed to register hotkey");
    return;
  }
}

UINT GetModifier(const char *key) {
  if (lstrcmpA(key, "alt") == 0) {
    return MOD_ALT;
  } else if (lstrcmpA(key, "win") == 0) {
    return MOD_WIN;
  } else if (lstrcmpA(key, "ctrl") == 0) {
    return MOD_CONTROL;
  } else if (lstrcmpA(key, "shift") == 0) {
    return MOD_SHIFT;
  }

  return MOD_ALT;
}

UINT GetKeycode(const char *key) {
  return VkKeyScanEx(key[lstrlenA(key) - 1], GetKeyboardLayout(0));
}

bool KeyboardInitializeConfig() {
  BindKeyboardHotkey(TOGGLE_FOCUS_MODE_HOTKEY_ID,
                     GetModifier(FOCUS_MODE_HOTKEY),
                     GetKeycode(FOCUS_MODE_HOTKEY));
  BindKeyboardHotkey(NEXT_WINDOW_HOTKEY_ID, GetModifier(NEXT_WINDOW_HOTKEY),
                     GetKeycode(NEXT_WINDOW_HOTKEY));
  BindKeyboardHotkey(PREVIOUS_WINDOW_HOTKEY_ID,
                     GetModifier(PREVIOUS_WINDOW_HOTKEY),
                     GetKeycode(PREVIOUS_WINDOW_HOTKEY));
  BindKeyboardHotkey(QUIT_MILTWM_HOTKEY_ID, GetModifier(QUIT_WINDOW_HOTKEY),
                     GetKeycode(QUIT_WINDOW_HOTKEY));
  return true;
}

void KeyboardCleanup() {
  UnregisterHotKey(NULL, TOGGLE_FOCUS_MODE_HOTKEY_ID);
  UnregisterHotKey(NULL, NEXT_WINDOW_HOTKEY_ID);
  UnregisterHotKey(NULL, PREVIOUS_WINDOW_HOTKEY_ID);
  UnregisterHotKey(NULL, QUIT_MILTWM_HOTKEY_ID);
}

void KeyboardHandleHotkey(WPARAM w_param, LPARAM l_param) {
  switch (w_param) {
  case TOGGLE_FOCUS_MODE_HOTKEY_ID:
    TilingToggleFocusedWindow(GetForegroundWindow());
    break;
  case NEXT_WINDOW_HOTKEY_ID:
    TilingFocusNextWindow(true);
    break;
  case PREVIOUS_WINDOW_HOTKEY_ID:
    TilingFocusNextWindow(false);
    break;
  default:
    break;
  }
}
