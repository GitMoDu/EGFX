#ifndef _EGFX_MODULES_FPS_DISPLAY_LAYOUT_DRAWABLE_h
#define _EGFX_MODULES_FPS_DISPLAY_LAYOUT_DRAWABLE_h

#include "Constant.h"
#include "Layout.h"

namespace Egfx
{
	namespace Modules
	{
		namespace FpsDisplay
		{
			namespace Drawable
			{
				template<FpsDrawerPosition fpsDrawerPosition>
				struct TextStyle;

				template<>
				struct TextStyle<FpsDrawerPosition::TopLeft>
				{
					static constexpr Framework::Layout::AlignmentEnum Alignment = Framework::Layout::AlignmentEnum::TopLeft;
				};

				template<>
				struct TextStyle<FpsDrawerPosition::TopRight>
				{
					static constexpr Framework::Layout::AlignmentEnum Alignment = Framework::Layout::AlignmentEnum::TopRight;
				};

				template<>
				struct TextStyle<FpsDrawerPosition::BottomLeft>
				{
					static constexpr Framework::Layout::AlignmentEnum Alignment = Framework::Layout::AlignmentEnum::BottomLeft;
				};

				template<>
				struct TextStyle<FpsDrawerPosition::BottomRight>
				{
					static constexpr Framework::Layout::AlignmentEnum Alignment = Framework::Layout::AlignmentEnum::BottomRight;
				};

				template<typename Layout, FpsDrawerPosition fpsDrawerPosition = FpsDrawerPosition::TopRight>
				class FpsText
				{
				private:
					using dimension_t = typename Framework::AutoDimension::ByLayout<Layout>::dimension_t;

					static constexpr uint8_t TextScale = Definitions::TextScale<Layout>::Value;
					static constexpr uint8_t Margin = 1 + TextScale;
					static constexpr dimension_t KerningWidth = TextScale;
					static constexpr dimension_t SpaceWidth = 2 * TextScale;

					using MarginLayout = Framework::Layout::Margin<Layout, Margin, Margin, Margin, Margin>;

					using FontType = Modules::Kit::Font::Bitmask::Sealant::FontType4x5;
					using GlyphStyle = Framework::Image::TemplateImageStyle<
						TextStyle<fpsDrawerPosition>::Alignment,
						false,
						RGB_COLOR_BLACK,
						TextScale,
						TextScale>;

					using TextDrawable = Framework::Text::Bitmask::Drawable::Text<
						dimension_t, MarginLayout, FontType, GlyphStyle>;

					TextDrawable TextDrawer{};
					char TextData[8]{ ' ', ' ', ' ', ' ', 'F', 'P', 'S', static_cast<char>(Framework::AsciiDefinition::Control::NUL) };

				public:
					FpsText()
					{
						TextDrawer.SetKerningWidth(KerningWidth, false);
						TextDrawer.SetSpaceWidth(SpaceWidth, false);
						TextDrawer.SetText(TextData, true);
					}
					~FpsText() = default;

					void SetFrameRate(const uint16_t frameRate)
					{
						uint16_t value = MinValue<uint16_t>(frameRate, 999);
						for (uint8_t index = 0; index < 3; index++)
							TextData[index] = static_cast<char>(Framework::AsciiDefinition::Printable::Space);

						uint8_t index = 3;
						do
						{
							TextData[--index] = static_cast<char>('0' + (value % 10));
							value /= 10;
						} while (value > 0 && index > 0);

						TextDrawer.RefreshLayout();
					}

					void Draw(IFrameBuffer* frame)
					{
						TextDrawer.Draw(frame);
					}
				};
			}
		}
	}
}
#endif