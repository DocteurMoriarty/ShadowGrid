#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <functional>
#include <TlHelp32.h>
#include <Psapi.h>
#include <atomic>
#include <thread>
#include <set>

// Forward declaration ProcessEvent
struct ProcessEvent;

// Structure to hold process event information
struct ProcessInfo {
    DWORD pid;
    std::wstring name;
    std::wstring path;
    SIZE_T memoryUsage;
    DWORD threadCount;
};

// Class to monitor system processes
class ProcessMonitor
{
public:
	// Constructor and Destructor
    ProcessMonitor();
    ~ProcessMonitor();

    bool start();
    void stop();
    void setCallback(std::function<void(const ProcessEvent&)> cb);
    std::vector<ProcessInfo> getActiveProcesses() const;

private:
    std::atomic<bool> running{ 
        false
    };
    std::set<DWORD> previousPIDs;
    std::thread monitorThread;
    std::function<void(const ProcessEvent&)> callback;
    void monitoringLoop();
    ProcessInfo fetchProcessInfo(
        DWORD pid
    );
    void detectNewProcesses();
    void detectTerminatedProcesses();
    ProcessEvent captureProcessEvent();
};

