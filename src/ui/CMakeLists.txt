SET(SOURCES screens/ui_Screen2.c
    screens/ui_Screen3.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_home_22_png.c
    images/ui_img_1329202414.c
    images/ui_img_performance_png.c
    fonts/ui_font_Font1.c
    fonts/ui_font_Font2.c
    fonts/ui_font_font350.c
    fonts/ui_font_font360.c
    fonts/ui_font_font500.c
    fonts/ui_font_Fontth100.c
    fonts/ui_font_Fontth35.c)

add_library(ui ${SOURCES})
