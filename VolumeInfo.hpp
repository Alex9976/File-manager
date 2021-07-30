#pragma once
#include <Windows.h>

class VolumeInfo
{
public:
	TCHAR path[256];
	TCHAR label[256];
	BOOL isAvailable;
	DWORDLONG allSpace, freeSpace, fullSpace;
	VolumeInfo();
	void Update(TCHAR* path);
	UINT GetNumberOfVolumes(TCHAR drives[][4]);
};

