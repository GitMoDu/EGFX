#ifndef _EGFX_BITMASK_FONT_ABSTRACT_FONT_DRAWER_h
#define _EGFX_BITMASK_FONT_ABSTRACT_FONT_DRAWER_h

#include "../Platform/Platform.h"

namespace Egfx
{
	namespace BitmaskFont
	{
		template<typename BitmaskFontType>
		class AbstractBitmaskDrawer
		{
		public:
			using dimension_t = typename BitmaskFontType::dimension_t;

		private:
			using intermediate_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

			static constexpr intermediate_t BitScale = (intermediate_t)BitmaskFontType::Width / 8 + (((intermediate_t)BitmaskFontType::Width % 8) != 0 ? 1 : 0);

		protected:
			dimension_t TabWidth;
			dimension_t SpaceWidth;
			dimension_t KerningWidth;

		public:
			AbstractBitmaskDrawer(const dimension_t tabWidth = 5,
				const dimension_t spaceWidth = 3,
				const dimension_t kerning = 1)
				: TabWidth(tabWidth), SpaceWidth(spaceWidth), KerningWidth(kerning)
			{
			}

			/// <summary>
			/// Gets the pixel width of a space character.
			/// </summary>
			dimension_t GetSpaceWidth() const
			{
				return SpaceWidth;
			}

			/// <summary>
			/// Gets the pixel width of a tab character.
			/// </summary>
			dimension_t GetTabWidth() const
			{
				return TabWidth;
			}

			/// <summary>
			/// Gets the kerning pixel width.
			/// </summary>
			dimension_t GetKerningWidth() const
			{
				return KerningWidth;
			}

		protected:
			bool IsPixelSet(const uint8_t* mask, const dimension_t x, const dimension_t y) const
			{
				const intermediate_t yByte = static_cast<intermediate_t>(y) * BitScale;
				const dimension_t xByte = SignedRightShift(x, 3);
				const intermediate_t offset = yByte + xByte;

				const uint8_t xBit = 7 - (x % 8);

#if defined(ARDUINO_ARCH_AVR)
				if (((pgm_read_byte(&mask[offset]) >> xBit) & 0x01) != 0)
#else
				if (((mask[offset] >> xBit) & 0x01) != 0)
#endif
				{
					return true;
				}
				else
				{
					return false;
				}

				return true;
			}
		};

		template<typename BitmaskFontType>
		class AbstractFontDrawer : public AbstractBitmaskDrawer<BitmaskFontType>
		{
		private:
			using Base = AbstractBitmaskDrawer<BitmaskFontType>;

		public:
			using typename Base::dimension_t;

		protected:
			using Base::SpaceWidth;
			using Base::TabWidth;
			using Base::KerningWidth;
			using Base::IsPixelSet;

		public:
			AbstractFontDrawer(const dimension_t tabWidth = 5,
				const dimension_t spaceWidth = 3,
				const dimension_t kerning = 1)
				: Base(tabWidth, spaceWidth, kerning)
			{
			}

			void SetKerningWidth(const dimension_t kerning)
			{
				KerningWidth = kerning;
			}

			void SetSpaceWidth(const dimension_t spaceWidth)
			{
				SpaceWidth = spaceWidth;
			}

			void SetTabWidth(const dimension_t tabWidth)
			{
				TabWidth = tabWidth;
			}

			/// <summary>
			/// Gets the pixel width of the font.
			/// </summary>
			static constexpr dimension_t GetFontWidth()
			{
				return BitmaskFontType::Width;
			}

			/// <summary>
			/// Gets the pixel height of the font.
			/// </summary>
			static constexpr dimension_t GetFontHeight()
			{
				return BitmaskFontType::Height;
			}
		};

		template<typename BitmaskFontType,
			uint8_t ScaleX,
			uint8_t ScaleY = ScaleX>
		class AbstractScaledFontDrawer : public AbstractBitmaskDrawer<BitmaskFontType>
		{
		private:
			using Base = AbstractBitmaskDrawer<BitmaskFontType>;

		public:
			using typename Base::dimension_t;

		protected:
			using Base::SpaceWidth;
			using Base::TabWidth;
			using Base::KerningWidth;
			using Base::IsPixelSet;

		public:
			AbstractScaledFontDrawer(const dimension_t tabWidth = 5,
				const dimension_t spaceWidth = 3,
				const dimension_t kerning = 1)
				: Base(tabWidth* ScaleX, spaceWidth* ScaleX, kerning* ScaleX)
			{
			}

			void SetKerningWidth(const dimension_t kerning)
			{
				KerningWidth = kerning * ScaleX;
			}

			void SetSpaceWidth(const dimension_t spaceWidth)
			{
				SpaceWidth = spaceWidth * ScaleX;
			}

			void SetTabWidth(const dimension_t tabWidth)
			{
				TabWidth = tabWidth * ScaleX;
			}

			/// <summary>
			/// Gets the pixel width of the font.
			/// </summary>
			static constexpr dimension_t GetFontWidth()
			{
				return BitmaskFontType::Width * ScaleX;
			}

			/// <summary>
			/// Gets the pixel height of the font.
			/// </summary>
			static constexpr dimension_t GetFontHeight()
			{
				return BitmaskFontType::Height * ScaleY;
			}
		};
	}
}
#endif