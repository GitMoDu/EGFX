#ifndef _EGFX_FRAMEWORK_VECTOR_DECODER_h
#define _EGFX_FRAMEWORK_VECTOR_DECODER_h

#include "Model.h"
#include "Reader.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Vector
		{
			using namespace Framework::Vector;
			using namespace IntegerSignal::FixedPoint::FactorScale;

			template<typename dimension_t,
				typename DecoderBaseType
			>
			class Decoder : public DecoderBaseType
			{
			private:
				using Base = DecoderBaseType;

			protected:
				using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;
				using signed_wide_t = typename AutoDimension::ByDimension<dimension_t>::signed_wide_t;
				using wide_t = typename AutoDimension::ByDimension<dimension_t>::wide_t;
				using thickness_t = typename IntegerSignal::TypeTraits::TypeNext::next_int_type<wide_t>::type;
				static constexpr thickness_t ThicknessUnit = UFraction8::FRACTION_1X;

				struct coordinates_t
				{
					signed_t x;
					signed_t y;
				};

			public:
				Decoder() : Base() {}
				Decoder(const dimension_t left, const dimension_t top,
					const dimension_t right, const dimension_t bottom)
					: Base(left, top, right, bottom) {}
				~Decoder() = default;

				template<typename Node, Framework::DataSourceTypeEnum dataSourceType = Framework::DataSourceTypeEnum::Ram>
				void Decode(IFrameBuffer* frame, const typename Node::packed_t* data, const size_t count,
					const dimension_t canvasWidth, const dimension_t canvasHeight,
					const ufraction8_t thicknessScale,
					const dimension_t width, const dimension_t height,
					const dimension_t inset = 0)
				{
					if (frame == nullptr || data == nullptr || count == 0 || canvasWidth == 0 || canvasHeight == 0
						|| width == 0 || height == 0)
						return;

					const dimension_t insetX = GetInsetPixels(inset, width, height, canvasWidth, canvasHeight, thicknessScale);
					const dimension_t insetY = GetInsetPixels(inset, height, width, canvasHeight, canvasWidth, thicknessScale);
					const dimension_t drawWidth = width - static_cast<dimension_t>(insetX * 2);
					const dimension_t drawHeight = height - static_cast<dimension_t>(insetY * 2);

					const Scale16::factor_t scaleX = Scale16::GetFactor<dimension_t>(drawWidth, canvasWidth);
					const Scale16::factor_t scaleY = Scale16::GetFactor<dimension_t>(drawHeight, canvasHeight);

					const Scale16::factor_t renderScale = MinValue(scaleX, scaleY);
					dimension_t runtimeThickness = GetScaledThickness(renderScale, thicknessScale, 0);

					coordinates_t previous{};
					bool hasPrevious = false;

					// Walk through the source data and decode each node, calling the base class methods to draw the primitives.
					Base::OnColorSet(0);
					for (size_t index = 0; index < count; ++index)
					{
						const Node node = ReadNode<Node, dataSourceType>(data, index);
						if (!node.IsOpCode())
						{
							const coordinates_t point{
								static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleX, node.x) + insetX),
								static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleY, node.y) + insetY)
							};

							if (hasPrevious)
								Base::OnDrawLine(frame,
									static_cast<dimension_t>(previous.x), static_cast<dimension_t>(previous.y),
									static_cast<dimension_t>(point.x), static_cast<dimension_t>(point.y), runtimeThickness);
							else
								Base::OnDrawPoint(frame,
									static_cast<dimension_t>(point.x), static_cast<dimension_t>(point.y), runtimeThickness);
							previous = point;
							hasPrevious = true;
							continue;
						}

						const auto command = static_cast<PrimitiveEnum>(node.opcode);
						hasPrevious = false;
						if (command == PrimitiveEnum::Break)
							continue;
						else if (command == PrimitiveEnum::SetWeight)
						{
							Node parameter{ 0, 0, 0 };
							if (ReadOperand<Node, dataSourceType>(data, count, index, parameter))
								runtimeThickness = GetScaledThickness(renderScale, thicknessScale, parameter.x);
						}
						else if (command == PrimitiveEnum::SetColor)
						{
							Node parameter{ 0, 0, 0 };
							if (ReadOperand<Node, dataSourceType>(data, count, index, parameter))
								Base::OnColorSet(static_cast<uint8_t>(parameter.x));
						}
						else if (command == PrimitiveEnum::Rectangle || command == PrimitiveEnum::RectangleFill)
						{
							Node first{ 0, 0, 0 };
							Node corner{ 0, 0, 0 };
							if (ReadOperand<Node, dataSourceType>(data, count, index, first) && ReadOperand<Node, dataSourceType>(data, count, index, corner))
							{
								const coordinates_t firstPoint{
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleX, first.x) + insetX),
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleY, first.y) + insetY)
								};
								const coordinates_t cornerPoint{
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleX, corner.x) + insetX),
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleY, corner.y) + insetY)
								};
								Base::OnDrawRectangle(frame,
									static_cast<dimension_t>(firstPoint.x), static_cast<dimension_t>(firstPoint.y),
									static_cast<dimension_t>(cornerPoint.x), static_cast<dimension_t>(cornerPoint.y),
									command == PrimitiveEnum::RectangleFill, runtimeThickness);
							}
						}
						else if (command == PrimitiveEnum::Triangle || command == PrimitiveEnum::TriangleFill)
						{
							Node first{ 0, 0, 0 };
							Node second{ 0, 0, 0 };
							Node third{ 0, 0, 0 };
							if (ReadOperand<Node, dataSourceType>(data, count, index, first) && ReadOperand<Node, dataSourceType>(data, count, index, second) && ReadOperand<Node, dataSourceType>(data, count, index, third))
							{
								const coordinates_t firstPoint{
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleX, first.x) + insetX),
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleY, first.y) + insetY)
								};
								const coordinates_t secondPoint{
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleX, second.x) + insetX),
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleY, second.y) + insetY)
								};
								const coordinates_t thirdPoint{
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleX, third.x) + insetX),
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleY, third.y) + insetY)
								};
								Base::OnDrawTriangle(frame,
									static_cast<dimension_t>(firstPoint.x), static_cast<dimension_t>(firstPoint.y),
									static_cast<dimension_t>(secondPoint.x), static_cast<dimension_t>(secondPoint.y),
									static_cast<dimension_t>(thirdPoint.x), static_cast<dimension_t>(thirdPoint.y),
									command == PrimitiveEnum::TriangleFill, runtimeThickness);
							}
						}
						else if (command == PrimitiveEnum::Circle || command == PrimitiveEnum::CircleFill)
						{
							Node center{ 0, 0, 0 };
							Node edge{ 0, 0, 0 };
							if (ReadOperand<Node, dataSourceType>(data, count, index, center) && ReadOperand<Node, dataSourceType>(data, count, index, edge))
							{
								const coordinates_t centerPoint{
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleX, center.x) + insetX),
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleY, center.y) + insetY)
								};
								const coordinates_t edgePoint{
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleX, edge.x) + insetX),
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleY, edge.y) + insetY)
								};
								Base::OnDrawCircle(frame,
									static_cast<dimension_t>(centerPoint.x), static_cast<dimension_t>(centerPoint.y),
									GetRadius(centerPoint, edgePoint), command == PrimitiveEnum::CircleFill, runtimeThickness);
							}
						}
						else if (command == PrimitiveEnum::Arc || command == PrimitiveEnum::ArcFill)
						{
							Node center{ 0, 0, 0 };
							Node radiusAndProperties{ 0, 0, 0 };
							Node angles{ 0, 0, 0 };
							if (ReadOperand<Node, dataSourceType>(data, count, index, center) && ReadOperand<Node, dataSourceType>(data, count, index, radiusAndProperties) && ReadOperand<Node, dataSourceType>(data, count, index, angles))
							{
								const coordinates_t centerPoint{
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleX, center.x) + insetX),
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleY, center.y) + insetY)
								};
								Base::OnDrawCircleArc(frame,
									static_cast<dimension_t>(centerPoint.x), static_cast<dimension_t>(centerPoint.y),
									ScaleAngle<Node>(angles.x), ScaleAngle<Node>(angles.y),
									static_cast<dimension_t>(Scale16::Scale<dimension_t>(scaleX, radiusAndProperties.x)),
									command == PrimitiveEnum::ArcFill
									? static_cast<dimension_t>(Scale16::Scale<dimension_t>(scaleX, radiusAndProperties.y))
									: 0,
									command == PrimitiveEnum::ArcFill, runtimeThickness);
							}
						}
						else if (command == PrimitiveEnum::Ring || command == PrimitiveEnum::RingFill)
						{
							Node center{ 0, 0, 0 };
							Node radii{ 0, 0, 0 };
							if (ReadOperand<Node, dataSourceType>(data, count, index, center) && ReadOperand<Node, dataSourceType>(data, count, index, radii))
							{
								const coordinates_t centerPoint{
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleX, center.x) + insetX),
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleY, center.y) + insetY)
								};
								Base::OnDrawCircleRing(frame,
									static_cast<dimension_t>(centerPoint.x), static_cast<dimension_t>(centerPoint.y),
									static_cast<dimension_t>(Scale16::Scale<dimension_t>(scaleX, radii.x)),
									static_cast<dimension_t>(Scale16::Scale<dimension_t>(scaleY, radii.y)),
									command == PrimitiveEnum::RingFill, runtimeThickness);
							}
						}
						else if (command == PrimitiveEnum::CircleQuadrant || command == PrimitiveEnum::CircleQuadrantFill)
						{
							Node center{ 0, 0, 0 };
							Node radiusAndProperties{ 0, 0, 0 };
							if (ReadOperand<Node, dataSourceType>(data, count, index, center) && ReadOperand<Node, dataSourceType>(data, count, index, radiusAndProperties))
							{
								const coordinates_t centerPoint{
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleX, center.x) + insetX),
									static_cast<signed_t>(Scale16::Scale<dimension_t>(scaleY, center.y) + insetY)
								};
								Base::OnDrawCircleQuadrant(frame,
									static_cast<dimension_t>(centerPoint.x), static_cast<dimension_t>(centerPoint.y),
									static_cast<dimension_t>(Scale16::Scale<dimension_t>(scaleX, radiusAndProperties.x)),
									static_cast<QuadrantEnum>(radiusAndProperties.y & 0x03),
									command == PrimitiveEnum::CircleQuadrantFill, runtimeThickness);
							}
						}
					}
				}

				template<PackedVectorEnum vectorType,
					Framework::DataSourceTypeEnum dataSourceType = Framework::DataSourceTypeEnum::Ram>
				void Decode(IFrameBuffer* frame, const void* data, const size_t count,
					const dimension_t canvasWidth, const dimension_t canvasHeight,
					const ufraction8_t thicknessScale,
					const dimension_t width, const dimension_t height,
					const dimension_t inset = 0)
				{
					using node_t = typename Format<vectorType>::node_t;
					Decode<node_t, dataSourceType>(frame, static_cast<const typename node_t::packed_t*>(data), count,
						canvasWidth, canvasHeight, thicknessScale, width, height, inset);
				}

			protected:
				template<typename Node>
				static angle_t ScaleAngle(const typename Node::packed_t value)
				{
					const uint32_t axisMax = sizeof(typename Node::packed_t) == 1 ? 14u : 254u;
					return static_cast<angle_t>((static_cast<uint32_t>(value) * static_cast<uint32_t>(ANGLE_RANGE)) / axisMax);
				}

				static dimension_t GetRadius(const coordinates_t center, const coordinates_t edge)
				{
					const signed_wide_t delta = static_cast<signed_wide_t>(edge.x) - static_cast<signed_wide_t>(center.x);
					return static_cast<dimension_t>(AbsValue(delta));
				}

				static dimension_t GetScaledThickness(const Scale16::factor_t renderScale,
					const ufraction8_t thicknessScale,
					const uint8_t weight)
				{
					return 1 + UFraction8::Fraction<uint16_t>(thicknessScale,
						Scale16::Scale<uint16_t>(renderScale, 1 + weight));
				}

				static dimension_t GetScaledThickness(const Scale8::factor_t renderScale,
					const ufraction8_t thicknessScale,
					const uint8_t weight)
				{
					return 1 + UFraction8::Fraction<uint16_t>(thicknessScale,
						Scale8::Scale<uint16_t>(renderScale, 1 + weight));
				}

				static dimension_t GetInsetPixels(const dimension_t edgeWeight, const dimension_t output,
					const dimension_t otherOutput, const dimension_t source,
					const dimension_t otherSource, const ufraction8_t thicknessScale)
				{
					if (output <= 1 || source <= 1 || edgeWeight == 0)
						return 0;

					const dimension_t maximum = static_cast<dimension_t>((output - 1) / 2);
					const auto scale = MaxValue(
						Scale16::GetFactor<dimension_t>(output - 1, source - 1),
						Scale16::GetFactor<dimension_t>(otherOutput - 1, otherSource - 1));
					const dimension_t effectiveThickness = GetScaledThickness(
						scale, thicknessScale, static_cast<uint8_t>(edgeWeight - 1));
					const dimension_t requiredRadius = effectiveThickness > 1
						? static_cast<dimension_t>((effectiveThickness - 1) / 2) : 0;
					return requiredRadius > maximum ? maximum : requiredRadius;
				}

				template<typename Node, Framework::DataSourceTypeEnum dataSourceType>
				static Node ReadNode(const typename Node::packed_t* data, const size_t index)
				{
					return Node::FromPacked(ReaderType<dataSourceType>::template ReadPacked(data, index));
				}

				template<typename Node, Framework::DataSourceTypeEnum dataSourceType>
				static bool ReadOperand(const typename Node::packed_t* data, const size_t count, size_t& index, Node& operand)
				{
					if (++index >= count)
						return false;
					operand = ReadNode<Node, dataSourceType>(data, index);
					return !operand.IsOpCode();
				}
			};
		}
	}
}
#endif