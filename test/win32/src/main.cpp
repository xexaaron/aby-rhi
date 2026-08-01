#include <Windows.h>
#include <aby-rhi/aby-rhi.hpp>
#include <cstdio>

struct Vertex {
	aby::rhi::vec3<float> pos = { 0.f, 0.f, 0.f };
	aby::rhi::vec2<float> uv  = { 0.f, 0.f };
	aby::rhi::Color color     = { 1.f, 1.f, 1.f, 1.f };
};
struct MVP {
	float model[16];
	float view[16];
	float projection[16];
};

HWND get_workerw() {
	HWND progman = FindWindowA("Progman", nullptr);

	// Ask Progman to spawn WorkerW
	DWORD_PTR result = 0;
	SendMessageTimeoutA(
	    progman,
	    0x052C,
	    0,
	    0,
	    SMTO_NORMAL,
	    1000,
	    &result);

	HWND workerw = nullptr;

	EnumWindows(
	    [](HWND hwnd, LPARAM lParam) -> BOOL {
		HWND shell = FindWindowExA(
		    hwnd,
		    nullptr,
		    "SHELLDLL_DefView",
		    nullptr);

		if (shell) {
			HWND* out = reinterpret_cast<HWND*>(lParam);
			*out      = FindWindowExA(
			    nullptr,
			    hwnd,
			    "WorkerW",
			    nullptr);
			return FALSE;
		}

		return TRUE;
	},
	    reinterpret_cast<LPARAM>(&workerw));

	return workerw;
}

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

auto create_mvp(float angle) -> MVP {
	float c = cos(angle);
	float s = sin(angle);

	float fov    = 45.0f * 3.1415926f / 180.0f;
	float aspect = 1280.0f / 720.0f;
	float pnear  = 0.1f;
	float pfar   = 100.0f;

	float f = 1.0f / tan(fov / 2.0f);

	return {
		.model = {
		          c, 0, -s, 0,
		          0,  1, 0, 0,
		          s, 0,		             c,  0,
		          0, 0,		                      -3, 1		                                                             },

		.view = {          1, 0,  0, 0, 0,  1, 0, 0, 0, 0,                     1,  0, 0, 0,                               0, 1 },

		.projection = { f / aspect, 0,  0, 0, 0, -f, 0, 0, 0, 0, pfar / (pnear - pfar), -1, 0, 0, (pnear * pfar) / (pnear - pfar), 0 }
	};
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

	RECT rect;

	GetClientRect(
	    GetDesktopWindow(),
	    &rect);

	int width  = rect.right;
	int height = rect.bottom;

	HWND window = CreateWindowExA(
	    WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE,
	    wc.lpszClassName,
	    "Aby RHI",
	    WS_POPUP,
	    CW_USEDEFAULT, CW_USEDEFAULT,
	    width, height,
	    nullptr,
	    nullptr,
	    hInstance,
	    nullptr);

	auto workerw = get_workerw();
	SetParent(window, workerw);

	SetLayeredWindowAttributes(
	    window,
	    0,
	    180,
	    LWA_ALPHA);

	SetWindowLongPtr(
	    window,
	    GWL_STYLE,
	    WS_VISIBLE | WS_POPUP);

	ShowWindow(window, SW_SHOW);
	SetWindowPos(
	    window,
	    HWND_TOPMOST,
	    0,
	    0,
	    width,
	    height,
	    SWP_NOACTIVATE | SWP_SHOWWINDOW);

	auto& ctx = Context::get();

	if (!ctx.init(ERenderer::vulkan, EWindow::win32, window))
		return 1;

	ctx.file_io()->set_cwd(fs::path(argv[0]).parent_path());
	ctx.file_io()->set_cache_dir(ctx.file_io()->cwd() / "cache");

	auto rpsb = RenderPassBuilder::create();
	auto pass = rpsb->add_shader("test_vertex.vert")
	                .add_uniform("mvp", 0, EShader::vert)
	                .vertex_description_builder()
	                .add_inputs<&Vertex::pos, &Vertex::uv, &Vertex::color>(EFormat::rgb_f32, EFormat::rg_f32, EFormat::rgba_f32)
	                .build()
	                ->add_shader("test_frag.frag")
	                .use_all_defaults()
	                .set_cull_mode(ECullMode::back, EFrontFace::counter_clockwise)
	                .disable_multisampling()
	                .disable_blending()
	                .disable_depthtest()
	                .build();

	auto* ren = ctx.renderer();
	ren->add_pass(pass);
	ren->set_clear_color(Color(0.f));

	using Index = uint32_t;

	auto vbuff = VertexBuffer::create(100, sizeof(Vertex));
	auto ibuff = IndexBuffer::create(60);

	Vertex vertices[] = {
		{  { -0.5f, -0.5f, 0.5f }, { 0.f, 0.f }, { 1, 0, 0, 1 } },
		{   { 0.5f, -0.5f, 0.5f }, { 0.f, 0.f }, { 0, 1, 0, 1 } },
		{    { 0.5f, 0.5f, 0.5f }, { 0.f, 0.f }, { 0, 0, 1, 1 } },
		{   { -0.5f, 0.5f, 0.5f }, { 0.f, 0.f }, { 1, 1, 0, 1 } },
		{ { -0.5f, -0.5f, -0.5f }, { 0.f, 0.f }, { 1, 0, 1, 1 } },
		{  { 0.5f, -0.5f, -0.5f }, { 0.f, 0.f }, { 0, 1, 1, 1 } },
		{   { 0.5f, 0.5f, -0.5f }, { 0.f, 0.f }, { 1, 1, 1, 1 } },
		{  { -0.5f, 0.5f, -0.5f }, { 0.f, 0.f }, { 0, 0, 0, 1 } },
	};

	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0, // front
		4, 6, 5, 6, 4, 7, // back
		0, 4, 5, 5, 1, 0, // bottom
		3, 2, 6, 6, 7, 3, // top
		1, 5, 6, 6, 2, 1, // right
		4, 0, 3, 3, 7, 4  // left
	};

	float angle = 0.785398f; // 45 degrees
	MVP mvp     = create_mvp(angle);

	for (size_t i = 0; i < std::size(vertices); i++) {
		vbuff->push(&vertices[i]);
	}
	vbuff->upload();

	for (size_t i = 0; i < std::size(indices); i++) {
		ibuff->push(indices[i]);
	}
	ibuff->upload();

	DrawCmd cmd(vbuff, ibuff, 1);

	pass->set_uniform("mvp", mvp);

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

		angle += 0.01f;

		auto mvp = create_mvp(angle);
		pass->set_uniform("mvp", mvp);
		pass->submit(cmd);

		ren->on_end();
	}

	ctx.deinit();

	return 0;
}
