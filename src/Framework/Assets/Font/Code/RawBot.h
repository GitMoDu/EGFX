#ifndef _EGFX_FRAMEWORK_ASSETS_FONT_CODE_RAW_BOT_h
#define _EGFX_FRAMEWORK_ASSETS_FONT_CODE_RAW_BOT_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Font
			{
				namespace Code
				{
					namespace AsciiDefinition = Framework::AsciiDefinition;

					template<uint8_t DefaultFontWidth = 5,
						uint8_t DefaultFontHeight = 7,
						uint8_t DefaultKerningWidth = 2,
						typename PrimitiveShaderType = Framework::Shader::Primitive::TemplateShader<pixel_t>
					>
					class RawBot : public Framework::Code::Font::AbstractDrawer<pixel_t, PrimitiveShaderType>
					{
					private:
						using Base = Framework::Code::Font::AbstractDrawer<pixel_t, PrimitiveShaderType>;

					public:
						using dimension_t = pixel_t;

					public:
						using Base::Prepare;
						using Base::Pixel;
						using Base::Line;

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
							Base::Prepare(x, y);

							switch (static_cast<uint8_t>(printableChar))
							{
							case uint8_t(AsciiDefinition::Printable::Digit0):
								Base::Line(framebuffer, 1, 0, Width - 1, 0);
								Base::Line(framebuffer, 1, Height - 1, Width - 1, Height - 1);
								Base::Line(framebuffer, 0, 1, 0, Height - 1);
								Base::Line(framebuffer, Width - 1, 1, Width - 1, Height - 1);
								Base::Line(framebuffer, 2, Height - 1, Width - 1, 1);
								break;
							case uint8_t(AsciiDefinition::Printable::Digit1):
								if (Width - 1 > 2)
								{
									Base::Line(framebuffer, 0, 1, MiddleX, 0);
									Base::Line(framebuffer, MiddleX, 0, MiddleX, Height - 1);
									Base::Line(framebuffer, 1, Height - 1, Width - 1, Height - 1);
								}
								else
								{
									Base::Line(framebuffer, MiddleX, 0, MiddleX, Height - 1);
									Base::Line(framebuffer, 0, Height - 1, Width - 1, Height - 1);
								}
								break;
							case uint8_t(AsciiDefinition::Printable::Digit2):
								Base::Line(framebuffer, 0, 0, Width - 1, 0);
								Base::Line(framebuffer, Width - 1, 1, Width - 1, MiddleY - 2);
								Base::Line(framebuffer, 0, Height - 1, Width - 1, MiddleY - 1);
								Base::Line(framebuffer, 0, Height - 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::Digit3):
								Base::Line(framebuffer, Width - 1, 1, Width - 1, Height - 1);
								Base::Line(framebuffer, 0, 0, Width - 1, 0);
								Base::Line(framebuffer, MiddleX, MiddleY - 1, Width - 1, MiddleY - 1);
								Base::Line(framebuffer, 0, Height - 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::Digit4):
								Base::Line(framebuffer, Width - 1, 0, Width - 1, Height - 1);
								Base::Line(framebuffer, 0, MiddleY + 1, Width - 1, MiddleY + 1);
								if (Width - 1 > 7)
								{
									const pixel_t referenceX = x - 1 + (MiddleX / 2);
									Base::Line(framebuffer, referenceX - x, 0, 0, MiddleY + 1);
								}
								else
								{
									Base::Line(framebuffer, 1, 0, 0, MiddleY + 1);
								}
								break;
							case uint8_t(AsciiDefinition::Printable::Digit5):
								Base::Line(framebuffer, 0, 0, Width - 1, 0);
								Base::Line(framebuffer, 0, 1, 0, MiddleY - 1);
								Base::Line(framebuffer, 1, MiddleY - 1, Width - 1, MiddleY - 1);
								Base::Line(framebuffer, Width - 1, MiddleY, Width - 1, Height - 1);
								Base::Line(framebuffer, 0, Height - 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::Digit6):
								Base::Line(framebuffer, Width - 1, 0, 0, 0);
								Base::Line(framebuffer, 0, 1, 0, Height - 1);
								Base::Line(framebuffer, 1, MiddleY - 1, Width - 1, MiddleY - 1);
								Base::Line(framebuffer, Width - 1, MiddleY, Width - 1, Height - 1);
								Base::Line(framebuffer, 0, Height - 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::Digit7):
								Base::Line(framebuffer, 0, 0, Width - 1, 0);
								Base::Line(framebuffer, Width - 1, 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::Digit8):
								Base::Line(framebuffer, 1, 0, Width - 1, 0);
								Base::Line(framebuffer, 1, Height - 1, Width - 1, Height - 1);
								Base::Line(framebuffer, 0, 1, 0, Height - 1);
								Base::Line(framebuffer, Width - 1, 1, Width - 1, Height - 1);
								Base::Line(framebuffer, 1, MiddleY - 1, Width - 1, MiddleY - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::Digit9):
								Base::Line(framebuffer, 1, 0, Width - 1, 0);
								Base::Line(framebuffer, 0, 1, 0, MiddleY - 1);
								Base::Line(framebuffer, 1, MiddleY - 1, Width - 1, MiddleY - 1);
								Base::Line(framebuffer, Width - 1, 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::Minus):
								Base::Line(framebuffer, 0, MiddleY, Width - 1, MiddleY);
								break;
							case uint8_t(AsciiDefinition::Printable::a):
							case uint8_t(AsciiDefinition::Printable::A):
							{
								const pixel_t margin = MiddleX / 2;
								Base::Line(framebuffer, MiddleX - 1, 0, 0, Height - 1);
								Base::Pixel(framebuffer, MiddleX, 0);
								Base::Line(framebuffer, MiddleX + 1, 0, Width - 1, Height - 1);
								Base::Line(framebuffer, margin, 1, Width - 1 - margin, 1);
								break;
							}
							case uint8_t(AsciiDefinition::Printable::b):
							case uint8_t(AsciiDefinition::Printable::B):
								Base::Line(framebuffer, 0, 0, 0, Height - 1);
								Base::Line(framebuffer, 0, 0, Width - 1 - 2, 0);
								Base::Pixel(framebuffer, Width - 1, 1);
								Base::Line(framebuffer, Width - 1, 2, Width - 1, MiddleY - 2);
								Base::Pixel(framebuffer, Width - 1, MiddleY - 1);
								Base::Line(framebuffer, 1, MiddleY, Width - 1 - 2, MiddleY);
								Base::Pixel(framebuffer, Width - 1, MiddleY + 1);
								Base::Line(framebuffer, Width - 1, MiddleY + 2, Width - 1, Height - 1 - 2);
								Base::Pixel(framebuffer, Width - 1, Height - 1);
								Base::Line(framebuffer, 1, Height - 1, Width - 1 - 2, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::c):
							case uint8_t(AsciiDefinition::Printable::C):
								Base::Line(framebuffer, 2, 0, Width - 1, 0);
								Base::Pixel(framebuffer, 1, 1);
								Base::Line(framebuffer, 0, 2, 0, Height - 1 - 2);
								Base::Pixel(framebuffer, 1, Height - 1);
								Base::Line(framebuffer, 2, Height - 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::d):
							case uint8_t(AsciiDefinition::Printable::D):
								Base::Line(framebuffer, 1, 0, Width - 1 - 2, 0);
								Base::Pixel(framebuffer, Width - 1, 1);
								Base::Line(framebuffer, 0, 0, 0, Height - 1);
								Base::Line(framebuffer, Width - 1, 2, Width - 1, Height - 1 - 2);
								Base::Pixel(framebuffer, Width - 1, Height - 1);
								Base::Line(framebuffer, 1, Height - 1, Width - 1 - 2, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::e):
							case uint8_t(AsciiDefinition::Printable::E):
								Base::Line(framebuffer, 0, 0, 0, Height - 1);
								Base::Line(framebuffer, 1, 0, Width - 1, 0);
								Base::Line(framebuffer, 0, MiddleY, Width - 1 - 2, MiddleY);
								Base::Line(framebuffer, 1, Height - 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::f):
							case uint8_t(AsciiDefinition::Printable::F):
								Base::Line(framebuffer, 0, 0, 0, Height - 1);
								Base::Line(framebuffer, 0, 0, Width - 1, 0);
								Base::Line(framebuffer, 0, MiddleY, MiddleX, MiddleY);
								break;
							case uint8_t(AsciiDefinition::Printable::g):
							case uint8_t(AsciiDefinition::Printable::G):
								Base::Line(framebuffer, 2, 0, Width - 1, 0);
								Base::Pixel(framebuffer, Width - 1, 1);
								Base::Pixel(framebuffer, 1, 1);
								Base::Line(framebuffer, 0, 2, 0, Height - 1 - 2);
								Base::Pixel(framebuffer, 1, Height - 1);
								Base::Line(framebuffer, 2, Height - 1, Width - 1 - 2, Height - 1);
								Base::Pixel(framebuffer, Width - 1, Height - 1);
								Base::Line(framebuffer, Width - 1, MiddleY, Width - 1, Height - 1 - 2);
								Base::Line(framebuffer, MiddleX + 1, MiddleY, Width - 1, MiddleY);
								break;
							case uint8_t(AsciiDefinition::Printable::h):
							case uint8_t(AsciiDefinition::Printable::H):
								Base::Line(framebuffer, 0, 0, 0, Height - 1);
								Base::Line(framebuffer, 1, MiddleY, Width - 1, MiddleY);
								Base::Line(framebuffer, Width - 1, 0, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::i):
							case uint8_t(AsciiDefinition::Printable::I):
								Base::Line(framebuffer, 1, 0, Width - 1, 0);
								Base::Line(framebuffer, MiddleX, 1, MiddleX, Height - 1);
								Base::Line(framebuffer, 1, Height - 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::j):
							case uint8_t(AsciiDefinition::Printable::J):
								Base::Line(framebuffer, Width - 1, 0, Width - 1, Height - 1 - 2);
								Base::Line(framebuffer, 0, MiddleY, 0, Height - 1 - 2);
								Base::Pixel(framebuffer, 1, Height - 1);
								Base::Pixel(framebuffer, Width - 1, Height - 1);
								Base::Line(framebuffer, 2, Height - 1, Width - 1 - 2, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::k):
							case uint8_t(AsciiDefinition::Printable::K):
								Base::Line(framebuffer, 0, 0, 0, Height - 1);
								Base::Line(framebuffer, 0, MiddleY, Width - 1, 0);
								Base::Line(framebuffer, 1, MiddleY, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::l):
							case uint8_t(AsciiDefinition::Printable::L):
								Base::Line(framebuffer, 0, 0, 0, Height - 1);
								Base::Line(framebuffer, 0, Height - 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::m):
							case uint8_t(AsciiDefinition::Printable::M):
								Base::Line(framebuffer, 0, 1, 0, Height - 1);
								Base::Line(framebuffer, Width - 1, 1, Width - 1, Height - 1);
								Base::Line(framebuffer, 0, 0, MiddleX, Height - 1);
								Base::Line(framebuffer, MiddleX + 1, Height - 1, Width - 1, 0);
								break;
							case uint8_t(AsciiDefinition::Printable::n):
							case uint8_t(AsciiDefinition::Printable::N):
								Base::Line(framebuffer, 0, 0, 0, Height - 1);
								Base::Line(framebuffer, Width - 1, 0, Width - 1, Height - 1);
								Base::Line(framebuffer, 1, 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::o):
							case uint8_t(AsciiDefinition::Printable::O):
								Base::Pixel(framebuffer, 1, 1);
								Base::Line(framebuffer, 2, 0, Width - 1 - 2, 0);
								Base::Pixel(framebuffer, Width - 1, 1);
								Base::Line(framebuffer, 0, 2, 0, Height - 1 - 2);
								Base::Pixel(framebuffer, 1, Height - 1);
								Base::Line(framebuffer, Width - 1, 2, Width - 1, Height - 1 - 2);
								Base::Pixel(framebuffer, Width - 1, Height - 1);
								Base::Line(framebuffer, 2, Height - 1, Width - 1 - 2, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::p):
							case uint8_t(AsciiDefinition::Printable::P):
								Base::Line(framebuffer, 0, 0, 0, Height - 1);
								Base::Line(framebuffer, 1, 0, Width - 1 - 2, 0);
								Base::Pixel(framebuffer, Width - 1, 1);
								Base::Line(framebuffer, 2, 0, Width - 1 - 2, 0);
								Base::Line(framebuffer, Width - 1, 2, Width - 1, MiddleY - 1);
								Base::Pixel(framebuffer, Width - 1, MiddleY);
								Base::Line(framebuffer, 1, MiddleY + 1, Width - 1 - 2, MiddleY + 1);
								break;
							case uint8_t(AsciiDefinition::Printable::q):
							case uint8_t(AsciiDefinition::Printable::Q):
								Base::Pixel(framebuffer, 1, 1);
								Base::Line(framebuffer, 2, 0, Width - 1 - 2, 0);
								Base::Pixel(framebuffer, Width - 1, 1);
								Base::Line(framebuffer, 0, 2, 0, Height - 1 - 3);
								Base::Pixel(framebuffer, 1, Height - 1 - 2);
								Base::Line(framebuffer, Width - 1, 2, Width - 1, Height - 1 - 3);
								Base::Pixel(framebuffer, Width - 1, Height - 1 - 2);
								Base::Line(framebuffer, 2, Height - 1, Width - 1 - 2, Height - 1);
								Base::Line(framebuffer, MiddleX, Height - 1 - 3, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::r):
							case uint8_t(AsciiDefinition::Printable::R):
								Base::Line(framebuffer, 0, 0, 0, Height - 1);
								Base::Line(framebuffer, 1, 0, Width - 1 - 2, 0);
								Base::Pixel(framebuffer, Width - 1, 1);
								Base::Line(framebuffer, 2, 0, Width - 1 - 2, 0);
								Base::Line(framebuffer, Width - 1, 2, Width - 1, MiddleY - 1);
								Base::Pixel(framebuffer, Width - 1, MiddleY);
								Base::Line(framebuffer, 1, MiddleY + 1, Width - 1 - 2, MiddleY + 1);
								Base::Line(framebuffer, MiddleX - 1, MiddleY + 1, Width - 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::s):
							case uint8_t(AsciiDefinition::Printable::S):
								Base::Line(framebuffer, 2, 0, Width - 1 - 2, 0);
								Base::Pixel(framebuffer, Width - 1, 1);
								Base::Pixel(framebuffer, 1, 1);
								Base::Line(framebuffer, 0, 2, 0, MiddleY - 2);
								Base::Pixel(framebuffer, 1, MiddleY - 1);
								Base::Line(framebuffer, 2, MiddleY, Width - 1 - 2, MiddleY);
								Base::Line(framebuffer, Width - 1, MiddleY + 2, Width - 1, Height - 1 - 2);
								Base::Pixel(framebuffer, Width - 1, MiddleY + 1);
								Base::Pixel(framebuffer, Width - 1, Height - 1);
								Base::Line(framebuffer, 2, Height - 1, Width - 1 - 2, Height - 1);
								Base::Pixel(framebuffer, 1, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::t):
							case uint8_t(AsciiDefinition::Printable::T):
								Base::Line(framebuffer, 0, 0, Width - 1, 0);
								Base::Line(framebuffer, MiddleX, 0, MiddleX, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::u):
							case uint8_t(AsciiDefinition::Printable::U):
								Base::Line(framebuffer, 0, 0, 0, Height - 1 - 2);
								Base::Pixel(framebuffer, 1, Height - 1);
								Base::Line(framebuffer, 2, Height - 1, Width - 1 - 2, Height - 1);
								Base::Pixel(framebuffer, Width - 1, Height - 1);
								Base::Line(framebuffer, Width - 1, 0, Width - 1, Height - 1 - 2);
								break;
							case uint8_t(AsciiDefinition::Printable::v):
							case uint8_t(AsciiDefinition::Printable::V):
								Base::Line(framebuffer, 0, 0, MiddleX - 1, Height - 1);
								Base::Pixel(framebuffer, MiddleX, Height - 1);
								Base::Line(framebuffer, MiddleX + 1, Height - 1, Width - 1, 0);
								break;
							case uint8_t(AsciiDefinition::Printable::w):
							case uint8_t(AsciiDefinition::Printable::W):
							{
								const pixel_t margin = Width - 1 / 3;
								Base::Line(framebuffer, 0, 0, MiddleX - margin, Height - 1);
								Base::Line(framebuffer, MiddleX - margin, Height - 1, MiddleX, 1);
								Base::Line(framebuffer, MiddleX + margin, Height - 1, MiddleX, 1);
								Base::Line(framebuffer, Width - 1, 0, MiddleX + margin, Height - 1);
								break;
							}
							case uint8_t(AsciiDefinition::Printable::x):
							case uint8_t(AsciiDefinition::Printable::X):
								Base::Line(framebuffer, 0, 0, Width - 1, Height - 1);
								Base::Line(framebuffer, 0, Height - 1, Width - 1, 0);
								break;
							case uint8_t(AsciiDefinition::Printable::y):
							case uint8_t(AsciiDefinition::Printable::Y):
								Base::Line(framebuffer, 0, 0, MiddleX - 1, MiddleY - 1);
								Base::Line(framebuffer, Width - 1, 0, MiddleX + 1, MiddleY - 1);
								Base::Line(framebuffer, MiddleX, MiddleY, MiddleX, Height - 1);
								break;
							case uint8_t(AsciiDefinition::Printable::z):
							case uint8_t(AsciiDefinition::Printable::Z):
								Base::Line(framebuffer, 0, 0, Width - 1, 0);
								Base::Line(framebuffer, 1, Height - 1, Width - 1, 1);
								Base::Line(framebuffer, 0, Height - 1, Width - 1, Height - 1);
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
	}
}
#endif