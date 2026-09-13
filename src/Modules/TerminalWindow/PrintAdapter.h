#ifndef _EGFX_MODULES_TERMINAL_WINDOW_PRINT_ADAPTER_h
#define _EGFX_MODULES_TERMINAL_WINDOW_PRINT_ADAPTER_h

#include <Print.h>
#include "Buffer.h"

namespace Egfx
{
	namespace Modules
	{
		namespace TerminalWindow
		{
			namespace Input
			{
				template<typename TerminalBuffer>
				class PrintAdapter : public Print
				{
				private:
					TerminalBuffer& Buffer;

				public:
					explicit PrintAdapter(TerminalBuffer& buffer) : Buffer(buffer) {}

					using Print::write;
					int availableForWrite() override { return Buffer.BufferSize; }
					size_t write(const uint8_t character) override { return Buffer.Write(character); }
					void Clear() { Buffer.Clear(); }
				};
			}
		}
	}
}
#endif
