#ifndef _EGFX_MODULES_EGFX_LOGO_LAYOUT_h
#define _EGFX_MODULES_EGFX_LOGO_LAYOUT_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace EgfxLogo
		{
			namespace Layout
			{

				/*template<pixel_t FontWidth, pixel_t FontHeight>
				struct FontLayout
				{
					static constexpr uint8_t Margin() { return 1; }

					static constexpr pixel_t RowHeight()
					{
						return FontHeight / Dimensions::LineCount;
					}

					static constexpr pixel_t EffectiveHeight()
					{
						return RowHeight() * (Dimensions::LineCount - 1);
					}

					static constexpr uint8_t LinesPerRow() { return RowHeight() >= (3 + Margin()) ? 2 : 1; }
				};*/

				template<pixel_t FontWidth, pixel_t FontHeight>
				struct RetroLinesLayout
				{
					static constexpr pixel_t RowHeight()
					{
						return FontHeight / Dimensions::LineCount;
					}

					static constexpr uint8_t ScanlineMargin() { return 1 + (FontWidth / 3 / 3); }
					static constexpr uint8_t ExtraMargin() { return 0; }
					static constexpr uint8_t Margin() { return Dimensions::FontLayoutMargin(); }

					static constexpr uint8_t LinesPerRow() { return RowHeight() >= (3 + Margin()) ? 2 : 1; }

					static constexpr pixel_t LineHeight()
					{
						return RowHeight() / LinesPerRow();
					}

					static constexpr pixel_t LineMargin()
					{
						return 1 + (LineHeight() / 3);
					}

					static constexpr pixel_t GetLineYOffset(const uint8_t lineIndex)
					{
						return static_cast<pixel_t>(pixel_t(lineIndex) * LineHeight());
					}
				};

				template<pixel_t FontWidth, pixel_t FontHeight>
				struct DotMatrixLayout
				{
					static constexpr uint8_t Margin() { return Dimensions::FontLayoutMargin(); }

					static constexpr pixel_t RowHeight()
					{
						return FontHeight / Dimensions::LineCount;
					}

					static constexpr uint8_t SquaresPerRow() { return RowHeight() >= (3 + Margin()) ? 2 : 1; }


					static constexpr pixel_t SquareSize()
					{
						return (FontWidth / Dimensions::ColumnCount);
					}

					static constexpr pixel_t SquareVisibleSize()
					{
						return SquareSize() - (Margin() * 2);
					}

					static constexpr uint8_t SquaresPerColumn()
					{
						return Dimensions::LineCount / 2;
					}

					static constexpr uint8_t LinesPerRow() { return RowHeight() >= (3 + Margin()) ? 2 : 1; }
					//static constexpr uint8_t LinesPerRow() { return Dimensions::FontLayout<FontWidth, FontHeight>::LinesPerRow(); }


					static constexpr pixel_point_t GetSquareOffset(const uint8_t squareX, const uint8_t squareY)
					{
						return pixel_point_t{
							static_cast<pixel_t>(pixel_t(squareX) * SquareSize()),
							static_cast<pixel_t>(pixel_t(squareY) * SquareSize())
						};
					}
				};


				template<typename ParentLayout>
				struct LogoLayout
				{
					static constexpr pixel_t MinDimension() { return MinValue(ParentLayout::Width(), ParentLayout::Height()); }
					static constexpr pixel_t MaxDimension() { return MaxValue(ParentLayout::Width(), ParentLayout::Height()); }

					static constexpr pixel_t Kerning()
					{
						return 0 +
							((uint32_t(Dimensions::KerningRatio()) * ParentLayout::Width()) / UINT8_MAX);
					}

					static constexpr pixel_t MarginRaw()
					{
						return MinDimension() / 12;
					}

					static constexpr pixel_t Margin()
					{
						return (MinDimension() > MarginRaw()) ? MarginRaw() : 0;
					}

					static constexpr pixel_t Padding()
					{
						return (MinDimension() / 32) | 0b1;
					}

					using ScreenRatio = Framework::Layout::Align<ParentLayout, Framework::Layout::ConstrainedRatio<ParentLayout, Dimensions::LogoAspectWidth(), Dimensions::LogoAspectHeight()>>;
					using ScreenMargin = Framework::Layout::Margin<ScreenRatio, Margin(), Margin(), Margin(), Margin()>;
					using LogoPadding = Framework::Layout::Margin<ScreenMargin, Padding(), Padding(), Padding(), Padding()>;

					struct Logo
					{
						static constexpr pixel_t FontWidth()
						{
							return ((LogoPadding::Width() - Padding() * 2) - (Kerning() * (Dimensions::LetterCount() - 1))) / Dimensions::LetterCount();
						}

						static constexpr pixel_t FontHeight()
						{
							return LogoPadding::Height();
						}

						static constexpr pixel_t RowHeight()
						{
							return FontHeight() / Dimensions::LineCount;
						}

						static constexpr pixel_t Height()
						{
							return RowHeight() * Dimensions::LineCount;
						}

						static constexpr pixel_t Width()
						{
							return (uint32_t(FontWidth()) * (Dimensions::LetterCount()))
								+ (uint32_t(Kerning()) * (Dimensions::LetterCount() - 1));
						}

						static constexpr pixel_t X()
						{
							return LogoPadding::X() + ((LogoPadding::Width() - Width()) / 2);
						}

						static constexpr pixel_t Y()
						{
							return LogoPadding::Y() + ((LogoPadding::Height() - Height()) / 2);
						}
					};

					struct LetterE
					{
						static constexpr pixel_t X() { return Logo::X(); }
						static constexpr pixel_t Y() { return Logo::Y(); }
						static constexpr pixel_t Width() { return Logo::FontWidth(); }
						static constexpr pixel_t Height() { return Logo::FontHeight(); }
					};

					struct LetterG
					{
						static constexpr pixel_t X() { return LetterE::X() + Logo::FontWidth() + Kerning(); }
						static constexpr pixel_t Y() { return Logo::Y(); }
						static constexpr pixel_t Width() { return Logo::FontWidth(); }
						static constexpr pixel_t Height() { return Logo::FontHeight(); }
					};

					struct LetterF
					{
						static constexpr pixel_t X() { return LetterG::X() + Logo::FontWidth() + Kerning(); }
						static constexpr pixel_t Y() { return Logo::Y(); }
						static constexpr pixel_t Width() { return Logo::FontWidth(); }
						static constexpr pixel_t Height() { return Logo::FontHeight(); }

						static constexpr pixel_t SquareSize()
						{
							return (Width() / Dimensions::ColumnCount);
						}
					};

					struct LetterX
					{
						static constexpr pixel_t X() { return LetterF::X() + Logo::FontWidth() + Kerning(); }
						static constexpr pixel_t Y() { return Logo::Y(); }
						static constexpr pixel_t Width() { return Logo::FontWidth(); }
						static constexpr pixel_t Height() { return Logo::FontHeight(); }

						static constexpr pixel_t SquareSize()
						{
							return (Width() / Dimensions::ColumnCount);
						}
					};
				};
			}
		}
	}
}
#endif