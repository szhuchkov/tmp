#pragma once


char* FS_LoadFile(const char* name, size_t* size);
bool FS_SaveFile(const char* name, const void* data, size_t size);
