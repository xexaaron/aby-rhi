#include <cstdio>
#include <aby-rhi/aby-rhi.hpp>
#include <Windows.h>

class Logger : public aby::rhi::ILogger {
public:
    auto log(aby::rhi::ELogLevel level, const std::string& msg) -> void override {
        switch (level) {
            case aby::rhi::ELogLevel::debug: printf("[debug] %s\n", msg.c_str()); break;
            case aby::rhi::ELogLevel::trace: printf("[trace] %s\n", msg.c_str()); break;
            case aby::rhi::ELogLevel::info:  printf("[info]  %s\n", msg.c_str()); break;
            case aby::rhi::ELogLevel::warn:  printf("[warn]  %s\n", msg.c_str()); break;
            case aby::rhi::ELogLevel::error: printf("[error] %s\n", msg.c_str()); break;
            case aby::rhi::ELogLevel::fatal: printf("[fatal] %s\n", msg.c_str()); break;
        }
    }

private:
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto* ren = reinterpret_cast<aby::rhi::IRenderer*>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));
    

    switch (msg) {
        case WM_SIZE: {
            if (ren) {
                ren->on_resize(LOWORD(lParam), HIWORD(lParam));
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


 int main()
{
    using namespace aby::rhi;

    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASSA wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "AbyWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    HWND window = CreateWindowExA(
        0,
        wc.lpszClassName,
        "Aby RHI",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    ShowWindow(window, SW_SHOW);

    auto& ctx = Context::get();
    ctx.set_interface<Logger>();

    if (!ctx.init(ERenderer::vulkan, EWindow::win32, window))
        return 1;

    auto* ren = ctx.renderer();
    ren->set_clear_color(Color(0.5f));
    
    MSG msg;
    while (true)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return 0;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!ren->on_begin()) continue;

        ren->on_end();

    }

    return 0;
}