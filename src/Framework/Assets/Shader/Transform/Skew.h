#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_SKEW_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_SKEW_h

#include <EgfxFramework.h>

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
					/// X-axis skew (shear) transform.
					/// Produces an X offset proportional to (y - ReferenceY), scaled by the configured skew amount.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class SkewX : public Base
					{
					private:
						using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;

						//static constexpr bool Use16 = (sizeof(dimension_t) > sizeof(uint8_t));
						static constexpr bool Use16 = true;
						using fraction_t = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type <
							fraction16_t,
							fraction8_t,
							Use16
						> ::type;

					private:
						dimension_t Height = 0;
						/// <summary>Skew reference line in Y (transform-space).</summary>
						dimension_t ReferenceY = 0;

						/// <summary>Cached value: skewPixels / (Height - 1) as a signed fixed-point fraction.</summary>
						fraction_t SkewFraction = 0;

					public:
						SkewX() : Base() {}
						~SkewX() = default;

						void SetHeight(const dimension_t height)
						{
							Height = height;
						}

						dimension_t GetHeight() const { return Height; }

						/// <summary>
						/// Sets the skew amount in pixels applied over the full Height range.
						/// Values are clamped to +/- (Height - 1). A value of 0 disables skew.
						/// </summary>
						/// <param name="skewPixels">Skew magnitude in pixels across the height.</param>
						void SetSkewX(const signed_t skewPixels)
						{
							if (skewPixels == 0 || Height <= 1)
							{
								SkewFraction = 0;
								return;
							}

							const signed_t limitedSkew = LimitValue<signed_t>(skewPixels,
								-static_cast<signed_t>(Height - 1),
								static_cast<signed_t>(Height - 1));

							if (Use16)
								SkewFraction = Fraction16::GetScalar<signed_t>(limitedSkew, static_cast<signed_t>(Height - 1));
							else
								SkewFraction = Fraction8::GetScalar<signed_t>(limitedSkew, static_cast<signed_t>(Height - 1));
						}

						/// <summary>Gets the configured skew amount in pixels across the height.</summary>
						signed_t GetSkewX() const
						{
							if (Height <= 1)
								return 0;

							return Fraction<signed_t>(SkewFraction, static_cast<signed_t>(Height - 1));
						}

						/// <summary>Sets the Y reference line for the skew.</summary>
						void SetReferenceY(const dimension_t reference)
						{
							ReferenceY = reference;
						}

						/// <summary>Gets the Y reference line for the skew.</summary>
						dimension_t GetReferenceY() const
						{
							return ReferenceY;
						}

						/// <summary>
						/// Applies the base transform chain and then applies X skew in-place.
						/// </summary>
						/// <param name="x">X coordinate to transform in-place.</param>
						/// <param name="y">Y coordinate to transform in-place.</param>
						/// <returns>True if the base transform chain succeeds; otherwise false.</returns>
						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y))
							{
								return false;
							}

							if (SkewFraction == 0 || Height <= 1)
							{
								return true;
							}

							const signed_t deltaY = static_cast<signed_t>(y) - static_cast<signed_t>(ReferenceY);
							x = static_cast<dimension_t>(static_cast<signed_t>(x) + Fraction<signed_t>(SkewFraction, deltaY));

							return true;
						}
					};

					/// <summary>
					/// Y-axis skew (shear) transform.
					/// Produces a Y offset proportional to (x - ReferenceX), scaled by the configured skew amount.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class SkewY : public Base
					{
					private:
						using signed_t = typename TypeTraits::TypeSign::make_signed<dimension_t>::type;
						//static constexpr bool Use16 = (sizeof(dimension_t) > sizeof(uint8_t));
						static constexpr bool Use16 = true;
						using fraction_t = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type <
							fraction16_t,
							fraction8_t,
							Use16
						> ::type;

					private:
						dimension_t Width = 0;
						/// <summary>Skew reference line in X (transform-space).</summary>
						dimension_t ReferenceX = 0;

						/// <summary>Cached value: skewPixels / (Width - 1) as a signed fixed-point fraction.</summary>
						fraction_t SkewFraction = 0;

					public:
						SkewY() : Base() {}
						~SkewY() = default;

						void SetWidth(const dimension_t width)
						{
							Width = width;
						}

						dimension_t GetWidth() const { return Width; }

						/// <summary>
						/// Sets the skew amount in pixels applied over the full Width range.
						/// Values are clamped to +/- (Width - 1). A value of 0 disables skew.
						/// </summary>
						/// <param name="skewPixels">Skew magnitude in pixels across the width.</param>
						void SetSkewY(const signed_t skewPixels)
						{
							if (skewPixels == 0 || Width <= 1)
							{
								SkewFraction = 0;
								return;
							}

							const signed_t limitedSkew = LimitValue<signed_t>(skewPixels,
								-static_cast<signed_t>(Width - 1),
								static_cast<signed_t>(Width - 1));

							if (Use16)
								SkewFraction = Fraction16::GetScalar<signed_t>(limitedSkew, static_cast<signed_t>(Width - 1));
							else
								SkewFraction = Fraction8::GetScalar<signed_t>(limitedSkew, static_cast<signed_t>(Width - 1));
						}

						/// <summary>Gets the configured skew amount in pixels across the width.</summary>
						signed_t GetSkewY() const
						{
							if (Width <= 1)
								return 0;

							return Fraction<signed_t>(SkewFraction, static_cast<signed_t>(Width - 1));
						}

						/// <summary>Sets the X reference line for the skew.</summary>
						void SetReferenceX(const dimension_t reference)
						{
							ReferenceX = reference;
						}

						/// <summary>Gets the X reference line for the skew.</summary>
						dimension_t GetReferenceX() const
						{
							return ReferenceX;
						}

						/// <summary>
						/// Applies the base transform chain and then applies Y skew in-place.
						/// </summary>
						/// <param name="x">X coordinate to transform in-place.</param>
						/// <param name="y">Y coordinate to transform in-place.</param>
						/// <returns>True if the base transform chain succeeds; otherwise false.</returns>
						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y))
							{
								return false;
							}

							if (SkewFraction == 0 || Width <= 1)
							{
								return true;
							}

							const signed_t deltaX = static_cast<signed_t>(x) - static_cast<signed_t>(ReferenceX);
							y = static_cast<dimension_t>(static_cast<signed_t>(y) + Fraction<signed_t>(SkewFraction, deltaX));

							return true;
						}
					};

					/// <summary>
					/// Combined skew transform (Y skew, then X skew) using the provided Base transform chain.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					using SkewXY = SkewY<dimension_t, SkewX<dimension_t, Base>>;
				}
			}
		}
	}
}
#endif