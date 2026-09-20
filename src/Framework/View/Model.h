#ifndef _INTEGERGLASS_FRAMEWORK_VIEW_MODEL_h
#define _INTEGERGLASS_FRAMEWORK_VIEW_MODEL_h

#include "../../Model/IFrameDraw.h"
#include "../Layout/Model.h"
#include "../Support.h"

namespace IntegerGlass
{
	/// <summary>
	/// INTEGERGLASS Framework is a compile-time toolkit for building drawables and views.
	/// Drawables are passive renderers; views orchestrate animation and composition over time.
	/// </summary>
	namespace Framework
	{
		namespace View
		{
			namespace Contract
			{
				/// <summary>
				/// View contract.
				///
				/// A view orchestrates animation and rendering and may require multiple calls to DrawCall()
				/// to complete a single frame/cycle.
				///
				/// A view publishes its intrinsic local-space rectangle through ViewLayout.
				/// Its effective local-space clip bounds and output translation are runtime state and
				/// may be supplied by a containing view or composite.
				/// </summary>
				template<typename dimensiont_t, typename Layout>
				struct View
				{
					/// <summary>
					/// Compile-time layout of the view in its local coordinate space.
					/// Implementations replace LayoutContract with their concrete layout type.
					/// </summary>
					using ViewLayout = Layout;

					/// <summary>
					/// Advances the view by one draw call.
					/// </summary>
					/// <param name="frame">Target framebuffer to draw into.</param>
					/// <param name="frameTime">Rolling frame timestamp (microseconds).</param>
					/// <param name="frameCounter">Rolling frame counter.</param>
					/// <returns>True when the view has completed its current draw cycle.</returns>
					bool DrawCall(IFrameBuffer* /*frame*/, const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) { return true; }

					/// <summary>
					/// Sets the effective local-space clipping rectangle propagated to the view's children.
					/// Implementations must preserve the intrinsic ViewLayout and forward the effective clip to children.
					/// </summary>
					/// <param name="left">Inclusive left edge in local coordinates.</param>
					/// <param name="top">Inclusive top edge in local coordinates.</param>
					/// <param name="right">Inclusive right edge in local coordinates.</param>
					/// <param name="bottom">Inclusive bottom edge in local coordinates.</param>
					void SetBounds(const dimensiont_t /*left*/, const dimensiont_t /*top*/,
						const dimensiont_t /*right*/, const dimensiont_t /*bottom*/) {}

					/// <summary>
					/// Sets the output translation propagated to the view's children.
					/// Translation does not modify the view's local-space ViewLayout or clipping rectangle.
					/// </summary>
					/// <param name="x">Horizontal output translation in pixels.</param>
					/// <param name="y">Vertical output translation in pixels.</param>
					void SetTranslation(const int16_t /*x*/, const int16_t /*y*/) {}

					bool IsVisible() const { return true; }
				};
			}

			/// <summary>
			/// Adapts a view contract to the engine's IFrameDraw interface.
			/// </summary>
			template<typename ViewType>
			class ViewAdapter : public IFrameDraw
			{
			public:
				ViewType ViewInstance{};

				bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) override
				{
					return ViewInstance.DrawCall(frame, frameTime, frameCounter);
				}
			};
		}
	}
}
#endif