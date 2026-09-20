#ifndef _DEMO_BITMASK_TEXT_h
#define _DEMO_BITMASK_TEXT_h

#include <IntegerGlassModules.h>

#include "../IDemoPage.h"
#include "../Constants.h"

namespace Showcase
{
	using namespace IntegerGlass;

	namespace BitmaskText
	{
		using FontType = Modules::Kit::Font::Bitmask::Contact::FontType5x6;

		template<typename ParentLayout>
		static constexpr uint8_t FontScale()
		{
			return MaxValue<uint8_t>(1, static_cast<uint8_t>(ParentLayout::Height() /
				(static_cast<uint16_t>(FontType::GetFontHeight()) * 4)));
		}

		template<typename ParentLayout>
		using GlyphStyle = Framework::Image::TemplateImageStyle<
			Framework::Layout::AlignmentEnum::MiddleLeft,
			true, RGB_COLOR_BLACK,
			FontScale<ParentLayout>(), FontScale<ParentLayout>(),
			Framework::Image::ScaledTransformOrderEnum::TransformThenScale>;

		template<typename ParentLayout, bool Monochrome = false>
		class View : public IDemoPage, public Framework::Text::Bitmask::TextView<
			typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, ParentLayout,
			FontType, GlyphStyle<ParentLayout>>
		{
		private:
			using Base = Framework::Text::Bitmask::TextView<	typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, ParentLayout, FontType, GlyphStyle<ParentLayout>>;
			static constexpr int16_t TextWidth = static_cast<int16_t>(
				(sizeof(Constants::Strings::FoxDoge) - 1) * FontType::GetFontWidth() * GlyphStyle<ParentLayout>::ScaleX);

		public:
			View() : IDemoPage(), Base()
			{
				auto& drawable = Base::Drawable();
				drawable.SetText(Constants::Strings::FoxDoge);
			}

			~View() = default;

			const char* GetPageName() const override { return "Bitmask Text"; }

		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
			{
				const auto progress = UFraction16::GetScalar<uint32_t>(frameTime % Constants::Text::ScrollPeriod, Constants::Text::ScrollPeriod);
				const int16_t range = TextWidth + static_cast<int16_t>(ParentLayout::Width());
				const int16_t offset = static_cast<int16_t>(ParentLayout::Width()) -
					UFraction16::Fraction<int16_t>(progress, range);
				Base::Drawable().SetOffset(offset, 0, true);
				return true;
			}
		};
	}
}

#endif

