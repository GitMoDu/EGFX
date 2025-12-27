#ifndef _EGFX_BITMASK_FONT_COLOR_FONT_DRAWER_h
#define _EGFX_BITMASK_FONT_COLOR_FONT_DRAWER_h

#include "AbstractFontDrawer.h"
#include "../GraphicsRender/SpriteRenderer.h"
#include "Arduino.h"

namespace Egfx
{
	namespace BitmaskFont
	{
		template<typename BitmaskFontType,
			typename FontColorSourceType>
		class TemplateColorFontDrawer : public AbstractFontDrawer<BitmaskFontType>
		{
		public:
			using dimension_t = typename BitmaskFontType::dimension_t;

		private:
			using Base = AbstractFontDrawer<BitmaskFontType>;

		private:
			using intermediate_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

			static constexpr intermediate_t BitScale = (intermediate_t)BitmaskFontType::Width / 8 + (((intermediate_t)BitmaskFontType::Width % 8) != 0 ? 1 : 0);

		protected:
			using Base::BitmaskFont;

		public:
			FontColorSourceType FontColor{};

		public:
			TemplateColorFontDrawer(const dimension_t kerning = 1)
				: Base(kerning)
			{
			}

		protected:
			// Protected API.
			pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
			{
				const uint8_t* bitmask = BitmaskFont.GetBitmask(printableChar);
				if (bitmask != nullptr)
				{
					const rgb_color_t color = FontColor.GetColor();

					for (dimension_t row = 0; row < Base::GetFontHeight(); row++)
					{
						for (dimension_t col = 0; col < Base::GetFontWidth(); col++)
						{
							if (IsPixelSet(bitmask, col, row))
							{
								framebuffer->Pixel(color, x + col, y + row);
							}
						}
					}

					return Base::GetFontWidth();
				}
				else
				{
					return 0;
				}
			}

			// Protected API, get only character advance.
			pixel_t Advance(const char printableChar) const
			{
				const uint8_t* bitmask = BitmaskFont.GetBitmask(printableChar);
				if (bitmask != nullptr)
				{
					return Base::GetFontWidth();
				}
				else
				{
					return 0;
				}
			}

		private:
			bool IsPixelSet(const uint8_t* mask, const dimension_t x, const dimension_t y) const
			{
				const intermediate_t yByte = static_cast<intermediate_t>(y) * BitScale;
				const dimension_t xByte = SignedRightShift(x, 3);
				const intermediate_t offset = yByte + xByte;

				const uint8_t xBit = 7 - (x % 8);

				if (((pgm_read_byte(&mask[offset]) >> xBit) & 0x01) != 0)
				{
					return true;
				}
				else
				{
					return false;
				}

				return true;
			}
		};

		template<typename BitmaskFontType,
			typename FontColorSourceType,
			uint8_t ScaleX = 1,
			uint8_t ScaleY = ScaleX>
		class TemplateColorScaledFontDrawer : public AbstractScaledFontDrawer<BitmaskFontType, ScaleX, ScaleY>
		{
		public:
			using dimension_t = typename BitmaskFontType::dimension_t;

		private:
			using Base = AbstractScaledFontDrawer<BitmaskFontType, ScaleX, ScaleY>;

		private:
			using intermediate_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

			static constexpr intermediate_t BitScale = (intermediate_t)BitmaskFontType::Width / 8 + (((intermediate_t)BitmaskFontType::Width % 8) != 0 ? 1 : 0);

		protected:
			using Base::BitmaskFont;

		public:
			FontColorSourceType FontColor{};

		public:
			TemplateColorScaledFontDrawer(const dimension_t kerning = 1)
				: Base(kerning)
			{
			}

		protected:
			// Protected API.
			pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
			{
				const uint8_t* bitmask = BitmaskFont.GetBitmask(printableChar);
				if (bitmask != nullptr)
				{
					const rgb_color_t color = FontColor.GetColor();

					pixel_t x1, y1, x2, y2;
					for (dimension_t row = 0; row < BitmaskFontType::Height; row++)
					{
						for (dimension_t col = 0; col < BitmaskFontType::Width; col++)
						{
							if (IsPixelSet(bitmask, col, row))
							{
								x1 = col * ScaleX;
								y1 = row * ScaleY;
								x2 = x1 + ScaleX;
								y2 = y1 + ScaleY;
								framebuffer->RectangleFill(color, x + x1, y + y1, x + x2, y + y2);
							}
						}
					}

					return Base::GetFontWidth();
				}
				else
				{
					return 0;
				}
			}

			// Protected API, get only character advance.
			pixel_t Advance(const char printableChar) const
			{
				const uint8_t* bitmask = BitmaskFont.GetBitmask(printableChar);
				if (bitmask != nullptr)
				{
					return Base::GetFontWidth();
				}
				else
				{
					return 0;
				}
			}

		private:
			bool IsPixelSet(const uint8_t* mask, const dimension_t x, const dimension_t y) const
			{
				const intermediate_t yByte = static_cast<intermediate_t>(y) * BitScale;
				const dimension_t xByte = SignedRightShift(x, 3);
				const intermediate_t offset = yByte + xByte;

				const uint8_t xBit = 7 - (x % 8);

				if (((pgm_read_byte(&mask[offset]) >> xBit) & 0x01) != 0)
				{
					return true;
				}
				else
				{
					return false;
				}

				return true;
			}
		};


	}
}
#endif