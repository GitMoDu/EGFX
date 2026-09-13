#ifndef _EGFX_MODULES_TERMINAL_WINDOW_VIEW_h
#define _EGFX_MODULES_TERMINAL_WINDOW_VIEW_h

#include <EgfxFramework.h>
#include "Drawable.h"
#include "Buffer.h"

namespace Egfx
{
	namespace Modules
	{
		namespace TerminalWindow
		{
			namespace View
			{
				template<typename ParentLayout,
					typename TextViewType,
					typename Config>
				class Terminal : public Egfx::Framework::View::AbstractView
				{
				private:
					static constexpr uint16_t FontWidth = Config::FontWidth;
					static constexpr uint16_t FontHeight = Config::FontHeight;
					static constexpr uint16_t Kerning = Config::Kerning;
					static constexpr uint16_t LineSpacing = Config::LineSpacing;
					static constexpr uint32_t NewLineAnimationDuration = Config::NewLineAnimationDuration;
					static constexpr uint32_t CharacterRevealInterval = Config::CharacterRevealInterval;
					static constexpr uint32_t CursorBlinkPeriod = Config::CursorBlinkPeriod;
					using dimension_t = typename Egfx::Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;
					using signed_t = typename Egfx::Framework::AutoDimension::ByLayout<ParentLayout>::signed_t;
					static constexpr dimension_t SpaceWidth = static_cast<dimension_t>(FontWidth);
					static constexpr dimension_t LineStride = static_cast<dimension_t>(FontHeight + LineSpacing);
					static constexpr size_t LineLengthValue = (static_cast<size_t>(ParentLayout::Width()) + Kerning) / (FontWidth + Kerning);
					static constexpr size_t LineCountValue = (static_cast<size_t>(ParentLayout::Height()) + LineSpacing) / LineStride;
					static constexpr size_t BufferCharacterCountValue = LineLengthValue * (LineCountValue + 1);
					using count_t = typename Egfx::Modules::TerminalWindow::Definitions::CountType<
						BufferCharacterCountValue>::type;
					static constexpr count_t LineLength = static_cast<count_t>(LineLengthValue);
					static constexpr count_t LineCount = static_cast<count_t>(LineCountValue);
					static constexpr count_t BufferLineCount = LineCount + 1;
					static constexpr size_t ScreenCharacterCount = size_t(LineLength) * size_t(LineCount);
					static constexpr size_t QuarterScreenCharacterCount = ScreenCharacterCount / 4 > 0 ? ScreenCharacterCount / 4 : 1;
					static constexpr size_t HalfScreenCharacterCount = ScreenCharacterCount / 2 > 0 ? ScreenCharacterCount / 2 : 1;

				public:
					using BufferType = Egfx::Modules::TerminalWindow::Buffer::TerminalBuffer<
						count_t, LineLength, LineCount>;

				private:
					char RenderBuffer[LineLength + 1]{};
					BufferType BufferInstance{};
					using RenderBufferType = Egfx::Modules::TerminalWindow::Buffer::RenderBuffer<
						BufferType, Config::DoubleBuffered>;
					RenderBufferType RenderBufferInstance{ BufferInstance };
					TextViewType TextViewInstance{};
					using CursorDrawableType = Egfx::Modules::TerminalWindow::Drawable::Cursor<
						dimension_t, ParentLayout, FontHeight>;
					CursorDrawableType CursorInstance{};
					count_t CallIndex = 0;
					count_t CursorX = 0;
					bool NewLineAnimating = false;
					uint32_t NewLineAnimationStart = 0;
					dimension_t AnimationOffset = 0;
					uint32_t LastFrameTime = 0;
					uint32_t LastCursorTime = 0;
					uint32_t LastRevealTime = 0;
					count_t VirtualCursorLine = LineCount - 1;
					count_t VirtualCursorColumn = 0;

				public:
					static constexpr size_t CharactersPerLine = LineLength;
					static constexpr size_t Lines = LineCount;
					static_assert(FontWidth > 0 && FontHeight > 0, "Terminal font dimensions must be positive.");
					static_assert(LineLength > 0, "Terminal layout must fit at least one character.");
					static_assert(LineCount > 0, "Terminal layout must fit at least one line.");
					static_assert(NewLineAnimationDuration > 0, "New-line animation duration must be positive.");
					static_assert(CharacterRevealInterval > 0, "Character reveal interval must be positive.");

				public:
					Terminal()
					{
						TextViewInstance.SetFontSize(static_cast<dimension_t>(FontWidth), static_cast<dimension_t>(FontHeight));
						TextViewInstance.SetKerningWidth(static_cast<dimension_t>(Kerning));
						TextViewInstance.SetSpaceWidth(static_cast<dimension_t>(SpaceWidth));
						TextViewInstance.SetLineSpacing(LineSpacing);
					}

					BufferType& Buffer() { return BufferInstance; }

