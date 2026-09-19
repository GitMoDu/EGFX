#ifndef _EGFX_FRAMEWORK_PROJECTION_VIEW_COMPOSITE_LIST_VIEW_h
#define _EGFX_FRAMEWORK_PROJECTION_VIEW_COMPOSITE_LIST_VIEW_h

#include "../Model.h"
#include "../../View/CompositeView.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Projection
		{
			namespace View
			{
				/// <summary>
				/// Generic projection host that snapshots controller state and forwards projector-calculated
				/// bounds and translation to a fixed pack of child views.
				/// </summary>
				/// <typeparam name="ParentLayout">The parent layout within which the child views are arranged.</typeparam>
				/// <typeparam name="ProjectorType">The projector policy that maps controller state to child viewports.</typeparam>
				/// <typeparam name="...ViewTypes">The fixed child view types managed by this host.</typeparam>
				template<typename ParentLayout,
					typename ProjectorType,
					typename... ViewTypes>
				class CompositeListView : public Framework::View::CompositeView<ParentLayout, ViewTypes...>
				{
				private:
					using Base = Framework::View::CompositeView<ParentLayout, ViewTypes...>;
					using ControllerType = typename ProjectorType::ControllerType;
					using StateType = typename ProjectorType::StateType;
					static constexpr uint8_t ChildViewCount = static_cast<uint8_t>(sizeof...(ViewTypes));

					ProjectorType Projector{};
					ControllerType Controller;
					uint8_t LastControllerVersion = 0;
					bool ProjectionInvalidated = true;

					template<uint8_t Index, uint8_t Count>
					struct ProjectionDispatcher
					{
						static void Project(CompositeListView* self, const StateType& state)
						{
							const viewport_t viewport = self->Projector.GetViewport(Index, state);
							auto& view = self->template view<Index>();
							const int16_t childOriginX = static_cast<int16_t>(ProjectorType::ItemLayouter::ViewLayout::X() - ParentLayout::X());
							const int16_t childOriginY = static_cast<int16_t>(ProjectorType::ItemLayouter::ViewLayout::Y() - ParentLayout::Y());
							view.SetTranslation(viewport.TranslationX, viewport.TranslationY);
							view.SetBounds(
								static_cast<int16_t>(viewport.BoundsLeft + childOriginX),
								static_cast<int16_t>(viewport.BoundsTop + childOriginY),
								static_cast<int16_t>(viewport.BoundsRight + childOriginX),
								static_cast<int16_t>(viewport.BoundsBottom + childOriginY));
							ProjectionDispatcher<Index + 1, Count>::Project(self, state);
						}
					};

					template<uint8_t Count>
					struct ProjectionDispatcher<Count, Count>
					{
						static void Project(CompositeListView*, const StateType&) {}
					};

					void ProjectViews(const StateType& state)
					{
						ProjectionDispatcher<0, ChildViewCount>::Project(this, state);
					}

				protected:
					ControllerType& ControllerState()
					{
						return Controller;
					}

					const ControllerType& ControllerState() const
					{
						return Controller;
					}

					bool ViewStep(const uint32_t, const uint16_t) override
					{
						const StateType state = Controller.GetState();
						if (ProjectionInvalidated || state.Version != LastControllerVersion)
						{
							ProjectViews(state);
							LastControllerVersion = state.Version;
							ProjectionInvalidated = false;
						}

						return true;
					}

				public:
					using ViewLayout = ParentLayout;

					CompositeListView()
						: Base(), Projector(), Controller(ChildViewCount)
					{}

					~CompositeListView() = default;

					void InvalidateProjection()
					{
						ProjectionInvalidated = true;
					}
				};
			}
		}
	}
}
#endif