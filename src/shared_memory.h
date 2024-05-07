#pragma once

#include <Windows.h>
#include <stdbool.h>

bool GetDwordFromSharedMemory(DWORD *);
void CleanupMemoryMapFile();
bool StoreDwordIntoSharedMemory(DWORD);
