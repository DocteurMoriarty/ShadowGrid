#pragma once

#include "Header.h"

class DivergenceAnalyzer
{
public:
	// Constructor and Destructor
	DivergenceAnalyzer();
	~DivergenceAnalyzer();

	DivergenceResult analyze(const ProcessEvent& realEv, const ShadowState& shadowEv);
	bool hasDivergence(const DivergenceResult& res) const;
	std::string formatReport(const DivergenceResult& res) const;

private:
	double computeScore(const ProcessEvent& realEv, const ShadowState& shadowEv);
	bool checkSyscallMismatch(const ProcessEvent&, const ShadowState&);
	bool checkTimingMismatch(const ProcessEvent&, const ShadowState&);
	bool checkMemoryDelta(const ProcessEvent&, const ShadowState&);
};