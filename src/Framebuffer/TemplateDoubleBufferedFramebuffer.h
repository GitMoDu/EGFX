#ifndef _EGFX_TEMPLATE_DOUBLE_BUFFERED_FRAME_BUFFER_h
#define _EGFX_TEMPLATE_DOUBLE_BUFFERED_FRAME_BUFFER_h

#include "../Platform/Platform.h"

namespace Egfx
{
	/// <summary>
	/// Template double-buffered framebuffer wrapper: provides double-buffering support for any base framebuffer type.
	/// </summary>
	/// <typeparam name="BaseFramebuffer">The base framebuffer type. Must be derived from TemplateFramebuffer.</typeparam>
	template<typename BaseFramebuffer>
	class TemplateDoubleBufferedFramebuffer : public BaseFramebuffer
	{
	public:
		using BaseFramebuffer::BufferSize;

	protected:
		using BaseFramebuffer::Buffer;

	protected:
		// Pointers to the two buffers.
		uint8_t* Buffers[2];

	private:
		// Index of the current front buffer (0 or 1).
		uint8_t FrontBufferIndex = 0;

	public:
		TemplateDoubleBufferedFramebuffer(uint8_t mainBuffer[BufferSize] = nullptr, uint8_t altBuffer[BufferSize] = nullptr)
			: BaseFramebuffer(mainBuffer)
			, Buffers{ mainBuffer, altBuffer }
		{
		}

		/// <summary>
		/// Sets framebuffer main buffer and forwards it to the base class implementation.
		/// </summary>
		/// <param name="buffer">Pointer to a byte array of size BufferSize to use as the framebuffer. The pointer is stored in Buffers[0] and then passed to BaseFramebuffer::SetBuffer.</param>
		virtual void SetBuffer(uint8_t buffer[BufferSize]) override
		{
			Buffers[0] = buffer;
			BaseFramebuffer::SetBuffer(Buffers[0]);
		}

		/// <summary>
		/// Sets the alternate buffer.
		/// </summary>
		/// <param name="altBuffer">Pointer to a buffer (array of uint8_t) with at least BufferSize elements. The array decays to a pointer; the function stores that pointer in Buffers[1].</param>
		void SetAltBuffer(uint8_t altBuffer[BufferSize])
		{
			Buffers[1] = altBuffer;
		}

		/// <summary>
		/// Toggles the drawing buffer between the main and alternative buffers.
		/// Must be called after the current front buffer has been pushed to the display.
		/// </summary>
		virtual bool Flip() override
		{
			FrontBufferIndex = (FrontBufferIndex + 1) & 1;
			BaseFramebuffer::Buffer = Buffers[FrontBufferIndex];

			return true;
		}
	};
}
#endif