#include "shared_memory.h"
#include "error.h"

HANDLE g_map_handle = NULL;

bool ReadOrWriteToSharedMemory(DWORD write, DWORD *read) {
  bool success = false;

  g_map_handle = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                                    0, sizeof(DWORD), "MilTWMThreadId");

  if (g_map_handle == NULL) {
    DisplayError(L"Could not create the file mapping");
    goto cleanup;
  }

  LPVOID map_address =
      MapViewOfFile(g_map_handle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(DWORD));

  if (map_address == NULL) {
    DisplayError(L"Could not create the map view of the file");
    goto cleanup;
  }

  if (write) {
    *(DWORD *)map_address = write;
  } else {

    *read = *(DWORD *)map_address;
  }
  success = true;

cleanup:
  if (map_address) {
    UnmapViewOfFile(map_address);
  }

  if (read) {
    CleanupMemoryMapFile();
  }

  return success;
}

void CleanupMemoryMapFile() {
  if (g_map_handle) {

    CloseHandle(g_map_handle);
  }
}

bool GetDwordFromSharedMemory(DWORD *output) {
  return ReadOrWriteToSharedMemory(0, output);
}

bool StoreDwordIntoSharedMemory(DWORD input) {
  return ReadOrWriteToSharedMemory(input, 0);
}
