#ifndef _DEMO_VECTOR_TEXT_h
#define _DEMO_VECTOR_TEXT_h

#include <IntegerGlassModules.h>

#include "../IDemoPage.h"
#include "../Constants.h"

namespace Showcase
{
	using namespace IntegerGlass;

	namespace VectorText
	{
		using FontType = Modules::Kit::Font::Vector::Epoxy::FontType8x8;

		using GlyphStyle = Framework::Image::TemplateImageStyle<
			Framework::Layout::AlignmentEnum::MiddleCenter>;

		template<typename ParentLayout, bool Monochrome = false>
		class View : public IDemoPage, public Framework::Text::Vector::TextView<
			typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, ParentLayout,
			FontType, GlyphStyle
		>
		{
		private:
			using Base = Framework::Text::Vector::TextView<
				typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, ParentLayout,
				FontType, GlyphStyle>;

			using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;
			using SizeAnimator = Framework::Animation::Animator<dimension_t>;

			static constexpr dimension_t MinimumSize = MinValue<dimension_t>(
				FontType::Width, FontType::Height);
			static constexpr dimension_t MaximumSize = MaxValue<dimension_t>(
				MinimumSize, MinValue<dimension_t>(ParentLayout::Width(), ParentLayout::Height()) / 4);
			static constexpr dimension_t TextWidth = static_cast<dimension_t>(
				(sizeof(Constants::Strings::FoxDoge) - 1) * FontType::Width);

			SizeAnimator Animator;

		public:
			View() : IDemoPage(), Base()
			{
				auto& drawable = Base::Drawable();
				drawable.SetText(Constants::Strings::FoxDoge);
				Animator.Start(MinimumSize, MaximumSize, Constants::Text::VectorSizePeriod);
			}

			~View() = default;

			const char* GetPageName() const override { return "Vector Text"; }

		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
			{
				const dimension_t size = Animator.Step(frameTime).Value;
				if (!Animator.IsAnimating())
				{
					Animator.Start(size == MaximumSize ? MaximumSize : MinimumSize,
						size == MaximumSize ? MinimumSize : MaximumSize, Constants::Text::VectorSizePeriod);
				}

				auto& drawable = Base::Drawable();
				drawable.SetFontSize(size, size, false); // Font canvas aspect ratio is 1:1, so width and height are the same.
				drawable.SetKerningWidth(1 + (size / 8), false); // Pass last parameter as false to avoid refreshing layout until all setters are called.
				drawable.SetLineSpacing(1 + (size / 9), false);

				const auto scrollProgress = UFraction16::GetScalar<uint32_t>(frameTime % Constants::Text::ScrollPeriod, Constants::Text::ScrollPeriod);
				const dimension_t scrollRange = TextWidth + ParentLayout::Width();
				const int16_t offset = static_cast<int16_t>(ParentLayout::Width()) -
					UFraction16::Fraction<int16_t>(scrollProgress, static_cast<int16_t>(scrollRange));
				drawable.SetOffset(offset, 0, true);

				return true;
			}
		};
	}
}

#endif

