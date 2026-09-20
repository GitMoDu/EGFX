#ifndef _INTEGERGLASS_MODULES_KIT_SHADER_TRANSFORM_ROTATE_h
#define _INTEGERGLASS_MODULES_KIT_SHADER_TRANSFORM_ROTATE_h

#include <IntegerGlassFramework.h>
#include <IntegerTrigonometry8.h>
#include <IntegerTrigonometry16.h>
#include "Skew.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Kit
		{
			namespace Shader
			{
				namespace Transform
				{
					/// <summary>
					/// Pixel-accurate rotation transform implemented using three skew passes.
					/// Matches `SpriteTransform::RotateTransform` logic but in the new shader/transform format.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">Optional base transform to chain.</typeparam>
					template<typename dimension_t,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class Rotate : public Base
					{
					private:
						using signed_t = typename Framework::AutoDimension::ByDimension<dimension_t>::signed_t;

					private:
						enum class FlippingEnum : uint8_t
						{
							NoFlip,
							Invert,       // 180
							FlipInvertX,  // 90
							FlipInvertY,  // 270
						};

					private:
						SkewX<dimension_t> SkewHorizontal{};
						SkewY<dimension_t> SkewVertical{};

						FlippingEnum Flip = FlippingEnum::NoFlip;
						angle_t Angle = 0;
						dimension_t Width = 0;
						dimension_t Height = 0;
						dimension_t CenterX = 0;
						dimension_t CenterY = 0;

					public:
						Rotate() : Base()
						{
							SetCenter(CenterX, CenterY);
						}
						~Rotate() = default;

						void SetWidth(const dimension_t width)
						{
							Width = width;
							SkewVertical.SetWidth(width);
						}

						void SetHeight(const dimension_t height)
						{
							Height = height;
							SkewHorizontal.SetHeight(height);
						}

						void SetDimensions(const dimension_t width, const dimension_t height)
						{
							SetWidth(width);
							SetHeight(height);
							SetCenter(static_cast<dimension_t>(width / 2), static_cast<dimension_t>(height / 2));
						}

						void SetCenter(const dimension_t x, const dimension_t y)
						{
							CenterX = x;
							CenterY = y;
							SkewHorizontal.SetReferenceY(y);
							SkewVertical.SetReferenceX(x);
						}

						dimension_t GetCenterX() const { return CenterX; }
						dimension_t GetCenterY() const { return CenterY; }

						void SetRotation(const angle_t angle)
						{
							Angle = angle;
							if (Width <= 1 || Height <= 1)
							{
								Flip = FlippingEnum::NoFlip;
								SkewHorizontal.SetSkewX(0);
								SkewVertical.SetSkewY(0);
								return;
							}

							angle_t adjustedAngle = angle;
							Flip = FlippingEnum::NoFlip;

							// Fast-path cardinal angles.
							switch (adjustedAngle)
							{
							case 0:
								return;
							case Trigonometry::ANGLE_90:
								Flip = FlippingEnum::FlipInvertX;
								return;
							case Trigonometry::ANGLE_180:
								Flip = FlippingEnum::Invert;
								return;
							case Trigonometry::ANGLE_270:
								Flip = FlippingEnum::FlipInvertY;
								return;
							}

							// Normalize to [0..180) and choose flip mode (matches old RotateTransform).
							if (adjustedAngle > Trigonometry::ANGLE_180)
							{
								Flip = FlippingEnum::Invert;
								adjustedAngle -= Trigonometry::ANGLE_180;

								if (adjustedAngle > Trigonometry::ANGLE_90)
								{
									Flip = FlippingEnum::FlipInvertY;
									adjustedAngle -= Trigonometry::ANGLE_90;
								}
							}
							else if (adjustedAngle > Trigonometry::ANGLE_90)
							{
								Flip = FlippingEnum::FlipInvertX;
								adjustedAngle -= Trigonometry::ANGLE_90;
							}

							// Compute skew factors (same formula as old code).
							signed_t skewX = 0;
							signed_t skewY = 0;

							skewX = Fraction<signed_t>(static_cast<fraction16_t>(-Tangent16(adjustedAngle >> 1)), static_cast<signed_t>(Width));
							skewY = Fraction<signed_t>(Sine16(adjustedAngle), static_cast<signed_t>(Height));

							SkewHorizontal.SetSkewX(skewX);
							SkewVertical.SetSkewY(skewY);
						}

						angle_t GetRotation() const
						{
							return Angle;
						}

						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y))
							{
								return false;
							}

							// Apply flip modes using the same semantics as the sprite transform version.
							switch (Flip)
							{
							case FlippingEnum::FlipInvertX:
							{
								const signed_t oldX = static_cast<signed_t>(x);
								x = static_cast<dimension_t>(static_cast<signed_t>(CenterX)
									+ static_cast<signed_t>(CenterY) - static_cast<signed_t>(y));
								y = static_cast<dimension_t>(static_cast<signed_t>(CenterY)
									+ oldX - static_cast<signed_t>(CenterX));
							}
							break;

							case FlippingEnum::FlipInvertY:
							{
								const signed_t oldX = static_cast<signed_t>(x);
								x = static_cast<dimension_t>(static_cast<signed_t>(CenterX)
									+ static_cast<signed_t>(y) - static_cast<signed_t>(CenterY));
								y = static_cast<dimension_t>(static_cast<signed_t>(CenterY)
									+ static_cast<signed_t>(CenterX) - oldX);
							}
							break;

							case FlippingEnum::Invert:
								x = static_cast<dimension_t>(static_cast<signed_t>(CenterX) * 2
									- static_cast<signed_t>(x));
								y = static_cast<dimension_t>(static_cast<signed_t>(CenterY) * 2
									- static_cast<signed_t>(y));
								break;

							case FlippingEnum::NoFlip:
							default:
								break;
							}

							// 3-pass skew rotation.
							if (SkewHorizontal.Transform(x, y) &&
								SkewVertical.Transform(x, y) &&
								SkewHorizontal.Transform(x, y))
							{
								return true;
							}

							return false;
						}
					};
				}
			}
		}
	}
}
#endif