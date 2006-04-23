// define public render device caps

#ifndef GNGFX_CAPS
#error GNGFX_CAPS is undefined!
#endif

// TODO: caps of non-power-of-two texture

GNGFX_CAPS( MAX_2D_TEXTURE_SIZE ) //!< max texture size in both X and Y dimension
GNGFX_CAPS( MAX_CLIP_PLANES     ) //!< max number of user clip planes
GNGFX_CAPS( MAX_RENDER_TARGETS  ) //!< max number of render targets
GNGFX_CAPS( MAX_PRIMITIVES      ) //!< max number of primitives in one draw
GNGFX_CAPS( MAX_TEXTURE_STAGES  ) //!< max texture stages supported by fixed pipeline
GNGFX_CAPS( PER_STAGE_CONSTANT  ) //!< support per-stage constant or not?
GNGFX_CAPS( PS                  ) //!< pixel shader caps
GNGFX_CAPS( VS                  ) //!< vertex shader caps
