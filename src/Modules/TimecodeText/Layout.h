#ifndef _LOLA_GRAPHICS_MODULES_TIMECODE_TEXT_LAYOUT_h
#define _LOLA_GRAPHICS_MODULES_TIMECODE_TEXT_LAYOUT_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace TimecodeText
		{
			namespace Layout
			{
				template<typename ParentLayout>
				using Time = Framework::Layout::HorizontalWeighted<ParentLayout,
					Dimensions::DigitLayoutWeight, // 0
					Dimensions::KerningLayoutWeight,
					Dimensions::DigitLayoutWeight, // 2

					Dimensions::KerningLayoutWeight,
					Dimensions::SeparatorLayoutWeight, // 4
					Dimensions::KerningLayoutWeight,

					Dimensions::DigitLayoutWeight, // 6
					Dimensions::KerningLayoutWeight,
					Dimensions::DigitLayoutWeight, // 8

					Dimensions::KerningLayoutWeight,
					Dimensions::SeparatorLayoutWeight, // 10
					Dimensions::KerningLayoutWeight,

					Dimensions::DigitLayoutWeight, // 12
					Dimensions::KerningLayoutWeight,
					Dimensions::DigitLayoutWeight // 14
				>;

				static constexpr uint8_t Digi1Index = 0;
				static constexpr uint8_t Digi2Index = 2;
				static constexpr uint8_t Sep1Index = 4;
				static constexpr uint8_t Digi3Index = 6;
				static constexpr uint8_t Digi4Index = 8;
				static constexpr uint8_t Sep2Index = 10;
				static constexpr uint8_t Digi5Index = 12;
				static constexpr uint8_t Digi6Index = 14;

				template<typename ParentLayout>
				using Digit1 =
					Framework::TemplateLayout<
					Time<ParentLayout>::template Cell<Digi1Index>::X(),
					Time<ParentLayout>::template Cell<Digi1Index>::Y(),
					Time<ParentLayout>::template Cell<Digi1Index>::Width(),
					Time<ParentLayout>::template Cell<Digi1Index>::Height()>;

				template<typename ParentLayout>
				using Digit2 =
					Framework::TemplateLayout<
					Time<ParentLayout>::template Cell<Digi2Index>::X(),
					Time<ParentLayout>::template Cell<Digi2Index>::Y(),
					Time<ParentLayout>::template Cell<Digi2Index>::Width(),
					Time<ParentLayout>::template Cell<Digi2Index>::Height()>;

				template<typename ParentLayout>
				using Digit3 =
					Framework::TemplateLayout<
					Time<ParentLayout>::template Cell<Digi3Index>::X(),
					Time<ParentLayout>::template Cell<Digi3Index>::Y(),
					Time<ParentLayout>::template Cell<Digi3Index>::Width(),
					Time<ParentLayout>::template Cell<Digi3Index>::Height()>;

				template<typename ParentLayout>
				using Digit4 =
					Framework::TemplateLayout<
					Time<ParentLayout>::template Cell<Digi4Index>::X(),
					Time<ParentLayout>::template Cell<Digi4Index>::Y(),
					Time<ParentLayout>::template Cell<Digi4Index>::Width(),
					Time<ParentLayout>::template Cell<Digi4Index>::Height()>;

				template<typename ParentLayout>
				using Digit5 =
					Framework::TemplateLayout<
					Time<ParentLayout>::template Cell<Digi5Index>::X(),
					Time<ParentLayout>::template Cell<Digi5Index>::Y(),
					Time<ParentLayout>::template Cell<Digi5Index>::Width(),
					Time<ParentLayout>::template Cell<Digi5Index>::Height()>;

				template<typename ParentLayout>
				using Digit6 =
					Framework::TemplateLayout<
					Time<ParentLayout>::template Cell<Digi6Index>::X(),
					Time<ParentLayout>::template Cell<Digi6Index>::Y(),
					Time<ParentLayout>::template Cell<Digi6Index>::Width(),
					Time<ParentLayout>::template Cell<Digi6Index>::Height()>;

				template<typename ParentLayout>
				using Separator1 =
					Framework::TemplateLayout<
					Time<ParentLayout>::template Cell<Sep1Index>::X(),
					Time<ParentLayout>::template Cell<Sep1Index>::Y(),
					Time<ParentLayout>::template Cell<Sep1Index>::Width(),
					Time<ParentLayout>::template Cell<Sep1Index>::Height()>;

				template<typename ParentLayout>
				using Separator2 =
					Framework::TemplateLayout<
					Time<ParentLayout>::template Cell<Sep2Index>::X(),
					Time<ParentLayout>::template Cell<Sep2Index>::Y(),
					Time<ParentLayout>::template Cell<Sep2Index>::Width(),
					Time<ParentLayout>::template Cell<Sep2Index>::Height()>;
			}
		}
	}
}
#endif