#include <iostream>
#include "Header.h"

ProcessMonitor monitor;

int main()
{
    monitor.setCallback([&](const ProcessEvent& ev) {
        ShadowState shadow = shadowCore.runShadowStep(ev);
        DivergenceResult res = analyzer.analyze(ev, shadow);

        if (analyzer.hasDivergence(res)) {
            ipc.sendDivergence(res);
        }
    });
}



