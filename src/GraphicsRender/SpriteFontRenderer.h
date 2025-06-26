#ifndef _EGFX_SPRITE_FONT_RENDERER_h
#define _EGFX_SPRITE_FONT_RENDERER_h

#if defined(ARDUINO)
#include <WString.h>
#endif
#include "../Model/Character.h"
#include "../Sprite/Shader/SpriteShader.h"

namespace Egfx
{
	/// <summary>
	/// Sprite based font renderer, with no color shading.
	/// Fixed dimensions (same as declared in sprite) and pixel kerning.
	/// Implementation of sprite selection is done through virtual call SetCharacter;
	/// By inheriting directy from the Sprite type,
	/// shaders and effects can still be template-chained as if this was a regular sprite.
	/// </summary>
	/// <typeparam name="SpriteType"></typeparam>
	template<typename SpriteType,
		uint8_t fontKerning = 1>
	class AbstractSpriteFontRenderer : public SpriteType
	{
	public:
		static constexpr pixel_t FontWidth()
		{
			return SpriteType::Width;
		}

		static constexpr pixel_t FontHeight()
		{
			return SpriteType::Height;
		}

		static constexpr uint8_t FontKerning()
		{
			return fontKerning;
		}

	private:
		using BaseClass = SpriteType;

	protected:
		virtual void SetCharacter(const int8_t character) {}

	public:
		AbstractSpriteFontRenderer() : BaseClass() {}

	public:
		void TextBottomLeft(IFrameBuffer* frame, const pixel_t x, const pixel_t y, const char* str)
		{
			TextTopLeft(frame, x, y - FontHeight(), str);
		}

		void TextBottomRight(IFrameBuffer* frame, const pixel_t x, const pixel_t y, const char* str)
		{
			TextTopRight(frame, x, y - FontHeight(), str);
		}

#if defined(ARDUINO)
		void TextBottomLeft(IFrameBuffer* frame, const pixel_t x, const pixel_t y, const __FlashStringHelper* ifsh)
		{
			TextTopLeft(frame, x, y - FontHeight(), ifsh);
		}

		void TextBottomRight(IFrameBuffer* frame, const pixel_t x, const pixel_t y, const __FlashStringHelper* ifsh)
		{
			TextTopRight(frame, x, y - FontHeight(), ifsh);
		}

		void TextTopLeft(IFrameBuffer* frame, const pixel_t x, const pixel_t y, const __FlashStringHelper* ifsh)
		{
#if defined(ARDUINO_ARCH_AVR)
			if (ifsh != NULL
				&& x < frame->GetFrameWidth() - FontWidth()
				&& y < frame->GetFrameHeight() - FontHeight())
			{
				char* ptr = (char*)reinterpret_cast<const char*>(ifsh);
				pixel_t offset = 0;
				while (x < frame->GetFrameWidth() - offset)
				{
					const int8_t character = pgm_read_byte(ptr++);
					if (character == Character::Break)
					{
						break;
					}
					else
					{
						Write(frame, x + offset, y, character);
						offset += FontWidth() + FontKerning();
					}
				}
			}
#else
			TextTopLeft(frame, x, y, reinterpret_cast<const char*>(ifsh));
#endif	
		}

		void TextTopRight(IFrameBuffer* frame, const pixel_t x, const pixel_t y, const __FlashStringHelper* ifsh)
		{
#if defined(ARDUINO_ARCH_AVR)
			if (ifsh != NULL
				&& x < frame->GetFrameWidth()
				&& y < frame->GetFrameHeight() - FontHeight())
			{
				const pixel_t x1 = x - FontHeight();

				size_t size = 0;
				char* ptr = (char*)reinterpret_cast<const char*>(ifsh);
				while (true)
				{
					const int8_t character = pgm_read_byte(ptr + size);
					if (character == Character::Break)
					{
						break;
					}
					else
					{
						size++;
					}
				}
				ptr += size - 1;
				pixel_t offset = 0;
				while (size--
					&& x > offset)
				{
					const int8_t character = pgm_read_byte(ptr--);
					Write(frame, x1 - offset, y, character);
					offset += FontWidth() + FontKerning();
				}
			}
#else
			TextTopRight(frame, x, y, reinterpret_cast<const char*>(ifsh));
#endif		
		}
#endif

		void TextTopLeft(IFrameBuffer* frame, const pixel_t x, const pixel_t y, const char* str)
		{
			const pixel_t width = frame->GetFrameWidth();
			const pixel_t height = frame->GetFrameHeight();

			if (str != NULL
				&& y < height)
			{
				size_t size = strlen(str);
				uint8_t* ch = (uint8_t*)str;
				pixel_t offset = 0;
				while (size--
					&& (x < (width - offset)))
				{
					Write(frame, x + offset, y, (const uint8_t)*ch++);
					offset += FontWidth() + FontKerning();
				}
			}
		}

		void TextTopRight(IFrameBuffer* frame, const pixel_t x, const pixel_t y, const char* str)
		{
			const pixel_t width = frame->GetFrameWidth();
			const pixel_t height = frame->GetFrameHeight();

			if (str != NULL
				&& y < height)
			{
				size_t size = strlen(str);
				uint8_t* ch = (uint8_t*)str + size - 1;
				pixel_t offset = 0;
				while (size--
					&& (x < (width - offset)))
				{
					Write(frame, x - offset, y, (const uint8_t)*ch--);
					offset += FontWidth() + FontKerning();
				}
			}
		}

		void WriteDigit(IFrameBuffer* frame, const pixel_t x, const pixel_t y, const uint8_t number)
		{
			Write(frame, x, y, '0' + number);
		}

		void Write(IFrameBuffer* frame, const pixel_t x, const pixel_t y, const char character)
		{
			SetCharacter(character);

			if (character == '\t'
				|| character == ' ')
			{
				return;
			}

			SpriteRenderer::Draw(frame, this, x, y);
		}
	};
}
#endif