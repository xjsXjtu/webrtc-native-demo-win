#pragma once

// szPathName : Specifies the pathname
// lpBits    : Specifies the bitmap bits
// w    : Specifies the image width
// h    : Specifies the image height
bool SaveBitMap(const char* szPathName, void* lpBits, int w, int h);