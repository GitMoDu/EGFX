#ifndef _EGFX_FRAMEWORK_TEXT_BITMAP_DRAWABLE_h
#define _EGFX_FRAMEWORK_TEXT_BITMAP_DRAWABLE_h

#include "Model.h"
#include "../TextWalker.h"
#include "../../Shader/Image/Bitmap/ImageShader.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Text
		{
			namespace Bitmap
			{
				namespace Drawable
				{
					template<typename dimension_t, typename ParentLayout, typename GlyphSourceType,
						typename GlyphStyle = Framework::Image::TemplateImageStyle<>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace>
					class Text : public Shader::Image::Bitmap::ImageShader<
						dimension_t,
						typename GlyphSourceType::color_t, GlyphSourceType::DataSourceType,
						GlyphStyle::UseTransparency, GlyphStyle::TransparentColor, GlyphStyle::ScaleX, GlyphStyle::ScaleY,
						GlyphStyle::TransformOrder, ColorShaderType, TransformShaderType, BlendMode>

					{
					private:
						using Base = Shader::Image::Bitmap::ImageShader<
							dimension_t,
							typename GlyphSourceType::color_t, GlyphSourceType::DataSourceType,
							GlyphStyle::UseTransparency, GlyphStyle::TransparentColor, GlyphStyle::ScaleX, GlyphStyle::ScaleY,
							GlyphStyle::TransformOrder, ColorShaderType, TransformShaderType, BlendMode>;
						using Walker = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							TemplateTextMonospaceWalker,
							TemplateTextWalker,
							GlyphSourceType::IsMonospace
						>::type;
						using signed_t = typename Walker::signed_t;

					private:
						GlyphSourceType GlyphSource{};

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
						Text()
							: Base(0, 0, static_cast<dimension_t>(ParentLayout::Width()), static_cast<dimension_t>(ParentLayout::Height()))
							, WalkerMetrics{ static_cast<typename Walker::unsigned_t>(GlyphSourceType::GetFontWidth() * GlyphStyle::ScaleX), static_cast<typename Walker::unsigned_t>(GlyphSourceType::GetFontHeight() * GlyphStyle::ScaleY), static_cast<typename Walker::unsigned_t>(4 * GlyphStyle::ScaleX), static_cast<typename Walker::unsigned_t>(GlyphStyle::ScaleX), static_cast<typename Walker::unsigned_t>(GlyphStyle::ScaleX), 0 }
						{
							RefreshLayout();
						}
						~Text() = default;

						void SetFontSize(const dimension_t width, const dimension_t height, const bool refreshLayout = true)
						{
							WalkerMetrics.FontWidth = static_cast<typename Walker::unsigned_t>(width * GlyphStyle::ScaleX);
							WalkerMetrics.FontHeight = static_cast<typename Walker::unsigned_t>(height * GlyphStyle::ScaleY);
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetTabWidth(const dimension_t width, const bool refreshLayout = true)
						{
							WalkerMetrics.TabWidth = static_cast<typename Walker::unsigned_t>(width * GlyphStyle::ScaleX);
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetSpaceWidth(const dimension_t width, const bool refreshLayout = true)
						{
							WalkerMetrics.SpaceWidth = static_cast<typename Walker::unsigned_t>(width * GlyphStyle::ScaleX);
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetKerningWidth(const dimension_t width, const bool refreshLayout = true)
						{
							WalkerMetrics.KerningWidth = static_cast<typename Walker::unsigned_t>(width * GlyphStyle::ScaleX);
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}
						void SetLineSpacing(const dimension_t spacing, const bool refreshLayout = true)
						{
							WalkerMetrics.LineSpacing = static_cast<typename Walker::unsigned_t>(spacing * GlyphStyle::ScaleY);
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}
						void SetText(const char* text, const bool refreshLayout = true)
						{
							TextData = text;
							TextDataSource = Framework::DataSourceTypeEnum::Ram;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}
#if defined(ARDUINO)
						void SetText(const __FlashStringHelper* text, const bool refreshLayout = true)
						{
							TextData = reinterpret_cast<const char*>(text);
							TextDataSource = Framework::DataSourceTypeEnum::Flash;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}
#endif
						void SetText(const char character, const bool refreshLayout = true)
						{
							SingleCharacterText[0] = character;
							SingleCharacterText[1] = static_cast<char>(AsciiDefinition::Control::NUL);
							TextData = SingleCharacterText;
							TextDataSource = Framework::DataSourceTypeEnum::Ram;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetText(const char* text, const dimension_t offsetX, const dimension_t offsetY, const bool refreshLayout = true)
						{
							TextData = text;
							TextDataSource = Framework::DataSourceTypeEnum::Ram;
							SetOffset(offsetX, offsetY);
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetOffset(const signed_t offsetX, const signed_t offsetY, const bool refreshLayout = true)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						signed_t GetOffsetX() const { return OffsetX; }
						signed_t GetOffsetY() const { return OffsetY; }

						void Draw(IFrameBuffer* frame)
						{
							if (TextData == nullptr)
								return;

							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							Walker::WalkText(TextData, WalkerMetrics, LineStartX, LineStartY, [this, frame](const char character, const signed_t cursorX, const signed_t cursorY, const bool)
								{
									const auto glyph = GlyphSourceType::GetGlyph(static_cast<uint8_t>(character));
									Base::Draw(frame, glyph.BitmapData, glyph.Width, glyph.Height, cursorX, cursorY);
									return static_cast<typename Walker::unsigned_t>(glyph.Width * GlyphStyle::ScaleX);
								}, TextDataSource);
						}

					private:
						void RefreshLayout()
						{
							if (TextData == nullptr)
								return;

							signed_t textWidth = 0;
							signed_t textHeight = 0;
							Walker::WalkText(TextData, WalkerMetrics, 0, 0, [this, &textWidth, &textHeight](const char character, const signed_t cursorX, const signed_t cursorY, const bool)
								{
									const auto glyph = GlyphSourceType::GetGlyph(static_cast<uint8_t>(character));
									const signed_t glyphWidth = static_cast<signed_t>(glyph.Width * GlyphStyle::ScaleX);
									const signed_t glyphHeight = static_cast<signed_t>(glyph.Height * GlyphStyle::ScaleY);
									const signed_t right = cursorX + glyphWidth;
									const signed_t bottom = cursorY + glyphHeight;
									if (right > textWidth) textWidth = right;
									if (bottom > textHeight) textHeight = bottom;
									return static_cast<typename Walker::unsigned_t>(glyphWidth);
								}, TextDataSource);
							signed_t alignX = 0;
							signed_t alignY = 0;
							ResolveAlignment(textWidth, textHeight, static_cast<signed_t>(ParentLayout::Width()), static_cast<signed_t>(ParentLayout::Height()), alignX, alignY);
							LineStartX = static_cast<signed_t>(OffsetX) + alignX;
							LineStartY = static_cast<signed_t>(OffsetY) + alignY;
						}

						static void ResolveAlignment(const signed_t textWidth, const signed_t textHeight, const signed_t layoutWidth, const signed_t layoutHeight, signed_t& x, signed_t& y)
						{
							switch (GlyphStyle::Alignment)
							{
							case Framework::Layout::AlignmentEnum::TopCenter:
							case Framework::Layout::AlignmentEnum::MiddleCenter:
							case Framework::Layout::AlignmentEnum::BottomCenter:
								x = SignedRightShift(layoutWidth - textWidth + 1, 1);
								break;
							case Framework::Layout::AlignmentEnum::TopRight:
							case Framework::Layout::AlignmentEnum::MiddleRight:
							case Framework::Layout::AlignmentEnum::BottomRight:
								x = layoutWidth - textWidth;
								break;
							default: x = 0; break;
							}
							switch (GlyphStyle::Alignment)
							{
							case Framework::Layout::AlignmentEnum::MiddleLeft:
							case Framework::Layout::AlignmentEnum::MiddleCenter:
							case Framework::Layout::AlignmentEnum::MiddleRight:
								y = SignedRightShift(layoutHeight - textHeight + 1, 1);
								break;
							case Framework::Layout::AlignmentEnum::BottomLeft:
							case Framework::Layout::AlignmentEnum::BottomCenter:
							case Framework::Layout::AlignmentEnum::BottomRight:
								y = layoutHeight - textHeight;
								break;
							default: y = 0; break;
							}
						}
					};

					template<typename dimension_t, typename ParentLayout, typename number_t, typename GlyphSourceType,
						typename GlyphStyle = Framework::Image::TemplateImageStyle<>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace>
					class Number : public Shader::Image::Bitmap::ImageShader<dimension_t,
						typename GlyphSourceType::color_t, GlyphSourceType::DataSourceType,
						GlyphStyle::UseTransparency, GlyphStyle::TransparentColor, GlyphStyle::ScaleX, GlyphStyle::ScaleY,
						GlyphStyle::TransformOrder, ColorShaderType, TransformShaderType, BlendMode>

					{
						using Base = Shader::Image::Bitmap::ImageShader<dimension_t,
							typename GlyphSourceType::color_t, GlyphSourceType::DataSourceType,
							GlyphStyle::UseTransparency, GlyphStyle::TransparentColor, GlyphStyle::ScaleX, GlyphStyle::ScaleY,
							GlyphStyle::TransformOrder, ColorShaderType, TransformShaderType, BlendMode>;
						using signed_t = typename TemplateTextNumberWalker::signed_t;

					private:
						static constexpr size_t MaxCharacters = sizeof(number_t) * 3 + (IntegerSignal::TypeTraits::TypeSign::is_signed<number_t>::value ? 1 : 0) + 1;

					private:
						char TextData[MaxCharacters]{};

						TextWalkerMetrics WalkerMetrics{};

						signed_t OffsetAlignmentX = 0;
						signed_t OffsetAlignmentY = 0;
						dimension_t OffsetX = 0;
						dimension_t OffsetY = 0;
						uint8_t TextIndex = 0;

					public:
						using DrawableLayout = ParentLayout;
						Number()
							: Base(0, 0, static_cast<dimension_t>(ParentLayout::Width()), static_cast<dimension_t>(ParentLayout::Height()))
							, WalkerMetrics{ GlyphSourceType::GetFontWidth(), GlyphSourceType::GetFontHeight(), 0, 0, 1, 0 }
						{
							RefreshAlignment();
						}

						void SetFontSize(const dimension_t width, const dimension_t height, const bool refreshLayout = true)
						{
							WalkerMetrics.FontWidth = width;
							WalkerMetrics.FontHeight = height;
							if (refreshLayout)
							{
								RefreshAlignment();
							}
						}

						void SetKerningWidth(const dimension_t width, const bool refreshLayout = true)
						{
							WalkerMetrics.KerningWidth = width;
							if (refreshLayout)
							{
								RefreshAlignment();
							}
						}

						void SetNumber(const number_t number, const bool refreshLayout = true)
						{
							TextIndex = 0;
							using unsigned_number_t = typename IntegerSignal::TypeTraits::TypeSign::make_unsigned<number_t>::type;
							unsigned_number_t magnitude;
							if (IntegerSignal::TypeTraits::TypeSign::is_signed<number_t>::value && number < 0)
							{
								TextData[TextIndex++] = AsciiDefinition::Printable::Minus;
								magnitude = static_cast<unsigned_number_t>(-(number + 1));
								++magnitude;
							}
							else magnitude = static_cast<unsigned_number_t>(number);
							TemplateTextNumberWalker::Walk(magnitude, [this](const char character) { TextData[TextIndex++] = character; });
							TextData[TextIndex] = 0;
							RefreshAlignment();
						}

						void SetNumber(const number_t number, const dimension_t offsetX, const dimension_t offsetY, const bool refreshLayout = true)
						{
							SetNumber(number, refreshLayout); SetOffset(offsetX, offsetY);
						}

						void SetOffset(const dimension_t offsetX, const dimension_t offsetY, const bool refreshLayout = true)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
							if (refreshLayout)
							{
								RefreshAlignment();
							}
						}

						dimension_t GetOffsetX() const { return OffsetX; }
						dimension_t GetOffsetY() const { return OffsetY; }

						void Draw(IFrameBuffer* frame)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							uint8_t index = 0;
							signed_t cursorX = static_cast<signed_t>(OffsetX) + OffsetAlignmentX;
							const signed_t cursorY = static_cast<signed_t>(OffsetY) + OffsetAlignmentY;
							while (index < TextIndex)
							{
								const auto glyph = GlyphSourceType::GetGlyph(static_cast<uint8_t>(TextData[index++]));
								Base::Draw(frame, glyph.BitmapData, glyph.Width, glyph.Height, cursorX, cursorY);

								cursorX += static_cast<signed_t>(glyph.Width * GlyphStyle::ScaleX);
								if (index < TextIndex)
									cursorX += static_cast<signed_t>(WalkerMetrics.KerningWidth * GlyphStyle::ScaleX);
							}
						}

					private:
						void RefreshAlignment()
						{
							const auto measurement = TemplateTextWalker::MeasureText(TextData, WalkerMetrics, 0, 0,
								[this](const char character, const signed_t, const signed_t, const bool)
								{
									return static_cast<typename TemplateTextWalker::unsigned_t>(GlyphSourceType::GetAdvance(character) * GlyphStyle::ScaleX);
								});
							const signed_t scaledTextWidth = static_cast<signed_t>(measurement.Width);
							const signed_t textHeight = static_cast<signed_t>(WalkerMetrics.FontHeight * GlyphStyle::ScaleY);
							OffsetAlignmentX = GetAlignmentOffset(scaledTextWidth, static_cast<signed_t>(ParentLayout::Width()), true);
							OffsetAlignmentY = GetAlignmentOffset(textHeight, static_cast<signed_t>(ParentLayout::Height()), false);
						}

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
				}
			}
		}
	}
}
#endif
