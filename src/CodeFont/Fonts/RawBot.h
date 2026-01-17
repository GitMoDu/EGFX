#ifndef _EGFX_CODE_FONTS_RAW_BOT_h
#define _EGFX_CODE_FONTS_RAW_BOT_h

#include "../Model.h"

namespace Egfx
{
	namespace CodeFont
	{
		namespace CodeFonts
		{
			template<uint8_t DefaultFontWidth = 5, uint8_t DefaultFontHeight = 7, uint8_t DefaultKerningWidth = 2>
			class RawBot : public AbstractCodeFont<uint8_t>
			{
			private:
				using Base = AbstractCodeFont<uint8_t>;

			public:
				using dimension_t = uint8_t;

			protected:
				using Base::Width;
				using Base::Height;

			private:
				// Cached middle points for drawing.
				dimension_t MiddleX;
				dimension_t MiddleY;

			public:
				RawBot(const pixel_t width = DefaultFontWidth,
					const pixel_t height = DefaultFontHeight,
					const pixel_t kerningWidth = DefaultKerningWidth)
					: Base(width, height, kerningWidth)
					, MiddleX(width / 2)
					, MiddleY(height / 2)
				{
				}

				/// <summary>
				/// Sets the font width and updates the internal horizontal bounds and midpoint.
				/// </summary>
				virtual void SetFontWidth(const dimension_t width) override
				{
					Base::SetFontWidth(width);
					MiddleX = Width / 2;
				}

				/// <summary>
				/// Sets the font height and recalculates internal vertical positioning values.
				/// </summary>
				virtual void SetFontHeight(const dimension_t height) override
				{
					Base::SetFontHeight(height);
					MiddleY = Height / 2;
				}

