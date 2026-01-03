#ifndef _EGFX_FRAMEWORK_VIEW_MODEL_h
#define _EGFX_FRAMEWORK_VIEW_MODEL_h

#include "../../Model/IFrameDraw.h"
#include "../Support.h"

namespace Egfx
{
	/// <summary>
	/// EGFX Framework is a compile-time toolkit for building drawable elements and views.
	/// Elements are passive renderers, Views orchestrate animation and composition.
	/// </summary>
	namespace Framework
	{
		namespace View
		{
			namespace Contract
			{
				/// <summary>
				/// Defines a contract for drawable views that can render to a frame buffer.
				/// Views orchestrate animation and rendering over time.
				/// Can have multiple draw calls to complete their rendering.
				/// </summary>
				struct View
				{
					bool DrawCall(IFrameBuffer* /*frame*/, const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) { return true; }
				};
			}

			/// <summary>
			/// Adapter exposing any view via the engine's IFrameDraw interface.
			/// Wraps views that implement DrawCall(frame, frameTime, frameCounter).
			/// Includes enable/disable control surfaced by IFrameDraw.
			/// </summary>
			template<typename ViewType = Contract::View>
			class FrameAdapter : public IFrameDraw
			{
			public:
				ViewType ViewInstance{};

			private:
				bool Enabled = true;

			public:
				FrameAdapter() : IFrameDraw()
				{
				}

				~FrameAdapter() = default;

				bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) override
				{
					return Enabled ? ViewInstance.DrawCall(frame, frameTime, frameCounter) : true;
				}

				bool IsEnabled() const override { return Enabled; }
				void SetEnabled(const bool enabled) override { Enabled = enabled; }
			};
		}
	}
}
#endif