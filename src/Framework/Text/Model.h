#ifndef _EGFX_FRAMEWORK_TEXT_MODEL_h
#define _EGFX_FRAMEWORK_TEXT_MODEL_h

#include "../../EgfxCore.h"

namespace Egfx
{
	namespace Framework
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
				Space = 32,             // Space
				Exclamation = 33,       // !
				DoubleQuote = 34,       // "
				NumberSign = 35,        // #
				Dollar = 36,            // $
				Percent = 37,           // %
				Ampersand = 38,         // &
				Apostrophe = 39,        // '
				ParenthesisOpen = 40,   // (
				ParenthesisClose = 41,  // )
				Asterisk = 42,           // *
				Plus = 43,               // +
				Comma = 44,              // ,
				Minus = 45,              // -
				Period = 46,             // .
				ForwardSlash = 47,       // Forward Slash

				Digit0 = 48,             // 0
				Digit1 = 49,             // 1
				Digit2 = 50,             // 2
				Digit3 = 51,             // 3
				Digit4 = 52,             // 4
				Digit5 = 53,             // 5
				Digit6 = 54,             // 6
				Digit7 = 55,             // 7
				Digit8 = 56,             // 8
				Digit9 = 57,             // 9

				Colon = 58,              // :
				Semicolon = 59,          // ;
				LessThan = 60,           // <
				Equals = 61,             // =
				GreaterThan = 62,        // >
				Question = 63,            // ?
				At = 64,                  // @

				A = 65,                   // A
				B = 66,                   // B
				C = 67,                   // C
				D = 68,                   // D
				E = 69,                   // E
				F = 70,                   // F
				G = 71,                   // G
				H = 72,                   // H
				I = 73,                   // I
				J = 74,                   // J
				K = 75,                   // K
				L = 76,                   // L
				M = 77,                   // M
				N = 78,                   // N
				O = 79,                   // O
				P = 80,                   // P
				Q = 81,                   // Q
				R = 82,                   // R
				S = 83,                   // S
				T = 84,                   // T
				U = 85,                   // U
				V = 86,                   // V
				W = 87,                   // W
				X = 88,                   // X
				Y = 89,                   // Y
				Z = 90,                   // Z

				BracketOpen = 91,        // [
				Backslash = 92,          // Backslash
				BracketClose = 93,       // ]
				Caret = 94,              // ^
				Underscore = 95,         // _
				GraveAccent = 96,        // `

				a = 97,                   // a
				b = 98,                   // b
				c = 99,                   // c
				d = 100,                  // d
				e = 101,                  // e
				f = 102,                  // f
				g = 103,                  // g
				h = 104,                  // h
				i = 105,                  // i
				j = 106,                  // j
				k = 107,                  // k
				l = 108,                  // l
				m = 109,                  // m
				n = 110,                  // n
				o = 111,                  // o
				p = 112,                  // p
				q = 113,                  // q
				r = 114,                  // r
				s = 115,                  // s
				t = 116,                  // t
				u = 117,                  // u
				v = 118,                  // v
				w = 119,                  // w
				x = 120,                  // x
				y = 121,                  // y
				z = 122,                  // z

				LeftBrace = 123,         // {
				VerticalBar = 124,       // |
				RightBrace = 125,        // }
				Tilde = 126              // ~
			};

			static constexpr uint8_t PrintableStart = static_cast<int8_t>(Printable::Space);
			static constexpr uint8_t PrintableEnd = static_cast<int8_t>(Printable::Tilde) + 1;

			static constexpr uint8_t NumbersStart = '0';

			static constexpr uint32_t PowerOfTen[]
			{
				1,
				10,
				100,
				1000,
				10000,
				100000,
				1000000,
				10000000,
				100000000,
				1000000000
			};
		}
	}
}
#endif