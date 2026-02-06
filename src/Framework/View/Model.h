#ifndef _EGFX_FRAMEWORK_VIEW_MODEL_h
#define _EGFX_FRAMEWORK_VIEW_MODEL_h

#include "../../Model/IFrameDraw.h"
#include "../Support.h"

namespace Egfx
{
	/// <summary>
	/// EGFX Framework is a compile-time toolkit for building drawables and views.
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
				/// </summary>
				struct View
				{
					/// <summary>
					/// Advances the view by one draw call.
					/// </summary>
					/// <param name="frame">Target framebuffer to draw into.</param>
					/// <param name="frameTime">Rolling frame timestamp (microseconds).</param>
					/// <param name="frameCounter">Rolling frame counter.</param>
					/// <returns>True when the view has completed its current draw cycle.</returns>
					bool DrawCall(IFrameBuffer* /*frame*/, const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) { return true; }
				};
			}

			/// <summary>
			/// Adapts a view to the engine's IFrameDraw interface.
			///
			/// Wraps a view that implements DrawCall(frame, frameTime, frameCounter) and exposes
			/// enable/disable control through IFrameDraw.
			/// </summary>
			/// <typeparam name="ViewType">Child view type implementing DrawCall().</typeparam>
			template<typename ViewType = Contract::View>
			class FrameAdapter : public IFrameDraw
			{
			public:
				/// <summary>
				/// Wrapped view instance.
				/// Exposed for direct configuration by the owner.
				/// </summary>
				ViewType ViewInstance{};

			private:
				bool Enabled = true;

			public:
				FrameAdapter() : IFrameDraw()
				{
				}

				~FrameAdapter() = default;

				/// <summary>
				/// Forwards DrawCall to the wrapped view when enabled.
				/// </summary>
				/// <param name="frame">Target framebuffer to draw into.</param>
				/// <param name="frameTime">Rolling frame timestamp (microseconds).</param>
				/// <param name="frameCounter">Rolling frame counter.</param>
				/// <returns>True when the wrapped view has completed its current draw cycle.</returns>
				bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) override
				{
					return Enabled ? ViewInstance.DrawCall(frame, frameTime, frameCounter) : true;
				}

				/// <summary>Returns whether the adapter is enabled.</summary>
				bool IsEnabled() const override { return Enabled; }

				/// <summary>Enables or disables DrawCall forwarding.</summary>
				void SetEnabled(const bool enabled) override { Enabled = enabled; }
			};
		}
	}
}
#endif