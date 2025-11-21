#pragma once

#include <string>

#include "Header.h"


class IPCClient
{
public:
	// Constructor and Destructor
	IPCClient();
	~IPCClient();

	bool connect();
	bool sendEvent(const ProcessEvent& event);
	bool sendDivergence(const DivergenceResult& res);
	void disconnect();
	bool isConnected() const;

private:
	bool openSocket();
	bool sendJSON(const std::string& json);
	bool establishTLS();
	std::string serializeEvent(const ProcessEvent& ev);
	std::string serializeDivergence(const DivergenceResult& res);
};