					void Clear()
					{
						BufferInstance.Clear();
						NewLineAnimating = false;
						AnimationOffset = 0;
						LastCursorTime = LastFrameTime;
						LastRevealTime = LastFrameTime;
						VirtualCursorLine = LineCount - 1;
						VirtualCursorColumn = 0;
					}

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
					{
						LastFrameTime = frameTime;
						if (BufferInstance.ConsumeChanged())
						{
							LastRevealTime = frameTime;
							LastCursorTime = frameTime;
						}
						const bool newLineStarted = BufferInstance.ConsumeNewLine();
						if (newLineStarted)
						{
							NewLineAnimating = true;
							NewLineAnimationStart = frameTime;
							AnimationOffset = 0;
						}
						else if (NewLineAnimating)
						{
							const uint32_t elapsed = frameTime - NewLineAnimationStart;
							if (elapsed >= NewLineAnimationDuration)
							{
								AnimationOffset = LineStride;
								NewLineAnimating = false;
							}
							else
							{
								AnimationOffset = (elapsed * LineStride) / NewLineAnimationDuration;
							}
						}

						const count_t pendingCharacters = BufferInstance.PendingRevealLength();
					const uint8_t catchupFactor = pendingCharacters >= HalfScreenCharacterCount
							? 4
							: pendingCharacters >= QuarterScreenCharacterCount ? 2 : 1;
						const uint32_t revealInterval = static_cast<uint32_t>(
							CharacterRevealInterval / catchupFactor > 0
							? CharacterRevealInterval / catchupFactor : 1);
						count_t revealBudget = static_cast<count_t>((frameTime - LastRevealTime) / revealInterval);
						while (revealBudget > 0)
						{
							count_t line = 0;
							for (; line < BufferLineCount; ++line)
							{
								if (BufferInstance.GetRevealedLength(line) < BufferInstance.GetLineLength(line))
									break;
							}

							if (line >= BufferLineCount)
								break;

							const count_t remaining = BufferInstance.GetLineLength(line) - BufferInstance.GetRevealedLength(line);
							const count_t revealCount = remaining < revealBudget ? remaining : revealBudget;
							BufferInstance.Reveal(line, revealCount);
							revealBudget -= revealCount;
							LastRevealTime += static_cast<uint32_t>(revealCount * revealInterval);
						}

						bool revealPending = false;
						for (count_t line = 0; line < BufferLineCount; ++line)
						{
							if (BufferInstance.GetRevealedLength(line) < BufferInstance.GetLineLength(line))
							{
								VirtualCursorLine = line;
								VirtualCursorColumn = BufferInstance.GetRevealedLength(line);
								revealPending = true;
								break;
							}
						}

						if (!revealPending)
						{
							VirtualCursorLine = BufferInstance.IsCurrentLineExtra() ? LineCount : LineCount - 1;
							VirtualCursorColumn = BufferInstance.GetLineLength(VirtualCursorLine);
						}

						const bool cursorBlinkVisible = (frameTime - LastCursorTime)
							% CursorBlinkPeriod
							<= CursorBlinkPeriod / 2;
						CursorInstance.Visible = revealPending || NewLineAnimating || cursorBlinkVisible;
						RenderBufferInstance.Capture();

						return true;
					}

					bool Draw(Egfx::IFrameBuffer* frame) override
					{
						auto& text = TextViewInstance.Drawable();
						const count_t lineLength = RenderBufferInstance.GetLineLength(CallIndex);
						if (RenderBufferInstance.GetRevealedLength(CallIndex) < lineLength)
						{
							memcpy(RenderBuffer, RenderBufferInstance.GetLine(CallIndex), RenderBufferInstance.GetRevealedLength(CallIndex));
							RenderBuffer[RenderBufferInstance.GetRevealedLength(CallIndex)] = 0;
							text.SetText(RenderBuffer);
						}
						else
						{
							text.SetText(RenderBufferInstance.GetLine(CallIndex));
						}
						const dimension_t animationOffset = RenderBufferInstance.HasPendingNewLine() ? 0 : AnimationOffset;
						const signed_t lineOffset = static_cast<signed_t>(CallIndex * LineStride);
						const signed_t animatedOffset = lineOffset - static_cast<signed_t>(animationOffset);
						text.SetOffset(0, static_cast<dimension_t>(animatedOffset));
						text.Draw(frame);

						if (++CallIndex < BufferLineCount)
							return false;

						const dimension_t cursorOffset = static_cast<dimension_t>(VirtualCursorColumn * FontWidth)
							+ (VirtualCursorColumn > 0 ? (VirtualCursorColumn - 1) * Kerning : 0);
						CursorInstance.SetOffsetX(static_cast<dimension_t>(cursorOffset));
						const signed_t cursorTop = static_cast<signed_t>(VirtualCursorLine * LineStride)
							- static_cast<signed_t>(RenderBufferInstance.HasPendingNewLine() ? 0 : AnimationOffset);
						const signed_t defaultCursorTop = static_cast<signed_t>(ParentLayout::Height() - FontHeight);
						CursorInstance.SetOffsetY(cursorTop - defaultCursorTop);
						CursorInstance.Draw(frame);

						CallIndex = 0;
						return true;
					}

				};
			}
		}
	}
}

#endif
