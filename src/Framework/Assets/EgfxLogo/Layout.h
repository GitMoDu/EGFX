#ifndef _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_LAYOUT_h
#define _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_LAYOUT_h

#include <EgfxDrawer.h>
#include "Drawables.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace EgfxLogo
			{
				template<typename ParentLayout>
				struct Layout
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
						return MinDimension() / 8;
					}

					static constexpr pixel_t Margin()
					{
						return (int32_t(MinDimension()) - MarginRaw()) > int32_t(Dimensions::LogoMinHeight())
							? MarginRaw() : 0;
					}

					static constexpr pixel_t Padding()
					{
						return MaxValue<pixel_t>(2, Margin() / 8);
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

						static constexpr pixel_t Height()
						{
							return CodeFonts::LogoFontFamily::Dimensions::FontLayout<FontWidth(), FontHeight()>::EffectiveHeight();
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
							return (Width() / CodeFonts::LogoFontFamily::Dimensions::ColumnCount);
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
							return (Width() / CodeFonts::LogoFontFamily::Dimensions::ColumnCount);
						}
					};
				};
			}
		}
	}
}
#endif