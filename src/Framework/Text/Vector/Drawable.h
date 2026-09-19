#ifndef _EGFX_FRAMEWORK_TEXT_VECTOR_DRAWABLE_h
#define _EGFX_FRAMEWORK_TEXT_VECTOR_DRAWABLE_h

#include "Model.h"
#include "../TextWalker.h"
#include "../../Shader/Image/Vector/ImageShader.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Text
		{
			namespace Vector
			{
				namespace Drawable
				{
					using namespace IntegerSignal::FixedPoint::FactorScale;

					template<typename dimension_t,
						typename ParentLayout,
						typename FontType,
						typename GlyphStyle = Framework::Image::TemplateImageStyle<>,
						typename PaletteType = Framework::Shader::Image::Vector::StaticColorPalette<RGB_COLOR_WHITE>,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class Text : public Framework::Shader::Image::Vector::Image<dimension_t,
						PaletteType,
						ColorShaderType, TransformShaderType, BlendMode
					>
					{
					private:
						using Base = Framework::Shader::Image::Vector::Image<dimension_t,
							PaletteType,
							ColorShaderType, TransformShaderType, BlendMode>;

						using Source = FontType;

						using Walker = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							TemplateTextMonospaceWalker,
							TemplateTextWalker,
							Source::IsMonospace
						>::type;
						using signed_t = typename Walker::signed_t;
						using unsigned_t = typename Walker::unsigned_t;

					private:
						const char* TextData = nullptr;
						Framework::DataSourceTypeEnum TextDataSource = Framework::DataSourceTypeEnum::Ram;
						char SingleCharacterText[2] = { 0, 0 };

						TextWalkerMetrics WalkerMetrics{};

						signed_t LineStartX = 0;
						signed_t LineStartY = 0;
						signed_t OffsetX = 0;
						signed_t OffsetY = 0;

					public:
						using DrawableLayout = ParentLayout;
						using ColorPaletteType = PaletteType;

						Text()
							: Base(0, 0, ParentLayout::Width() - 1, ParentLayout::Height() - 1)
							, WalkerMetrics
							{
								Source::Width, Source::Height,
								4,
								1,
								1,
								1
							}
						{
							RefreshLayout();
						}
						~Text() = default;

						PaletteType& GetPalette() { return Base::GetPalette(); }

						void SetPaletteColor(const uint8_t colorIndex, const rgb_color_t color)
						{
							Base::SetPaletteColor(colorIndex, color);
						}

						void SetFontWidth(const dimension_t width, const bool refreshLayout = true)
						{
							WalkerMetrics.FontWidth = width;
							if (refreshLayout)
								RefreshLayout();
						}

						void SetFontHeight(const dimension_t height, const bool refreshLayout = true)
						{
							WalkerMetrics.FontHeight = height;
							if (refreshLayout)
								RefreshLayout();
						}

						void SetFontSize(const dimension_t width, const dimension_t height, const bool refreshLayout = true)
						{
							WalkerMetrics.FontWidth = width;
							WalkerMetrics.FontHeight = height;
							if (refreshLayout)
								RefreshLayout();
						}

						void SetTabWidth(const dimension_t width, const bool refreshLayout = true)
						{
							WalkerMetrics.TabWidth = width;
							if (refreshLayout)
								RefreshLayout();
						}

						void SetSpaceWidth(const dimension_t width, const bool refreshLayout = true)
						{
							WalkerMetrics.SpaceWidth = width;
							if (refreshLayout)
								RefreshLayout();
						}

						void SetKerningWidth(const dimension_t width, const bool refreshLayout = true)
						{
							WalkerMetrics.KerningWidth = width;
							if (refreshLayout)
								RefreshLayout();
						}

						void SetLineSpacing(const dimension_t spacing, const bool refreshLayout = true)
						{
							WalkerMetrics.LineSpacing = spacing;
							if (refreshLayout)
								RefreshLayout();
						}

						void SetText(const char* text, const bool refreshLayout = true)
						{
							TextData = text;
							TextDataSource = Framework::DataSourceTypeEnum::Ram;
							if (refreshLayout)
								RefreshLayout();
						}
#if defined(ARDUINO)
						void SetText(const __FlashStringHelper* text, const bool refreshLayout = true)
						{
							TextData = reinterpret_cast<const char*>(text);
							TextDataSource = Framework::DataSourceTypeEnum::Flash;
							if (refreshLayout)
								RefreshLayout();
						}
#endif
						void SetText(const char character, const bool refreshLayout = true)
						{
							SingleCharacterText[0] = character;
							SingleCharacterText[1] = static_cast<char>(AsciiDefinition::Control::NUL);
							TextData = SingleCharacterText;
							TextDataSource = Framework::DataSourceTypeEnum::Ram;
							if (refreshLayout)
								RefreshLayout();
						}
						void SetOffset(const signed_t x, const signed_t y, const bool refreshLayout = true)
						{
							OffsetX = x;
							OffsetY = y;
							if (refreshLayout)
								RefreshLayout();
						}

						void SetBounds(const dimension_t left, const dimension_t top,
							const dimension_t right, const dimension_t bottom)
						{
							Base::SetBounds(left, top, right, bottom);
						}

						void SetTranslation(const int16_t x, const int16_t y)
						{
							Base::SetTranslation(x, y);
						}

						dimension_t GetFontWidth() const { return WalkerMetrics.FontWidth; }
						dimension_t GetFontHeight() const { return WalkerMetrics.FontHeight; }
						dimension_t GetTabWidth() const { return WalkerMetrics.TabWidth; }
						dimension_t GetSpaceWidth() const { return WalkerMetrics.SpaceWidth; }
						dimension_t GetKerningWidth() const { return WalkerMetrics.KerningWidth; }
						dimension_t GetLineSpacing() const { return WalkerMetrics.LineSpacing; }
						signed_t GetOffsetX() const { return OffsetX; }
						signed_t GetOffsetY() const { return OffsetY; }


						void Draw(IFrameBuffer* frame)
						{
							if (TextData == nullptr)
								return;

							Base::Prepare(ParentLayout::X(), ParentLayout::Y());

							const Scale16::factor_t scaleX = Scale16::GetFactor<dimension_t>(WalkerMetrics.FontWidth, Source::Width);

							Walker::WalkText(TextData, WalkerMetrics, LineStartX, LineStartY, [this, frame, scaleX](const char character, const signed_t x, const signed_t y, const bool)
								{
									const auto glyph = Source::GetGlyph(static_cast<uint8_t>(character));
									const dimension_t glyphWidth = Scale16::Scale<dimension_t>(scaleX, glyph.Width);
									if (glyphWidth < 1)
									{
										return static_cast<unsigned_t>(0);
									}

									if (!(x > Base::GetBoundsRight()
										|| x + glyphWidth < Base::GetBoundsLeft()
										|| y > Base::GetBoundsBottom()
										|| y + WalkerMetrics.FontHeight < Base::GetBoundsTop()))
									{
										Base::template Draw<Source::VectorType, Source::DataSourceType>(frame, static_cast<const void*>(glyph.Nodes), glyph.Count,
											Source::Width, Source::Height,
											Vector::ThicknessScale,
											WalkerMetrics.FontWidth, WalkerMetrics.FontHeight, x, y, Source::Inset);
									}

									return static_cast<unsigned_t>(glyphWidth);
								}, TextDataSource);
						}

						void RefreshLayout()
						{
							const Scale16::factor_t scaleX = Scale16::GetFactor<dimension_t>(WalkerMetrics.FontWidth, Source::Width);
							signed_t width = 0;
							signed_t height = 0;
							if (TextData != nullptr)
							{
								Walker::WalkText(TextData, WalkerMetrics, 0, 0, [this, &width, &height, scaleX](const char character, const signed_t x, const signed_t y, const bool)
									{
										const signed_t glyphWidth = Scale16::Scale<dimension_t>(scaleX, Source::GetAdvance(character));
										const signed_t right = x + glyphWidth;
										const signed_t bottom = y + static_cast<signed_t>(WalkerMetrics.FontHeight);
										if (right > width)
											width = right;
										if (bottom > height)
											height = bottom;
										return static_cast<typename Walker::unsigned_t>(glyphWidth);
									}, TextDataSource);
							}
							LineStartX = static_cast<signed_t>(OffsetX);
							LineStartY = static_cast<signed_t>(OffsetY);
							LineStartX += GetAlignmentOffset(width, static_cast<signed_t>(ParentLayout::Width()), true);
							LineStartY += GetAlignmentOffset(height, static_cast<signed_t>(ParentLayout::Height()), false);
						}

					private:
						static signed_t GetAlignmentOffset(const signed_t content, const signed_t layout, const bool horizontal)
						{
							const auto alignment = GlyphStyle::Alignment;
							if (horizontal)
								return alignment == Framework::Layout::AlignmentEnum::TopRight
								|| alignment == Framework::Layout::AlignmentEnum::MiddleRight
								|| alignment == Framework::Layout::AlignmentEnum::BottomRight ? layout - content
								: alignment == Framework::Layout::AlignmentEnum::TopCenter
								|| alignment == Framework::Layout::AlignmentEnum::MiddleCenter
								|| alignment == Framework::Layout::AlignmentEnum::BottomCenter ? SignedRightShift(layout - content + 1, 1) : 0;

							return alignment == Framework::Layout::AlignmentEnum::BottomLeft
								|| alignment == Framework::Layout::AlignmentEnum::BottomCenter
								|| alignment == Framework::Layout::AlignmentEnum::BottomRight ? layout - content
								: alignment == Framework::Layout::AlignmentEnum::MiddleLeft
								|| alignment == Framework::Layout::AlignmentEnum::MiddleCenter
								|| alignment == Framework::Layout::AlignmentEnum::MiddleRight ? SignedRightShift(layout - content + 1, 1) : 0;
						}
					};

					template<typename dimension_t, typename ParentLayout, typename number_t, typename FontType,
						typename GlyphStyle = Framework::Image::TemplateImageStyle<>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						typename PaletteType = Framework::Shader::Image::Vector::StaticColorPalette<RGB_COLOR_WHITE>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class Number
					{
					private:
						using TextDrawerType = Text<dimension_t, ParentLayout,
							FontType, GlyphStyle, PaletteType,
							ColorShaderType, TransformShaderType, BlendMode>;
						using unsigned_number_t = typename IntegerSignal::TypeTraits::TypeSign::make_unsigned<number_t>::type;

						static constexpr size_t MaxCharacters =
							sizeof(number_t) * 3
							+ (IntegerSignal::TypeTraits::TypeSign::is_signed<number_t>::value ? 1 : 0)
							+ 1;

						TextDrawerType TextDrawer{};
						char TextData[MaxCharacters] = { '0', '\0' };

					public:
						using DrawableLayout = ParentLayout;

						Number()
						{
							TextDrawer.SetText(TextData, false);
						}

						~Number() = default;

						TextDrawerType& GetTextDrawer()
						{
							return TextDrawer;
						}

						PaletteType& GetPalette()
						{
							return TextDrawer.GetPalette();
						}

						void SetNumber(const number_t number)
						{
							size_t textIndex = 0;
							unsigned_number_t magnitude;

							if (IntegerSignal::TypeTraits::TypeSign::is_signed<number_t>::value && number < 0)
							{
								TextData[textIndex++] = static_cast<char>(AsciiDefinition::Printable::Minus);
								magnitude = static_cast<unsigned_number_t>(-(number + 1));
								++magnitude;
							}
							else
							{
								magnitude = static_cast<unsigned_number_t>(number);
							}

							size_t digitCount = 0;
							TemplateTextNumberWalker::Walk(magnitude,
								[this, &textIndex, &digitCount](const char character)
								{
									TextData[textIndex++] = character;
									++digitCount;
								});

							TextData[textIndex] = static_cast<char>(AsciiDefinition::Control::NUL);
							TextDrawer.SetText(TextData, true);
						}

						void SetNumber(const number_t number, const dimension_t offsetX, const dimension_t offsetY)
						{
							SetOffset(offsetX, offsetY);
							SetNumber(number);
						}

						void SetOffset(const dimension_t offsetX, const dimension_t offsetY)
						{
							TextDrawer.SetOffset(offsetX, offsetY, false);
						}

						void SetBounds(const dimension_t left, const dimension_t top,
							const dimension_t right, const dimension_t bottom)
						{
							TextDrawer.SetBounds(left, top, right, bottom);
						}

						void SetTranslation(const int16_t x, const int16_t y)
						{
							TextDrawer.SetTranslation(x, y);
						}

						dimension_t GetOffsetX() const { return TextDrawer.GetOffsetX(); }
						dimension_t GetOffsetY() const { return TextDrawer.GetOffsetY(); }

						void SetFontSize(const dimension_t width, const dimension_t height)
						{
							TextDrawer.SetFontSize(width, height);
						}

						void SetKerningWidth(const dimension_t width)
						{
							TextDrawer.SetKerningWidth(width);
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
}
#endif