#ifndef _INTEGERGLASS_FRAMEWORK_IMAGE_BITMAP_DRAWABLE_h
#define _INTEGERGLASS_FRAMEWORK_IMAGE_BITMAP_DRAWABLE_h

#include "../../Shader/Image/Bitmap/ImageShader.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Image
		{
			namespace Bitmap
			{
				namespace Drawable
				{
					template<typename dimension_t,
						typename ParentLayout,
						typename ImageSourceType, // Contract::StaticImageSource,
						typename Style = Framework::Image::TemplateImageStyle<>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class StaticImage : public Shader::Image::Bitmap::ImageShader<
						dimension_t, typename ImageSourceType::color_t,
						ImageSourceType::DataSourceType,
						Style::UseTransparency, Style::TransparentColor,
						Style::ScaleX, Style::ScaleY,
						Style::TransformOrder,
						ColorShaderType, TransformShaderType, BlendMode>
					{
					private:
						using Base = Shader::Image::Bitmap::ImageShader<
							dimension_t, typename ImageSourceType::color_t,
							ImageSourceType::DataSourceType,
							Style::UseTransparency, Style::TransparentColor,
							Style::ScaleX, Style::ScaleY,
							Style::TransformOrder,
							ColorShaderType, TransformShaderType, BlendMode>;

					public:
						using DrawableLayout = ParentLayout;

					private:
						using color_t = typename ImageSourceType::color_t;

						struct RawImageLayout
						{
							static constexpr dimension_t X() { return 0; }
							static constexpr dimension_t Y() { return 0; }
							static constexpr dimension_t Width()
							{
								return static_cast<dimension_t>(ImageSourceType::Width * Style::ScaleX);
							}
							static constexpr dimension_t Height()
							{
								return static_cast<dimension_t>(ImageSourceType::Height * Style::ScaleY);
							}
						};

						using ImageLayout = Framework::Layout::Align<ParentLayout, RawImageLayout, Style::Alignment>;

					private:
						const color_t* BitmapData = ImageSourceType::GetBitmap();

					protected:
						dimension_t OffsetX = static_cast<dimension_t>(ImageLayout::X() - ParentLayout::X());
						dimension_t OffsetY = static_cast<dimension_t>(ImageLayout::Y() - ParentLayout::Y());

					public:
						StaticImage()
							: Base(0, 0, static_cast<dimension_t>(ParentLayout::Width()), static_cast<dimension_t>(ParentLayout::Height())) {}

						~StaticImage() = default;

						void Draw(IFrameBuffer* frame)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							Base::Draw(frame, BitmapData,
								ImageSourceType::Width, ImageSourceType::Height,
								OffsetX, OffsetY);
						}

						void SetOffset(const dimension_t offsetX, const dimension_t offsetY)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
						}

						dimension_t GetOffsetX() const { return OffsetX; }
						dimension_t GetOffsetY() const { return OffsetY; }
					};


					template<typename dimension_t,
						typename ParentLayout,
						typename color_t,
						Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Ram,
						typename Style = Framework::Image::TemplateImageStyle<>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class Image : public Shader::Image::Bitmap::ImageShader<
						dimension_t, color_t,
						DataSourceType,
						Style::UseTransparency, Style::TransparentColor,
						Style::ScaleX, Style::ScaleY,
						Style::TransformOrder,
						ColorShaderType, TransformShaderType, BlendMode>
					{
					private:
						using Base = Shader::Image::Bitmap::ImageShader<
							dimension_t, color_t,
							DataSourceType,
							Style::UseTransparency, Style::TransparentColor,
							Style::ScaleX, Style::ScaleY,
							Style::TransformOrder,
							ColorShaderType, TransformShaderType, BlendMode>;

					public:
						using DrawableLayout = ParentLayout;

					private:
						const color_t* BitmapData = nullptr;
						dimension_t Width = 0;
						dimension_t Height = 0;
						dimension_t OffsetX = 0;
						dimension_t OffsetY = 0;

					public:
						Image()
							: Base(0, 0, static_cast<dimension_t>(ParentLayout::Width()), static_cast<dimension_t>(ParentLayout::Height())) {}
						~Image() = default;

						void Draw(IFrameBuffer* frame)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							Base::Draw(frame, BitmapData, Width, Height, OffsetX, OffsetY);
						}

						void SetImage(const color_t* bitmapData, const dimension_t width, const dimension_t height)
						{
							BitmapData = bitmapData;
							Width = width;
							Height = height;
						}

						void SetImage(const color_t* bitmapData, const dimension_t width, const dimension_t height, const dimension_t offsetX, const dimension_t offsetY)
						{
							BitmapData = bitmapData;
							Width = width;
							Height = height;
							OffsetX = offsetX;
							OffsetY = offsetY;
						}

						void SetOffset(const dimension_t offsetX, const dimension_t offsetY)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
						}

						dimension_t GetOffsetX() const { return OffsetX; }
						dimension_t GetOffsetY() const { return OffsetY; }
					};
				}
			}
		}
	}
}
#endif