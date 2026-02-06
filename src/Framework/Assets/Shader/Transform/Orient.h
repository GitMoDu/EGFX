#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_ORIENT_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_ORIENT_h

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
					/// Coordinate orientation transform (rotation + mirroring) reusing `DisplayOptions`.
					/// Unified into a single-pass mapping (one cached switch) matching
					/// `TemplateFramebuffer::TransformCoordinates` / `TemplateFramebuffer::GetTransformCase`.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Source width in pixels.</typeparam>
					/// <typeparam name="Height">Source height in pixels.</typeparam>
					/// <typeparam name="Base">Optional base transform to chain.</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>>
						class Orient : public Base
					{
					private:
						enum class TransformCase : uint8_t
						{
							Identity,
							MirrorX,
							MirrorY,
							MirrorXY,
							Rotate90,
							Rotate90MirrorX,
							Rotate90MirrorY,
							Rotate90MirrorXY,
							Rotate180,
							Rotate180MirrorX,
							Rotate180MirrorY,
						};

					private:
						DisplayOptions::RotationEnum Rotation = DisplayOptions::RotationEnum::None;
						DisplayOptions::MirrorEnum Mirror = DisplayOptions::MirrorEnum::None;

						TransformCase Case = TransformCase::Identity;

					public:
						Orient() : Base() {}
						~Orient() = default;

						void SetRotation(const DisplayOptions::RotationEnum rotation)
						{
							Rotation = rotation;
							Case = GetTransformCase();
						}

						DisplayOptions::RotationEnum GetRotation() const
						{
							return Rotation;
						}

						void SetMirror(const DisplayOptions::MirrorEnum mirror)
						{
							Mirror = mirror;
							Case = GetTransformCase();
						}

						DisplayOptions::MirrorEnum GetMirror() const
						{
							return Mirror;
						}

						void SetOrientation(const DisplayOptions::RotationEnum rotation, const DisplayOptions::MirrorEnum mirror)
						{
							Rotation = rotation;
							Mirror = mirror;
							Case = GetTransformCase();
						}

						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y))
							{
								return false;
							}

							// One-pass mapping to match framebuffer behavior (single switch).
							switch (Case)
							{
							case TransformCase::Identity:
							default:
								return true;

							case TransformCase::MirrorX:
								x = static_cast<dimension_t>(Width - 1 - x);
								return true;

							case TransformCase::MirrorY:
								y = static_cast<dimension_t>(Height - 1 - y);
								return true;

							case TransformCase::MirrorXY:
								x = static_cast<dimension_t>(Width - 1 - x);
								y = static_cast<dimension_t>(Height - 1 - y);
								return true;

							case TransformCase::Rotate90:
								// Framebuffer: { PhysicalWidth - 1 - y, x }.
								// For a source-space transform we use Height as the X extent post-rotation: { Height - 1 - y, x }.
							{
								const dimension_t oldX = x;
								x = static_cast<dimension_t>(Height - 1 - y);
								y = oldX;
							}
							return true;

							case TransformCase::Rotate90MirrorX:
								// Framebuffer: { PhysicalWidth - 1 - y, FrameWidth - 1 - x }.
							{
								const dimension_t oldX = x;
								x = static_cast<dimension_t>(Height - 1 - y);
								y = static_cast<dimension_t>(Width - 1 - oldX);
							}
							return true;

							case TransformCase::Rotate90MirrorY:
								// Framebuffer: { y, x }.
							{
								const dimension_t oldX = x;
								x = y;
								y = oldX;
							}
							return true;

							case TransformCase::Rotate90MirrorXY:
								// Framebuffer: { y, PhysicalHeight - 1 - x }.
							{
								const dimension_t oldX = x;
								x = y;
								y = static_cast<dimension_t>(Height - 1 - oldX);
							}
							return true;

							case TransformCase::Rotate180:
								// Framebuffer: { PhysicalWidth - 1 - x, PhysicalHeight - 1 - y }.
								x = static_cast<dimension_t>(Width - 1 - x);
								y = static_cast<dimension_t>(Height - 1 - y);
								return true;

							case TransformCase::Rotate180MirrorX:
								// Framebuffer: { x, PhysicalHeight - 1 - y }.
								y = static_cast<dimension_t>(Height - 1 - y);
								return true;

							case TransformCase::Rotate180MirrorY:
								// Framebuffer: { PhysicalWidth - 1 - x, y }.
								x = static_cast<dimension_t>(Width - 1 - x);
								return true;
							}
						}

					private:
						TransformCase GetTransformCase() const
						{
							// Explicitly map each rotation+mirror combination to a named case.
							// Mirrors TemplateFramebuffer::GetTransformCase().
							return
								(Rotation == DisplayOptions::RotationEnum::None) ?
								((Mirror == DisplayOptions::MirrorEnum::None) ? TransformCase::Identity :
									(Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCase::MirrorX :
									(Mirror == DisplayOptions::MirrorEnum::MirrorY) ? TransformCase::MirrorY :
									TransformCase::MirrorXY)
								: (Rotation == DisplayOptions::RotationEnum::Rotate90) ?
								((Mirror == DisplayOptions::MirrorEnum::None) ? TransformCase::Rotate90 :
									(Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCase::Rotate90MirrorX :
									(Mirror == DisplayOptions::MirrorEnum::MirrorY) ? TransformCase::Rotate90MirrorY :
									TransformCase::Rotate90MirrorXY)
								: (Rotation == DisplayOptions::RotationEnum::Rotate180) ?
								((Mirror == DisplayOptions::MirrorEnum::None ||
									Mirror == DisplayOptions::MirrorEnum::MirrorXY) ? TransformCase::Rotate180 :
									(Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCase::Rotate180MirrorX :
									TransformCase::Rotate180MirrorY)
								: ((Mirror == DisplayOptions::MirrorEnum::None) ? TransformCase::Rotate90MirrorXY :
									(Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCase::Rotate90MirrorY :
									(Mirror == DisplayOptions::MirrorEnum::MirrorY) ? TransformCase::Rotate90MirrorX :
									TransformCase::Rotate90);
						}
					};
				}
			}
		}
	}
}
#endif