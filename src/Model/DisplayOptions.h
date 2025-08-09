#ifndef _DISPLAY_OPTIONS_h
#define _DISPLAY_OPTIONS_h

#include <stdint.h>

namespace Egfx
{
	/// <summary>
	/// Defines compile-time display options using template parameters for mirroring, rotation, anti-aliasing, and inversion.
	/// </summary>
	namespace DisplayOptions
	{
		enum class MirrorEnum
		{
			None,
			MirrorX,
			MirrorY,
			MirrorXY
		};

		enum class RotationEnum
		{
			None,
			Plus90,
			Minus90
		};

		/// <summary>
		/// Anti-aliasing modes for edge rendering.
		/// </summary>
		enum class AntiAliasingEnum
		{
			/// <summary>
			/// No anti-aliasing. 
			/// - Fastest, no extra computation or memory.
			/// - Jagged edges (aliasing artifacts) are visible, especially on diagonal or curved shapes.
			/// </summary>
			None,

			/// <summary>
			/// EdgeBlend: Simple scanline edge blending.
			/// - Fast, visually softens edges by blending adjacent pixels.
			/// - Only blends a single pixel on each edge, so results are not as smooth as more advanced methods.
			/// - May not be effective for very thin or high-contrast shapes.
			/// </summary>
			EdgeBlend,

			/// <summary>
			/// PixelCoverage: Subpixel coverage-based alpha blending.
			/// - Produces smoother, more accurate edges by calculating the fractional coverage of edge pixels.
			/// - Reduces visible aliasing and works well for both thin and thick shapes.
			/// - Slightly higher computational cost than EdgeBlend.
			/// - AA Coverage is biased towards vertical/diagonal edges, so it may not work as well for horizontal edges.
			/// </summary>
			PixelCoverage
		};

		/// <summary>
		/// Defines compile-time options for mirroring, rotation, anti-aliasing, and inversion using template parameters.
		/// </summary>
		/// <typeparam name="rotation">The rotation mode, specified as a value of RotationEnum.</typeparam>
		/// <typeparam name="mirror">The mirroring mode, specified as a value of MirrorEnum.</typeparam>
		/// <typeparam name="antiAliasing">The anti-aliasing mode, specified as a value of AntiAliasingEnum.</typeparam>
		/// <typeparam name="inverted">A boolean indicating whether color inversion is enabled.</typeparam>
		template<MirrorEnum mirror
			, RotationEnum rotation
			, AntiAliasingEnum antiAliasing
			, bool inverted>
		struct TemplateOptions
		{
			static constexpr MirrorEnum Mirror = mirror;
			static constexpr RotationEnum Rotation = rotation;
			static constexpr AntiAliasingEnum AntiAliasing = antiAliasing;
			static constexpr bool Inverted = inverted;

			static constexpr bool HasRotation()
			{
				return rotation != RotationEnum::None;
			}

			static constexpr bool HasMirroring()
			{
				return mirror != MirrorEnum::None;
			}

			static constexpr bool HasAntiAliasing()
			{
				return antiAliasing != AntiAliasingEnum::None;
			}
		};

		/// <summary>
		/// Alias for DisplayOptions with default settings.
		/// </summary>
		using Default = TemplateOptions<MirrorEnum::None, RotationEnum::None, AntiAliasingEnum::None, false>;
	}

}
#endif