#ifndef _EGFX_FRAMEWORK_ASSETS_FONT_VECTOR_EPOXY_MODEL_h
#define _EGFX_FRAMEWORK_ASSETS_FONT_VECTOR_EPOXY_MODEL_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Font
			{
				namespace Vector
				{
					namespace Epoxy
					{
						using namespace Framework::Vector;
						using namespace Framework::Vector::Font;

						// 8x8 Character Set type.
						using CharacterSetType = CharacterSet8x8;

						// Vector model for this character set (8x8).
						using VectorModel = CharacterSet8x8::VectorModel;

						// Convenience function to create an 8x8 node.
						static constexpr VectorModel::node_t Node(const NodeEnum primitive, const uint8_t x, const uint8_t y)
						{
							return Node8x8(primitive, x, y);
						}

						// Style dimensions for Epoxy font characters.
						namespace Dimensions
						{
							static constexpr uint8_t AxisMax = VectorModel::AxisMax;
							static constexpr uint8_t AxisCenter = AxisMax / 2;
							static constexpr uint8_t FalseBottom = AxisMax - 1;

							namespace Single
							{
								static constexpr uint8_t Width = 1;
								static constexpr uint8_t EndX = Width - 1;
								static constexpr uint8_t CenterX = Width / 2;
							}

							namespace Double
							{
								static constexpr uint8_t Width = 2;
								static constexpr uint8_t EndX = Width - 1;
								static constexpr uint8_t CenterX = Width / 2;
							}

							namespace Triple
							{
								static constexpr uint8_t Width = 3;
								static constexpr uint8_t EndX = Width - 1;
								static constexpr uint8_t CenterX = Width / 2;
							}

							namespace Lowercase
							{
								static constexpr uint8_t Width = 4;
								static constexpr uint8_t EndX = Width - 1;
								static constexpr uint8_t CenterX = Width / 2;
							}

							namespace Small
							{
								static constexpr uint8_t Width = 5;
								static constexpr uint8_t EndX = Width - 1;
								static constexpr uint8_t CenterX = Width / 2;
							}

							namespace Big
							{
								static constexpr uint8_t Width = 6;
								static constexpr uint8_t EndX = Width - 1;
								static constexpr uint8_t CenterX = Width / 2;
							}

							namespace Largest
							{
								static constexpr uint8_t Width = 7;
								static constexpr uint8_t EndX = Width - 1;
								static constexpr uint8_t CenterX = Width / 2;
							}
						}

						// Static font properties. Must be declared for use in VectorFont::TextDrawer.
						static constexpr CharacterSetType::VectorModel::axis_t SpaceWidth = Dimensions::Single::Width;
						static constexpr CharacterSetType::VectorModel::axis_t TabWidth = Dimensions::Triple::Width;
						static constexpr bool Monospace = false;
					}
				}
			}
		}
	}
}
#endif