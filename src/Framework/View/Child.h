#ifndef _EGFX_FRAMEWORK_VIEW_CHILD_h
#define _EGFX_FRAMEWORK_VIEW_CHILD_h

#include "../Layout/Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace View
		{
			namespace Detail
			{
				template<typename OwnerLayout, typename ParentLayout>
				struct ChildBounds
				{
					template<typename ChildType>
					static void Apply(ChildType& child, const int16_t left, const int16_t top,
						const int16_t right, const int16_t bottom)
					{
						const int16_t ownerLeft = OwnerLayout::X();
						const int16_t ownerTop = OwnerLayout::Y();
						const int16_t ownerRight = ownerLeft + OwnerLayout::Width() - 1;
						const int16_t ownerBottom = ownerTop + OwnerLayout::Height() - 1;
						const int16_t parentLeft = ParentLayout::X();
						const int16_t parentTop = ParentLayout::Y();
						const int16_t parentRight = parentLeft + ParentLayout::Width() - 1;
						const int16_t parentBottom = parentTop + ParentLayout::Height() - 1;

						const int16_t clippedLeft = left > ownerLeft ? left : ownerLeft;
						const int16_t clippedTop = top > ownerTop ? top : ownerTop;
						const int16_t clippedRight = right < ownerRight ? right : ownerRight;
						const int16_t clippedBottom = bottom < ownerBottom ? bottom : ownerBottom;
						const int16_t childLeft = clippedLeft > parentLeft ? clippedLeft : parentLeft;
						const int16_t childTop = clippedTop > parentTop ? clippedTop : parentTop;
						const int16_t childRight = clippedRight < parentRight ? clippedRight : parentRight;
						const int16_t childBottom = clippedBottom < parentBottom ? clippedBottom : parentBottom;

						child.SetBounds(childLeft, childTop, childRight, childBottom);
					}
				};

				template<typename ChildType, typename OwnerLayout, typename ParentLayout>
				class LayoutChild
					: public ChildType
				{
				public:
					using ChildType::ChildType;

					void SetBounds(const int16_t left, const int16_t top,
						const int16_t right, const int16_t bottom)
					{
						ChildBounds<OwnerLayout, ParentLayout>::template Apply<ChildType>(*this,
							left, top, right, bottom);
					}

					void SetTranslation(const int16_t x, const int16_t y)
					{
						ChildType::SetTranslation(x, y);
					}

					int16_t GetBoundsWidth() const
					{
						return ChildType::GetBoundsWidth();
					}

					int16_t GetBoundsHeight() const
					{
						return ChildType::GetBoundsHeight();
					}

					int16_t GetBoundsLeft() const
					{
						return ChildType::GetBoundsLeft();
					}

					int16_t GetBoundsTop() const
					{
						return ChildType::GetBoundsTop();
					}

					int16_t GetBoundsRight() const
					{
						return ChildType::GetBoundsRight();
					}

					int16_t GetBoundsBottom() const
					{
						return ChildType::GetBoundsBottom();
					}

					int16_t GetTranslationX() const
					{
						return ChildType::GetTranslationX();
					}

					int16_t GetTranslationY() const
					{
						return ChildType::GetTranslationY();
					}
				};
			}
		}
	}
}
#endif
