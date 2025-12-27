#ifndef _EGFX_ASCII_DEFINITION_h
#define _EGFX_ASCII_DEFINITION_h

#include <stdint.h>

namespace Egfx
{
	namespace AsciiDefinition
	{
		// Control characters (0..31, 127)
		enum class Control : uint8_t
		{
			NUL = 0,   // Null: string terminator, no operation
			SOH = 1,   // Start of Header: marks the beginning of a header
			STX = 2,   // Start of Text: begins the text section
			ETX = 3,   // End of Text: ends the text section
			EOT = 4,   // End of Transmission: terminates a transmission
			ENQ = 5,   // Enquiry: request for a response (status/ping)
			ACK = 6,   // Acknowledge: positive acknowledgment
			BEL = 7,   // Bell: alert/notification (audible/visual)
			BS = 8,   // Backspace: move cursor one position left
			HT = 9,   // Horizontal Tab: move to next tab stop
			LF = 10,  // Line Feed: move cursor to next line (Unix newline)
			VT = 11,  // Vertical Tab: move cursor vertically to next tab stop
			FF = 12,  // Form Feed: page break / clear screen
			CR = 13,  // Carriage Return: move cursor to column 0 (classic Mac newline)
			SO = 14,  // Shift Out: switch to alternate character set
			SI = 15,  // Shift In: return to primary character set
			DLE = 16,  // Data Link Escape: escape for control signaling in data streams
			DC1 = 17,  // Device Control 1 (XON): start/resume transmission
			DC2 = 18,  // Device Control 2: implementation-defined device control
			DC3 = 19,  // Device Control 3 (XOFF): pause transmission
			DC4 = 20,  // Device Control 4: implementation-defined device control
			NAK = 21,  // Negative Acknowledge: indicates an error or rejection
			SYN = 22,  // Synchronous Idle: sync timing / fill character
			ETB = 23,  // End of Transmission Block: block terminator
			CAN = 24,  // Cancel: abort current transmission/operation
			EM = 25,  // End of Medium: logical end of physical medium
			SUB = 26,  // Substitute: indicates invalid or replaced data
			ESC = 27,  // Escape: introduces an escape sequence
			FS = 28,  // File Separator: hierarchical separator (level 3)
			GS = 29,  // Group Separator: hierarchical separator (level 2)
			RS = 30,  // Record Separator: hierarchical separator (level 1)
			US = 31,  // Unit Separator: hierarchical separator (level 0)
			DEL = 127  // Delete: originally rubout; non-printable
		};

		// Printable ASCII (32..126).
		enum class Printable : uint8_t
		{
			Space = ' ',
			Exclamation = '!',
			DoubleQuote = '"',
			NumberSign = '#',
			Dollar = '$',
			Percent = '%',
			Ampersand = '&',
			Apostrophe = '\'',
			ParenthesisOpen = '(',
			ParenthesisClose = ')',
			Asterisk = '*',
			Plus = '+',
			Comma = ',',
			Minus = '-',
			Period = '.',
			ForwardSlash = '/',

			Digit0 = '0',
			Digit1 = '1',
			Digit2 = '2',
			Digit3 = '3',
			Digit4 = '4',
			Digit5 = '5',
			Digit6 = '6',
			Digit7 = '7',
			Digit8 = '8',
			Digit9 = '9',

			Colon = ':',
			Semicolon = ';',
			LessThan = '<',
			Equals = '=',
			GreaterThan = '>',
			Question = '?',
			At = '@',

			A = 'A',
			B = 'B',
			C = 'C',
			D = 'D',
			E = 'E',
			F = 'F',
			G = 'G',
			H = 'H',
			I = 'I',
			J = 'J',
			K = 'K',
			L = 'L',
			M = 'M',
			N = 'N',
			O = 'O',
			P = 'P',
			Q = 'Q',
			R = 'R',
			S = 'S',
			T = 'T',
			U = 'U',
			V = 'V',
			W = 'W',
			X = 'X',
			Y = 'Y',
			Z = 'Z',

			BracketOpen = '[',
			Backslash = '\\',
			BracketClose = ']',
			Caret = '^',
			Underscore = '_',
			GraveAccent = '`',

			a = 'a',
			b = 'b',
			c = 'c',
			d = 'd',
			e = 'e',
			f = 'f',
			g = 'g',
			h = 'h',
			i = 'i',
			j = 'j',
			k = 'k',
			l = 'l',
			m = 'm',
			n = 'n',
			o = 'o',
			p = 'p',
			q = 'q',
			r = 'r',
			s = 's',
			t = 't',
			u = 'u',
			v = 'v',
			w = 'w',
			x = 'x',
			y = 'y',
			z = 'z',

			LeftBrace = '{',
			VerticalBar = '|',
			RightBrace = '}',
			Tilde = '~'
		};

		static constexpr uint8_t PrintableStart = static_cast<int8_t>(Printable::Space);
		static constexpr uint8_t PrintableEnd = static_cast<int8_t>(Printable::Tilde) + 1;

		static constexpr uint8_t NumbersStart = '0';
	}
}
#endif