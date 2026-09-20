#ifndef _INTEGERGLASS_FRAMEWORK_VIEW_CHILD_h
#define _INTEGERGLASS_FRAMEWORK_VIEW_CHILD_h

#include "../Layout/Model.h"

namespace IntegerGlass
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
						const int16_t childOriginX = ParentLayout::X() - OwnerLayout::X();
						const int16_t childOriginY = ParentLayout::Y() - OwnerLayout::Y();
						const int16_t childRight = childOriginX + ParentLayout::Width();
						const int16_t childBottom = childOriginY + ParentLayout::Height();

						if (left >= right || top >= bottom)
						{
							child.SetBounds(0, 0, 0, 0);
							return;
						}

						const int16_t clippedLeft = left > childOriginX ? left : childOriginX;
						const int16_t clippedTop = top > childOriginY ? top : childOriginY;
						const int16_t clippedRight = right < childRight ? right : childRight;
						const int16_t clippedBottom = bottom < childBottom ? bottom : childBottom;

						if (clippedLeft >= clippedRight || clippedTop >= clippedBottom)
						{
							child.SetBounds(0, 0, 0, 0);
							return;
						}

						const int16_t childLeft = clippedLeft - childOriginX;
						const int16_t childTop = clippedTop - childOriginY;
						const int16_t childBoundsRight = clippedRight - childOriginX;
						const int16_t childBoundsBottom = clippedBottom - childOriginY;

						child.SetBounds(childLeft, childTop, childBoundsRight, childBoundsBottom);
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
