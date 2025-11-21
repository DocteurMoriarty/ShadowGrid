#pragma once

#include <windows.h>
#include <string>

struct ProcessEvent {
    enum class Type {
        CREATED,
        TERMINATED
    } type;

    DWORD pid;
    std::wstring processName;
};