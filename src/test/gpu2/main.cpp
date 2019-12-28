#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

struct gpu2ex
{
    AutoRef<Gpu2> gpu;
    AutoRef<Gpu2::CommandList> cl;
    AutoRef<Gpu2::MemoryBlock> um, dm;

    gpu2ex(GN::win::Window * window)
    {
        gpu = GN::gfx::Gpu2::createGpu2({window});
        cl = gpu->createCommandList({});
        um = gpu->createMemoryBlock({16, Gpu2::MemoryType::UPLOAD});
        dm = gpu->createMemoryBlock({16, Gpu2::MemoryType::DEFAULT});
    }
};

class DX12Triangle : public StdClass
{
    GN_DECLARE_STDCLASS(DX12Triangle, StdClass);

    struct Vertex
    {
        float x, y, z;
        int : 32; // padding;
    };

    gpu2ex & _g;
    AutoRef<Gpu2::Surface> _vb;
    uint64_t _pso = 0;

    void clear()
    {
        _pso = 0;
    }

    void initVB()
    {
        // declare vertices
        Vertex vertices[] = {
            { -0.5f, -0.5f, .0f },
            {  0.5f, -0.5f, .0f },
            {  0.0f,  0.5f, .0f },
        };

        // create a upload buffer that holds vertices
        Gpu2::SurfaceCreationParameters vbcp;
        vbcp.memory = _g.um;
        vbcp.offset = 0;
        vbcp.dim = Gpu2::SurfaceDimension::BUFFER;
        vbcp.b.bytes = sizeof(vertices);
        auto upload = _g.gpu->createSurface(vbcp);

        // copy vertices to upload buffer
        auto mapped = upload->map(0);
        memcpy(mapped.ptr, vertices, sizeof(vertices));
        upload->unmap(0);

        // create vb for rendering
        vbcp.memory = _g.dm;
        _vb = _g.gpu->createSurface(vbcp);

        // copy vertices to vb for rendering
        auto cl = _g.gpu->createCommandList({});
        cl->copySurface(upload.rawptr(), _vb.rawptr());
        _g.gpu->kickoff(*cl);
        _g.gpu->finish();
    }

    void initPSO()
    {
        // compile vertex and pixel shader
        const char * hlsl = R"(
            struct VSInput
            {
                float3 position : POSITION0;
            };

            struct VSOutput
            {
                float4 position : SV_POSITION;
            };

            VSOutput vsmain(VSInput v)
            {
                VSOutput o;
                o.position = float4(v, 1.0f);
                return o;
            };

            float4 psmain(VSOutput v) : SV_TARGET0
            {
                return float4(1.0, 1.0, 1.0, 1.0);
            }
        )";
        auto vs = compileHLSL({hlsl, 0, "vsmain", "vs_5_0"});
        auto ps = compileHLSL({hlsl, 0, "psmain", "ps_5_0"});
        if (vs.empty() || ps.empty()) return;

        // create pso
        Gpu2::InputElement ie[] = { {"POSITION", 0, ColorFormat::RGB_32_32_32_FLOAT, 0, 0, false, 0} };
        Gpu2::PipelineCreationParameters pcp = {};
        pcp.vs = { vs.rawptr(), vs.size() };
        pcp.ps = { ps.rawptr(), ps.size() };
        pcp.inputElements = ie;
        pcp.numInputElements = 1;
        _pso = _g.gpu->createPipelineStates(&pcp, 1)[0];
    }

public:
    
    DX12Triangle(gpu2ex & g) : _g(g)
    {
        clear();
    }

    ~DX12Triangle()
    {
        quit();
    }

    bool init()
    {
        GN_STDCLASS_INIT();

        initVB();
        initPSO();

        // done
        return success();
    }

    void quit()
    {
        _vb = nullptr;
        if (_g.gpu && _pso) _g.gpu->deletePipelineStates(&_pso, 1), _pso = 0;
        GN_STDCLASS_QUIT();
    }

    void render(gpu2ex & g)
    {
        Gpu2::VertexBufferView vb[] = {{_vb, 0, sizeof(Vertex)}};
        g.cl->draw({
            _pso,
            GN_ARRAY_COUNT(vb), vb,
            nullptr, // no ib
            PrimitiveType::TRIANGLE_LIST,
            3});
    }
};

int main()
{
    enableCRTMemoryCheck();

    // create a window
    std::unique_ptr<GN::win::Window> mainWindow(GN::win::createWindow({"GPU2 test", 1280, 720}));
    mainWindow->show();

    // initialize input system
    initializeInputSystem( InputAPI::NATIVE );
    gInput.attachToWindow( mainWindow->getDisplayHandle(), mainWindow->getWindowHandle() );

    // create GPU2
    gpu2ex g(mainWindow.get());

    // create a triangle
    DX12Triangle tri(g);
    GN_VERIFY(tri.init());

    // main loop
    while(mainWindow->runUntilNoNewEvents()) {
        gInput.processInputEvents();
        if( gInput.getKeyStatus( KeyCode::ESCAPE ).down || gInput.getKeyStatus( KeyCode::XB360_A ).down )
        {
            break;
        }

        g.cl->reset();
        g.cl->clear({{0.f, 1.f, 0.f, 0.f}});
        tri.render(g);
        g.gpu->kickoff(*g.cl);
        g.gpu->present({});
    }

    // ensure all rendering is done before everything tears down.
    g.gpu->finish();

    // done
    GN::input::shutdownInputSystem();
    return 0;
}
