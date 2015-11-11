#pragma once

namespace fastbird{
	struct MouseEvent
	{
		unsigned short usFlags;
		union {
			unsigned long ulButtons;
			struct  {
				unsigned short  usButtonFlags;
				unsigned short  usButtonData;
			};
		};
		unsigned long ulRawButtons;
		long lLastX;
		long lLastY;
		unsigned long ulExtraInformation;
	};
}