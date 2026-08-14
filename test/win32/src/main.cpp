#include <Windows.h>
#include <aby-rhi/aby-rhi.hpp>
#include <chrono>
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
struct Material {
	uint32_t albedo;
	uint32_t ao;
	uint32_t height;
	uint32_t normal;
	uint32_t roughness;
	uint32_t orm;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	auto* ren = reinterpret_cast<aby::rhi::Renderer*>(
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

	constexpr float fov    = 45.0f * 3.1415926f / 180.0f;
	constexpr float aspect = 1280.0f / 720.0f;
	constexpr float pnear  = 0.1f;
	constexpr float pfar   = 100.0f;

	float f = 1.0f / tan(fov / 2.0f);

	return {
		.model = {
		          c, 0, -s, 0,
		          0,  1, 0, 0,
		          s, 0,		             c,  0,
		          0, 0,		                    -7.5, 1		                                                             },

		.view = {          1, 0,  0, 0, 0,  1, 0, 0, 0, 0,                     1,  0, 0, 0,                               0, 1 },

		.projection = { f / aspect, 0,  0, 0, 0, -f, 0, 0, 0, 0, pfar / (pnear - pfar), -1, 0, 0, (pnear * pfar) / (pnear - pfar), 0 }
	};
}

int main(int argc, char** argv) {
	using namespace aby::rhi;
	HWND window;
	std::shared_ptr<RenderPass> pass    = nullptr;
	std::shared_ptr<VertexBuffer> vbuff = nullptr;
	std::shared_ptr<IndexBuffer> ibuff  = nullptr;
	float angle;
	MVP mvp;

	TexturePtr tex_albedo    = nullptr;
	TexturePtr tex_ao        = nullptr;
	TexturePtr tex_height    = nullptr;
	TexturePtr tex_normal    = nullptr;
	TexturePtr tex_roughness = nullptr;
	TexturePtr tex_orm       = nullptr;

	Material material = {};

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	WNDCLASSA wc{};
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = "AbyWindow";
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	RegisterClassA(&wc);

	int width  = 1280;
	int height = 720;

	window = CreateWindowExA(
	    0,
	    wc.lpszClassName,
	    "Aby RHI",
	    WS_OVERLAPPEDWINDOW,
	    CW_USEDEFAULT, CW_USEDEFAULT,
	    width, height,
	    nullptr,
	    nullptr,
	    hInstance,
	    nullptr);

	ShowWindow(window, SW_SHOW);
	auto& ctx = Context::get();

	ContextParams context_params{
		.renderer_backend = ERenderer::vulkan,
		.window_backend   = EWindow::win32,
		.native_window    = window,
		.graphics         = GraphicsParams{
		    .aliasing = EAntiAliasing::none }
	};

	if (!ctx.init(context_params))
		return 1;

	ctx.file_io()->set_cwd(fs::path(argv[0]).parent_path());
	ctx.file_io()->set_cache_dir(ctx.file_io()->cwd() / "cache");

	auto rpsb = RenderPassBuilder::create();
	rpsb->add_shader("test_vertex.vert")
	    .add_shader("test_frag.frag")
	    .add_uniform("mvp", 0, EShader::vert)
	    .add_uniform("texs", 1, EShader::vert)
	    .vertex_description_builder()
	    /**/.add_inputs<&Vertex::pos, &Vertex::uv, &Vertex::color>(EFormat::rgb_f32, EFormat::rg_f32, EFormat::rgba_f32)
	    /**/.build()
	    ->set_topology(ETopology::triangle_list)
	    .set_cull_mode(ECullMode::front, EFrontFace::counter_clockwise)
	    .set_polygon_mode(EPolygonMode::fill, 1.f)
	    .set_blend_mask(EChannels::rgba)
	    .set_blend_color(true, Blend{ .op = EBlendOp::add, .src = EBlendFactor::src_alpha, .dst = EBlendFactor::one_minus_src_alpha })
	    .set_blend_alpha(Blend{ .op = EBlendOp::add, .src = EBlendFactor::one, .dst = EBlendFactor::one_minus_src_alpha })
	    .set_depth(true, true, ECompareOp::less)
	    .set_stencil(true, ECompareOp::always)
	    .use_default_attachment_formats();

	TextureParams texture_params{
		.mip_levels           = 0,
		.anisotropy_filtering = 16.f,
		.filtering            = EFiltering::nearest,
		.repeat_mode          = ERepeatMode::repeat
	};

	tex_albedo = Texture::create("Cobblestone.png", texture_params);
	// tex_albedo = Texture::create("cobblestone_pavement_2k/Cobblestone_BaseColor_2K.png", texture_params);

	tex_ao        = Texture::create("cobblestone_pavement_2k/Cobblestone_AO_2K.png");
	tex_height    = Texture::create("cobblestone_pavement_2k/Cobblestone_Height_2K.png");
	tex_normal    = Texture::create("cobblestone_pavement_2k/Cobblestone_Normal_2K.png");
	tex_roughness = Texture::create("cobblestone_pavement_2k/Cobblestone_Roughness_2K.png");
	tex_orm       = Texture::create("cobblestone_pavement_2k/Cobblestone_ORM_2K.png");

	vbuff = VertexBuffer::create(100, sizeof(Vertex));
	ibuff = IndexBuffer::create(60);

	Vertex vertices[] = {
		// Front (+Z)
		{  { -0.5f, -0.5f, 0.5f }, { 0.f, 0.f }, { 1, 1, 1, 1 } },
		{   { 0.5f, -0.5f, 0.5f }, { 1.f, 0.f }, { 1, 1, 1, 1 } },
		{    { 0.5f, 0.5f, 0.5f }, { 1.f, 1.f }, { 1, 1, 1, 1 } },
		{   { -0.5f, 0.5f, 0.5f }, { 0.f, 1.f }, { 1, 1, 1, 1 } },

		// Back (-Z)
		{  { 0.5f, -0.5f, -0.5f }, { 0.f, 0.f }, { 1, 1, 1, 1 } },
		{ { -0.5f, -0.5f, -0.5f }, { 1.f, 0.f }, { 1, 1, 1, 1 } },
		{  { -0.5f, 0.5f, -0.5f }, { 1.f, 1.f }, { 1, 1, 1, 1 } },
		{   { 0.5f, 0.5f, -0.5f }, { 0.f, 1.f }, { 1, 1, 1, 1 } },

		// Left (-X)
		{ { -0.5f, -0.5f, -0.5f }, { 0.f, 0.f }, { 1, 1, 1, 1 } },
		{  { -0.5f, -0.5f, 0.5f }, { 1.f, 0.f }, { 1, 1, 1, 1 } },
		{   { -0.5f, 0.5f, 0.5f }, { 1.f, 1.f }, { 1, 1, 1, 1 } },
		{  { -0.5f, 0.5f, -0.5f }, { 0.f, 1.f }, { 1, 1, 1, 1 } },

		// Right (+X)
		{   { 0.5f, -0.5f, 0.5f }, { 0.f, 0.f }, { 1, 1, 1, 1 } },
		{  { 0.5f, -0.5f, -0.5f }, { 1.f, 0.f }, { 1, 1, 1, 1 } },
		{   { 0.5f, 0.5f, -0.5f }, { 1.f, 1.f }, { 1, 1, 1, 1 } },
		{    { 0.5f, 0.5f, 0.5f }, { 0.f, 1.f }, { 1, 1, 1, 1 } },

		// Top (+Y)
		{   { -0.5f, 0.5f, 0.5f }, { 0.f, 0.f }, { 1, 1, 1, 1 } },
		{    { 0.5f, 0.5f, 0.5f }, { 1.f, 0.f }, { 1, 1, 1, 1 } },
		{   { 0.5f, 0.5f, -0.5f }, { 1.f, 1.f }, { 1, 1, 1, 1 } },
		{  { -0.5f, 0.5f, -0.5f }, { 0.f, 1.f }, { 1, 1, 1, 1 } },

		// Bottom (-Y)
		{ { -0.5f, -0.5f, -0.5f }, { 0.f, 0.f }, { 1, 1, 1, 1 } },
		{  { 0.5f, -0.5f, -0.5f }, { 1.f, 0.f }, { 1, 1, 1, 1 } },
		{   { 0.5f, -0.5f, 0.5f }, { 1.f, 1.f }, { 1, 1, 1, 1 } },
		{  { -0.5f, -0.5f, 0.5f }, { 0.f, 1.f }, { 1, 1, 1, 1 } },
	};

	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0,       // Front
		4, 5, 6, 6, 7, 4,       // Back
		8, 9, 10, 10, 11, 8,    // Left
		12, 13, 14, 14, 15, 12, // Right
		16, 17, 18, 18, 19, 16, // Top
		20, 21, 22, 22, 23, 20  // Bottom
	};

	for (size_t i = 0; i < std::size(vertices); i++) {
		vbuff->push(&vertices[i]);
	}
	vbuff->upload();

	for (size_t i = 0; i < std::size(indices); i++) {
		ibuff->push(indices[i]);
	}
	ibuff->upload();

	material = Material{
		.albedo    = tex_albedo->id(),
		.ao        = tex_ao->id(),
		.height    = tex_ao->id(),
		.normal    = tex_normal->id(),
		.roughness = tex_roughness->id(),
		.orm       = tex_orm->id()
	};

	angle     = 0.785398f; // 45 degrees
	mvp       = create_mvp(angle);
	auto* ren = ctx.renderer();

	pass = rpsb->build();

	ren->add_pass(pass);
	ren->set_clear_color(Color(0.2f, 0.2f, 0.2f, 1.f));

	pass->set_uniform("mvp", mvp);
	pass->set_uniform("texs", material);

	MSG msg;
	DrawCmd cmd(vbuff, ibuff, 27);

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
