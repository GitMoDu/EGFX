#ifndef _EGFX_BITMASK_FONT_TEMPLATE_FONT_DRAWER_h
#define _EGFX_BITMASK_FONT_TEMPLATE_FONT_DRAWER_h

#include "Model.h"
#include "../Bitmask/TemplateReader.h"

namespace Egfx
{
	namespace BitmaskFont
	{
		template<typename BitmaskFontType,
			typename PrimitiveShaderType = VectorGraphics::Shaders::Primitive::SingleColor<typename BitmaskFontType::dimension_t>
		>
		class TemplateFontDrawer : public PrimitiveShaderType
		{
		public:
			using dimension_t = typename BitmaskFontType::dimension_t;
			using PixelReader = Bitmask::TemplateReader<dimension_t, BitmaskFontType::Width>;

		public:
			using PrimitiveShaderType::PrimitivePrepare;
			using PrimitiveShaderType::PrimitivePixel;

		protected:
			dimension_t TabWidth;
			dimension_t SpaceWidth;
			dimension_t KerningWidth;

		public:
			TemplateFontDrawer(const dimension_t tabWidth = 5,
				const dimension_t spaceWidth = 3,
				const dimension_t kerning = 1)
				: PrimitiveShaderType()
				, TabWidth(tabWidth), SpaceWidth(spaceWidth), KerningWidth(kerning)
			{
			}

			~TemplateFontDrawer() = default;

			pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
			{
				const uint8_t* bitmask = BitmaskFontType::GetBitmask(printableChar);
				if (bitmask != nullptr)
				{
					PrimitivePrepare(x, y);
					for (dimension_t row = 0; row < GetFontHeight(); row++)
					{
						for (dimension_t col = 0; col < GetFontWidth(); col++)
						{
							if (PixelReader::IsPixelSet(bitmask, col, row))
							{
								PrimitivePixel(framebuffer, col, row);
							}
						}
					}

					return GetFontWidth();
				}
				else
				{
					return 0;
				}
			}

			// Get only character advance.
			dimension_t Advance(const char printableChar) const
			{
				const uint8_t* bitmask = BitmaskFontType::GetBitmask(printableChar);
				if (bitmask != nullptr)
				{
					return GetFontWidth();
				}
				else
				{
					return 0;
				}
			}

			/// <summary>
			/// Gets the pixel width of a space character.
			/// </summary>
			dimension_t GetSpaceWidth() const
			{
				return SpaceWidth;
			}

			/// <summary>
			/// Gets the pixel width of a tab character.
			/// </summary>
			dimension_t GetTabWidth() const
			{
				return TabWidth;
			}

			/// <summary>
			/// Gets the kerning pixel width.
			/// </summary>
			dimension_t GetKerningWidth() const
			{
				return KerningWidth;
			}

			void SetKerningWidth(const dimension_t kerning)
			{
				KerningWidth = kerning;
			}

			void SetSpaceWidth(const dimension_t spaceWidth)
			{
				SpaceWidth = spaceWidth;
			}

			void SetTabWidth(const dimension_t tabWidth)
			{
				TabWidth = tabWidth;
			}

			/// <summary>
			/// Gets the pixel width of the font.
			/// </summary>
			static constexpr dimension_t GetFontWidth()
			{
				return BitmaskFontType::Width;
			}

			/// <summary>
			/// Gets the pixel height of the font.
			/// </summary>
			static constexpr dimension_t GetFontHeight()
			{
				return BitmaskFontType::Height;
			}
		};

		template<typename BitmaskFontType,
			uint8_t ScaleX,
			uint8_t ScaleY = ScaleX,
			typename PrimitiveShaderType = VectorGraphics::Shaders::Primitive::SingleColor<typename BitmaskFontType::dimension_t>
		>
		class TemplateScaledFontDrawer : public PrimitiveShaderType
		{
		public:
			using dimension_t = typename BitmaskFontType::dimension_t;
			using PixelReader = Bitmask::TemplateReader<dimension_t, BitmaskFontType::Width>;

		public:
			using PrimitiveShaderType::PrimitivePrepare;
			using PrimitiveShaderType::PrimitivePixel;
			using PrimitiveShaderType::PrimitiveRectangleFill;

		protected:
			dimension_t TabWidth;
			dimension_t SpaceWidth;
			dimension_t KerningWidth;

		public:
			TemplateScaledFontDrawer(const dimension_t tabWidth = 5,
				const dimension_t spaceWidth = 3,
				const dimension_t kerning = 1)
				: PrimitiveShaderType()
				, TabWidth(tabWidth), SpaceWidth(spaceWidth), KerningWidth(kerning)
			{
			}

			~TemplateScaledFontDrawer() = default;

			pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
			{
				const uint8_t* bitmask = BitmaskFontType::GetBitmask(printableChar);
				if (bitmask != nullptr)
				{
					PrimitivePrepare(x, y);
					for (dimension_t row = 0; row < BitmaskFontType::Height; row++)
					{
						const pixel_t y1 = row * ScaleY;

						for (dimension_t col = 0; col < BitmaskFontType::Width; col++)
						{
							if (PixelReader::IsPixelSet(bitmask, col, row))
							{
								const pixel_t x1 = col * ScaleX;

								PrimitiveRectangleFill(framebuffer,
									x1, y1, x1 + ScaleX - 1, y1 + ScaleY - 1);
							}
						}
					}

					return GetFontWidth();
				}
				else
				{
					return 0;
				}
			}

			// Get only character advance.
			dimension_t Advance(const char printableChar) const
			{
				const uint8_t* bitmask = BitmaskFontType::GetBitmask(printableChar);
				if (bitmask != nullptr)
				{
					return GetFontWidth();
				}
				else
				{
					return 0;
				}
			}

			/// <summary>
			/// Gets the pixel width of a space character.
			/// </summary>
			dimension_t GetSpaceWidth() const
			{
				return SpaceWidth;
			}

			/// <summary>
			/// Gets the pixel width of a tab character.
			/// </summary>
			dimension_t GetTabWidth() const
			{
				return TabWidth;
			}

			/// <summary>
			/// Gets the kerning pixel width.
			/// </summary>
			dimension_t GetKerningWidth() const
			{
				return KerningWidth;
			}

			void SetKerningWidth(const dimension_t kerning)
			{
				KerningWidth = kerning * ScaleX;
			}

			void SetSpaceWidth(const dimension_t spaceWidth)
			{
				SpaceWidth = spaceWidth * ScaleX;
			}

			void SetTabWidth(const dimension_t tabWidth)
			{
				TabWidth = tabWidth * ScaleX;
			}

			/// <summary>
			/// Gets the pixel width of the font.
			/// </summary>
			static constexpr dimension_t GetFontWidth()
			{
				return BitmaskFontType::Width * ScaleX;
			}

			/// <summary>
			/// Gets the pixel height of the font.
			/// </summary>
			static constexpr dimension_t GetFontHeight()
			{
				return BitmaskFontType::Height * ScaleY;
			}
		};
	}
}
#endif