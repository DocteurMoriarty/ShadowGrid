#pragma once
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <windows.h>

enum class ShadowEdition {
    BASIC = 0,       // Free version product
    PRO = 1,         // Professional version product
    EDR = 2,         // PME version product
    ELITE = 3        // Military version product
};

struct ShadowState {

    // ------------------------------
    //   COMMON
    // ------------------------------
    DWORD pid;
    std::wstring imagePath;
    uint64_t timestamp;

    bool isAlive;

    // BASIC: flux simple
    uint64_t basicCpuTime;
    uint64_t basicMemoryUsage;
    std::vector<std::wstring> basicModules;
    std::vector<std::wstring> basicHandles;

    // ------------------------------
    //   PRO EDITION
    // ------------------------------
    struct CpuState {
        uint64_t rip;
        uint64_t rsp;
        uint64_t regs[16];
    } cpu;

    std::vector<std::wstring> syscallTrace;
    std::wstring lastSyscall;

    // ------------------------------
    //   EDR EDITION
    // ------------------------------
    struct MemoryRegion {
        uint64_t base;
        uint64_t size;
        std::wstring permissions;
    };

    std::vector<MemoryRegion> memoryLayout;

    std::vector<std::wstring> createdFiles;
    std::vector<std::wstring> registryWrites;
    std::vector<std::wstring> networkConnections;

    std::vector<uint64_t> executionFlow;
    uint64_t expectedNextInstruction;
    uint64_t realNextInstruction;

    // ------------------------------
    //   ELITE EDITION
    // ------------------------------
    std::map<std::string, uint64_t> concreteRegs;
    std::map<std::string, std::string> symbolicRegs;

    struct SymbolicMemory {
        uint64_t addr;
        uint64_t size;
        std::vector<uint8_t> concreteData;
        std::string symbolicExpr;
    };
    std::vector<SymbolicMemory> symbolicMemory;

    struct ShadowSyscallEntry {
        std::string name;
        std::vector<std::string> symbolicArgs;
        uint64_t concreteReturn;
    };
    std::vector<ShadowSyscallEntry> symbolicSyscalls;

    bool symbolicActive = false;

    // ------------------------------
    //   Divergence detection
    // ------------------------------
    bool diverged = false;
    std::wstring divergenceReason;
    uint64_t divergenceAddress;
    uint64_t stepsExecuted = 0;

    // ------------------------------
    //   Edition active
    // ------------------------------
    ShadowEdition edition = ShadowEdition::BASIC;
};

class ShadowCore
{
public:
	// Constructor and Destructor
	ShadowCore();
	~ShadowCore();

	bool initialize(const Config& cfg);
	ShadowState runShadowStep(const ProcessEvent& event);
	bool isShadowReady() const;
	void resetShadowState();

private:
	ShadowState simulateProcess(const ProcessSnapshot& snapshot);
	ShadowState forkShadowState(const ShadowState& prev);
	ShadowState computeNextState(const ProcessEvent& ev);

};
