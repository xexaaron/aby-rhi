#include <cstdio>
#include <aby-rhi/aby-rhi.hpp>
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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


int main(int argc, char** argv) {
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
    
    if (!ctx.init(ERenderer::vulkan, EWindow::win32, window))
        return 1;
        
    ctx.file_io()->set_cwd(fs::path(argv[0]).parent_path());
    ctx.file_io()->set_cache_dir(ctx.file_io()->cwd() / "cache");
    
    if (!ctx.init_renderer()) 
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