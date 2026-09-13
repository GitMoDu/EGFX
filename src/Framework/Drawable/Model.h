#ifndef _EGFX_FRAMEWORK_DRAWABLE_MODEL_h
#define _EGFX_FRAMEWORK_DRAWABLE_MODEL_h

#include "../../Model/IFrameBuffer.h"
#include "../Layout/Model.h"
#include "../Support.h"

namespace Egfx
{
	namespace Framework
	{
		/// <summary>
		/// Framework for drawable objects that can render to a frame buffer.
		/// Drawables are passive renderers - they draw their current state without animation logic.
		/// </summary>
		namespace Drawable
		{
			namespace Contract
			{
				/// <summary>
				/// Defines a contract for drawable objects that can render to a frame buffer.
				/// Drawables perform their rendering in a single Draw() call without any parameters, unless where required (eg: fonts).
				///
				/// A drawable publishes its intrinsic local-space rectangle through DrawableLayout.
				/// Its effective local-space clip bounds and output translation are runtime state and
				/// may be supplied by a containing view or composite.
				/// </summary>
				template<typename dimensiont_t,
					typename Layout>
				struct Drawable
				{
					/// <summary>
					/// Compile-time layout of the drawable in its local coordinate space.
					/// Implementations replace LayoutContract with their concrete layout type.
					/// </summary>
					using DrawableLayout = Layout;

					/// <summary>
					/// Draw the drawable's current state to the provided framebuffer.
					/// </summary>
					/// <param name="frame">Target framebuffer to draw into.</param>
					void Draw(IFrameBuffer* /*frame*/) {}

					/// <summary>
					/// Sets the drawable's effective local-space clipping rectangle.
					/// Implementations must reject pixels outside this rectangle without changing the intrinsic DrawableLayout.
					/// </summary>
					/// <param name="left">Inclusive left edge in local coordinates.</param>
					/// <param name="top">Inclusive top edge in local coordinates.</param>
					/// <param name="right">Inclusive right edge in local coordinates.</param>
					/// <param name="bottom">Inclusive bottom edge in local coordinates.</param>
					void SetBounds(const dimensiont_t /*left*/, const dimensiont_t /*top*/,
						const dimensiont_t /*right*/, const dimensiont_t /*bottom*/) {}

					/// <summary>
					/// Sets the output translation applied when the drawable writes to the framebuffer.
					/// Translation does not modify the drawable's local-space DrawableLayout or clipping rectangle.
					/// </summary>
					/// <param name="x">Horizontal output translation in pixels.</param>
					/// <param name="y">Vertical output translation in pixels.</param>
					void SetTranslation(const int16_t /*x*/, const int16_t /*y*/) {}
				};
			}

			template<typename dimensiont_t,
				typename PixelShaderType>
			struct MockDrawable
			{
				MockDrawable() {}
				~MockDrawable() = default;

				void Draw(IFrameBuffer* /*frame*/) {}
				void SetBounds(const int16_t /*left*/, const int16_t /*top*/,
					const int16_t /*right*/, const int16_t /*bottom*/) {}
				void SetTranslation(const int16_t /*x*/, const int16_t /*y*/) {}
			};
		}
	}
}
#endif