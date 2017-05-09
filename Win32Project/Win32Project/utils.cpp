#include "stdafx.h"
#include "stdio.h"
#include "stdint.h"
#include "utils.h"

bool SaveBitMap(const char* szPathName, void* lpBits, int w, int h)
{
	// Create a new file for writing
	FILE *pFile = fopen(szPathName, "wb");
	if (pFile == NULL) {
		return false;
	}

	BITMAPINFOHEADER BMIH;
	BMIH.biSize = sizeof(BITMAPINFOHEADER);
	BMIH.biSize = sizeof(BITMAPINFOHEADER);
	BMIH.biWidth = w;
	BMIH.biHeight = h;
	BMIH.biPlanes = 1;
	BMIH.biBitCount = 32;
	BMIH.biCompression = BI_RGB;
	BMIH.biSizeImage = w * h * 4;

	BITMAPFILEHEADER bmfh;
	int nBitsOffset = sizeof(BITMAPFILEHEADER) + BMIH.biSize;
	LONG lImageSize = BMIH.biSizeImage;
	LONG lFileSize = nBitsOffset + lImageSize;
	bmfh.bfType = 'B' + ('M' << 8);
	bmfh.bfOffBits = nBitsOffset;
	bmfh.bfSize = lFileSize;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;

	// Write the bitmap file header
	size_t nWrittenFileHeaderSize = fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), pFile);
	// And then the bitmap info header
	size_t nWrittenInfoHeaderSize = fwrite(&BMIH, 1, sizeof(BITMAPINFOHEADER), pFile);
	// Finally, write the image data itself
	size_t nWrittenDIBDataSize    = fwrite(lpBits, 1, lImageSize, pFile);

	fclose(pFile);
	return true;
}