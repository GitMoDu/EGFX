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
				template<typename ParentLayout>
				struct Calc
				{
					static constexpr int16_t MinDimension() { return MinValue(ParentLayout::Width(), ParentLayout::Height()); }

					static constexpr int16_t Margin()
					{
						return 1 + (MinDimension() / Dimensions::MarginDivisor());
					}

					using Margined = Framework::Layout::Margin<ParentLayout, Margin(), Margin(), Margin(), Margin()>;

					using Constrained = Framework::Layout::ConstrainedRatio<Margined, Dimensions::AspectWidth(), Dimensions::AspectHeight()>;

					using LogoLayout = Framework::Layout::Align<ParentLayout, Constrained>;

					static constexpr int16_t Kerning()
					{
						return 1 +
							((uint32_t(Dimensions::KerningRatio()) * Constrained::Width()) / UINT8_MAX);
					}

					static constexpr int16_t Padding()
					{
						return 1 + (Constrained::Height() / Dimensions::PaddingDivisor());
					}

					using PaddedLayout = Framework::Layout::Margin<LogoLayout, Padding(), Padding(), Padding(), Padding()>;

					static constexpr int16_t FontWidth()
					{
						return (PaddedLayout::Width() - (Kerning() * (Dimensions::LetterCount() - 1))) / Dimensions::LetterCount();
					}

					static constexpr int16_t FontHeight()
					{
						return PaddedLayout::Height();
					}

					using LettersLayout = Framework::Layout::Align<ParentLayout, PaddedLayout>;

					using SingleLetterLayout = Framework::Layout::Align<LettersLayout,
						Framework::Layout::ConstrainedDimensions<PaddedLayout, 1, FontWidth(), 1, FontHeight()>,
						Framework::Layout::AlignmentEnum::MiddleLeft
					>;

					template<uint8_t LetterIndex>
					using LetterLayout = Framework::Layout::Translate<SingleLetterLayout,
						(FontWidth() + Kerning())* LetterIndex
						, 0>;
				};

				template<typename ParentLayout>
				using LetterE = typename Calc<ParentLayout>::template LetterLayout<0>;

				template<typename ParentLayout>
				using LetterG = typename Calc<ParentLayout>::template LetterLayout<1>;

				template<typename ParentLayout>
				using LettersEG = Framework::Layout::Combine<LetterE<ParentLayout>, LetterG<ParentLayout>>;

				template<typename ParentLayout>
				using LetterF = typename Calc<ParentLayout>::template LetterLayout<2>;

				template<typename ParentLayout>
				using LetterX = typename Calc<ParentLayout>::template LetterLayout<3>;

				template<typename ParentLayout>
				using LettersFX = Framework::Layout::Combine<LetterF<ParentLayout>, LetterX<ParentLayout>>;

				namespace Letters
				{

					struct ScanLine
					{
						pixel_rectangle_t Line;
						pixel_rectangle_t Overscan;
					};

					template<int16_t FontWidth, int16_t FontHeight>
					struct SquareLetterLayout
					{
						static constexpr int16_t LineHeight = 1 + ((FontHeight / Dimensions::LineCount) / Dimensions::ScanlineDivisor());
						static constexpr int16_t OverscanMargin = FontWidth / Dimensions::ScanlineOverscanDivisor();
						static constexpr int16_t OverscanHeight = FontHeight / Dimensions::ScanlineOverscanHeightDivisor();

						static constexpr Egfx::pixel_rectangle_t Row(const uint8_t rowIndex)
						{
							return Egfx::pixel_rectangle_t{
								{0, (rowIndex * FontHeight) / Dimensions::LineCount},
								{FontWidth, ((rowIndex + 1) * FontHeight) / Dimensions::LineCount}
							};
						}

						static constexpr Egfx::pixel_rectangle_t Row(const uint8_t rowIndex, const uint8_t columnStartIndex, const uint8_t columnEndIndex)
						{
							return Egfx::pixel_rectangle_t{
								Egfx::pixel_point_t{
									static_cast<pixel_t>((columnStartIndex * FontWidth) / Dimensions::ColumnCount),
									static_cast<pixel_t>((rowIndex * FontHeight) / Dimensions::LineCount)},
								Egfx::pixel_point_t{
									static_cast<pixel_t>(((columnEndIndex + 1) * FontWidth) / Dimensions::ColumnCount),
									static_cast<pixel_t>(((rowIndex + 1) * FontHeight) / Dimensions::LineCount)}
							};
						}

						static constexpr Egfx::pixel_rectangle_t Square(const uint8_t rowIndex, const uint8_t columnIndex)
						{
							return Row(rowIndex, columnIndex, columnIndex + 1);
						}
					};


					template<int16_t FontWidth, int16_t FontHeight>
					struct ScanLineLetterLayout : SquareLetterLayout<FontWidth, FontHeight>
					{
						using Base = SquareLetterLayout<FontWidth, FontHeight>;
						using signed_wide_t = typename Framework::AutoDimension::ByDimension<pixel_t>::signed_wide_t;
						static constexpr int16_t SquareHeight() { return Base::Square(0, 0).bottomRight.y - Base::Square(0, 0).topLeft.y; }

						static ScanLine Row(const uint8_t rowIndex, const uint8_t columnStartIndex, const uint8_t columnEndIndex, const int16_t offsetX)
						{
							const int8_t offsetY = rowIndex > 0 ? 1 * (rowIndex - 1) : 0;

							const auto square = Base::Row(rowIndex, columnStartIndex, columnEndIndex);
							return ScanLine{
									{
										static_cast<pixel_t>(static_cast<signed_wide_t>(square.topLeft.x) + offsetX),
										static_cast<pixel_t>(static_cast<signed_wide_t>(square.topLeft.y) + offsetY),
										static_cast<pixel_t>(static_cast<signed_wide_t>(square.bottomRight.x) + offsetX - 1),
										static_cast<pixel_t>(static_cast<signed_wide_t>(square.topLeft.y) + offsetY + Base::LineHeight - 1)
									},
									{
										static_cast<pixel_t>(static_cast<signed_wide_t>(square.topLeft.x) + Base::OverscanMargin + offsetX),
										static_cast<pixel_t>(static_cast<signed_wide_t>(square.topLeft.y) + offsetY + Base::LineHeight),
										static_cast<pixel_t>(static_cast<signed_wide_t>(square.bottomRight.x) - Base::OverscanMargin + offsetX - 1),
										static_cast<pixel_t>(static_cast<signed_wide_t>(square.topLeft.y) + offsetY + Base::LineHeight + Base::OverscanHeight - 1)
									}
							};
						}
					};


					template<int16_t FontWidth, int16_t FontHeight>
					struct SquareDotLetterLayout : SquareLetterLayout<FontWidth, FontHeight>
					{
						using Base = SquareLetterLayout<FontWidth, FontHeight>;
						using signed_wide_t = typename Framework::AutoDimension::ByDimension<pixel_t>::signed_wide_t;
						static constexpr int16_t LineHeight = (FontHeight / Dimensions::LineCount) / 7;

						static pixel_rectangle_t Cell(const uint8_t rowIndex, const uint8_t columnIndex, const int16_t offsetX)
						{
							const int8_t offsetY = rowIndex > 0 ? 1 * (rowIndex - 1) : 0;

							const auto square = Base::Square(rowIndex, columnIndex);

							return pixel_rectangle_t{
									{static_cast<pixel_t>(static_cast<signed_wide_t>(square.topLeft.x) + offsetX),
									 static_cast<pixel_t>(static_cast<signed_wide_t>(square.topLeft.y) + offsetY)},
									{static_cast<pixel_t>(static_cast<signed_wide_t>(square.bottomRight.x) + offsetX),
									 static_cast<pixel_t>(static_cast<signed_wide_t>(square.bottomRight.y) + offsetY)}
							};
						}

						static constexpr int16_t CellSize()
						{
							return (Base::Square(0, 0).bottomRight.x - Base::Square(0, 0).topLeft.x);
						}

						static constexpr uint8_t DotCount()
						{
							return CellSize() >= 3 ? 2 : 1;
						}

						static constexpr int16_t DotSize()
						{
							return DotCount() > 1 ? ((CellSize() / 3) - 0) : 1;
						}

						static constexpr int16_t DotOffset()
						{
							return (CellSize() / 2) + 1;
						}
					};
				}
			}
		}
	}
}
#endif