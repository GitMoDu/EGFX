#ifndef _INTEGERGLASS_FRAMEWORK_ICON_VECTOR_DRAWABLE_h
#define _INTEGERGLASS_FRAMEWORK_ICON_VECTOR_DRAWABLE_h

#include "../../Image/Vector/Drawable.h"
#include "../Model.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Icon
		{
			namespace Vector
			{
				namespace Drawable
				{
					template<typename dimension_t,
						typename ParentLayout,
						typename IconSetType,
						typename Style = Framework::Image::TemplateImageStyle<>,
						typename PaletteType = Framework::Shader::Image::Vector::StaticColorPalette<RGB_COLOR_WHITE>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class Icon : public Shader::Image::Vector::Image<dimension_t, PaletteType, ColorShaderType, TransformShaderType, BlendMode>
					{
					private:
						using Base = Shader::Image::Vector::Image<dimension_t, PaletteType, ColorShaderType, TransformShaderType, BlendMode>;
						using icon_enum_t = Framework::Icon::IconEnum;
						using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;

					public:
						using DrawableLayout = ParentLayout;
						using coordinate_t = signed_t;

					private:
						icon_enum_t SelectedIcon = icon_enum_t::Add;
						dimension_t Width = static_cast<dimension_t>(IconSetType::CanvasWidth);
						dimension_t Height = static_cast<dimension_t>(IconSetType::CanvasHeight);
						signed_t OffsetX = 0;
						signed_t OffsetY = 0;
						dimension_t Inset = static_cast<dimension_t>(IconSetType::EdgeInset);

					public:
						Icon() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height())
						{
							RefreshOffset();
						}
						~Icon() = default;

						void Draw(IFrameBuffer* frame)
						{
							const auto source = IconSetType::GetIcon(SelectedIcon);
							if (source.Data == nullptr || source.Count == 0)
								return;

							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							Base::template Draw<IconSetType::VectorType, IconSetType::DataSourceType>(frame,
								static_cast<const void*>(source.Data), source.Count,
								IconSetType::CanvasWidth, IconSetType::CanvasHeight,
								Framework::Vector::THICKNESS_SCALE_1X / 2,
								Width, Height, OffsetX, OffsetY, Inset);
						}

						void SetIcon(const icon_enum_t icon)
						{
							SelectedIcon = icon;
						}

						icon_enum_t GetIcon() const { return SelectedIcon; }

						void SetDimensions(const dimension_t width, const dimension_t height)
						{
							Width = width == 0 ? static_cast<dimension_t>(IconSetType::CanvasWidth) : width;
							Height = height == 0 ? static_cast<dimension_t>(IconSetType::CanvasHeight) : height;
							RefreshOffset();
						}

						dimension_t GetWidth() const { return Width; }
						dimension_t GetHeight() const { return Height; }

						void SetOffset(const signed_t offsetX, const signed_t offsetY)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
						}

						signed_t GetOffsetX() const { return OffsetX; }
						signed_t GetOffsetY() const { return OffsetY; }

						void SetInset(const dimension_t inset) { Inset = inset; }
						dimension_t GetInset() const { return Inset; }

					private:
						static signed_t GetAlignmentOffset(const dimension_t content, const dimension_t layout, const bool horizontal)
						{
							const signed_t difference = static_cast<signed_t>(layout) - static_cast<signed_t>(content);

							if (horizontal)
								return Style::Alignment == Framework::Layout::AlignmentEnum::TopRight
								|| Style::Alignment == Framework::Layout::AlignmentEnum::MiddleRight
								|| Style::Alignment == Framework::Layout::AlignmentEnum::BottomRight
								? difference : (Style::Alignment == Framework::Layout::AlignmentEnum::TopCenter
									|| Style::Alignment == Framework::Layout::AlignmentEnum::MiddleCenter
									? SignedRightShift(difference + 1, 1) : 0);

							return Style::Alignment == Framework::Layout::AlignmentEnum::BottomLeft
								|| Style::Alignment == Framework::Layout::AlignmentEnum::BottomCenter
								|| Style::Alignment == Framework::Layout::AlignmentEnum::BottomRight
								? difference : (Style::Alignment == Framework::Layout::AlignmentEnum::MiddleLeft
									|| Style::Alignment == Framework::Layout::AlignmentEnum::MiddleCenter
									|| Style::Alignment == Framework::Layout::AlignmentEnum::MiddleRight
									? SignedRightShift(difference + 1, 1) : 0);
						}

						void RefreshOffset()
						{
							OffsetX = static_cast<signed_t>(GetAlignmentOffset(Width, ParentLayout::Width(), true));
							OffsetY = static_cast<signed_t>(GetAlignmentOffset(Height, ParentLayout::Height(), false));
						}
					};
				}
			}
		}
	}
}
#endif
