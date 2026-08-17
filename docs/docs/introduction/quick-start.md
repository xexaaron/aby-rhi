# Quick Start

C++ Quick start template that is "window agnostic". You must create the window, etc.

If you want a more in depth example into the test directory at the platform specific tests.

```cpp
// Context is a singleton. Retrieve the instance instead of creating one.
auto& ctx = Context::get();

// Fill out the context parameters with:
// - the renderer backend
// - the window backend/platform
// - the native window handle
// - any additional features
ContextParams context_params{...};

// Initialize the context before doing any other work.
if (!ctx.init(context_params))
    return 1;

// If using the default file IO interface, set the working directory
// and cache directory. These paths are used for loading resources such
// as textures, shaders, and other files.
ctx.file_io()->set_cwd(fs::path(argv[0]).parent_path());
ctx.file_io()->set_cache_dir(ctx.file_io()->cwd() / "cache");


// Create a render pass.
//
// Add shaders, configure uniforms, and define the vertex layout.
// The vertex description builder returns back to the render pass builder.
auto rpsb = RenderPassBuilder::create();

auto pass = rpsb->add_shader("vertex_shader.vert")
    .add_uniform(...)
    .vertex_description_builder()
        .add_input<&Vertex::pos>(EFormat::rgb_f32)
        .add_inputs<&Vertex::uv, &Vertex::color>(EFormat::rg_f32, EFormat::rgba_f32)
        .build()
    ->add_shader("fragment_shader.frag")
    .use_all_defaults()
    .set_cull_mode(ECullMode::front, EFrontFace::counter_clockwise)
    .disable_blending()
    .disable_depthtest()
    .build();

// Register the render pass with the renderer.
ctx.renderer().add_pass(pass);


// Create vertex and index buffers.
//
// This creates a vertex buffer capable of holding 100 vertices.
auto vbuff = VertexBuffer::create(100, sizeof(Vertex));

// This creates an index buffer capable of holding 60 uint32_t indices.
auto ibuff = IndexBuffer::create(60);

// Push individual vertices and indices into the buffers.
//
// sizeof(Vertex) must match the size passed when creating the vertex buffer.
Vertex v{...};

vbuff->push(&v);
ibuff->push(uint32_t(1));


// Create a draw command from the vertex and index buffers.
DrawCmd cmd(vbuff, ibuff);

// Main render loop.
auto* renderer = ctx.renderer();

while (running) {
    ///// event loop logic

    if (!running)
        break;

    if (!renderer->on_begin())
        continue;

    ///// update logic

    // Update dynamic uniforms.
    pass->set_uniforms(...);

    // Submit draw commands.
    pass->submit(cmd);

    renderer->on_end();
}


// Cleanup.
ctx.deinit();
```