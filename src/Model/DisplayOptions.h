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
			Rotate90,
			Rotate180,
			Rotate270
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
			, bool inverted = false
		>
		struct TemplateOptions
		{
			static constexpr MirrorEnum Mirror = mirror;
			static constexpr RotationEnum Rotation = rotation;
			static constexpr bool Inverted = inverted;

			static constexpr bool HasRotation()
			{
				return rotation == RotationEnum::Rotate90
					|| rotation == RotationEnum::Rotate270;
			}

			static constexpr bool HasMirroring()
			{
				return mirror != MirrorEnum::None;
			}

		};

		/// <summary>
		/// Alias for DisplayOptions with default settings.
		/// </summary>
		using Default = TemplateOptions<MirrorEnum::None, RotationEnum::None, false>;
	}
}
#endif