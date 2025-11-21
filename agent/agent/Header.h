#pragma once
#include <windows.h>
#include <string>
#include <vector>

struct ModuleInfo {
    std::wstring name;
    std::wstring path;
    void* baseAddress;
    size_t size;
};

struct ThreadInfo {
    DWORD threadId;
    DWORD startAddress;
};

struct MemoryRegion {
    void* baseAddress;
    size_t size;
    DWORD protection;
};

struct ProcessEvent {
    DWORD pid;               
    std::wstring processName;
    std::wstring fullPath;   
    size_t memoryUsage;      
    std::vector<ThreadInfo> threads; 
    std::vector<ModuleInfo> modules; 
    std::vector<MemoryRegion> memoryRegions; 
    DWORD eventType;                 
    ULONGLONG timestamp;             
    std::vector<unsigned char> rawData;
};
