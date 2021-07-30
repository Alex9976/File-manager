#pragma once
#include <Windows.h>

#define MAX_COUNT_FORMATS 17

TCHAR exeFormat[4][5] =
{
	TEXT(".exe"),
	TEXT(".com")
};

TCHAR audioFormat[17][7] =
{
	TEXT(".3gp"),
	TEXT(".aac"),
	TEXT(".aax"),
	TEXT(".act"),
	TEXT(".aiff"),
	TEXT(".amr"),
	TEXT(".ape"),
	TEXT(".flac"),
	TEXT(".m4a"),
	TEXT(".mp3"),
	TEXT(".mpc"),
	TEXT(".ogg"),
	TEXT(".oga"),
	TEXT(".raw"),
	TEXT(".vox"),
	TEXT(".wav"),
	TEXT(".wma"),
};

TCHAR videoFormat[11][7] =
{
	TEXT(".webm"),
	TEXT(".mkv"),
	TEXT(".flv"),
	TEXT(".vob"),
	TEXT(".avi"),
	TEXT(".wmv"),
	TEXT(".rmvb"),
	TEXT(".mp4"),
	TEXT(".m4p"),
	TEXT(".mpg"),
	TEXT(".mpeg")
};

TCHAR imageFormat[7][6] =
{
	TEXT(".jpg"),
	TEXT(".jpeg"),
	TEXT(".exif"),
	TEXT(".tiff"),
	TEXT(".gif"),
	TEXT(".png"),
	TEXT(".bmp")
};

TCHAR documentFormat[6][6] =
{
	TEXT(".pdf"),
	TEXT(".txt"),
	TEXT(".doc"),
	TEXT(".docx"),
	TEXT(".pptx"),
	TEXT(".pub"),
};