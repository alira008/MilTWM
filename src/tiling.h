#pragma once

#include <Windows.h> 
#include <ShObjIdl.h>
#include <stdbool.h>

void TilingTileWindows();
void TilingToggleFocusedWindow(HWND);
void TilingFocusNextWindow(bool);
void DisplayWindowNames(IVirtualDesktopManager *);
