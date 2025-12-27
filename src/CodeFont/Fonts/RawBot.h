#ifndef _EGFX_CODE_FONTS_RAW_BOT_h
#define _EGFX_CODE_FONTS_RAW_BOT_h

#include "../Model.h"

namespace Egfx
{
	namespace CodeFont
	{
		namespace CodeFonts
		{
			struct RawBot : AbstractCodeFont<uint8_t>
			{
			private:
				using Base = AbstractCodeFont<uint8_t>;

			public:
				using dimension_t = uint8_t;

			protected:
				using Base::EndX;
				using Base::EndY;
				using Base::MiddleX;
				using Base::MiddleY;

			public:
				RawBot(const pixel_t fontWidth, const pixel_t fontHeight)
					: Base(fontWidth, fontHeight)
				{
				}

				// Protected API, get only character advance.
				dimension_t Advance(const char printableChar) const
				{
					// Monospaced font.
					return EndX + 1;
				}

				pixel_t Draw(IFrameBuffer* framebuffer, const rgb_color_t color, const pixel_t x, const pixel_t y, const char printableChar)
				{
					switch (static_cast<uint8_t>(printableChar))
					{
					case uint8_t(AsciiDefinition::Printable::Digit0):
						framebuffer->Line(color, x + 1, y, x + EndX, y);
						framebuffer->Line(color, x + 1, y + EndY, x + EndX, y + EndY);
						framebuffer->Line(color, x, y + 1, x, y + EndY);
						framebuffer->Line(color, x + EndX, y + 1, x + EndX, y + EndY);
						framebuffer->Line(color, x + 2, y + EndY, x + EndX, y + 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit1):
						if (EndX > 2)
						{
							framebuffer->Line(color, x, y + 1, x + MiddleX, y);
							framebuffer->Line(color, x + MiddleX, y, x + MiddleX, y + EndY - 1);
							framebuffer->Line(color, x + 1, y + EndY, x + EndX, y + EndY);
						}
						else
						{
							framebuffer->Line(color, x + MiddleX, y, x + MiddleX, y + EndY - 1);
							framebuffer->Line(color, x, y + EndY, x + EndX, y + EndY);
						}
						break;
					case uint8_t(AsciiDefinition::Printable::Digit2):
						framebuffer->Line(color, x, y, x + EndX - 1, y);
						framebuffer->Line(color, x + EndX, y + 1, x + EndX, y + MiddleY - 2);
						framebuffer->Line(color, x, y + EndY - 1, x + EndX, y + MiddleY - 1);
						framebuffer->Line(color, x, y + EndY, x + EndX, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit3):
						framebuffer->Line(color, x + EndX, y + 1, x + EndX, y + EndY);
						framebuffer->Line(color, x, y, x + EndX - 1, y);
						framebuffer->Line(color, x + MiddleX, y + MiddleY - 1, x + EndX - 1, y + MiddleY - 1);
						framebuffer->Line(color, x, y + EndY, x + EndX - 1, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit4):
						framebuffer->Line(color, x + EndX, y, x + EndX, y + EndY);
						framebuffer->Line(color, x, y + MiddleY + 1, x + EndX - 1, y + MiddleY + 1);
						if (EndX > 7)
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
						framebuffer->Line(color, x, y, x + EndX, y);
						framebuffer->Line(color, x, y + 1, x, y + MiddleY - 1);
						framebuffer->Line(color, x + 1, y + MiddleY - 1, x + EndX - 1, y + MiddleY - 1);
						framebuffer->Line(color, x + EndX, y + MiddleY, x + EndX, y + EndY - 1);
						framebuffer->Line(color, x, y + EndY, x + EndX - 1, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit6):
						framebuffer->Line(color, x + EndX - 1, y, x, y);
						framebuffer->Line(color, x, y + 1, x, y + EndY - 1);
						framebuffer->Line(color, x + 1, y + MiddleY - 1, x + EndX - 1, y + MiddleY - 1);
						framebuffer->Line(color, x + EndX, y + MiddleY, x + EndX, y + EndY - 1);
						framebuffer->Line(color, x, y + EndY, x + EndX - 1, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit7):
						framebuffer->Line(color, x, y, x + EndX - 1, y);
						framebuffer->Line(color, x + EndX - 1, y + 1, x + EndX - 1, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit8):
						framebuffer->Line(color, x + 1, y, x + EndX - 1, y);
						framebuffer->Line(color, x + 1, y + EndY, x + EndX - 1, y + EndY);
						framebuffer->Line(color, x, y + 1, x, y + EndY - 1);
						framebuffer->Line(color, x + EndX, y + 1, x + EndX, y + EndY - 1);
						framebuffer->Line(color, x + 1, y + MiddleY - 1, x + EndX - 1, y + MiddleY - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit9):
						framebuffer->Line(color, x + 1, y, x + EndX - 1, y);
						framebuffer->Line(color, x + EndX, y + 1, x + EndX, y + EndY - 1);
						framebuffer->Line(color, x + 1, y + MiddleY - 1, x + EndX - 1, y + MiddleY - 1);
						framebuffer->Line(color, x + EndX - 1, y + MiddleY, x + EndX - 1, y + EndY - 1);
						framebuffer->Line(color, x, y + EndY, x + EndX - 1, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::Minus):
						framebuffer->Line(color, x, y + MiddleY, x + EndX, y + MiddleY);
						break;
					case uint8_t(AsciiDefinition::Printable::a):
					case uint8_t(AsciiDefinition::Printable::A):
					{
						const pixel_t margin = MiddleX / 2;
						framebuffer->Line(color, x + MiddleX - 1, y, x, y + EndY);
						framebuffer->Pixel(color, x + MiddleX, y);
						framebuffer->Line(color, x + MiddleX + 1, y, x + EndX, y + EndY);
						framebuffer->Line(color, x + margin, y + 1, x + EndX - margin, y + 1);
						break;
					}
					case uint8_t(AsciiDefinition::Printable::b):
					case uint8_t(AsciiDefinition::Printable::B):
						framebuffer->Line(color, x, y, x, y + EndY);
						framebuffer->Line(color, x, y, x + EndX - 2, y);
						framebuffer->Pixel(color, x + EndX - 1, y + 1);
						framebuffer->Line(color, x + EndX, y + 2, x + EndX, y + MiddleY - 2);
						framebuffer->Pixel(color, x + EndX - 1, y + MiddleY - 1);
						framebuffer->Line(color, x + 1, y + MiddleY, x + EndX - 2, y + MiddleY);
						framebuffer->Pixel(color, x + EndX - 1, y + MiddleY + 1);
						framebuffer->Line(color, x + EndX, y + MiddleY + 2, x + EndX, y + EndY - 2);
						framebuffer->Pixel(color, x + EndX - 1, y + EndY - 1);
						framebuffer->Line(color, x + 1, y + EndY, x + EndX - 2, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::c):
					case uint8_t(AsciiDefinition::Printable::C):
						framebuffer->Line(color, x + 2, y, x + EndX, y);
						framebuffer->Pixel(color, x + 1, y + 1);
						framebuffer->Line(color, x, y + 2, x, y + EndY - 2);
						framebuffer->Pixel(color, x + 1, y + EndY - 1);
						framebuffer->Line(color, x + 2, y + EndY, x + EndX, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::d):
					case uint8_t(AsciiDefinition::Printable::D):
						framebuffer->Line(color, x + 1, y, x + EndX - 2, y);
						framebuffer->Pixel(color, x + EndX - 1, y + 1);
						framebuffer->Line(color, x, y, x, y + EndY);
						framebuffer->Line(color, x + EndX, y + 2, x + EndX, y + EndY - 2);
						framebuffer->Pixel(color, x + EndX - 1, y + EndY - 1);
						framebuffer->Line(color, x + 1, y + EndY, x + EndX - 2, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::e):
					case uint8_t(AsciiDefinition::Printable::E):
						framebuffer->Line(color, x, y, x, y + EndY);
						framebuffer->Line(color, x + 1, y, x + EndX, y);
						framebuffer->Line(color, x, y + MiddleY, x + EndX - 2, y + MiddleY);
						framebuffer->Line(color, x + 1, y + EndY, x + EndX, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::f):
					case uint8_t(AsciiDefinition::Printable::F):
						framebuffer->Line(color, x, y, x, y + EndY);
						framebuffer->Line(color, x, y, x + EndX, y);
						framebuffer->Line(color, x, y + MiddleY, x + EndX - 3, y + MiddleY);
						break;
					case uint8_t(AsciiDefinition::Printable::g):
					case uint8_t(AsciiDefinition::Printable::G):
						framebuffer->Line(color, x + 2, y, x + EndX - 1, y);
						framebuffer->Pixel(color, x + EndX, y + 1);
						framebuffer->Pixel(color, x + 1, y + 1);
						framebuffer->Line(color, x, y + 2, x, y + EndY - 2);
						framebuffer->Pixel(color, x + 1, y + EndY - 1);
						framebuffer->Line(color, x + 2, y + EndY, x + EndX - 2, y + EndY);
						framebuffer->Pixel(color, x + EndX - 1, y + EndY - 1);
						framebuffer->Line(color, x + EndX, y + MiddleY, x + EndX, y + EndY - 2);
						framebuffer->Line(color, x + MiddleX + 1, y + MiddleY, x + EndX - 1, y + MiddleY);
						break;
					case uint8_t(AsciiDefinition::Printable::h):
					case uint8_t(AsciiDefinition::Printable::H):
						framebuffer->Line(color, x, y, x, y + EndY);
						framebuffer->Line(color, x + 1, y + MiddleY, x + EndX - 1, y + MiddleY);
						framebuffer->Line(color, x + EndX, y, x + EndX, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::i):
					case uint8_t(AsciiDefinition::Printable::I):
						framebuffer->Line(color, x + 1, y, x + EndX - 1, y);
						framebuffer->Line(color, x + MiddleX, y + 1, x + MiddleX, y + EndY - 1);
						framebuffer->Line(color, x + 1, y + EndY, x + EndX - 1, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::j):
					case uint8_t(AsciiDefinition::Printable::J):
						framebuffer->Line(color, x + EndX, y, x + EndX, y + EndY - 2);
						framebuffer->Line(color, x, y + MiddleY, x, y + EndY - 2);
						framebuffer->Pixel(color, x + 1, y + EndY - 1);
						framebuffer->Pixel(color, x + EndX - 1, y + EndY - 1);
						framebuffer->Line(color, x + 2, y + EndY, x + EndX - 2, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::k):
					case uint8_t(AsciiDefinition::Printable::K):
						framebuffer->Line(color, x, y, x, y + EndY);
						framebuffer->Line(color, x, y + MiddleY, x + EndX, y);
						framebuffer->Line(color, x + 1, y + MiddleY, x + EndX, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::l):
					case uint8_t(AsciiDefinition::Printable::L):
						framebuffer->Line(color, x, y, x, y + EndY - 1);
						framebuffer->Line(color, x, y + EndY, x + EndX, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::m):
					case uint8_t(AsciiDefinition::Printable::M):
						framebuffer->Line(color, x, y + 1, x, y + EndY);
						framebuffer->Line(color, x + EndX, y + 1, x + EndX, y + EndY);
						framebuffer->Line(color, x, y, x + MiddleX, y + EndY);
						framebuffer->Line(color, x + MiddleX + 1, y + EndY, x + EndX, y);
						break;
					case uint8_t(AsciiDefinition::Printable::n):
					case uint8_t(AsciiDefinition::Printable::N):
						framebuffer->Line(color, x, y, x, y + EndY);
						framebuffer->Line(color, x + EndX, y, x + EndX, y + EndY);
						framebuffer->Line(color, x + 1, y + 1, x + EndX - 1, y + EndY - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::o):
					case uint8_t(AsciiDefinition::Printable::O):
						framebuffer->Pixel(color, x + 1, y + 1);
						framebuffer->Line(color, x + 2, y, x + EndX - 2, y);
						framebuffer->Pixel(color, x + EndX - 1, y + 1);
						framebuffer->Line(color, x, y + 2, x, y + EndY - 2);
						framebuffer->Pixel(color, x + 1, y + EndY - 1);
						framebuffer->Line(color, x + EndX, y + 2, x + EndX, y + EndY - 2);
						framebuffer->Pixel(color, x + EndX - 1, y + EndY - 1);
						framebuffer->Line(color, x + 2, y + EndY, x + EndX - 2, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::p):
					case uint8_t(AsciiDefinition::Printable::P):
						framebuffer->Line(color, x, y, x, y + EndY);
						framebuffer->Line(color, x + 1, y, x + EndX - 2, y);
						framebuffer->Pixel(color, x + EndX - 1, y + 1);
						framebuffer->Line(color, x + 2, y, x + EndX - 2, y);
						framebuffer->Line(color, x + EndX, y + 2, x + EndX, y + MiddleY - 1);
						framebuffer->Pixel(color, x + EndX - 1, y + MiddleY);
						framebuffer->Line(color, x + 1, y + MiddleY + 1, x + EndX - 2, y + MiddleY + 1);
						break;
					case uint8_t(AsciiDefinition::Printable::q):
					case uint8_t(AsciiDefinition::Printable::Q):
						framebuffer->Pixel(color, x + 1, y + 1);
						framebuffer->Line(color, x + 2, y, x + EndX - 2, y);
						framebuffer->Pixel(color, x + EndX - 1, y + 1);
						framebuffer->Line(color, x, y + 2, x, y + EndY - 3);
						framebuffer->Pixel(color, x + 1, y + EndY - 2);
						framebuffer->Line(color, x + EndX, y + 2, x + EndX, y + EndY - 3);
						framebuffer->Pixel(color, x + EndX - 1, y + EndY - 2);
						framebuffer->Line(color, x + 2, y + EndY - 1, x + EndX - 2, y + EndY - 1);
						framebuffer->Line(color, x + MiddleX, y + EndY - 3, x + EndX, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::r):
					case uint8_t(AsciiDefinition::Printable::R):
						framebuffer->Line(color, x, y, x, y + EndY);
						framebuffer->Line(color, x + 1, y, x + EndX - 2, y);
						framebuffer->Pixel(color, x + EndX - 1, y + 1);
						framebuffer->Line(color, x + 2, y, x + EndX - 2, y);
						framebuffer->Line(color, x + EndX, y + 2, x + EndX, y + MiddleY - 1);
						framebuffer->Pixel(color, x + EndX - 1, y + MiddleY);
						framebuffer->Line(color, x + 1, y + MiddleY + 1, x + EndX - 2, y + MiddleY + 1);
						framebuffer->Line(color, x + MiddleX - 1, y + MiddleY + 1, x + EndX, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::s):
					case uint8_t(AsciiDefinition::Printable::S):
						framebuffer->Line(color, x + 2, y, x + EndX - 2, y);
						framebuffer->Pixel(color, x + EndX - 1, y + 1);
						framebuffer->Pixel(color, x + 1, y + 1);
						framebuffer->Line(color, x, y + 2, x, y + MiddleY - 2);
						framebuffer->Pixel(color, x + 1, y + MiddleY - 1);
						framebuffer->Line(color, x + 2, y + MiddleY, x + EndX - 2, y + MiddleY);
						framebuffer->Line(color, x + EndX, y + MiddleY + 2, x + EndX, y + EndY - 2);
						framebuffer->Pixel(color, x + EndX - 1, y + MiddleY + 1);
						framebuffer->Pixel(color, x + EndX - 1, y + EndY - 1);
						framebuffer->Line(color, x + 2, y + EndY, x + EndX - 2, y + EndY);
						framebuffer->Pixel(color, x + 1, y + EndY - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::t):
					case uint8_t(AsciiDefinition::Printable::T):
						framebuffer->Line(color, x, y, x + EndX, y);
						framebuffer->Line(color, x + MiddleX, y, x + MiddleX, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::u):
					case uint8_t(AsciiDefinition::Printable::U):
						framebuffer->Line(color, x, y, x, y + EndY - 2);
						framebuffer->Pixel(color, x + 1, y + EndY - 1);
						framebuffer->Line(color, x + 2, y + EndY, x + EndX - 2, y + EndY);
						framebuffer->Pixel(color, x + EndX - 1, y + EndY - 1);
						framebuffer->Line(color, x + EndX, y, x + EndX, y + EndY - 2);
						break;
					case uint8_t(AsciiDefinition::Printable::v):
					case uint8_t(AsciiDefinition::Printable::V):
						framebuffer->Line(color, x, y, x + MiddleX - 1, y + EndY);
						framebuffer->Pixel(color, x + MiddleX, y + EndY);
						framebuffer->Line(color, x + MiddleX + 1, y + EndY, x + EndX, y);
						break;
					case uint8_t(AsciiDefinition::Printable::w):
					case uint8_t(AsciiDefinition::Printable::W):
					{
						const pixel_t margin = EndX / 3;
						framebuffer->Line(color, x, y, x + MiddleX - margin, y + EndY);
						framebuffer->Line(color, x + MiddleX - margin, y + EndY, x + MiddleX, y + 1);
						framebuffer->Line(color, x + MiddleX + margin, y + EndY, x + MiddleX, y + 1);
						framebuffer->Line(color, x + EndX - 1, y, x + MiddleX + margin, y + EndY);
						break;
					}
					case uint8_t(AsciiDefinition::Printable::x):
					case uint8_t(AsciiDefinition::Printable::X):
						framebuffer->Line(color, x, y, x + EndX, y + EndY);
						framebuffer->Line(color, x, y + EndY, x + EndX, y);
						break;
					case uint8_t(AsciiDefinition::Printable::y):
					case uint8_t(AsciiDefinition::Printable::Y):
						framebuffer->Line(color, x, y, x + MiddleX - 1, y + MiddleY - 1);
						framebuffer->Line(color, x + EndX, y, x + MiddleX + 1, y + MiddleY - 1);
						framebuffer->Line(color, x + MiddleX, y + MiddleY, x + MiddleX, y + EndY);
						break;
					case uint8_t(AsciiDefinition::Printable::z):
					case uint8_t(AsciiDefinition::Printable::Z):
						framebuffer->Line(color, x, y, x + EndX, y);
						framebuffer->Line(color, x + 1, y + EndY - 1, x + EndX, y + 1);
						framebuffer->Line(color, x, y + EndY, x + EndX, y + EndY);
						break;
					default:
						return 0; // No advance on no-draw.
					};

					// Monospaced font.
					return EndX;
				}
			};
		}
	}
}
#endif