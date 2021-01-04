#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

int select_path(char* result);
int verify_extension(char* path, char* extension);
int verify_input(char* path, std::ifstream* file, const char* extension);
int reverse_filename(char* result, char* path);