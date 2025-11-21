#include "ProcessMonitor.h"
#include "ProcessEvent.h"

// Start thraed to monitor system processes
bool ProcessMonitor::start()
{
    if (running)
        return false;

    running = true;

    monitorThread = std::thread(&ProcessMonitor::monitoringLoop, this);
    return true;
}

// Stop monitoring system processes
void ProcessMonitor::stop()
{
    if (!running)
        return;

    running = false;

    if (monitorThread.joinable())
        monitorThread.join();
}


void ProcessMonitor::setCallback(
	std::function<void(const ProcessEvent&)> cb
) {
    this->callback = cb;
};

std::vector<ProcessInfo> ProcessMonitor::getActiveProcesses() const {
    std::vector<ProcessInfo> result;

    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS, 
        0
    );
    if (snapshot == INVALID_HANDLE_VALUE)
        return result;

    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    if (!Process32FirstW(snapshot, &entry)) {
        CloseHandle(snapshot);
        return result;
    }

    do {
        ProcessInfo info;
        info.pid = entry.th32ProcessID;
        info.name = entry.szExeFile;
        info.threadCount = entry.cntThreads;

        HANDLE hProc = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
            FALSE, 
            info.pid
        );

        if (hProc) {
            WCHAR buffer[MAX_PATH];
            if (GetModuleFileNameExW(hProc, NULL, buffer, MAX_PATH))
                info.path = buffer;
            else
                info.path = L"";

            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc)))
                info.memoryUsage = pmc.WorkingSetSize;
            else
                info.memoryUsage = 0;

            CloseHandle(hProc);
        }
        else {
            info.path = L"";
            info.memoryUsage = 0;
        }

        result.push_back(info);

    } while (Process32NextW(snapshot, &entry));

    CloseHandle(snapshot);
    return result;
};

ProcessEvent ProcessMonitor::captureProcessEvent() {
    ProcessEvent ev;

    auto currentProcesses = getActiveProcesses();
    std::set<DWORD> currentPIDs;

    for (const auto& p : currentProcesses)
        currentPIDs.insert(p.pid);

    for (DWORD pid : currentPIDs) {
        if (previousPIDs.find(pid) == previousPIDs.end()) {
            ev.type = ProcessEvent::Type::CREATED;
            ev.pid = pid;
            for (const auto& p : currentProcesses) {
                if (p.pid == pid) {
                    ev.processName = p.name;
                    break;
                }
            }
            previousPIDs.insert(pid);
            return ev;
        }
    }

    for (DWORD pid : previousPIDs) {
        if (currentPIDs.find(pid) == currentPIDs.end()) {
            ev.type = ProcessEvent::Type::TERMINATED;
            ev.pid = pid;
            ev.processName = L"";
            previousPIDs.erase(pid);
            return ev;
        }
    }

    ev.type = ProcessEvent::Type::CREATED; 
    ev.pid = 0;
    ev.processName = L"";
    return ev;
}

void ProcessMonitor::monitoringLoop()
{
    while (running) {

        ProcessEvent ev = captureProcessEvent();

        if (callback) {
            callback(ev);
        }

        Sleep(5);
    }
}
