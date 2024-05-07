#pragma once

#include <Windows.h>
#include <stdbool.h>

#define TOGGLE_FOCUS_MODE_HOTKEY_ID 0
#define NEXT_WINDOW_HOTKEY_ID 1
#define PREVIOUS_WINDOW_HOTKEY_ID 2
#define QUIT_MILTWM_HOTKEY_ID 3

#define FOCUS_MODE_HOTKEY "alt+f"
#define NEXT_WINDOW_HOTKEY "alt+j"
#define PREVIOUS_WINDOW_HOTKEY "alt+k"
#define QUIT_WINDOW_HOTKEY "alt+q"

bool KeyboardInitializeConfig();
void KeyboardCleanup();
void KeyboardHandleHotkey(WPARAM, LPARAM);
