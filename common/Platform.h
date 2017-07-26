#pragma once


#if defined __ANDROID__
#define PLATFORM_ANDROID	1
#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <android/sensor.h>

#include <android/log.h>
#endif


#if defined _WIN32
#define PLATFORM_PC			1
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#endif


#ifndef PLATFORM_ANDROID
#define PLATFORM_ANDROID	0
#endif


#ifndef PLATFORM_PC
#define PLATFORM_PC			0
#endif

