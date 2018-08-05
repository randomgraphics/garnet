# ifndef __EXAMPLES_portaldemo_portaldemo_H
# define __EXAMPLES_portaldemo_portaldemo_H
/*****************************************************************************\
  filename          : portaldemo.h
  create time       : 2002/10/11
  created by        : chenlee
  description       :
\*****************************************************************************/

#include <garnet/GNutil.h>
#include "level.h"

// main class
class portaldemo_c : public GN::util::SampleApp
{
    //*********************************
    // from demotask_c
    //*********************************
protected :

    bool onInit(); override;
    void onQuit(); override;
    void onUpdate() override;
    void onRender() override;
    void onKeyPress( input::KeyEvent ) override;
    void onAxisMove( input::Axis, int ) override;

    //*********************************
    //    private variables
    //*********************************
private:

    //! \brief portal clipping camera stuff
    //camctrl_c                m_camctrl2;
    //frame_c                  m_camframe2;

    //render_i::light_s        m_light0; //! \brief light0

    //! \brief level data
    level_c                  m_level;

    //*********************************
    //    private functions
    //*********************************
private:
};

/*****************************************************************************\
                              END of portaldemo.h
\*****************************************************************************/
# endif
