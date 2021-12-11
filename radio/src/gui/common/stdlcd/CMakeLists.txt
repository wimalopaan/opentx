set(GUI_SRC
  ${GUI_SRC}
  ../common/stdlcd/widgets.cpp
  ../common/stdlcd/popups.cpp
  )

#define (PCBI6)
#if(PXX2 OR LUA OR MULTIMODULE OR PCBI6)
  set(GUI_SRC ${GUI_SRC} ../common/stdlcd/radio_tools.cpp)
#endif()

if(HELI)
  set(GUI_SRC ${GUI_SRC} ../common/stdlcd/model_heli.cpp)
endif()

add_definitions(-DGRAPHICS)
set(GUI_SRC
 ${GUI_SRC}
  ../common/stdlcd/model_inputs.cpp
  ../common/stdlcd/model_mixes.cpp
  ../common/stdlcd/view_text.cpp
)
set(CURVE_EDIT_SRC )


set(GUI_SRC
  ${GUI_SRC}
  ../common/stdlcd/model_curves.cpp
  model_curve_edit.cpp
  )

if(SDCARD)
  set(GUI_SRC
    ${GUI_SRC}
    ../common/stdlcd/radio_sdmanager.cpp
    )
endif()
