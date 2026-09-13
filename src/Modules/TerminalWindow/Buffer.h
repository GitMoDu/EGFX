#ifndef _EGFX_MODULES_TERMINAL_WINDOW_BUFFER_h
#define _EGFX_MODULES_TERMINAL_WINDOW_BUFFER_h

#include <string.h>

namespace Egfx
{
    namespace Modules
    {
        namespace TerminalWindow
        {
            namespace Buffer
            {
                template<typename count_t, count_t CharactersPerLineValue, count_t LinesValue>
                class TerminalBuffer
                {
                public:
                    using count_type = count_t;
                    static constexpr count_t CharactersPerLine = CharactersPerLineValue;
                    static constexpr count_t Lines = LinesValue;
                    static constexpr count_t BufferLines = Lines + 1;
                    static constexpr size_t BufferSize = size_t(BufferLines) * (size_t(CharactersPerLine) + 1);

                private:
                    char TextBuffer[BufferLines][CharactersPerLine + 1]{};
                    count_t LineLengths[BufferLines]{};
                    count_t RevealedLength[BufferLines]{};
                    count_t CursorX = 0;
                    bool PreviousWasCR = false;
                    bool PendingWrap = false;
                    bool CurrentLineIsExtra = false;
                    bool NewLinePending = false;
                    bool ContentChanged = false;

                public:
                    const char* GetLine(const count_t line) const { return line < BufferLines ? TextBuffer[line] : nullptr; }
                    count_t GetLineLength(const count_t line) const { return line < BufferLines ? LineLengths[line] : 0; }
                    count_t GetRevealedLength(const count_t line) const { return line < BufferLines ? RevealedLength[line] : 0; }
                    count_t GetCursorX() const { return CursorX; }
                    bool IsCurrentLineExtra() const { return CurrentLineIsExtra; }
                    bool HasPendingNewLine() const { return NewLinePending; }
                    bool ConsumeNewLine() { const bool pending = NewLinePending; NewLinePending = false; return pending; }
                    bool ConsumeChanged() { const bool changed = ContentChanged; ContentChanged = false; return changed; }

                    count_t PendingRevealLength() const
                    {
                        count_t result = 0;
                        for (count_t line = 0; line < BufferLines; ++line)
                        {
                            const count_t length = LineLengths[line];
                            if (RevealedLength[line] < length) result += length - RevealedLength[line];
                        }
                        return result;
                    }

                    void Reveal(const count_t line, const count_t count)
                    {
                        if (line < BufferLines)
                        {
                            const count_t length = LineLengths[line];
                            const count_t next = RevealedLength[line] + count;
                            RevealedLength[line] = next < length ? next : length;
                        }
                    }

                    void Clear()
                    {
                        memset(TextBuffer, 0, sizeof(TextBuffer));
                        memset(LineLengths, 0, sizeof(LineLengths));
                        memset(RevealedLength, 0, sizeof(RevealedLength));
                        CursorX = 0; PreviousWasCR = false; PendingWrap = false;
                        CurrentLineIsExtra = false; NewLinePending = false; ContentChanged = false;
                    }

                    size_t Write(const uint8_t character)
                    {
                        if (character == 0) return 0;
                        if (character == '\r') { NewLine(); PreviousWasCR = true; }
                        else if (character == '\n') { if (!PreviousWasCR) NewLine(); PreviousWasCR = false; }
                        else
                        {
                            PreviousWasCR = false; if (PendingWrap) NewLine();
                            if (CursorX < CharactersPerLine)
                            {
                                const count_t line = CurrentLineIsExtra ? Lines : Lines - 1;
                                TextBuffer[line][CursorX++] = static_cast<char>(character);
                                TextBuffer[line][CursorX] = 0;
                                LineLengths[line] = CursorX;
                                PendingWrap = CursorX >= CharactersPerLine;
                            }
                        }
                        ContentChanged = true;
                        return 1;
                    }

                private:
                    void NewLine()
                    {
                        if (CurrentLineIsExtra)
                            for (count_t line = 1; line <= Lines; ++line) { memcpy(TextBuffer[line - 1], TextBuffer[line], sizeof(TextBuffer[line])); LineLengths[line - 1] = LineLengths[line]; RevealedLength[line - 1] = RevealedLength[line]; }
                        memset(TextBuffer[Lines], 0, sizeof(TextBuffer[Lines])); LineLengths[Lines] = 0; RevealedLength[Lines] = 0;
                        CursorX = 0; PendingWrap = false; CurrentLineIsExtra = true;
                        NewLinePending = true; ContentChanged = true;
                    }
                };

                template<typename TerminalBuffer, bool DoubleBuffered>
                class RenderBuffer;

                template<typename TerminalBuffer>
                class RenderBuffer<TerminalBuffer, false>
                {
                private:
                    TerminalBuffer& Buffer;

                public:
                    explicit RenderBuffer(TerminalBuffer& buffer) : Buffer(buffer) {}
                    void Capture() {}
                    const char* GetLine(const typename TerminalBuffer::count_type line) const { return Buffer.GetLine(line); }
                    typename TerminalBuffer::count_type GetLineLength(const typename TerminalBuffer::count_type line) const { return Buffer.GetLineLength(line); }
                    typename TerminalBuffer::count_type GetRevealedLength(const typename TerminalBuffer::count_type line) const { return Buffer.GetRevealedLength(line); }
                    bool HasPendingNewLine() const { return Buffer.HasPendingNewLine(); }
                };

                template<typename TerminalBuffer>
                class RenderBuffer<TerminalBuffer, true>
                {
                private:
                    TerminalBuffer& Buffer;
                    char TextBuffer[TerminalBuffer::BufferLines][TerminalBuffer::CharactersPerLine + 1]{};
                    typename TerminalBuffer::count_type LineLengths[TerminalBuffer::BufferLines]{};
                    typename TerminalBuffer::count_type RevealedLength[TerminalBuffer::BufferLines]{};

                public:
                    explicit RenderBuffer(TerminalBuffer& buffer) : Buffer(buffer) {}
                    void Capture()
                    {
                        for (typename TerminalBuffer::count_type line = 0; line < TerminalBuffer::BufferLines; ++line)
                        {
                            memcpy(TextBuffer[line], Buffer.GetLine(line), TerminalBuffer::CharactersPerLine + 1);
                            LineLengths[line] = Buffer.GetLineLength(line);
                            RevealedLength[line] = Buffer.GetRevealedLength(line);
                        }
                    }
                    const char* GetLine(const typename TerminalBuffer::count_type line) const { return line < TerminalBuffer::BufferLines ? TextBuffer[line] : nullptr; }
                    typename TerminalBuffer::count_type GetLineLength(const typename TerminalBuffer::count_type line) const { return line < TerminalBuffer::BufferLines ? LineLengths[line] : 0; }
                    typename TerminalBuffer::count_type GetRevealedLength(const typename TerminalBuffer::count_type line) const { return line < TerminalBuffer::BufferLines ? RevealedLength[line] : 0; }
                    bool HasPendingNewLine() const { return false; }
                };
            }
        }
    }
}
#endif
