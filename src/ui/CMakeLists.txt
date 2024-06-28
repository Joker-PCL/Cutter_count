SET(SOURCES ui_Main.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    ui_img_1989276256.c
    ui_img_home_22_png.c
    fonts/ui_font_Font1.c
    fonts/ui_font_Font2.c
    fonts/ui_font_Font340.c
    fonts/ui_font_font350.c
    fonts/ui_font_font360.c
    fonts/ui_font_font500.c
    fonts/ui_font_Fontth100.c
    fonts/ui_font_Fontth35.c)

add_library(ui ${SOURCES})
