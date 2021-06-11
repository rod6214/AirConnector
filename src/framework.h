#pragma once

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#ifdef DLLTEMPLATE_EXPORTS
#define DLLTEMPLATE_API __declspec(dllexport)
#else
#define DLLTEMPLATE_API __declspec(dllimport)
#endif

#endif //FRAMEWORK_H
