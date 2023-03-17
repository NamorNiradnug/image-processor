#pragma once

#include <iostream>

#include "cmdline/exceptions.h"
#include "image.h"

class BmpFileError : public ErrorWithMessage {
public:
    BmpFileError(const char* file, const std::string& msg);
};

Image LoadImage(const char* path);
void SaveImage(const Image& image, const char* path);
