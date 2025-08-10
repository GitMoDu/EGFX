#ifndef _EGFX_FRAMEBUFFERS_h
#define _EGFX_FRAMEBUFFERS_h

#include "BinaryFramePainter.h"
#include "Color8FramePainter.h"
#include "Grayscale8FramePainter.h"
#include "Color16FramePainter.h"
#include "Color32FramePainter.h"

#include "TemplateFramebuffer.h"

namespace Egfx
{
	/// <summary>
	/// 1 bit color framebuffer.
	/// </summary>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="threshold">Grayscale threshold for color.</typeparam>
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, const uint8_t threshold = 0
		, typename displayOptions = DisplayOptions::Default>
	class BinaryFramebuffer final
		: public TemplateFramebuffer<BinaryFramePainter<frameWidth, frameHeight, threshold>, clearDivisorPower, displayOptions>
	{
	private:
		using Base = TemplateFramebuffer<BinaryFramePainter<frameWidth, frameHeight, threshold>, clearDivisorPower, displayOptions>;

	public:
		BinaryFramebuffer(uint8_t buffer[Base::BufferSize] = nullptr) : Base(buffer) {}
	};

	/// <summary>
	/// 8 bit color framebuffer.
	/// </summary>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename displayOptions = DisplayOptions::Default>
	class Color8Framebuffer final
		: public TemplateFramebuffer<Color8FramePainter<frameWidth, frameHeight>, clearDivisorPower, displayOptions>
	{
	private:
		using Base = TemplateFramebuffer<Color8FramePainter<frameWidth, frameHeight>, clearDivisorPower, displayOptions>;

	public:
		Color8Framebuffer(uint8_t buffer[Base::BufferSize] = nullptr) : Base(buffer) {}
	};

	/// <summary>
	/// 8 bit grayscale framebuffer.
	/// </summary>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename displayOptions = DisplayOptions::Default>
	class Grayscale8Framebuffer final
		: public TemplateFramebuffer<Grayscale8FramePainter<frameWidth, frameHeight>, clearDivisorPower, displayOptions>
	{
	private:
		using Base = TemplateFramebuffer<Grayscale8FramePainter<frameWidth, frameHeight>, clearDivisorPower, displayOptions>;

	public:
		Grayscale8Framebuffer(uint8_t buffer[Base::BufferSize] = nullptr) : Base(buffer) {}
	};

	/// <summary>
	/// 16 bit color frame buffer.
	/// </summary>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<pixel_t frameWidth, pixel_t frameHeight
		, uint8_t clearDivisorPower = 0
		, typename displayOptions = DisplayOptions::Default>
	class Color16Framebuffer final
		: public TemplateFramebuffer<Color16FramePainter<frameWidth, frameHeight>, clearDivisorPower, displayOptions>
	{
	private:
		using Base = TemplateFramebuffer<Color16FramePainter<frameWidth, frameHeight>, clearDivisorPower, displayOptions>;

	public:
		Color16Framebuffer(uint8_t buffer[Base::BufferSize] = nullptr) : Base(buffer) {}
	};

	/// <summary>
	/// 32 bit color frame buffer.
	/// </summary>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter32.</typeparam>
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename displayOptions = DisplayOptions::Default>
	class Color32FrameBuffer final
		: public TemplateFramebuffer<Color32FramePainter<frameWidth, frameHeight>, clearDivisorPower, displayOptions>
	{
	private:
		using Base = TemplateFramebuffer<Color32FramePainter<frameWidth, frameHeight>, clearDivisorPower, displayOptions>;

	public:
		Color32FrameBuffer(uint8_t buffer[Base::BufferSize] = nullptr) : Base(buffer) {}
	};
}
#endif