				pixel_t Draw(IFrameBuffer* framebuffer, const rgb_color_t color, const pixel_t x, const pixel_t y, const char printableChar)
				{
					switch (static_cast<uint8_t>(printableChar))
					{
					case uint8_t(AsciiDefinition::Printable::Digit0):
						framebuffer->Line(color, x + 1, y, x + Width - 1, y);
						framebuffer->Line(color, x + 1, y + Height - 1, x + Width - 1, y + Height - 1);
						framebuffer->Line(color, x, y + 1, x, y + Height - 1);
						framebuffer->Line(color, x + Width - 1, y + 1, x + Width - 1, y + Height - 1);
						framebuffer->Line(color, x + 2, y + Height - 1, x + Width - 1, y + 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit1):
						if (Width - 1 > 2)
						{
							framebuffer->Line(color, x, y + 1, x + MiddleX, y);
							framebuffer->Line(color, x + MiddleX, y, x + MiddleX, y + Height - 1 - 1);
							framebuffer->Line(color, x + 1, y + Height - 1, x + Width - 1, y + Height - 1);
						}
						else
						{
							framebuffer->Line(color, x + MiddleX, y, x + MiddleX, y + Height - 1 - 1);
							framebuffer->Line(color, x, y + Height - 1, x + Width - 1, y + Height - 1);
						}
						break;
					case uint8_t(AsciiDefinition::Printable::Digit2):
						framebuffer->Line(color, x, y, x + Width - 1 - 1, y);
						framebuffer->Line(color, x + Width - 1, y + 1, x + Width - 1, y + MiddleY - 2);
						framebuffer->Line(color, x, y + Height - 1 - 1, x + Width - 1, y + MiddleY - 1);
						framebuffer->Line(color, x, y + Height - 1, x + Width - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit3):
						framebuffer->Line(color, x + Width - 1, y + 1, x + Width - 1, y + Height - 1);
						framebuffer->Line(color, x, y, x + Width - 1 - 1, y);
						framebuffer->Line(color, x + MiddleX, y + MiddleY - 1, x + Width - 1 - 1, y + MiddleY - 1);
						framebuffer->Line(color, x, y + Height - 1, x + Width - 1 - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit4):
						framebuffer->Line(color, x + Width - 1, y, x + Width - 1, y + Height - 1);
						framebuffer->Line(color, x, y + MiddleY + 1, x + Width - 1 - 1, y + MiddleY + 1);
						if (Width - 1 > 7)
						{
							const pixel_t referenceX = x - 1 + (MiddleX / 2);
							framebuffer->Line(color, referenceX, y, x, y + MiddleY + 1);
						}
						else
						{
							framebuffer->Line(color, x + 1, y, x, y + MiddleY + 1);
						}
						break;
					case uint8_t(AsciiDefinition::Printable::Digit5):
						framebuffer->Line(color, x, y, x + Width - 1, y);
						framebuffer->Line(color, x, y + 1, x, y + MiddleY - 1);
						framebuffer->Line(color, x + 1, y + MiddleY - 1, x + Width - 1 - 1, y + MiddleY - 1);
						framebuffer->Line(color, x + Width - 1, y + MiddleY, x + Width - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x, y + Height - 1, x + Width - 1 - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit6):
						framebuffer->Line(color, x + Width - 1 - 1, y, x, y);
						framebuffer->Line(color, x, y + 1, x, y + Height - 1 - 1);
						framebuffer->Line(color, x + 1, y + MiddleY - 1, x + Width - 1 - 1, y + MiddleY - 1);
						framebuffer->Line(color, x + Width - 1, y + MiddleY, x + Width - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x, y + Height - 1, x + Width - 1 - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit7):
						framebuffer->Line(color, x, y, x + Width - 1 - 1, y);
						framebuffer->Line(color, x + Width - 1 - 1, y + 1, x + Width - 1 - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit8):
						framebuffer->Line(color, x + 1, y, x + Width - 1 - 1, y);
						framebuffer->Line(color, x + 1, y + Height - 1, x + Width - 1 - 1, y + Height - 1);
						framebuffer->Line(color, x, y + 1, x, y + Height - 1 - 1);
						framebuffer->Line(color, x + Width - 1, y + 1, x + Width - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + 1, y + MiddleY - 1, x + Width - 1 - 1, y + MiddleY - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit9):
						framebuffer->Line(color, x + 1, y, x + Width - 1 - 1, y);
						framebuffer->Line(color, x + Width - 1, y + 1, x + Width - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + 1, y + MiddleY - 1, x + Width - 1 - 1, y + MiddleY - 1);
						framebuffer->Line(color, x + Width - 1 - 1, y + MiddleY, x + Width - 1 - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x, y + Height - 1, x + Width - 1 - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Minus):
						framebuffer->Line(color, x, y + MiddleY, x + Width - 1, y + MiddleY);
						break;
					case uint8_t(AsciiDefinition::Printable::a):
					case uint8_t(AsciiDefinition::Printable::A):
					{
						const pixel_t margin = MiddleX / 2;
						framebuffer->Line(color, x + MiddleX - 1, y, x, y + Height - 1);
						framebuffer->Pixel(color, x + MiddleX, y);
						framebuffer->Line(color, x + MiddleX + 1, y, x + Width - 1, y + Height - 1);
						framebuffer->Line(color, x + margin, y + 1, x + Width - 1 - margin, y + 1);
						break;
					}
					case uint8_t(AsciiDefinition::Printable::b):
					case uint8_t(AsciiDefinition::Printable::B):
						framebuffer->Line(color, x, y, x, y + Height - 1);
						framebuffer->Line(color, x, y, x + Width - 1 - 2, y);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + 1);
						framebuffer->Line(color, x + Width - 1, y + 2, x + Width - 1, y + MiddleY - 2);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + MiddleY - 1);
						framebuffer->Line(color, x + 1, y + MiddleY, x + Width - 1 - 2, y + MiddleY);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + MiddleY + 1);
						framebuffer->Line(color, x + Width - 1, y + MiddleY + 2, x + Width - 1, y + Height - 1 - 2);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + 1, y + Height - 1, x + Width - 1 - 2, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::c):
					case uint8_t(AsciiDefinition::Printable::C):
						framebuffer->Line(color, x + 2, y, x + Width - 1, y);
						framebuffer->Pixel(color, x + 1, y + 1);
						framebuffer->Line(color, x, y + 2, x, y + Height - 1 - 2);
						framebuffer->Pixel(color, x + 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + 2, y + Height - 1, x + Width - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::d):
					case uint8_t(AsciiDefinition::Printable::D):
						framebuffer->Line(color, x + 1, y, x + Width - 1 - 2, y);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + 1);
						framebuffer->Line(color, x, y, x, y + Height - 1);
						framebuffer->Line(color, x + Width - 1, y + 2, x + Width - 1, y + Height - 1 - 2);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + 1, y + Height - 1, x + Width - 1 - 2, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::e):
					case uint8_t(AsciiDefinition::Printable::E):
						framebuffer->Line(color, x, y, x, y + Height - 1);
						framebuffer->Line(color, x + 1, y, x + Width - 1, y);
						framebuffer->Line(color, x, y + MiddleY, x + Width - 1 - 2, y + MiddleY);
						framebuffer->Line(color, x + 1, y + Height - 1, x + Width - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::f):
					case uint8_t(AsciiDefinition::Printable::F):
						framebuffer->Line(color, x, y, x, y + Height - 1);
						framebuffer->Line(color, x, y, x + Width - 1, y);
						framebuffer->Line(color, x, y + MiddleY, x + Width - 1 - 3, y + MiddleY);
						break;
					case uint8_t(AsciiDefinition::Printable::g):
					case uint8_t(AsciiDefinition::Printable::G):
						framebuffer->Line(color, x + 2, y, x + Width - 1 - 1, y);
						framebuffer->Pixel(color, x + Width - 1, y + 1);
						framebuffer->Pixel(color, x + 1, y + 1);
						framebuffer->Line(color, x, y + 2, x, y + Height - 1 - 2);
						framebuffer->Pixel(color, x + 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + 2, y + Height - 1, x + Width - 1 - 2, y + Height - 1);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + Width - 1, y + MiddleY, x + Width - 1, y + Height - 1 - 2);
						framebuffer->Line(color, x + MiddleX + 1, y + MiddleY, x + Width - 1 - 1, y + MiddleY);
						break;
					case uint8_t(AsciiDefinition::Printable::h):
					case uint8_t(AsciiDefinition::Printable::H):
						framebuffer->Line(color, x, y, x, y + Height - 1);
						framebuffer->Line(color, x + 1, y + MiddleY, x + Width - 1 - 1, y + MiddleY);
						framebuffer->Line(color, x + Width - 1, y, x + Width - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::i):
					case uint8_t(AsciiDefinition::Printable::I):
						framebuffer->Line(color, x + 1, y, x + Width - 1 - 1, y);
						framebuffer->Line(color, x + MiddleX, y + 1, x + MiddleX, y + Height - 1 - 1);
						framebuffer->Line(color, x + 1, y + Height - 1, x + Width - 1 - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::j):
					case uint8_t(AsciiDefinition::Printable::J):
						framebuffer->Line(color, x + Width - 1, y, x + Width - 1, y + Height - 1 - 2);
						framebuffer->Line(color, x, y + MiddleY, x, y + Height - 1 - 2);
						framebuffer->Pixel(color, x + 1, y + Height - 1 - 1);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + 2, y + Height - 1, x + Width - 1 - 2, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::k):
					case uint8_t(AsciiDefinition::Printable::K):
						framebuffer->Line(color, x, y, x, y + Height - 1);
						framebuffer->Line(color, x, y + MiddleY, x + Width - 1, y);
						framebuffer->Line(color, x + 1, y + MiddleY, x + Width - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::l):
					case uint8_t(AsciiDefinition::Printable::L):
						framebuffer->Line(color, x, y, x, y + Height - 1 - 1);
						framebuffer->Line(color, x, y + Height - 1, x + Width - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::m):
					case uint8_t(AsciiDefinition::Printable::M):
						framebuffer->Line(color, x, y + 1, x, y + Height - 1);
						framebuffer->Line(color, x + Width - 1, y + 1, x + Width - 1, y + Height - 1);
						framebuffer->Line(color, x, y, x + MiddleX, y + Height - 1);
						framebuffer->Line(color, x + MiddleX + 1, y + Height - 1, x + Width - 1, y);
						break;
					case uint8_t(AsciiDefinition::Printable::n):
					case uint8_t(AsciiDefinition::Printable::N):
						framebuffer->Line(color, x, y, x, y + Height - 1);
						framebuffer->Line(color, x + Width - 1, y, x + Width - 1, y + Height - 1);
						framebuffer->Line(color, x + 1, y + 1, x + Width - 1 - 1, y + Height - 1 - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::o):
					case uint8_t(AsciiDefinition::Printable::O):
						framebuffer->Pixel(color, x + 1, y + 1);
						framebuffer->Line(color, x + 2, y, x + Width - 1 - 2, y);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + 1);
						framebuffer->Line(color, x, y + 2, x, y + Height - 1 - 2);
						framebuffer->Pixel(color, x + 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + Width - 1, y + 2, x + Width - 1, y + Height - 1 - 2);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + 2, y + Height - 1, x + Width - 1 - 2, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::p):
					case uint8_t(AsciiDefinition::Printable::P):
						framebuffer->Line(color, x, y, x, y + Height - 1);
						framebuffer->Line(color, x + 1, y, x + Width - 1 - 2, y);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + 1);
						framebuffer->Line(color, x + 2, y, x + Width - 1 - 2, y);
						framebuffer->Line(color, x + Width - 1, y + 2, x + Width - 1, y + MiddleY - 1);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + MiddleY);
						framebuffer->Line(color, x + 1, y + MiddleY + 1, x + Width - 1 - 2, y + MiddleY + 1);
						break;
					case uint8_t(AsciiDefinition::Printable::q):
					case uint8_t(AsciiDefinition::Printable::Q):
						framebuffer->Pixel(color, x + 1, y + 1);
						framebuffer->Line(color, x + 2, y, x + Width - 1 - 2, y);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + 1);
						framebuffer->Line(color, x, y + 2, x, y + Height - 1 - 3);
						framebuffer->Pixel(color, x + 1, y + Height - 1 - 2);
						framebuffer->Line(color, x + Width - 1, y + 2, x + Width - 1, y + Height - 1 - 3);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + Height - 1 - 2);
						framebuffer->Line(color, x + 2, y + Height - 1 - 1, x + Width - 1 - 2, y + Height - 1 - 1);
						framebuffer->Line(color, x + MiddleX, y + Height - 1 - 3, x + Width - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::r):
					case uint8_t(AsciiDefinition::Printable::R):
						framebuffer->Line(color, x, y, x, y + Height - 1);
						framebuffer->Line(color, x + 1, y, x + Width - 1 - 2, y);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + 1);
						framebuffer->Line(color, x + 2, y, x + Width - 1 - 2, y);
						framebuffer->Line(color, x + Width - 1, y + 2, x + Width - 1, y + MiddleY - 1);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + MiddleY);
						framebuffer->Line(color, x + 1, y + MiddleY + 1, x + Width - 1 - 2, y + MiddleY + 1);
						framebuffer->Line(color, x + MiddleX - 1, y + MiddleY + 1, x + Width - 1, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::s):
					case uint8_t(AsciiDefinition::Printable::S):
						framebuffer->Line(color, x + 2, y, x + Width - 1 - 2, y);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + 1);
						framebuffer->Pixel(color, x + 1, y + 1);
						framebuffer->Line(color, x, y + 2, x, y + MiddleY - 2);
						framebuffer->Pixel(color, x + 1, y + MiddleY - 1);
						framebuffer->Line(color, x + 2, y + MiddleY, x + Width - 1 - 2, y + MiddleY);
						framebuffer->Line(color, x + Width - 1, y + MiddleY + 2, x + Width - 1, y + Height - 1 - 2);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + MiddleY + 1);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + 2, y + Height - 1, x + Width - 1 - 2, y + Height - 1);
						framebuffer->Pixel(color, x + 1, y + Height - 1 - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::t):
					case uint8_t(AsciiDefinition::Printable::T):
						framebuffer->Line(color, x, y, x + Width - 1, y);
						framebuffer->Line(color, x + MiddleX, y, x + MiddleX, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::u):
					case uint8_t(AsciiDefinition::Printable::U):
						framebuffer->Line(color, x, y, x, y + Height - 1 - 2);
						framebuffer->Pixel(color, x + 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + 2, y + Height - 1, x + Width - 1 - 2, y + Height - 1);
						framebuffer->Pixel(color, x + Width - 1 - 1, y + Height - 1 - 1);
						framebuffer->Line(color, x + Width - 1, y, x + Width - 1, y + Height - 1 - 2);
						break;
					case uint8_t(AsciiDefinition::Printable::v):
					case uint8_t(AsciiDefinition::Printable::V):
						framebuffer->Line(color, x, y, x + MiddleX - 1, y + Height - 1);
						framebuffer->Pixel(color, x + MiddleX, y + Height - 1);
						framebuffer->Line(color, x + MiddleX + 1, y + Height - 1, x + Width - 1, y);
						break;
					case uint8_t(AsciiDefinition::Printable::w):
					case uint8_t(AsciiDefinition::Printable::W):
					{
						const pixel_t margin = Width - 1 / 3;
						framebuffer->Line(color, x, y, x + MiddleX - margin, y + Height - 1);
						framebuffer->Line(color, x + MiddleX - margin, y + Height - 1, x + MiddleX, y + 1);
						framebuffer->Line(color, x + MiddleX + margin, y + Height - 1, x + MiddleX, y + 1);
						framebuffer->Line(color, x + Width - 1 - 1, y, x + MiddleX + margin, y + Height - 1);
						break;
					}
					case uint8_t(AsciiDefinition::Printable::x):
					case uint8_t(AsciiDefinition::Printable::X):
						framebuffer->Line(color, x, y, x + Width - 1, y + Height - 1);
						framebuffer->Line(color, x, y + Height - 1, x + Width - 1, y);
						break;
					case uint8_t(AsciiDefinition::Printable::y):
					case uint8_t(AsciiDefinition::Printable::Y):
						framebuffer->Line(color, x, y, x + MiddleX - 1, y + MiddleY - 1);
						framebuffer->Line(color, x + Width - 1, y, x + MiddleX + 1, y + MiddleY - 1);
						framebuffer->Line(color, x + MiddleX, y + MiddleY, x + MiddleX, y + Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::z):
					case uint8_t(AsciiDefinition::Printable::Z):
						framebuffer->Line(color, x, y, x + Width - 1, y);
						framebuffer->Line(color, x + 1, y + Height - 1 - 1, x + Width - 1, y + 1);
						framebuffer->Line(color, x, y + Height - 1, x + Width - 1, y + Height - 1);
						break;
					default:
						return 0; // No advance on no-draw.
					};

					// Monospaced font.
					return Width;
				}

				pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
				{
					return Draw(framebuffer, RGB_COLOR_WHITE, x, y, printableChar);
				}
			};
		}
	}
}
#endif