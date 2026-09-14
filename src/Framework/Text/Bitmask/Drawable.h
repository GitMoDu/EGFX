#ifndef _EGFX_FRAMEWORK_TEXT_BITMASK_DRAWABLE_h
#define _EGFX_FRAMEWORK_TEXT_BITMASK_DRAWABLE_h

#include "Model.h"
#include "../TextWalker.h"
#include "../../Shader/Image/Bitmask/ImageShader.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Text
		{
			namespace Bitmask
			{
				namespace Drawable
				{
					template<typename dimension_t,
						typename ParentLayout,
						typename GlyphSourceType,
						typename GlyphStyle = Framework::Image::TemplateImageStyle<>,
						typename SourceShaderType = Shader::Source::StaticColor<dimension_t>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class Text : public Shader::Image::Bitmask::ImageShader<
						dimension_t,
						GlyphSourceType::DataSourceType,
						GlyphStyle::UseTransparency, GlyphStyle::TransparentColor,
						GlyphStyle::ScaleX, GlyphStyle::ScaleY,
						GlyphStyle::TransformOrder,
						SourceShaderType, ColorShaderType, TransformShaderType, BlendMode>
					{
					private:
						using Base = Shader::Image::Bitmask::ImageShader<
							dimension_t,
							GlyphSourceType::DataSourceType,
							GlyphStyle::UseTransparency, GlyphStyle::TransparentColor,
							GlyphStyle::ScaleX, GlyphStyle::ScaleY, GlyphStyle::TransformOrder,
							SourceShaderType, ColorShaderType, TransformShaderType, BlendMode>;
						using Walker = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							TemplateTextMonospaceWalker,
							TemplateTextWalker,
							GlyphSourceType::IsMonospace
						>::type;
						using signed_t = typename TemplateTextNumberWalker::signed_t;

					public:
						using DrawableLayout = ParentLayout;

					private:
						char SingleCharacterText[2] = { 0, 0 };
						const char* TextData = nullptr;
						Framework::DataSourceTypeEnum TextDataSource = Framework::DataSourceTypeEnum::Ram;

						TextWalkerMetrics WalkerMetrics;

						signed_t LineStartX = 0;
						signed_t LineStartY = 0;
						dimension_t OffsetX = 0;
						dimension_t OffsetY = 0;

					public:
						Text()
							: Base(0, 0, static_cast<dimension_t>(ParentLayout::Width()), static_cast<dimension_t>(ParentLayout::Height()))
							, WalkerMetrics{
								static_cast<typename Walker::unsigned_t>(GlyphSourceType::GetFontWidth() * GlyphStyle::ScaleX),
								static_cast<typename Walker::unsigned_t>(GlyphSourceType::GetFontHeight() * GlyphStyle::ScaleY),
								static_cast<typename Walker::unsigned_t>(4 * GlyphStyle::ScaleX),
								static_cast<typename Walker::unsigned_t>(GlyphStyle::ScaleX),
								static_cast<typename Walker::unsigned_t>(GlyphStyle::ScaleX),
								0 }
						{
							RefreshLayout();
						}
						~Text() = default;

						void Draw(IFrameBuffer* frame)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							if (TextData == nullptr)
							{
								return;
							}

							Walker::WalkText(TextData, WalkerMetrics, LineStartX, LineStartY,
								[this, frame](const char character, const signed_t cursorX, const signed_t cursorY, const bool)
								{
									const auto glyph = GlyphSourceType::GetGlyph(static_cast<uint8_t>(character));
									const signed_t glyphWidth = static_cast<signed_t>(glyph.Width * GlyphStyle::ScaleX);
									const signed_t glyphHeight = static_cast<signed_t>(glyph.Height * GlyphStyle::ScaleY);
									if (!(cursorX >= static_cast<signed_t>(ParentLayout::Width()) || cursorX + glyphWidth < 0 || cursorY >= static_cast<signed_t>(ParentLayout::Height()) || cursorY + glyphHeight < 0))
										Base::Draw(frame, glyph.BitmaskData, glyph.Width, glyph.Height, cursorX, cursorY);
									return static_cast<typename Walker::unsigned_t>(glyph.Width * GlyphStyle::ScaleX);
								}, TextDataSource);
						}


						void SetFontSize(const dimension_t width, const dimension_t height, const bool refreshLayout = true)
						{
							WalkerMetrics.FontWidth = width;
							WalkerMetrics.FontHeight = height;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetTabWidth(const dimension_t width, const bool refreshLayout = true)
						{
							WalkerMetrics.TabWidth = width;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetSpaceWidth(const dimension_t width, const bool refreshLayout = true)
						{
							WalkerMetrics.SpaceWidth = width;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetKerningWidth(const dimension_t width, const bool refreshLayout = true)
						{
							WalkerMetrics.KerningWidth = width;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetLineSpacing(const dimension_t spacing, const bool refreshLayout = true)
						{
							WalkerMetrics.LineSpacing = spacing;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

					
						void SetText(char* text, const bool refreshLayout = true)
						{
							TextData = text;
							TextDataSource = Framework::DataSourceTypeEnum::Ram;
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
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetText(char* text, const dimension_t offsetX, const dimension_t offsetY, const bool refreshLayout = true)
						{
							TextData = text;
							TextDataSource = Framework::DataSourceTypeEnum::Ram;
							OffsetX = offsetX;
							OffsetY = offsetY;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetText(const char* text, const dimension_t offsetX, const dimension_t offsetY, const bool refreshLayout = true)
						{
							TextData = text;
							TextDataSource = Framework::DataSourceTypeEnum::Ram;
							OffsetX = offsetX;
							OffsetY = offsetY;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						void SetOffset(const dimension_t offsetX, const dimension_t offsetY, const bool refreshLayout = true)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
							if (refreshLayout)
							{
								RefreshLayout();
							}
						}

						dimension_t GetOffsetX() const { return OffsetX; }
						dimension_t GetOffsetY() const { return OffsetY; }

						void RefreshLayout()
						{
							const auto measurement = Walker::MeasureText(TextData, WalkerMetrics, 0, 0,
								[this](const char character, const signed_t, const signed_t, const bool)
								{
									return static_cast<typename Walker::unsigned_t>(GlyphSourceType::GetAdvance(character) * GlyphStyle::ScaleX);
								}, TextDataSource);
							const signed_t textWidth = static_cast<signed_t>(measurement.Width);
							const signed_t glyphHeight = static_cast<signed_t>(WalkerMetrics.FontHeight);
							const signed_t textHeight = static_cast<signed_t>(measurement.LineCount) * glyphHeight
								+ (measurement.LineCount > 0 ? static_cast<signed_t>(measurement.LineCount - 1) * static_cast<signed_t>(WalkerMetrics.LineSpacing) : 0);

							signed_t alignmentX = 0;
							signed_t alignmentY = 0;
							ResolveAlignment(textWidth, textHeight,
								static_cast<signed_t>(ParentLayout::Width()),
								static_cast<signed_t>(ParentLayout::Height()), alignmentX, alignmentY);
							LineStartX = static_cast<signed_t>(OffsetX) + alignmentX;
							LineStartY = static_cast<signed_t>(OffsetY) + alignmentY;
						}

					private:
						void ResolveAlignment(
							const signed_t textWidth,
							const signed_t textHeight,
							const signed_t layoutWidth,
							const signed_t layoutHeight,
							signed_t& offsetAlignmentX,
							signed_t& offsetAlignmentY)
						{
							switch (GlyphStyle::Alignment)
							{
							case Framework::Layout::AlignmentEnum::TopLeft:
							case Framework::Layout::AlignmentEnum::MiddleLeft:
							case Framework::Layout::AlignmentEnum::BottomLeft:
								offsetAlignmentX = 0;
								break;
							case Framework::Layout::AlignmentEnum::TopCenter:
							case Framework::Layout::AlignmentEnum::MiddleCenter:
							case Framework::Layout::AlignmentEnum::BottomCenter:
								offsetAlignmentX = SignedRightShift(layoutWidth - textWidth + 1, 1);
								break;
							case Framework::Layout::AlignmentEnum::TopRight:
							case Framework::Layout::AlignmentEnum::MiddleRight:
							case Framework::Layout::AlignmentEnum::BottomRight:
								offsetAlignmentX = layoutWidth - textWidth;
								break;
							default:
								offsetAlignmentX = 0;
								break;
							}

							switch (GlyphStyle::Alignment)
							{
							case Framework::Layout::AlignmentEnum::TopLeft:
							case Framework::Layout::AlignmentEnum::TopCenter:
							case Framework::Layout::AlignmentEnum::TopRight:
								offsetAlignmentY = 0;
								break;
							case Framework::Layout::AlignmentEnum::MiddleLeft:
							case Framework::Layout::AlignmentEnum::MiddleCenter:
							case Framework::Layout::AlignmentEnum::MiddleRight:
								offsetAlignmentY = SignedRightShift(layoutHeight - textHeight + 1, 1);
								break;
							case Framework::Layout::AlignmentEnum::BottomLeft:
							case Framework::Layout::AlignmentEnum::BottomCenter:
							case Framework::Layout::AlignmentEnum::BottomRight:
								offsetAlignmentY = layoutHeight - textHeight;
								break;
							default:
								offsetAlignmentY = 0;
								break;
							}
						}
					};

					template<typename dimension_t,
						typename ParentLayout,
						typename number_t,
						typename GlyphSourceType,
						typename GlyphStyle = Framework::Image::TemplateImageStyle<>,
						typename SourceShaderType = Shader::Source::StaticColor<dimension_t>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class Number : public Shader::Image::Bitmask::ImageShader<
						dimension_t,
						GlyphSourceType::DataSourceType,
						GlyphStyle::UseTransparency, GlyphStyle::TransparentColor,
						GlyphStyle::ScaleX, GlyphStyle::ScaleY,
						GlyphStyle::TransformOrder,
						SourceShaderType, ColorShaderType, TransformShaderType, BlendMode>

					{
					private:
						using Base = Shader::Image::Bitmask::ImageShader<
							dimension_t,
							GlyphSourceType::DataSourceType,
							GlyphStyle::UseTransparency, GlyphStyle::TransparentColor,
						GlyphStyle::ScaleX, GlyphStyle::ScaleY, GlyphStyle::TransformOrder,
						SourceShaderType, ColorShaderType, TransformShaderType, BlendMode>;
						using Walker = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							TemplateTextMonospaceWalker,
							TemplateTextWalker,
							GlyphSourceType::IsMonospace
						>::type;

					public:
						using DrawableLayout = ParentLayout;

					public:
						static constexpr size_t MaxCharacters =
							sizeof(number_t) * 3 // Conservative estimate: 3 characters per byte (e.g., 255 -> "255")
							+ (IntegerSignal::TypeTraits::TypeSign::is_signed<number_t>::value ? 1 : 0)
							+ 1; // +1 for null terminator

						using signed_t = typename TemplateTextNumberWalker::signed_t;

					private:

					private:
						char TextData[MaxCharacters]{};
						uint8_t TextIndex = 0;

						dimension_t OffsetX = 0;
						dimension_t OffsetY = 0;

						signed_t OffsetAlignmentX = 0;
						signed_t OffsetAlignmentY = 0;

						TextWalkerMetrics WalkerMetrics{};

					public:
						Number()
							: Base(0, 0, static_cast<dimension_t>(ParentLayout::Width()), static_cast<dimension_t>(ParentLayout::Height()))
							, WalkerMetrics{
								static_cast<typename Walker::unsigned_t>(GlyphSourceType::GetFontWidth() * GlyphStyle::ScaleX),
								static_cast<typename Walker::unsigned_t>(GlyphSourceType::GetFontHeight() * GlyphStyle::ScaleY),
								0, 0, 0, 0 }
						{
							RefreshAlignment();
						}
						~Number() = default;

						void SetFontSize(const dimension_t width, const dimension_t height) { WalkerMetrics.FontWidth = width; WalkerMetrics.FontHeight = height; RefreshAlignment(); }
						void SetKerningWidth(const dimension_t width) { WalkerMetrics.KerningWidth = width; RefreshAlignment(); }

						void Draw(IFrameBuffer* frame)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							uint8_t index = 0;

							signed_t cursorX = static_cast<signed_t>(OffsetX) + OffsetAlignmentX;
							const signed_t cursorY = static_cast<signed_t>(OffsetY) + OffsetAlignmentY;
							while (index < TextIndex)
							{
								const auto character = TextData[index++];
								if (character == static_cast<char>(AsciiDefinition::Control::NUL))
								{
									break;
								}

								const auto glyph = GlyphSourceType::GetGlyph(static_cast<uint8_t>(character));
								const signed_t glyphWidth = static_cast<signed_t>(glyph.Width * GlyphStyle::ScaleX);
								const signed_t glyphHeight = static_cast<signed_t>(glyph.Height * GlyphStyle::ScaleY);
								const signed_t layoutWidth = static_cast<signed_t>(ParentLayout::Width());
								const signed_t layoutHeight = static_cast<signed_t>(ParentLayout::Height());

								if (!(cursorX >= layoutWidth
									|| cursorX + glyphWidth < 0
									|| cursorY >= layoutHeight
									|| cursorY + glyphHeight < 0))
								{
									Base::Draw(frame, glyph.BitmaskData, glyph.Width, glyph.Height, cursorX, cursorY);
								}

								cursorX += GlyphSourceType::IsMonospace
									? static_cast<signed_t>(WalkerMetrics.FontWidth)
									: glyphWidth;
								if (index < TextIndex)
								{
									cursorX += static_cast<signed_t>(WalkerMetrics.KerningWidth);
								}
							}
						}

						void SetNumber(const number_t number)
						{
							TextIndex = 0;
							using unsigned_number_t = typename IntegerSignal::TypeTraits::TypeSign::make_unsigned<number_t>::type;
							unsigned_number_t magnitude;
							if (IntegerSignal::TypeTraits::TypeSign::is_signed<number_t>::value && number < 0)
							{
								TextData[TextIndex++] = static_cast<char>(AsciiDefinition::Printable::Minus);
								magnitude = static_cast<unsigned_number_t>(-(number + 1));
								++magnitude;
							}
							else
							{
								magnitude = static_cast<unsigned_number_t>(number);
							}

							TemplateTextNumberWalker::Walk(magnitude,
								[this](const char character)
								{
									TextData[TextIndex++] = character;
								}, Framework::DataSourceTypeEnum::Ram);
							TextData[TextIndex] = static_cast<char>(AsciiDefinition::Control::NUL);

							RefreshAlignment();
						}

						void SetNumber(const number_t number, const dimension_t offsetX, const dimension_t offsetY)
						{
							SetNumber(number);
							SetOffset(offsetX, offsetY);
						}

						void SetOffset(const dimension_t offsetX, const dimension_t offsetY)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
							RefreshAlignment();
						}

						dimension_t GetOffsetX() const { return OffsetX; }
						dimension_t GetOffsetY() const { return OffsetY; }

					private:
						void RefreshAlignment()
						{
							const auto measurement = Walker::MeasureText(TextData, WalkerMetrics, 0, 0,
								[this](const char character, const signed_t, const signed_t, const bool)
								{
									return static_cast<typename Walker::unsigned_t>(GlyphSourceType::GetAdvance(character) * GlyphStyle::ScaleX);
								}, Framework::DataSourceTypeEnum::Ram);
							const signed_t textWidth = static_cast<signed_t>(measurement.Width);
							const signed_t textHeight =
								static_cast<signed_t>(measurement.LineCount) * static_cast<signed_t>(WalkerMetrics.FontHeight);
							const signed_t layoutWidth = static_cast<signed_t>(ParentLayout::Width());
							const signed_t layoutHeight = static_cast<signed_t>(ParentLayout::Height());

							OffsetAlignmentX = GetAlignmentOffsetX(textWidth, layoutWidth);
							OffsetAlignmentY = GetAlignmentOffsetY(textHeight, layoutHeight);
						}

						static signed_t GetAlignmentOffsetX(const signed_t contentSize, const signed_t layoutSize)
						{
							switch (GlyphStyle::Alignment)
							{
							case Framework::Layout::AlignmentEnum::TopLeft:
							case Framework::Layout::AlignmentEnum::MiddleLeft:
							case Framework::Layout::AlignmentEnum::BottomLeft:
								return 0;
							case Framework::Layout::AlignmentEnum::TopCenter:
							case Framework::Layout::AlignmentEnum::MiddleCenter:
							case Framework::Layout::AlignmentEnum::BottomCenter:
								return SignedRightShift(layoutSize - contentSize + 1, 1);
							case Framework::Layout::AlignmentEnum::TopRight:
							case Framework::Layout::AlignmentEnum::MiddleRight:
							case Framework::Layout::AlignmentEnum::BottomRight:
								return layoutSize - contentSize;
							default:
								return 0;
							}
						}

						static signed_t GetAlignmentOffsetY(const signed_t contentSize, const signed_t layoutSize)
						{
							switch (GlyphStyle::Alignment)
							{
							case Framework::Layout::AlignmentEnum::TopLeft:
							case Framework::Layout::AlignmentEnum::TopCenter:
							case Framework::Layout::AlignmentEnum::TopRight:
								return 0;
							case Framework::Layout::AlignmentEnum::MiddleLeft:
							case Framework::Layout::AlignmentEnum::MiddleCenter:
							case Framework::Layout::AlignmentEnum::MiddleRight:
								return SignedRightShift(layoutSize - contentSize + 1, 1);
							case Framework::Layout::AlignmentEnum::BottomLeft:
							case Framework::Layout::AlignmentEnum::BottomCenter:
							case Framework::Layout::AlignmentEnum::BottomRight:
								return layoutSize - contentSize;
							default:
								return 0;
							}
						}

					};
				}
			}
		}
	}
}
#endif