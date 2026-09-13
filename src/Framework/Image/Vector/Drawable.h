#ifndef _EGFX_FRAMEWORK_IMAGE_VECTOR_DRAWABLE_h
#define _EGFX_FRAMEWORK_IMAGE_VECTOR_DRAWABLE_h

#include "Model.h"
#include "../../Shader/Image/Vector/ImageShader.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Image
		{
			namespace Vector
			{
				namespace Drawable
				{
					using namespace Framework::Vector;
					using namespace IntegerSignal::FixedPoint::FactorScale;

					template<typename dimension_t,
						typename ParentLayout,
						typename ImageSourceType,
						typename Style = Framework::Image::TemplateImageStyle<>,
						typename PaletteType = Framework::Shader::Image::Vector::StaticColorPalette<RGB_COLOR_WHITE>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>>
						class StaticImage : public Shader::Image::Vector::Image<dimension_t, PaletteType, ColorShaderType, TransformShaderType>
					{
					private:
						using Base = Shader::Image::Vector::Image<dimension_t, PaletteType, ColorShaderType, TransformShaderType>;
						using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;
						using canvas_t = Framework::Vector::canvas_t;

						dimension_t Width = static_cast<dimension_t>(ImageSourceType::CanvasWidth);
						dimension_t Height = static_cast<dimension_t>(ImageSourceType::CanvasHeight);
						signed_t OffsetX = 0;
						signed_t OffsetY = 0;

					public:
						using DrawableLayout = ParentLayout;

						StaticImage() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height())
						{
							RefreshOffset();
						}
						~StaticImage() = default;

						void Draw(IFrameBuffer* frame)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							Base::template Draw<ImageSourceType::VectorType, ImageSourceType::DataSourceType>(frame,
								static_cast<const void*>(ImageSourceType::Data), ImageSourceType::Count,
								ImageSourceType::CanvasWidth, ImageSourceType::CanvasHeight,
								ImageSourceType::ThicknessScale, Width, Height, OffsetX, OffsetY);
						}

						void SetDimensions(const dimension_t width, const dimension_t height)
						{
							Width = width == 0 ? static_cast<dimension_t>(ImageSourceType::CanvasWidth) : width;
							Height = height == 0 ? static_cast<dimension_t>(ImageSourceType::CanvasHeight) : height;
							RefreshOffset();
						}

						void SetOffset(const signed_t offsetX, const signed_t offsetY)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
						}

						dimension_t GetWidth() const { return Width; }
						dimension_t GetHeight() const { return Height; }
						signed_t GetOffsetX() const { return OffsetX; }
						signed_t GetOffsetY() const { return OffsetY; }

					private:
						void RefreshOffset()
						{
							OffsetX = static_cast<signed_t>(ParentLayout::X());
							OffsetY = static_cast<signed_t>(ParentLayout::Y());
						}
					};

					template<typename dimension_t,
						typename ParentLayout,
						typename Style = Framework::Image::TemplateImageStyle<>,
						typename PaletteType = Framework::Shader::Image::Vector::StaticColorPalette<RGB_COLOR_WHITE>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>>
						class Image : public Shader::Image::Vector::Image<dimension_t, PaletteType, ColorShaderType, TransformShaderType>
					{
					private:
						using Base = Shader::Image::Vector::Image<dimension_t, PaletteType, ColorShaderType, TransformShaderType>;
						using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;
						using canvas_t = Framework::Vector::canvas_t;

						enum class SourceType : uint8_t
						{
							None,
							Image15x15,
							Image255x255
						};

						const void* Data = nullptr;
						size_t Count = 0;
						canvas_t CanvasWidth = 0;
						canvas_t CanvasHeight = 0;
						ufraction8_t ThicknessScale{};
						SourceType Type = SourceType::None;

						dimension_t Width = ParentLayout::Width();
						dimension_t Height = ParentLayout::Height();
						dimension_t OffsetX = 0;
						dimension_t OffsetY = 0;
						dimension_t Inset = 0;

					public:
						using DrawableLayout = ParentLayout;

						Image() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}
						~Image() = default;

						void Draw(IFrameBuffer* frame)
						{
							if (Data == nullptr || Type == SourceType::None)
								return;

							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							if (Type == SourceType::Image15x15)
							{
								Base::template Draw<Framework::Vector::PackedVectorEnum::Image15x15>(frame,
									static_cast<const void*>(Data), Count,
									CanvasWidth, CanvasHeight, ThicknessScale, Width, Height,
									static_cast<signed_t>(OffsetX), static_cast<signed_t>(OffsetY), Inset);
							}
							else
							{
								Base::template Draw<Framework::Vector::PackedVectorEnum::Image255x255>(frame,
									static_cast<const void*>(Data), Count,
									CanvasWidth, CanvasHeight, ThicknessScale, Width, Height,
									static_cast<signed_t>(OffsetX), static_cast<signed_t>(OffsetY), Inset);
							}
						}

						void SetImage(const Image15x15::packed_t* data, const size_t count,
							const dimension_t canvasWidth, const dimension_t canvasHeight,
							const ufraction8_t thicknessScale, const dimension_t inset = 0)
						{
							Data = data;
							Count = count;
							CanvasWidth = canvasWidth;
							CanvasHeight = canvasHeight;
							ThicknessScale = thicknessScale;
							Inset = inset;
							Type = SourceType::Image15x15;
							RefreshOffset();
						}

						void SetImage(const Image255x255::packed_t* data, const size_t count,
							const dimension_t canvasWidth, const dimension_t canvasHeight,
							const ufraction8_t thicknessScale, const dimension_t inset = 0)
						{
							Data = data;
							Count = count;
							CanvasWidth = canvasWidth;
							CanvasHeight = canvasHeight;
							ThicknessScale = thicknessScale;
							Inset = inset;
							Type = SourceType::Image255x255;
							RefreshOffset();
						}

						void SetDimensions(const dimension_t width, const dimension_t height)
						{
							Width = width == 0 ? ParentLayout::Width() : width;
							Height = height == 0 ? ParentLayout::Height() : height;
							RefreshOffset();
						}

						dimension_t GetWidth() const { return Width; }
						dimension_t GetHeight() const { return Height; }

						void SetOffset(const dimension_t offsetX, const dimension_t offsetY)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
						}

						void SetInset(const dimension_t inset)
						{
							Inset = inset;
						}

						dimension_t GetOffsetX() const { return OffsetX; }
						dimension_t GetOffsetY() const { return OffsetY; }

					private:
						static constexpr dimension_t GetAlignmentOffset(const dimension_t content, const dimension_t layout, const bool horizontal)
						{
							if (horizontal)
								return Style::Alignment == Framework::Layout::AlignmentEnum::TopRight
								|| Style::Alignment == Framework::Layout::AlignmentEnum::MiddleRight
								|| Style::Alignment == Framework::Layout::AlignmentEnum::BottomRight
								? layout - content : (Style::Alignment == Framework::Layout::AlignmentEnum::TopCenter
									|| Style::Alignment == Framework::Layout::AlignmentEnum::MiddleCenter
									|| Style::Alignment == Framework::Layout::AlignmentEnum::BottomCenter
									? SignedRightShift(layout - content + 1, 1) : 0);

							return Style::Alignment == Framework::Layout::AlignmentEnum::BottomLeft
								|| Style::Alignment == Framework::Layout::AlignmentEnum::BottomCenter
								|| Style::Alignment == Framework::Layout::AlignmentEnum::BottomRight
								? layout - content : (Style::Alignment == Framework::Layout::AlignmentEnum::MiddleLeft
									|| Style::Alignment == Framework::Layout::AlignmentEnum::MiddleCenter
									|| Style::Alignment == Framework::Layout::AlignmentEnum::MiddleRight
									? SignedRightShift(layout - content + 1, 1) : 0);
						}

						void RefreshOffset()
						{
							OffsetX = GetAlignmentOffset(Width, ParentLayout::Width(), true);
							OffsetY = GetAlignmentOffset(Height, ParentLayout::Height(), false);
						}
					};
				}
			}
		}
	}
}
#endif