#pragma once

namespace fastbird{
	struct KeyboardEvent
	{
		unsigned short MakeCode;
		unsigned short Flags;
		unsigned short Reserved;
		unsigned short VKey;
		unsigned int   Message;
		unsigned long  ExtraInformation;
	};
}