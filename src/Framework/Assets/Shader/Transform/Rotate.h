#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_ROTATE_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_ROTATE_h

#include <EgfxFramework.h>
#include <IntegerTrigonometry8.h>
#include <IntegerTrigonometry16.h>
#include "Skew.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
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
					/// <typeparam name="Width">Source width in pixels.</typeparam>
					/// <typeparam name="Height">Source height in pixels.</typeparam>
					/// <typeparam name="Base">Optional base transform to chain.</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class Rotate : public Base
					{
					private:
						using signed_t = typename TypeTraits::TypeSign::make_signed<dimension_t>::type;
						using Selector = Framework::Shader::Transform::FractionSelector<Width, Height>;

					private:
						enum class FlippingEnum : uint8_t
						{
							NoFlip,
							Invert,       // 180
							FlipInvertX,  // 90
							FlipInvertY,  // 270
						};

					private:
						SkewX<dimension_t, Height> SkewHorizontal{};
						SkewY<dimension_t, Width> SkewVertical{};

						FlippingEnum Flip = FlippingEnum::NoFlip;
						angle_t Angle = 0;

					public:
						Rotate() : Base() {}
						~Rotate() = default;

						void SetRotation(const angle_t angle)
						{
							Angle = angle;

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

							if (Selector::Use16)
							{
								skewX = Fraction<signed_t>(static_cast<fraction16_t>(-Tangent16(adjustedAngle >> 1)), Width);
								skewY = Fraction<signed_t>(Sine16(adjustedAngle), Height);
							}
							else
							{
								skewX = Fraction<signed_t>(static_cast<fraction8_t>(-Tangent8(adjustedAngle >> 1)), Width);
								skewY = Fraction<signed_t>(Sine8(adjustedAngle), Height);
							}

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
								const dimension_t oldX = x;
								x = static_cast<dimension_t>(Height - 1 - y);
								y = oldX;
							}
							break;

							case FlippingEnum::FlipInvertY:
							{
								const dimension_t oldX = x;
								x = y;
								y = static_cast<dimension_t>(Width - 1 - oldX);
							}
							break;

							case FlippingEnum::Invert:
								x = static_cast<dimension_t>(Width - 1 - x);
								y = static_cast<dimension_t>(Height - 1 - y);
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