# include "pch.h"
# include "portaldemo.h"

bool halt = false;

//
// initialization
// ----------------------------------------------------------------------------
bool portaldemo_c::onInit()
{
    GN_GUARD;

    // initialize overview camera
    get_camctrl().lookat( Vector3f(1000.0f, 1000.0f, 1000.0f),
                          Vector3f(0.0f, 0.0f, 0.0f),
                          Vector3f(0.0f, 1.0f, 0.0f) );
    get_camctrl().set_move_speed( 500.0f );

    // initialize camera 2 stuff
    m_camctrl2.set_position( Vector3f(-1205.96f,243.55f,954.04f) );
    m_camctrl2.set_orientation( quaternion_c::IDENTITY );
    m_camctrl2.set_move_speed( 200.0f );
    input::KeyCode keys[] = {
        input::KeyCode::I,
        input::KeyCode::K,
        input::KeyCode::J,
        input::KeyCode::L,
        input::KeyCode::INSERT,
        input::KeyCode::_DELETE,
        input::KeyCode::NUMPAD_5,
        input::KeyCode::NUMPAD_8,
        input::KeyCode::NUMPAD_4,
        input::KeyCode::NUMPAD_6,
        input::KeyCode::NONE,
        input::KeyCode::NONE,
        input::KeyCode::MOUSEBTN_1,
        input::KeyCode::MOUSEBTN_2,
    };
    m_camctrl2.bind_all(keys);

    // 初始化照相机
    get_camera().set_frustum( GN_PI/4.0f, // fovy
                              4.0f/3.0f,  // ratio
                              100.0f,     // z-near
                              50000.0f ); // z-far

    // load level
    if ( !m_level.init("data/level.lvl") ) return false;

    // set default lighting parameters
    GN_INFO( GN_T("Set Default Rendering Status ....") );
    g_render.enable_light( 0, true );
    g_render.set_light_type( 0, render_i::POINT_LIGHT );

    // create effects
    effitem_s eff;

    // no light effect
    eff.from_file = false;
    eff.desc.reset( effdesc_s::RESET_TO_DEFAULT );
    eff.desc.rsblock.set_rs(
        GNgfx::RS_DEPTH_TEST, GNgfx::RSVAL_FALSE );
    eff.desc.rsblock.set_tss(
        0, GNgfx::TSS_COLOROP, GNgfx::TSSVAL_ARG0 );
    eff.desc.rsblock.set_tss(
        0, GNgfx::TSS_COLORARG0, GNgfx::TSSVAL_CONSTANT_COLOR );
    eff.desc.rsblock.set_tss(
        0, GNgfx::TSS_ALPHAOP, GNgfx::TSSVAL_ARG0 );
    eff.desc.rsblock.set_tss(
        0, GNgfx::TSS_ALPHAARG0, GNgfx::TSSVAL_CONSTANT_ALPHA );
    if ( !g_efflib.add( GN_T("portaldemo_nolight.gfx"), eff, 0 ) )
        return false;

    // light effect
    eff.desc.rsblock.set_rs(
        GNgfx::RS_LIGHTING_ENABLE, GNgfx::RSVAL_TRUE );
    eff.desc.rsblock.set_rs(
        GNgfx::RS_BLEND_ENABLE, GNgfx::RSVAL_TRUE );
    eff.desc.rsblock.set_tss(
        0, GNgfx::TSS_COLORARG0, GNgfx::TSSVAL_CURRENT_COLOR );
    eff.desc.rsblock.set_tss(
        0, GNgfx::TSS_ALPHAARG0, GNgfx::TSSVAL_CURRENT_ALPHA );
    if ( !g_efflib.add( GN_T("portaldemo_light.gfx"), eff, 0 ) )
        return false;

    // initialize help screen
    const wchar_t help [] =
        L" F1              切换帮助屏幕\n"
        L" F2              Toogle FFP Mode\n"
        L" F5              刷新资源库\n"
        L"\n"
        L" TAB             切换当前视点\n"
        L" 鼠标左键        转动当前视点\n"
        L"\n"
        L" A/S/D/W         平移鸟瞰视点\n"
        L" 方向键          转动鸟瞰视点\n"
        L"\n"
        L" I/J/K/L         平移剪裁视点\n"
        L" 小键盘4/5/6/8   转动剪裁视点\n";
    set_help_text( help );

    // success
    return true;

    GN_UNGUARD;
}

//
// finalization
// ----------------------------------------------------------------------------
void portaldemo_c::on_finalize()
{
    GN_GUARD;

    // clear level data
    m_level.quit();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void portaldemo_c::on_frame_update( float elapsed_time )
{
    GN_GUARD_SLOW;

    // update portal clipping camera stuff
    if ( !halt ) m_camctrl2.update( elapsed_time );

    m_camframe2.set_l2p( Matrix44f(
        m_camctrl2.get_orientation().to_matrix33(),
        m_camctrl2.get_position() ) );

    // make light 0 always stays at eye's position
    // FIXME: position of light is wrong, when using "rgl" module
    g_render.set_light_position( 0, m_camframe2.get_pos_rs() );

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void portaldemo_c::on_frame_render()
{
    GN_GUARD;

    // clear screen
    g_render.clear_screen();

    // update transformation matrices
    get_camera().update_renderer();

    // draw coordinate
    draw_coordinates();

    // draw level
    if ( m_level.isok() )
    {
        g_render.set_world( Matrix44f::IDENTITY );

        float fo,ra,zn,zf;
        get_camera().get_frustum( fo, ra, zn, zf );
        zf /= 2.0f;
        Matrix44f proj;
        g_render.compute_perspective_matrix( proj, fo, ra, zn, zf );
        m_level.draw( proj, m_camframe2.get_r2l() );
    }

    const render_i::scrdesc_s & sd = g_render.get_scrdesc();

    // write FPS
    draw_statistics( 10.0f, sd.height-80.0f );

    // write camera 2 info. on screen
    const Vector3f & c_pos = m_camctrl2.get_position();
    string_wc str = GN_formatwcs(
        L"Camera 2 : (%.2f,%.2f,%.2f)",
        c_pos.x, c_pos.y, c_pos.z );
    draw_textw( str, 10, sd.height-20.0f, 0 );

    GN_UNGUARD;
}

//
// handle keyboard events
// ----------------------------------------------------------------------------
bool portaldemo_c::on_key_press( input_i::keyevent_s key )
{
    GN_GUARD;

    // process camera shifting
    if ( input_i::KEY_TAB == key.code && key.down )
    {
        if ( &get_camframe() == get_camera().get_frame() )
        {
            get_camera().set_frame( &m_camframe2 );
        }
        else
        {
            get_camera().set_frame( &get_camframe() );
        }
        return true;
    }

    // unprocessed key events
    return false;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void portaldemo_c::on_axis_move( input_i::axis_t a, int d )
{
    if ( &get_camframe() == get_camera().get_frame() )
        get_camctrl().on_axis_move( a, d );
    else
        m_camctrl2.on_axis_move( a, d );
}

//
//! \brief main function
// ----------------------------------------------------------------------------
int main( int argc, const char * argv[] )
{
    GN_GUARD_ALWAYS;

    GNapp::import_standard_cif_libs();

    GNapp::application_c app;

    // 初始化应用程序
    config_c conf;
    config_c::load_from_cmdline( conf, argc, argv );
    if ( !app.init( GN_T("Portal Demo"), conf ) ) return -1;

    // create task
    app.push_client_task( new portaldemo_c );

    // main loop
    return app.run();

    GN_UNGUARD_ALWAYS_NOTHROW;
    return -1;
}
