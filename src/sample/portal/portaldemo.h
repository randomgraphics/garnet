#ifndef __EXAMPLES_portaldemo_portaldemo_H
#define __EXAMPLES_portaldemo_portaldemo_H
/*****************************************************************************\
  filename          : portaldemo.h
  create time       : 2002/10/11
  created by        : chenlee
  description       :
\*****************************************************************************/

#include "level.h"

// main class
class portaldemo_c : public SampleApp {
    //*********************************
    // from demotask_c
    //*********************************
protected:
    bool on_initialize();
    void on_finalize();
    void on_frame_update(float);
    void on_frame_render();
    bool on_key_press(input_i::keyevent_s);
    void on_axis_move(input_i::axis_t, int);

    //*********************************
    //    private variables
    //*********************************
private:
    //! \brief portal clipping camera stuff
    camctrl_c m_camctrl2;
    frame_c   m_camframe2;

    render_i::light_s m_light0; //! \brief light0

    //! \brief level data
    level_c m_level;

    //*********************************
    //    private functions
    //*********************************
private:
};

/*****************************************************************************\
                              END of portaldemo.h
\*****************************************************************************/
#endif
