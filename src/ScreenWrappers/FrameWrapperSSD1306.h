//#ifndef _INTEGERGLASS_FRAME_WRAPPER_SSD1306_h
//#define _INTEGERGLASS_FRAME_WRAPPER_SSD1306_h
//
//
//#include "../Model/RgbColor.h"
//#include "../FrameBuffer/AbstractFrameBuffer.h"
//
//#include "../ScreenDrivers/ScreenDriverSSD1306_I2C.h"
//
//namespace IntegerGlass
//{
//	/// <summary>
//	/// TODO: Accelerate rects and triangles: https://community.arduboy.com/t/ssd1306-hardware-accelerated-rectangles-undocumented-command-0x24/11108/5
//	/// </summary>
//	/// <typeparam name="ColorConverter"></typeparam>
//	/// <typeparam name="width"></typeparam>
//	/// <typeparam name="height"></typeparam>
//	/// <typeparam name="verticalOffset"></typeparam>
//	/// <typeparam name="i2cAddress"></typeparam>
//	/// <typeparam name="displayOptions"></typeparam>
//	template<const uint8_t width, const uint8_t height, const int8_t verticalOffset
//		, const uint8_t i2cAddress
//		, typename ColorConverter = BinaryColorConverter1
//		, typename displayOptions = DisplayOptions::Default>
//	class AbstractFrameWrapperSSD1306_I2C
//		: public IFrameBuffer
//		, public AbstractScreenDriverSSD1306_I2C<width, height, verticalOffset, i2cAddress>
//	{
//	public:
//		static constexpr pixel_t FrameWidth = width;
//		static constexpr pixel_t FrameHeight = height;
//
//	private:
//		using BaseClass = AbstractScreenDriverSSD1306_I2C<width, height, verticalOffset, i2cAddress>;
//
//		enum class FlipStateEnum
//		{
//			A,
//			B
//		};
//
//	protected:
//		using BaseClass::WireInstance;
//		using BaseClass::I2CAddress;
//		using BaseClass::CommandStart;
//		using BaseClass::CommandEnd;
//
//	protected:
//		FlipStateEnum FlipState = FlipStateEnum::A;
//
//		static constexpr uint8_t FlipOffset = 32;
//
//	protected:
//		bool Inverted = false;
//
//	public:
//		AbstractFrameWrapperSSD1306_I2C(TwoWire& wire)
//			: IFrameBuffer()
//			, BaseClass(wire)
//		{
//		}
//
//		/// <summary>
//		/// Framebuffer implementation.
//		/// </summary>
//	public:
//		/// <summary>
//		/// No in-memory framebuffer.
//		/// </summary>
//		/// <returns>nullptr always.</returns>
//		virtual uint8_t* GetFrameBuffer() final
//		{
//			return nullptr;
//		}
//
//		/// <summary>
//		/// Clear the "frame buffer".
//		/// </summary>
//		virtual const bool ClearFrameBuffer()
//		{
//			//SetStartBlock(0, 0);
//			CommandStart();
//
//			WireInstance.write(0);
//			WireInstance.write(0);
//			WireInstance.write(8);
//
//			for (uint_fast8_t y = 0; y < height / 8; y++)
//			{
//				for (uint_fast8_t x = 0; x < width / 8; x++)
//				{
//					WireInstance.write(0);
//				}
//			}
//
//			CommandEnd();
//
//			return true;
//		}
//
//		/// <summary>
//		/// Fill the whole frame with the provided color.
//		/// </summary>
//		/// <param name="color"></param>
//		virtual void Fill(const rgb_color_t color)
//		{
//			//TODO: similar to clear frame buffer.
//		}
//
//		/// <summary>
//		/// Draw a single pixel.
//		/// </summary>
//		/// <param name="color"></param>
//		/// <param name="x"></param>
//		/// <param name="y"></param>
//		virtual void Pixel(const rgb_color_t color, const pixel_t x, const pixel_t y) final
//		{
//			const pixel_point_t point{ x,y };
//			Pixel(color, point);
//		}
//
//		virtual void Pixel(const rgb_color_t color, const pixel_point_t point) final
//		{
//			if (point.x >= 0 && point.x < width &&
//				point.y >= 0 && point.y < height)
//			{
//				const bool rawColor = ColorConverter::GetRawColor(color);
//
//				switch (displayOptions::Mirror)
//				{
//				case DisplayOptions::MirrorEnum::MirrorX:
//					PixelRaw(rawColor, MirrorX(point.x), point.y);
//					break;
//				case DisplayOptions::MirrorEnum::MirrorY:
//					PixelRaw(rawColor, point.x, MirrorY(point.y));
//					break;
//				case DisplayOptions::MirrorEnum::MirrorXY:
//					PixelRaw(rawColor, MirrorX(point.x), MirrorY(point.y));
//					break;
//				case DisplayOptions::MirrorEnum::None:
//				default:
//					PixelRaw(rawColor, point.x, point.y);
//					break;
//				}
//			}
//		}
//
//		/// <summary>
//		/// Draw a line. 
//		/// Special orientations (horizontal and vertical) are internally optimized.
//		/// </summary>
//		/// <param name="color"></param>
//		/// <param name="x1"></param>
//		/// <param name="y1"></param>
//		/// <param name="x2"></param>
//		/// <param name="y2"></param>
//		virtual void Line(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) final
//		{
//			const pixel_line_t line{ {x1, y1}, {x2, y2} };
//
//			Line(color, line);
//		}
//
//		virtual void Line(const rgb_color_t color, const pixel_line_t& line) final
//		{
//			const bool rawColor = ColorConverter::GetRawColor(color);
//
//			pixel_t startX;
//			pixel_t startY;
//			pixel_t endX;
//			pixel_t endY;
//
//			// Apply mirroring.
//			switch (displayOptions::Mirror)
//			{
//			case DisplayOptions::MirrorEnum::MirrorX:
//				startX = MirrorX(line.start.x);
//				endX = MirrorX(line.end.x);
//				startY = line.start.y;
//				endY = line.end.y;
//				break;
//			case DisplayOptions::MirrorEnum::MirrorY:
//				startX = line.start.x;
//				startY = MirrorY(line.start.y);
//				endX = line.end.x;
//				endY = MirrorY(line.end.y);
//				break;
//			case DisplayOptions::MirrorEnum::MirrorXY:
//				startX = MirrorX(line.start.x);
//				startY = MirrorY(line.start.y);
//				endX = MirrorX(line.end.x);
//				endY = MirrorY(line.end.y);
//				break;
//			case DisplayOptions::MirrorEnum::None:
//			default:
//				startX = line.start.x;
//				startY = line.start.y;
//				endX = line.end.x;
//				endY = line.end.y;
//				break;
//			}
//
//			if (startX == endX)
//			{
//				if (startX >= 0 && startX < width)
//				{
//					if (startY == endY)
//					{
//						if (startY >= 0 && startY < height)
//						{
//							PixelRaw(rawColor, startX, startY);
//						}
//					}
//					else if (endY > startY)
//					{
//						startY = MaxValue((pixel_t)0, (pixel_t)(startY));
//						endY = MinValue((pixel_t)(height - 1), (pixel_t)(endY));
//						//CommandVLine(startX, startY, endY, rawColor);
//					}
//					else
//					{
//						endY = MaxValue((pixel_t)0, (pixel_t)(endY));
//						startY = MinValue((pixel_t)(height - 1), (pixel_t)(startY));
//						//CommandVLine(startX, endY, startY, rawColor);
//					}
//				}
//			}
//			else if (startY == endY)
//			{
//				if (startY >= 0 && startY < height)
//				{
//					if (endX > startX)
//					{
//						startX = MaxValue((pixel_t)0, (pixel_t)(startX));
//						endX = MinValue((pixel_t)(width - 1), (pixel_t)(endX));
//						LineHorizontalRaw(rawColor, startX, startY, endX);
//						//CommandHLine(startX, startY, endX, rawColor);
//					}
//					else
//					{
//						endX = MaxValue((pixel_t)0, (pixel_t)(endX));
//						startX = MinValue((pixel_t)(width - 1), (pixel_t)(startX));
//						LineHorizontalRaw(rawColor, endX, startY, startX);
//						//CommandHLine(endX, startY, startX, rawColor);
//					}
//				}
//			}
//			else
//			{
//				// No bounds checking for BresenhamDiagonal
//				//BresenhamDiagonal({ startX, startY }, { endX, endY }, rawColor);
//			}
//		}
//
//		/// <summary>
//		/// Draw a rectangle outline.
//		/// </summary>
//		/// <param name="color"></param>
//		/// <param name="x1"></param>
//		/// <param name="y1"></param>
//		/// <param name="x2"></param>
//		/// <param name="y2"></param>
//		virtual void Rectangle(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) {}
//		virtual void Rectangle(const rgb_color_t color, const pixel_rectangle_t& rectangle) {}
//
//		/// <summary>
//		/// Draw a full rectangle.
//		/// </summary>
//		/// <param name="color"></param>
//		/// <param name="x1"></param>
//		/// <param name="y1"></param>
//		/// <param name="x2"></param>
//		/// <param name="y2"></param>
//		virtual void RectangleFill(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) {}
//		virtual void RectangleFill(const rgb_color_t color, const pixel_rectangle_t& rectangle) {}
//
//		/// <summary>
//		/// Screen Driver implementation.
//		/// </summary>
//	public:
//		virtual const bool Start()
//		{
//			return BaseClass::Start();
//		}
//
//		virtual const bool CanPushBuffer()
//		{
//			return true;
//		}
//
//		virtual void StartBuffer()
//		{
//			// Update inverted status.
//			//CommandSetInverted(Inverted);
//
//			// Update mirroring.
//			//CommandSetRemap();
//			CommandSetVerticalOffset(0);
//			return;
//			// Flip current drawing frame buffer.
//			switch (FlipState)
//			{
//			case FlipStateEnum::B:
//				// Was drawing to B, show B and flip drawing to A.
//				FlipState = FlipStateEnum::A;
//				CommandSetVerticalOffset(FlipOffset);
//				break;
//			case FlipStateEnum::A:
//			default:
//				// Was drawing to A, show A and flip drawing to B.
//				FlipState = FlipStateEnum::B;
//				CommandSetVerticalOffset(0);
//				break;
//			}
//		}
//
//		virtual const uint32_t PushBuffer(const uint8_t* frameBuffer)
//		{
//			return 0;
//		}
//
//		virtual const bool PushingBuffer(const uint8_t* frameBuffer)
//		{
//			return false;
//		}
//
//		virtual void EndBuffer()
//		{
//		}
//
//	protected:
//		pixel_t MirrorX(const pixel_t x) const { return FrameWidth - 1 - x; }
//		pixel_t MirrorY(const pixel_t y) const { return FrameHeight - 1 - y; }
//
//		void CommandSetVerticalOffset(const uint8_t offset)
//		{
//			CommandStart();
//			WireInstance.write((uint8_t)SSD1306::CommandEnum::CommandStart);
//			WireInstance.write((uint8_t)SSD1306::CommandEnum::SetLineStart);
//			WireInstance.write(0x40 + offset);// Vertical offset with no remap.
//			CommandEnd();
//		}
//
//		void SetStartBlock(const uint8_t offsetX, const uint8_t offsetY)
//		{
//			WireInstance.write((uint8_t)SSD1306::CommandEnum::Column);
//			WireInstance.write(0);
//			WireInstance.write(0 + width - 1);
//			WireInstance.write((uint8_t)SSD1306::CommandEnum::Page);
//
//			switch (FlipState)
//			{
//			case FlipStateEnum::B:
//				// Drawing to B.
//				WireInstance.write((FlipOffset / 8) + verticalOffset);
//				WireInstance.write((FlipOffset / 8) + (SSD1306::Height / 8) - 1);
//				break;
//			case FlipStateEnum::A:
//			default:
//				// Drawing to A.
//				WireInstance.write(verticalOffset);
//				WireInstance.write((SSD1306::Height / 8) - 1);
//				break;
//			}
//		}
//
//		void PixelRaw(bool color, const uint8_t x, const uint8_t y)
//		{
//			CommandStart();
//
//			//SetStartBlock(x, y);
//			WireInstance.write(x);
//			WireInstance.write(y >> 3);
//			WireInstance.write(1);
//			WireInstance.write(((color > 0) << (y & 0x07)));
//			CommandEnd();
//		}
//
//		void LineHorizontalRaw(const bool rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2)
//		{
//			CommandStart();
//			//SetStartBlock(x1, y);
//
//			WireInstance.write(x1);
//			WireInstance.write(y >> 3);
//			//WireInstance.write(x2 - x1 + 1);
//			WireInstance.write(1);
//			//WireInstance.write((uint8_t)rawColor);
//
//			//for (uint8_t x = x1; x <= x2; x++)
//			//{
//			//	WireInstance.write((rawColor << (y & 0x07)));
//			//}
//			const uint8_t yByte = y >> 3;
//			const uint8_t yBit = y % 8;
//
//			const size_t offset = ((width)*yByte) + x1;
//
//			if (rawColor > 0)
//			{
//				for (uint_fast8_t i = 0; i <= x2 - x1; i++)
//				{
//					WireInstance.write((uint8_t)rawColor);
//				}
//			}
//			else
//			{
//				for (uint_fast8_t i = 0; i <= x2 - x1; i++)
//				{
//					WireInstance.write(~(uint8_t)rawColor);
//				}
//			}
//
//			CommandEnd();
//		}
//
//		void CommandHLine(const uint8_t x1, const uint8_t y, const uint8_t x2, bool color)
//		{
//			CommandStart();
//			//SetStartBlock(x1, y);
//
//			WireInstance.write(x1);
//			WireInstance.write(y >> 3);
//			WireInstance.write(x2 - x1 + 1);
//			for (uint8_t x = x1; x <= x2; x++)
//			{
//				WireInstance.write((color << (y & 0x07)));
//			}
//			CommandEnd();
//		}
//	};
//
//	template<typename ColorConverter = BinaryColorConverter1
//		, typename displayOptions = DisplayOptions::Default>
//	class FrameWrapperSSD1306_64x32x1_I2C : public AbstractFrameWrapperSSD1306_I2C<SSD1306_64x32::Width, SSD1306_64x32::Height, 32, SSD1306::I2CAddress, ColorConverter, displayOptions>
//	{
//	private:
//		using BaseClass = AbstractFrameWrapperSSD1306_I2C<SSD1306_64x32::Width, SSD1306_64x32::Height, 32, SSD1306::I2CAddress, ColorConverter, displayOptions>;
//
//	public:
//		FrameWrapperSSD1306_64x32x1_I2C(TwoWire& wire) : BaseClass(wire) {}
//
//		virtual const bool Start()
//		{
//			return BaseClass::Start() && BaseClass::Initialize(false);
//		}
//	};
//}
//#endif