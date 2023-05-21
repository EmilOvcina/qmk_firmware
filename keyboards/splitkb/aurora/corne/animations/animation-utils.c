/**
 * Utilities for QMK oled animations
 *
 * Copyright (c) Marek Piechut
 * MIT License
 */
#pragma once

//-------- CONFIGURATION START --------
#ifndef FAST_TYPE_WPM
  #define FAST_TYPE_WPM 10 //Switch to fast animation when over words per minute
#endif
#ifndef ANIM_INVERT
  #define ANIM_INVERT false //Invert animation color and background
#endif
#ifndef ANIM_BG
  #define ANIM_BG 0x00
#endif
#ifndef ANIM_FRAME_TIME
  #define ANIM_FRAME_TIME 250
#endif
//-------- CONFIGURATION END--------
#define OLED_ROWS OLED_DISPLAY_HEIGHT / 4

static void oled_render_anim_frame(const char **fast_frames, const char **slow_frames, uint8_t frames_len) {

  static uint32_t anim_timer = 0;
  static uint8_t current_frame = 0;
  static int16_t frame_offset = 0;

  const uint8_t speed = 50;


  if (timer_elapsed32(anim_timer) > ANIM_FRAME_TIME && speed > 0 && rgblight_get_val() == 0) {
    oled_set_cursor(0, 0);
    anim_timer = timer_read32();

    const bool is_fast = speed > FAST_TYPE_WPM;
    const char *frame = is_fast ? fast_frames[current_frame] : slow_frames[current_frame];
    const int8_t bg = ANIM_BG;

    uint8_t frame_start_offset =  0;
    current_frame = (current_frame + 1) % frames_len;

    for(int offset = 0; offset <= OLED_MATRIX_SIZE; offset++) {
      uint16_t col = offset % OLED_DISPLAY_WIDTH;

      if(col < frame_start_offset) {
        continue;
      } else if (col < frame_offset || col > frame_offset + ANIM_FRAME_WIDTH) {
        oled_write_raw_byte((ANIM_INVERT ? ~bg : bg), offset);
      } else if (col <= OLED_DISPLAY_WIDTH) {
        uint8_t row = offset / OLED_DISPLAY_WIDTH;
        int frame_data_offset = ANIM_FRAME_WIDTH * row + col - frame_offset;
        uint8_t col_data = pgm_read_byte(frame + frame_data_offset);
        oled_write_raw_byte(ANIM_INVERT ? ~col_data : col_data, offset );
      }
    }
  }
}


// /**
//  * Utilities for QMK oled animations
//  *
//  * Copyright (c) Marek Piechut
//  * MIT License
//  */
// #pragma once

// //-------- CONFIGURATION START --------
// #ifndef ANIM_INVERT
//   #define ANIM_INVERT false //Invert animation color and background
// #endif
// #ifndef ANIM_BG
//   #define ANIM_BG 0x00
// #endif
// #ifndef ANIM_FRAME_TIME
//   #define ANIM_FRAME_TIME 250
// #endif
// //-------- CONFIGURATION END--------

// #define OLED_ROWS OLED_DISPLAY_HEIGHT / 4

// #include <stdio.h>

// static void oled_render_anim_frame(const char **fast_frames, const char **slow_frames, uint8_t frames_len) {

//   static uint32_t anim_timer = 0;
//   static uint8_t current_frame = 0;
//   static int16_t frame_offset = 0;
// //   static int8_t step = 8;

//   const uint8_t speed = 50;


//   if (timer_elapsed32(anim_timer) > ANIM_FRAME_TIME && speed > 0) {
//     oled_set_cursor(0, 0);
//     anim_timer = timer_read32();

//     const bool is_fast = true;
//     const char *frame = is_fast ? fast_frames[current_frame] : slow_frames[current_frame];
//     const int8_t bg = ANIM_BG;

//     uint8_t frame_start_offset = 0;
//     current_frame = (current_frame + 1) % frames_len;

//     for(int offset = 0; offset <= OLED_MATRIX_SIZE; offset++) {
//       uint16_t col = offset % OLED_DISPLAY_WIDTH;

//       if(col < frame_start_offset) {
//         continue;
//       } else if (col < frame_offset || col > frame_offset + ANIM_FRAME_WIDTH) {
//         oled_write_raw_byte((ANIM_INVERT ? ~bg : bg), offset);
//       } else if (col <= OLED_DISPLAY_WIDTH) {
//         uint8_t row = offset / OLED_DISPLAY_WIDTH;
//         int frame_data_offset = ANIM_FRAME_WIDTH * row + col - frame_offset;
//         uint8_t col_data = pgm_read_byte(frame + frame_data_offset);
//         oled_write_raw_byte(ANIM_INVERT ? ~col_data : col_data, offset );
//       }
//     }
//   }
// }
