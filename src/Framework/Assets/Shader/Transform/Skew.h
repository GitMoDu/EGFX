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
					/// <typeparam name="Height">Transform-space height used to normalize the skew fraction.</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						dimension_t Height,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class SkewX : public Base
					{
					private:
						using signed_t = typename TypeTraits::TypeSign::make_signed<dimension_t>::type;
						using Selector = Framework::Shader::Transform::FractionSelector<1, Height>;
						using fraction_t = typename Selector::fraction_t;

					private:
						/// <summary>Skew reference line in Y (transform-space).</summary>
						dimension_t ReferenceY = static_cast<dimension_t>(Height / 2);

						/// <summary>Cached value: skewPixels / (Height - 1) as a signed fixed-point fraction.</summary>
						fraction_t SkewFraction = 0;

					public:
						SkewX() : Base() {}
						~SkewX() = default;

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

							const signed_t limitedSkew = LimitValue<signed_t,
								-static_cast<signed_t>(Height - 1),
								static_cast<signed_t>(Height - 1)>(skewPixels);

							if (Selector::Use16)
								SkewFraction = Fraction16::GetScalar<signed_t>(limitedSkew, static_cast<signed_t>(Height - 1));
							else
								SkewFraction = Fraction8::GetScalar<signed_t>(limitedSkew, static_cast<signed_t>(Height - 1));
						}

						/// <summary>Gets the configured skew amount in pixels across the height.</summary>
						signed_t GetSkewX() const
						{
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
					/// <typeparam name="Width">Transform-space width used to normalize the skew fraction.</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class SkewY : public Base
					{
					private:
						using signed_t = typename TypeTraits::TypeSign::make_signed<dimension_t>::type;
						using Selector = Framework::Shader::Transform::FractionSelector<Width>;
						using fraction_t = typename Selector::fraction_t;

					private:
						/// <summary>Skew reference line in X (transform-space).</summary>
						dimension_t ReferenceX = static_cast<dimension_t>(Width / 2);

						/// <summary>Cached value: skewPixels / (Width - 1) as a signed fixed-point fraction.</summary>
						fraction_t SkewFraction = 0;

					public:
						SkewY() : Base() {}
						~SkewY() = default;

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

							const signed_t limitedSkew = LimitValue<signed_t,
								-static_cast<signed_t>(Width - 1),
								static_cast<signed_t>(Width - 1)>(skewPixels);

							if (Selector::Use16)
								SkewFraction = Fraction16::GetScalar<signed_t>(limitedSkew, static_cast<signed_t>(Width - 1));
							else
								SkewFraction = Fraction8::GetScalar<signed_t>(limitedSkew, static_cast<signed_t>(Width - 1));
						}

						/// <summary>Gets the configured skew amount in pixels across the width.</summary>
						signed_t GetSkewY() const
						{
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
					/// <typeparam name="Width">Transform-space width used by SkewY.</typeparam>
					/// <typeparam name="Height">Transform-space height used by SkewX.</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					using SkewXY = SkewY<dimension_t, Width, SkewX<dimension_t, Height, Base>>;
				}
			}
		}
	}
}
#endif