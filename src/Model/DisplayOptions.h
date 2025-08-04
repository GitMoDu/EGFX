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

		enum class AntiAliasingEnum
		{
			None,
			ScanlineEdge
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
				return rotation != RotationEnum::NoRotation;
			}

			static constexpr bool HasMirroring()
			{
				return mirror != MirrorEnum::NoMirror;
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