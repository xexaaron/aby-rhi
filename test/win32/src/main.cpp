#include <Windows.h>
#include <aby-rhi/aby-rhi.hpp>
#include <cstdio>

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
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
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

	struct Vertex {
		vec3<float> pos = { 0.f, 0.f, 0.f };
		vec2<float> uv  = { 0.f, 0.f };
		Color color     = { 1.f, 1.f, 1.f, 1.f };
	};
	struct MVP {
		float model[16];
		float view[16];
		float projection[16];
	};

	auto rpsb = RenderPassBuilder::create();
	auto pass = rpsb->add_shader("test_vertex.vert")
	                .add_uniform("mvp", 0, EShader::vert)
	                .vertex_description_builder()
	                .add_inputs<&Vertex::pos, &Vertex::uv, &Vertex::color>(EFormat::rgb_f32, EFormat::rg_f32, EFormat::rgba_f32)
	                .build()
	                ->add_shader("test_frag.frag")
	                .use_all_defaults()
	                .disable_multisampling()
	                .disable_blending()
	                .disable_depthtest()
	                .build();

	auto* ren = ctx.renderer();
	ren->add_pass(pass);
	ren->set_clear_color(Color(0.5f));

	using Index = uint32_t;

	auto vbuff = VertexBuffer::create(100, sizeof(Vertex));
	auto ibuff = IndexBuffer::create(60);

	Vertex v0{
		.pos   = { 400.0f, 500.0f, 0.0f },
		.uv    = { 0.0f, 0.0f },
		.color = { 1.0f, 0.0f, 0.0f, 1.0f }
	};
	Vertex v1{
		.pos   = { 800.0f, 500.0f, 0.0f },
		.uv    = { 1.0f, 0.0f },
		.color = { 0.0f, 1.0f, 0.0f, 1.0f }
	};
	Vertex v2{
		.pos   = { 600.0f, 200.0f, 0.0f },
		.uv    = { 0.5f, 1.0f },
		.color = { 0.0f, 0.0f, 1.0f, 1.0f }
	};

	MVP mvp = {
		// Model (identity)
		{
         1,    0,    0,    0,
         0,           1,    0,    0,
         0,    0,    1,    0,
         0,    0,    0,    1    },

		// View (identity)
		{
         1,    0,    0,    0,
         0,           1,    0,    0,
         0,    0,    1,    0,
         0,    0,    0,    1    },

		// Projection (orthographic, Vulkan clip space)
		{
         2.0f / 1280, 0.0f, 0.0f, 0.0f,
         0.0f, -2.0f / 720, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
         -1.0f, 1.0f, 0.0f, 1.0f }
	};

	vbuff->push(&v0);
	vbuff->push(&v1);
	vbuff->push(&v2);
	ibuff->push(0);
	ibuff->push(1);
	ibuff->push(2);

	vbuff->upload();
	ibuff->upload();

	DrawCmd cmd(vbuff, ibuff, 1);

	pass->set_uniform("mvp", &mvp, sizeof(MVP));

	MSG msg;

	bool running = true;

	while (running) {
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				running = false;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!running)
			break;

		if (!ren->on_begin())
			continue;

		pass->submit(cmd);

		ren->on_end();
	}

	ctx.deinit();

	return 0;
}
