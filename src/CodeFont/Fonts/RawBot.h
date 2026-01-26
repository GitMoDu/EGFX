#ifndef _EGFX_CODE_FONTS_RAW_BOT_h
#define _EGFX_CODE_FONTS_RAW_BOT_h

#include "../Model.h"

namespace Egfx
{
	namespace CodeFont
	{
		namespace CodeFonts
		{
			template<uint8_t DefaultFontWidth = 5,
				uint8_t DefaultFontHeight = 7,
				uint8_t DefaultKerningWidth = 2,
				typename PrimitiveShaderType = VectorGraphics::Shaders::Primitive::SingleColor<pixel_t>
			>
			class RawBot : public AbstractCodeFont<pixel_t, PrimitiveShaderType>
			{
			private:
				using Base = AbstractCodeFont<pixel_t, PrimitiveShaderType>;

			public:
				using dimension_t = pixel_t;

			public:
				using Base::PrimitivePrepare;
				using Base::PrimitivePixel;
				using Base::PrimitiveLine;

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

				pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
				{
					PrimitivePrepare(x, y);

					switch (static_cast<uint8_t>(printableChar))
					{
					case uint8_t(AsciiDefinition::Printable::Digit0):
						PrimitiveLine(framebuffer, 1, 0, Width - 1, 0);
						PrimitiveLine(framebuffer, 1, Height - 1, Width - 1, Height - 1);
						PrimitiveLine(framebuffer, 0, 1, 0, Height - 1);
						PrimitiveLine(framebuffer, Width - 1, 1, Width - 1, Height - 1);
						PrimitiveLine(framebuffer, 2, Height - 1, Width - 1, 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit1):
						if (Width - 1 > 2)
						{
							PrimitiveLine(framebuffer, 0, 1, MiddleX, 0);
							PrimitiveLine(framebuffer, MiddleX, 0, MiddleX, Height - 1 - 1);
							PrimitiveLine(framebuffer, 1, Height - 1, Width - 1, Height - 1);
						}
						else
						{
							PrimitiveLine(framebuffer, MiddleX, 0, MiddleX, Height - 1 - 1);
							PrimitiveLine(framebuffer, 0, Height - 1, Width - 1, Height - 1);
						}
						break;
					case uint8_t(AsciiDefinition::Printable::Digit2):
						PrimitiveLine(framebuffer, 0, 0, Width - 1 - 1, 0);
						PrimitiveLine(framebuffer, Width - 1, 1, Width - 1, MiddleY - 2);
						PrimitiveLine(framebuffer, 0, Height - 1 - 1, Width - 1, MiddleY - 1);
						PrimitiveLine(framebuffer, 0, Height - 1, Width - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit3):
						PrimitiveLine(framebuffer, Width - 1, 1, Width - 1, Height - 1);
						PrimitiveLine(framebuffer, 0, 0, Width - 1 - 1, 0);
						PrimitiveLine(framebuffer, MiddleX, MiddleY - 1, Width - 1 - 1, MiddleY - 1);
						PrimitiveLine(framebuffer, 0, Height - 1, Width - 1 - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit4):
						PrimitiveLine(framebuffer, Width - 1, 0, Width - 1, Height - 1);
						PrimitiveLine(framebuffer, 0, MiddleY + 1, Width - 1 - 1, MiddleY + 1);
						if (Width - 1 > 7)
						{
							const pixel_t referenceX = x - 1 + (MiddleX / 2);
							PrimitiveLine(framebuffer, referenceX - x, 0, 0, MiddleY + 1);
						}
						else
						{
							PrimitiveLine(framebuffer, 1, 0, 0, MiddleY + 1);
						}
						break;
					case uint8_t(AsciiDefinition::Printable::Digit5):
						PrimitiveLine(framebuffer, 0, 0, Width - 1, 0);
						PrimitiveLine(framebuffer, 0, 1, 0, MiddleY - 1);
						PrimitiveLine(framebuffer, 1, MiddleY - 1, Width - 1 - 1, MiddleY - 1);
						PrimitiveLine(framebuffer, Width - 1, MiddleY, Width - 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, 0, Height - 1, Width - 1 - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit6):
						PrimitiveLine(framebuffer, Width - 1 - 1, 0, 0, 0);
						PrimitiveLine(framebuffer, 0, 1, 0, Height - 1 - 1);
						PrimitiveLine(framebuffer, 1, MiddleY - 1, Width - 1 - 1, MiddleY - 1);
						PrimitiveLine(framebuffer, Width - 1, MiddleY, Width - 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, 0, Height - 1, Width - 1 - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit7):
						PrimitiveLine(framebuffer, 0, 0, Width - 1 - 1, 0);
						PrimitiveLine(framebuffer, Width - 1 - 1, 1, Width - 1 - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit8):
						PrimitiveLine(framebuffer, 1, 0, Width - 1 - 1, 0);
						PrimitiveLine(framebuffer, 1, Height - 1, Width - 1 - 1, Height - 1);
						PrimitiveLine(framebuffer, 0, 1, 0, Height - 1 - 1);
						PrimitiveLine(framebuffer, Width - 1, 1, Width - 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, 1, MiddleY - 1, Width - 1 - 1, MiddleY - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Digit9):
						PrimitiveLine(framebuffer, 1, 0, Width - 1 - 1, 0);
						PrimitiveLine(framebuffer, 0, 1, 0, MiddleY - 1);
						PrimitiveLine(framebuffer, 1, MiddleY - 1, Width - 1 - 1, MiddleY - 1);
						PrimitiveLine(framebuffer, Width - 1, 1, Width - 1, Height - 1 - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::Minus):
						PrimitiveLine(framebuffer, 0, MiddleY, Width - 1, MiddleY);
						break;
					case uint8_t(AsciiDefinition::Printable::a):
					case uint8_t(AsciiDefinition::Printable::A):
					{
						const pixel_t margin = MiddleX / 2;
						PrimitiveLine(framebuffer, MiddleX - 1, 0, 0, Height - 1);
						PrimitivePixel(framebuffer, MiddleX, 0);
						PrimitiveLine(framebuffer, MiddleX + 1, 0, Width - 1, Height - 1);
						PrimitiveLine(framebuffer, margin, 1, Width - 1 - margin, 1);
						break;
					}
					case uint8_t(AsciiDefinition::Printable::b):
					case uint8_t(AsciiDefinition::Printable::B):
						PrimitiveLine(framebuffer, 0, 0, 0, Height - 1);
						PrimitiveLine(framebuffer, 0, 0, Width - 1 - 2, 0);
						PrimitivePixel(framebuffer, Width - 1 - 1, 1);
						PrimitiveLine(framebuffer, Width - 1, 2, Width - 1, MiddleY - 2);
						PrimitivePixel(framebuffer, Width - 1 - 1, MiddleY - 1);
						PrimitiveLine(framebuffer, 1, MiddleY, Width - 1 - 2, MiddleY);
						PrimitivePixel(framebuffer, Width - 1 - 1, MiddleY + 1);
						PrimitiveLine(framebuffer, Width - 1, MiddleY + 2, Width - 1, Height - 1 - 2);
						PrimitivePixel(framebuffer, Width - 1 - 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, 1, Height - 1, Width - 1 - 2, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::c):
					case uint8_t(AsciiDefinition::Printable::C):
						PrimitiveLine(framebuffer, 2, 0, Width - 1, 0);
						PrimitivePixel(framebuffer, 1, 1);
						PrimitiveLine(framebuffer, 0, 2, 0, Height - 1 - 2);
						PrimitivePixel(framebuffer, 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, 2, Height - 1, Width - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::d):
					case uint8_t(AsciiDefinition::Printable::D):
						PrimitiveLine(framebuffer, 1, 0, Width - 1 - 2, 0);
						PrimitivePixel(framebuffer, Width - 1 - 1, 1);
						PrimitiveLine(framebuffer, 0, 0, 0, Height - 1);
						PrimitiveLine(framebuffer, Width - 1, 2, Width - 1, Height - 1 - 2);
						PrimitivePixel(framebuffer, Width - 1 - 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, 1, Height - 1, Width - 1 - 2, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::e):
					case uint8_t(AsciiDefinition::Printable::E):
						PrimitiveLine(framebuffer, 0, 0, 0, Height - 1);
						PrimitiveLine(framebuffer, 1, 0, Width - 1, 0);
						PrimitiveLine(framebuffer, 0, MiddleY, Width - 1 - 2, MiddleY);
						PrimitiveLine(framebuffer, 1, Height - 1, Width - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::f):
					case uint8_t(AsciiDefinition::Printable::F):
						PrimitiveLine(framebuffer, 0, 0, 0, Height - 1);
						PrimitiveLine(framebuffer, 0, 0, Width - 1, 0);
						PrimitiveLine(framebuffer, 0, MiddleY, Width - 1 - 3, MiddleY);
						break;
					case uint8_t(AsciiDefinition::Printable::g):
					case uint8_t(AsciiDefinition::Printable::G):
						PrimitiveLine(framebuffer, 2, 0, Width - 1 - 1, 0);
						PrimitivePixel(framebuffer, Width - 1, 1);
						PrimitivePixel(framebuffer, 1, 1);
						PrimitiveLine(framebuffer, 0, 2, 0, Height - 1 - 2);
						PrimitivePixel(framebuffer, 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, 2, Height - 1, Width - 1 - 2, Height - 1);
						PrimitivePixel(framebuffer, Width - 1 - 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, Width - 1, MiddleY, Width - 1, Height - 1 - 2);
						PrimitiveLine(framebuffer, MiddleX + 1, MiddleY, Width - 1 - 1, MiddleY);
						break;
					case uint8_t(AsciiDefinition::Printable::h):
					case uint8_t(AsciiDefinition::Printable::H):
						PrimitiveLine(framebuffer, 0, 0, 0, Height - 1);
						PrimitiveLine(framebuffer, 1, MiddleY, Width - 1 - 1, MiddleY);
						PrimitiveLine(framebuffer, Width - 1, 0, Width - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::i):
					case uint8_t(AsciiDefinition::Printable::I):
						PrimitiveLine(framebuffer, 1, 0, Width - 1 - 1, 0);
						PrimitiveLine(framebuffer, MiddleX, 1, MiddleX, Height - 1 - 1);
						PrimitiveLine(framebuffer, 1, Height - 1, Width - 1 - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::j):
					case uint8_t(AsciiDefinition::Printable::J):
						PrimitiveLine(framebuffer, Width - 1, 0, Width - 1, Height - 1 - 2);
						PrimitiveLine(framebuffer, 0, MiddleY, 0, Height - 1 - 2);
						PrimitivePixel(framebuffer, 1, Height - 1 - 1);
						PrimitivePixel(framebuffer, Width - 1 - 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, 2, Height - 1, Width - 1 - 2, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::k):
					case uint8_t(AsciiDefinition::Printable::K):
						PrimitiveLine(framebuffer, 0, 0, 0, Height - 1);
						PrimitiveLine(framebuffer, 0, MiddleY, Width - 1, 0);
						PrimitiveLine(framebuffer, 1, MiddleY, Width - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::l):
					case uint8_t(AsciiDefinition::Printable::L):
						PrimitiveLine(framebuffer, 0, 0, 0, Height - 1 - 1);
						PrimitiveLine(framebuffer, 0, Height - 1, Width - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::m):
					case uint8_t(AsciiDefinition::Printable::M):
						PrimitiveLine(framebuffer, 0, 1, 0, Height - 1);
						PrimitiveLine(framebuffer, Width - 1, 1, Width - 1, Height - 1);
						PrimitiveLine(framebuffer, 0, 0, MiddleX, Height - 1);
						PrimitiveLine(framebuffer, MiddleX + 1, Height - 1, Width - 1, 0);
						break;
					case uint8_t(AsciiDefinition::Printable::n):
					case uint8_t(AsciiDefinition::Printable::N):
						PrimitiveLine(framebuffer, 0, 0, 0, Height - 1);
						PrimitiveLine(framebuffer, Width - 1, 0, Width - 1, Height - 1);
						PrimitiveLine(framebuffer, 1, 1, Width - 1 - 1, Height - 1 - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::o):
					case uint8_t(AsciiDefinition::Printable::O):
						PrimitivePixel(framebuffer, 1, 1);
						PrimitiveLine(framebuffer, 2, 0, Width - 1 - 2, 0);
						PrimitivePixel(framebuffer, Width - 1 - 1, 1);
						PrimitiveLine(framebuffer, 0, 2, 0, Height - 1 - 2);
						PrimitivePixel(framebuffer, 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, Width - 1, 2, Width - 1, Height - 1 - 2);
						PrimitivePixel(framebuffer, Width - 1 - 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, 2, Height - 1, Width - 1 - 2, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::p):
					case uint8_t(AsciiDefinition::Printable::P):
						PrimitiveLine(framebuffer, 0, 0, 0, Height - 1);
						PrimitiveLine(framebuffer, 1, 0, Width - 1 - 2, 0);
						PrimitivePixel(framebuffer, Width - 1 - 1, 1);
						PrimitiveLine(framebuffer, 2, 0, Width - 1 - 2, 0);
						PrimitiveLine(framebuffer, Width - 1, 2, Width - 1, MiddleY - 1);
						PrimitivePixel(framebuffer, Width - 1 - 1, MiddleY);
						PrimitiveLine(framebuffer, 1, MiddleY + 1, Width - 1 - 2, MiddleY + 1);
						break;
					case uint8_t(AsciiDefinition::Printable::q):
					case uint8_t(AsciiDefinition::Printable::Q):
						PrimitivePixel(framebuffer, 1, 1);
						PrimitiveLine(framebuffer, 2, 0, Width - 1 - 2, 0);
						PrimitivePixel(framebuffer, Width - 1 - 1, 1);
						PrimitiveLine(framebuffer, 0, 2, 0, Height - 1 - 3);
						PrimitivePixel(framebuffer, 1, Height - 1 - 2);
						PrimitiveLine(framebuffer, Width - 1, 2, Width - 1, Height - 1 - 3);
						PrimitivePixel(framebuffer, Width - 1 - 1, Height - 1 - 2);
						PrimitiveLine(framebuffer, 2, Height - 1 - 1, Width - 1 - 2, Height - 1 - 1);
						PrimitiveLine(framebuffer, MiddleX, Height - 1 - 3, Width - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::r):
					case uint8_t(AsciiDefinition::Printable::R):
						PrimitiveLine(framebuffer, 0, 0, 0, Height - 1);
						PrimitiveLine(framebuffer, 1, 0, Width - 1 - 2, 0);
						PrimitivePixel(framebuffer, Width - 1 - 1, 1);
						PrimitiveLine(framebuffer, 2, 0, Width - 1 - 2, 0);
						PrimitiveLine(framebuffer, Width - 1, 2, Width - 1, MiddleY - 1);
						PrimitivePixel(framebuffer, Width - 1 - 1, MiddleY);
						PrimitiveLine(framebuffer, 1, MiddleY + 1, Width - 1 - 2, MiddleY + 1);
						PrimitiveLine(framebuffer, MiddleX - 1, MiddleY + 1, Width - 1, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::s):
					case uint8_t(AsciiDefinition::Printable::S):
						PrimitiveLine(framebuffer, 2, 0, Width - 1 - 2, 0);
						PrimitivePixel(framebuffer, Width - 1 - 1, 1);
						PrimitivePixel(framebuffer, 1, 1);
						PrimitiveLine(framebuffer, 0, 2, 0, MiddleY - 2);
						PrimitivePixel(framebuffer, 1, MiddleY - 1);
						PrimitiveLine(framebuffer, 2, MiddleY, Width - 1 - 2, MiddleY);
						PrimitiveLine(framebuffer, Width - 1, MiddleY + 2, Width - 1, Height - 1 - 2);
						PrimitivePixel(framebuffer, Width - 1 - 1, MiddleY + 1);
						PrimitivePixel(framebuffer, Width - 1 - 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, 2, Height - 1, Width - 1 - 2, Height - 1);
						PrimitivePixel(framebuffer, 1, Height - 1 - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::t):
					case uint8_t(AsciiDefinition::Printable::T):
						PrimitiveLine(framebuffer, 0, 0, Width - 1, 0);
						PrimitiveLine(framebuffer, MiddleX, 0, MiddleX, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::u):
					case uint8_t(AsciiDefinition::Printable::U):
						PrimitiveLine(framebuffer, 0, 0, 0, Height - 1 - 2);
						PrimitivePixel(framebuffer, 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, 2, Height - 1, Width - 1 - 2, Height - 1);
						PrimitivePixel(framebuffer, Width - 1 - 1, Height - 1 - 1);
						PrimitiveLine(framebuffer, Width - 1, 0, Width - 1, Height - 1 - 2);
						break;
					case uint8_t(AsciiDefinition::Printable::v):
					case uint8_t(AsciiDefinition::Printable::V):
						PrimitiveLine(framebuffer, 0, 0, MiddleX - 1, Height - 1);
						PrimitivePixel(framebuffer, MiddleX, Height - 1);
						PrimitiveLine(framebuffer, MiddleX + 1, Height - 1, Width - 1, 0);
						break;
					case uint8_t(AsciiDefinition::Printable::w):
					case uint8_t(AsciiDefinition::Printable::W):
					{
						const pixel_t margin = Width - 1 / 3;
						PrimitiveLine(framebuffer, 0, 0, MiddleX - margin, Height - 1);
						PrimitiveLine(framebuffer, MiddleX - margin, Height - 1, MiddleX, 1);
						PrimitiveLine(framebuffer, MiddleX + margin, Height - 1, MiddleX, 1);
						PrimitiveLine(framebuffer, Width - 1 - 1, 0, MiddleX + margin, Height - 1);
						break;
					}
					case uint8_t(AsciiDefinition::Printable::x):
					case uint8_t(AsciiDefinition::Printable::X):
						PrimitiveLine(framebuffer, 0, 0, Width - 1, Height - 1);
						PrimitiveLine(framebuffer, 0, Height - 1, Width - 1, 0);
						break;
					case uint8_t(AsciiDefinition::Printable::y):
					case uint8_t(AsciiDefinition::Printable::Y):
						PrimitiveLine(framebuffer, 0, 0, MiddleX - 1, MiddleY - 1);
						PrimitiveLine(framebuffer, Width - 1, 0, MiddleX + 1, MiddleY - 1);
						PrimitiveLine(framebuffer, MiddleX, MiddleY, MiddleX, Height - 1);
						break;
					case uint8_t(AsciiDefinition::Printable::z):
					case uint8_t(AsciiDefinition::Printable::Z):
						PrimitiveLine(framebuffer, 0, 0, Width - 1, 0);
						PrimitiveLine(framebuffer, 1, Height - 1 - 1, Width - 1, 1);
						PrimitiveLine(framebuffer, 0, Height - 1, Width - 1, Height - 1);
						break;
					default:
						return 0; // No advance on no-draw.
					};

					// Monospaced font.
					return Width;
				}
			};
		}
	}
}
#endif