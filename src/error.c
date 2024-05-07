#include "error.h"

void DisplayError(WCHAR *msg) {
  MessageBoxW(NULL, msg, L"MilTWM Error", MB_OK);
}
