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

        
    auto vertex_shader   = Shader::create("colored_triangle.vert");
    auto fragment_shader = Shader::create("colored_triangle.frag");
    auto* ren = ctx.renderer();

    auto rpsb = RenderPassBuilder::create();
    auto pass = rpsb->add_shader(vertex_shader)
        .add_shader(fragment_shader)
        .set_color_attachment_format(EFormat::rgba_sf16)
        .set_depth_format(EFormat::none)
        .use_default_cull_mode()
        .use_default_polygon_mode()
        .use_default_topology()
        .disable_multisampling()
        .disable_blending()
        .disable_depthtest()
        .build();

    ren->add_pass(pass);
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