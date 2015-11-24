#pragma once

namespace fastbird{
	int GetNumProcessors();
	bool IsWindowForeground(HWindow window);
	HWindow ForegroundWindow();
	HWindow WindowFromMousePosition();
}