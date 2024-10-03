#ifdef CIMGUI_USE_SDL2

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct _SDL_GameController _SDL_GameController;
struct SDL_Window;
struct SDL_Renderer;
struct _SDL_GameController;
typedef union SDL_Event SDL_Event;
typedef enum { ImGui_ImplSDL2_GamepadMode_AutoFirst, ImGui_ImplSDL2_GamepadMode_AutoAll, ImGui_ImplSDL2_GamepadMode_Manual }ImGui_ImplSDL2_GamepadMode;CIMGUI_API bool ImGui_ImplSDL2_InitForOpenGL(SDL_Window* window,void* sdl_gl_context);
CIMGUI_API bool ImGui_ImplSDL2_InitForVulkan(SDL_Window* window);
CIMGUI_API bool ImGui_ImplSDL2_InitForD3D(SDL_Window* window);
CIMGUI_API bool ImGui_ImplSDL2_InitForMetal(SDL_Window* window);
CIMGUI_API bool ImGui_ImplSDL2_InitForSDLRenderer(SDL_Window* window,SDL_Renderer* renderer);
CIMGUI_API bool ImGui_ImplSDL2_InitForOther(SDL_Window* window);
CIMGUI_API void ImGui_ImplSDL2_Shutdown(void);
CIMGUI_API void ImGui_ImplSDL2_NewFrame(void);
CIMGUI_API bool ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event);
CIMGUI_API void ImGui_ImplSDL2_SetGamepadMode(ImGui_ImplSDL2_GamepadMode mode,struct _SDL_GameController** manual_gamepads_array,int manual_gamepads_count);

#endif
#ifdef CIMGUI_USE_SDLRENDERER2

typedef struct SDL_Renderer SDL_Renderer;
struct SDL_Renderer;CIMGUI_API bool ImGui_ImplSDLRenderer2_Init(SDL_Renderer* renderer);
CIMGUI_API void ImGui_ImplSDLRenderer2_Shutdown(void);
CIMGUI_API void ImGui_ImplSDLRenderer2_NewFrame(void);
CIMGUI_API void ImGui_ImplSDLRenderer2_RenderDrawData(ImDrawData* draw_data,SDL_Renderer* renderer);
CIMGUI_API bool ImGui_ImplSDLRenderer2_CreateFontsTexture(void);
CIMGUI_API void ImGui_ImplSDLRenderer2_DestroyFontsTexture(void);
CIMGUI_API bool ImGui_ImplSDLRenderer2_CreateDeviceObjects(void);
CIMGUI_API void ImGui_ImplSDLRenderer2_DestroyDeviceObjects(void);

#endif
