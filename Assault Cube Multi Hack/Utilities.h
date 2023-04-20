#include <Windows.h>

#include "Includes/imgui/imgui.h"
#include "Math.h"

#include <string>

#define str_length(array) ((sizeof(array)) / (sizeof(array[0])))

class Utils
{
public:
	static int HEXtoDEC(uintptr_t hexVal);
	static ImVec2 GetWindowRes();
	static void TypewriterPrint(std::string msg, int speed);
	static BOOL IsInFullscreen();
};