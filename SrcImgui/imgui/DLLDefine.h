#pragma once

#ifdef IMGUI_DLL_
#define ImGuiDLLExport __declspec( dllexport )
#else
#define ImGuiDLLExport __declspec( dllimport )
#endif
