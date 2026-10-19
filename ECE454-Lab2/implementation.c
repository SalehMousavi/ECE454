#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

#define KEY_D 'D'
#define KEY_A 'A'
#define KEY_W 'W'
#define KEY_S 'S'
#define KEY_CW 'W'
#define KEY_CCW 'C'
#define KEY_MX 'X'
#define KEY_MY 'Y'


typedef struct pixel {
    unsigned rgb; // 8b padding 8b R 8b G 8b B
    unsigned x;
    unsigned y; 
} pixel_t;

typedef struct shift_matrix {
    int m11;
    int m12;
    int m13;
    int m21;
    int m22;
    int m23; // last row is always 0 0 1
} shift_matrix_t;

// static pixel_t * global_buffer = NULL;
// static unsigned non_white = 0;

// Declariations
void convertFrame(unsigned char* frame_buffer, unsigned width, shift_matrix_t* global_shift_matrix, unsigned non_white, pixel_t* global_buffer);

static inline unsigned max(unsigned a, unsigned b)
{
    return (a < b)? b : a;
}

static inline unsigned min(unsigned a, unsigned b)
{
    return (a > b)? b : a;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
void convertFrame(unsigned char* frame_buffer, unsigned width, shift_matrix_t* global_shift_matrix, unsigned non_white, pixel_t* global_buffer)
{
    register unsigned wid_r = width;
    register unsigned m11_r = global_shift_matrix->m11;
    register unsigned m12_r = global_shift_matrix->m12;
    register unsigned m13_r = global_shift_matrix->m13;
    register unsigned m21_r = global_shift_matrix->m21;
    register unsigned m22_r = global_shift_matrix->m22;
    register unsigned m23_r = global_shift_matrix->m23;
    unsigned pixel_idx;
    for(pixel_idx = 0; pixel_idx < (non_white - (non_white & 7)); pixel_idx+= 8)
    {
        unsigned cur_x = global_buffer[pixel_idx].x;
        unsigned cur_y = global_buffer[pixel_idx].y;

        unsigned new_x = global_buffer[pixel_idx].x = cur_x * m11_r + cur_y * m12_r + m13_r;
        unsigned new_y = global_buffer[pixel_idx].y = cur_x * m21_r + cur_y * m22_r + m23_r;

        unsigned cur_pixel_idx = (new_y * wid_r + new_x) * 3;
        unsigned cur_pixel = global_buffer[pixel_idx].rgb;
        *(unsigned short*)(frame_buffer + cur_pixel_idx) = cur_pixel & 0xFFFF;
        frame_buffer[cur_pixel_idx + 2] = (cur_pixel >> 16) & 0xFF;

        // unroll 2
        unsigned cur_x2 = global_buffer[pixel_idx+1].x;
        unsigned cur_y2 = global_buffer[pixel_idx+1].y;

        unsigned new_x2 = global_buffer[pixel_idx+1].x = cur_x2 * m11_r + cur_y2 * m12_r + m13_r;
        unsigned new_y2 = global_buffer[pixel_idx+1].y = cur_x2 * m21_r + cur_y2 * m22_r + m23_r;

        unsigned cur_pixel_idx2 = (new_y2 * wid_r + new_x2) * 3;
        unsigned cur_pixel2 = global_buffer[pixel_idx+1].rgb;
        *(unsigned short*)(frame_buffer + cur_pixel_idx2) = cur_pixel2 & 0xFFFF;
        frame_buffer[cur_pixel_idx2 + 2] = (cur_pixel2 >> 16) & 0xFF;

        // unroll 3
        unsigned cur_x3 = global_buffer[pixel_idx+2].x;
        unsigned cur_y3 = global_buffer[pixel_idx+2].y;

        unsigned new_x3 = global_buffer[pixel_idx+2].x = cur_x3 * m11_r + cur_y3 * m12_r + m13_r;
        unsigned new_y3 = global_buffer[pixel_idx+2].y = cur_x3 * m21_r + cur_y3 * m22_r + m23_r;

        unsigned cur_pixel_idx3 = (new_y3 * wid_r + new_x3) * 3;
        unsigned cur_pixel3 = global_buffer[pixel_idx+2].rgb;
        *(unsigned short*)(frame_buffer + cur_pixel_idx3) = cur_pixel3 & 0xFFFF;
        frame_buffer[cur_pixel_idx3 + 2] = (cur_pixel3 >> 16) & 0xFF;

        // unroll 4
        unsigned cur_x4 = global_buffer[pixel_idx+3].x;
        unsigned cur_y4 = global_buffer[pixel_idx+3].y;

        unsigned new_x4 = global_buffer[pixel_idx+3].x = cur_x4 * m11_r + cur_y4 * m12_r + m13_r;
        unsigned new_y4 = global_buffer[pixel_idx+3].y = cur_x4 * m21_r + cur_y4 * m22_r + m23_r;

        unsigned cur_pixel_idx4 = (new_y4 * wid_r + new_x4) * 3;
        unsigned cur_pixel4 = global_buffer[pixel_idx+3].rgb;
        *(unsigned short*)(frame_buffer + cur_pixel_idx4) = cur_pixel4 & 0xFFFF;
        frame_buffer[cur_pixel_idx4 + 2] = (cur_pixel4 >> 16) & 0xFF;

        // unroll 5
        unsigned cur_x5 = global_buffer[pixel_idx+4].x;
        unsigned cur_y5 = global_buffer[pixel_idx+4].y;

        unsigned new_x5 = global_buffer[pixel_idx+4].x = cur_x5 * m11_r + cur_y5 * m12_r + m13_r;
        unsigned new_y5 = global_buffer[pixel_idx+4].y = cur_x5 * m21_r + cur_y5 * m22_r + m23_r;

        unsigned cur_pixel_idx5 = (new_y5 * wid_r + new_x5) * 3;
        unsigned cur_pixel5 = global_buffer[pixel_idx+4].rgb;
        *(unsigned short*)(frame_buffer + cur_pixel_idx5) = cur_pixel5 & 0xFFFF;
        frame_buffer[cur_pixel_idx5 + 2] = (cur_pixel5 >> 16) & 0xFF;

        // unroll 6
        unsigned cur_x6 = global_buffer[pixel_idx+5].x;
        unsigned cur_y6 = global_buffer[pixel_idx+5].y;

        unsigned new_x6 = global_buffer[pixel_idx+5].x = cur_x6 * m11_r + cur_y6 * m12_r + m13_r;
        unsigned new_y6 = global_buffer[pixel_idx+5].y = cur_x6 * m21_r + cur_y6 * m22_r + m23_r;

        unsigned cur_pixel_idx6 = (new_y6 * wid_r + new_x6) * 3;
        unsigned cur_pixel6 = global_buffer[pixel_idx+5].rgb;
        *(unsigned short*)(frame_buffer + cur_pixel_idx6) = cur_pixel6 & 0xFFFF;
        frame_buffer[cur_pixel_idx6 + 2] = (cur_pixel6 >> 16) & 0xFF;

        // unroll 7
        unsigned cur_x7 = global_buffer[pixel_idx+6].x;
        unsigned cur_y7 = global_buffer[pixel_idx+6].y;

        unsigned new_x7 = global_buffer[pixel_idx+6].x = cur_x7 * m11_r + cur_y7 * m12_r + m13_r;
        unsigned new_y7 = global_buffer[pixel_idx+6].y = cur_x7 * m21_r + cur_y7 * m22_r + m23_r;

        unsigned cur_pixel_idx7 = (new_y7 * wid_r + new_x7) * 3;
        unsigned cur_pixel7 = global_buffer[pixel_idx+6].rgb;
        *(unsigned short*)(frame_buffer + cur_pixel_idx7) = cur_pixel7 & 0xFFFF;
        frame_buffer[cur_pixel_idx7 + 2] = (cur_pixel7 >> 16) & 0xFF;

        // unroll 8
        unsigned cur_x8 = global_buffer[pixel_idx+7].x;
        unsigned cur_y8 = global_buffer[pixel_idx+7].y;

        unsigned new_x8 = global_buffer[pixel_idx+7].x = cur_x8 * m11_r + cur_y8 * m12_r + m13_r;
        unsigned new_y8 = global_buffer[pixel_idx+7].y = cur_x8 * m21_r + cur_y8 * m22_r + m23_r;

        unsigned cur_pixel_idx8 = (new_y8 * wid_r + new_x8) * 3;
        unsigned cur_pixel8 = global_buffer[pixel_idx+7].rgb;
        *(unsigned short*)(frame_buffer + cur_pixel_idx8) = cur_pixel8 & 0xFFFF;
        frame_buffer[cur_pixel_idx8 + 2] = (cur_pixel8 >> 16) & 0xFF;

    }
    for(;pixel_idx < non_white; pixel_idx++)
    {
        unsigned cur_x = global_buffer[pixel_idx].x;
        unsigned cur_y = global_buffer[pixel_idx].y;

        unsigned new_x = global_buffer[pixel_idx].x = cur_x * m11_r + cur_y * m12_r + m13_r;
        unsigned new_y = global_buffer[pixel_idx].y = cur_x * m21_r + cur_y * m22_r + m23_r;

        unsigned cur_pixel_idx = (new_y * wid_r + new_x) * 3;
        unsigned cur_pixel = global_buffer[pixel_idx].rgb;
        *(unsigned short*)(frame_buffer + cur_pixel_idx) = cur_pixel & 0xFFFF;
        frame_buffer[cur_pixel_idx + 2] = (cur_pixel >> 16) & 0xFF;
    }
}

/***********************************************************************************************************************
 * @param sensor_values - structure stores parsed key value pairs of program instructions
 * @param sensor_values_count - number of valid sensor values parsed from sensor log file or commandline console
 * @param frame_buffer - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param grading_mode - turns off verification and turn on instrumentation
 ***********************************************************************************************************************
 *
 **********************************************************************************************************************/

void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "SM";

    // Please fill in your information
    char student_first_name[] = "Saleh";
    char student_last_name[] = "Mousavi";
    char student_student_number[] = "1008848945";

    // Printing out team information
    printf("*******************************************************************************************************\n");
    printf("Team Information:\n");
    printf("\tteam_name: %s\n", team_name);
    printf("\tstudent_first_name: %s\n", student_first_name);
    printf("\tstudent_last_name: %s\n", student_last_name);
    printf("\tstudent_student_number: %s\n", student_student_number);
}

void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                                     unsigned int width, unsigned int height, bool grading_mode) {
    int processed_frames = 0;
    static shift_matrix_t global_shift_matrix = 
    {
        .m11 = 1,
        .m12 = 0,
        .m13 = 0,
        .m21 = 0,
        .m22 = 1,
        .m23 = 0
    };

    pixel_t* global_buffer = malloc(sizeof (pixel_t) * width * height);
    unsigned non_white = 0;
    if (global_buffer == NULL) 
    {
        printf("Failed to allocate!");
        exit(-1);
    }

    // unsigned boundary_x = 0;
    // unsigned boundary_y = 0;

    unsigned min_x, min_y;
    min_x = min_y = width - 1;
    unsigned max_x, max_y;
    max_x = max_y = 0;

    for (unsigned int i = 0; i < height; i++)
    {
        unsigned int j;
        unsigned row_offset = i * width * 3;
        for (j = 0; j < (width & ~31u); j+=32)
        {
            unsigned column_offset = j * 3;
            unsigned curr_index = row_offset + column_offset;

            unsigned word1 = *(unsigned*)(frame_buffer + curr_index);
            unsigned word2 = *(unsigned*)(frame_buffer + curr_index+ 4);
            unsigned word3 = *(unsigned*)(frame_buffer + curr_index + 8);
            
            unsigned word5 = *(unsigned*)(frame_buffer + curr_index + 12);
            unsigned word6 = *(unsigned*)(frame_buffer + curr_index + 16);
            unsigned word7 = *(unsigned*)(frame_buffer + curr_index + 20);


            unsigned word9 = *(unsigned*)(frame_buffer + curr_index + 24);
            unsigned word10 = *(unsigned*)(frame_buffer + curr_index + 28);
            unsigned word11 = *(unsigned*)(frame_buffer + curr_index + 32);


            unsigned word13 = *(unsigned*)(frame_buffer + curr_index + 36);
            unsigned word14 = *(unsigned*)(frame_buffer + curr_index + 40);
            unsigned word15 = *(unsigned*)(frame_buffer + curr_index + 44);


            unsigned word17 = *(unsigned*)(frame_buffer + curr_index + 48);
            unsigned word18 = *(unsigned*)(frame_buffer + curr_index + 52);
            unsigned word19 = *(unsigned*)(frame_buffer + curr_index + 56);


            unsigned word21 = *(unsigned*)(frame_buffer + curr_index + 60);
            unsigned word22 = *(unsigned*)(frame_buffer + curr_index + 64);
            unsigned word23 = *(unsigned*)(frame_buffer + curr_index + 68);


            unsigned word25 = *(unsigned*)(frame_buffer + curr_index + 72);
            unsigned word26 = *(unsigned*)(frame_buffer + curr_index + 76);
            unsigned word27 = *(unsigned*)(frame_buffer + curr_index + 80);


            unsigned word29 = *(unsigned*)(frame_buffer + curr_index + 84);
            unsigned word30 = *(unsigned*)(frame_buffer + curr_index + 88);
            unsigned word31 = *(unsigned*)(frame_buffer + curr_index + 92);


            signed local_min_x = -1;
            signed local_max_x = 0;

            bool non_white_found = false;
            if (word1 != 0xFFFFFFFF || word2 != 0xFFFFFFFF || word3 != 0xFFFFFFFF)
            {
                unsigned pixel1 = (word1 & 0xFFFFFF);
                unsigned pixel2 = ((word1 & 0xFF000000) >> 24) | (word2 & 0xFFFF) << 8;
                unsigned pixel3 = ((word2 & 0xFFFF0000) >> 16) | (word3 &0xFF) << 16;
                unsigned pixel4 = (word3 & 0xFFFFFF00) >> 8;
            
                if (pixel1 != 0xFFFFFF)
                {
                    global_buffer[non_white].rgb = pixel1;
                    global_buffer[non_white].x = j;
                    global_buffer[non_white].y = i;
                    non_white++;
                     local_min_x = j;
                     local_max_x = j;
                     non_white_found = true;
                    frame_buffer[curr_index] = 0xFF;
                    frame_buffer[curr_index+1] = 0xFF;
                    frame_buffer[curr_index+2] = 0xFF;
                }
                if (pixel2 != 0xFFFFFF)
                {
                    global_buffer[non_white].rgb = pixel2;
                    global_buffer[non_white].x = j+1;
                    global_buffer[non_white].y = i;
                    non_white++;
                     if (local_min_x == -1)
                         local_min_x = j+1;
                     local_max_x = j+1;
                     non_white_found = true;
                    frame_buffer[curr_index+3] = 0xFF;
                    frame_buffer[curr_index+4] = 0xFF;
                    frame_buffer[curr_index+5] = 0xFF;
                }
                if (pixel3 != 0xFFFFFF)
                {
                    global_buffer[non_white].rgb = pixel3;
                    global_buffer[non_white].x = j+2;
                    global_buffer[non_white].y = i;
                    non_white++;
                    if (local_min_x == -1)
                        local_min_x = j+2;
                    local_max_x = j+2;
                    non_white_found = true;
                    frame_buffer[curr_index+6] = 0xFF;
                    frame_buffer[curr_index+7] = 0xFF;
                    frame_buffer[curr_index+8] = 0xFF;
                }
                if (pixel4 != 0xFFFFFF)
                {
                    global_buffer[non_white].rgb = pixel4;
                    global_buffer[non_white].x = j+3;
                    global_buffer[non_white].y = i;
                    non_white++;
                     if (local_min_x == -1)
                         local_min_x = j+3;
                     local_max_x = j+3;
                     non_white_found = true;
                    frame_buffer[curr_index+9] = 0xFF;
                    frame_buffer[curr_index+10] = 0xFF;
                    frame_buffer[curr_index+11] = 0xFF;
                }
            }
            if (word5 != 0xFFFFFFFF || word6 != 0xFFFFFFFF || word7 != 0xFFFFFFFF)
            {
                unsigned pixel5 = (word5 & 0xFFFFFF);
                unsigned pixel6 = ((word5 & 0xFF000000) >> 24) | (word6 & 0xFFFF) << 8;
                unsigned pixel7 = ((word6 & 0xFFFF0000) >> 16) | (word7 &0xFF) << 16;
                unsigned pixel8 = (word7 & 0xFFFFFF00) >> 8;
            if (pixel5 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel5;
                global_buffer[non_white].x = j+4;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+4;
                 local_max_x = j+4;
                 non_white_found = true;
                frame_buffer[curr_index+12] = 0xFF;
                frame_buffer[curr_index+13] = 0xFF;
                frame_buffer[curr_index+14] = 0xFF;
            }
            if (pixel6 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel6;
                global_buffer[non_white].x = j+5;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+5;
                 local_max_x = j+5;
                 non_white_found = true;
                frame_buffer[curr_index+15] = 0xFF;
                frame_buffer[curr_index+16] = 0xFF;
                frame_buffer[curr_index+17] = 0xFF;
            }
            if (pixel7 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel7;
                global_buffer[non_white].x = j+6;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+6;
                 local_max_x = j+6;
                 non_white_found = true;
                frame_buffer[curr_index+18] = 0xFF;
                frame_buffer[curr_index+19] = 0xFF;
                frame_buffer[curr_index+20] = 0xFF;
            }
            if (pixel8 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel8;
                global_buffer[non_white].x = j+7;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+7;
                 local_max_x = j+7;
                 non_white_found = true;
                frame_buffer[curr_index+21] = 0xFF;
                frame_buffer[curr_index+22] = 0xFF;
                frame_buffer[curr_index+23] = 0xFF;
            }
            }
            if (word9 != 0xFFFFFFFF || word10 != 0xFFFFFFFF || word11 != 0xFFFFFFFF)
            {
                unsigned pixel9 = (word9 & 0xFFFFFF);
                unsigned pixel10 = ((word9 & 0xFF000000) >> 24) | (word10 & 0xFFFF) << 8;
                unsigned pixel11 = ((word10 & 0xFFFF0000) >> 16) | (word11 &0xFF) << 16;
                unsigned pixel12 = (word11 & 0xFFFFFF00) >> 8;
            if (pixel9 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel9;
                global_buffer[non_white].x = j+8;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+8;
                 local_max_x = j+8;
                 non_white_found = true;
                frame_buffer[curr_index+24] = 0xFF;
                frame_buffer[curr_index+25] = 0xFF;
                frame_buffer[curr_index+26] = 0xFF;
            }
            if (pixel10 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel10;
                global_buffer[non_white].x = j+9;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+9;
                 local_max_x = j+9;
                 non_white_found = true;
                frame_buffer[curr_index+27] = 0xFF;
                frame_buffer[curr_index+28] = 0xFF;
                frame_buffer[curr_index+29] = 0xFF;
            }
            if (pixel11 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel11;
                global_buffer[non_white].x = j+10;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+10;
                 local_max_x = j+10;
                 non_white_found = true;
                frame_buffer[curr_index+30] = 0xFF;
                frame_buffer[curr_index+31] = 0xFF;
                frame_buffer[curr_index+32] = 0xFF;
            }
            if (pixel12 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel12;
                global_buffer[non_white].x = j+11;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+11;
                 local_max_x = j+11;
                 non_white_found = true;
                frame_buffer[curr_index+33] = 0xFF;
                frame_buffer[curr_index+34] = 0xFF;
                frame_buffer[curr_index+35] = 0xFF;
            }
            }
            if (word13 != 0xFFFFFFFF || word14 != 0xFFFFFFFF || word15 != 0xFFFFFFFF)
            {
                unsigned pixel13 = (word13 & 0xFFFFFF);
                unsigned pixel14 = ((word13 & 0xFF000000) >> 24) | (word14 & 0xFFFF) << 8;
                unsigned pixel15 = ((word14 & 0xFFFF0000) >> 16) | (word15 &0xFF) << 16;
                unsigned pixel16 = (word15 & 0xFFFFFF00) >> 8;
            if (pixel13 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel13;
                global_buffer[non_white].x = j+12;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+12;
                 local_max_x = j+12;
                 non_white_found = true;
                frame_buffer[curr_index+36] = 0xFF;
                frame_buffer[curr_index+37] = 0xFF;
                frame_buffer[curr_index+38] = 0xFF;
            }
            if (pixel14 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel14;
                global_buffer[non_white].x = j+13;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+13;
                 local_max_x = j+13;
                 non_white_found = true;
                frame_buffer[curr_index+39] = 0xFF;
                frame_buffer[curr_index+40] = 0xFF;
                frame_buffer[curr_index+41] = 0xFF;
            }
            if (pixel15 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel15;
                global_buffer[non_white].x = j+14;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+14;
                 local_max_x = j+14;
                 non_white_found = true;
                frame_buffer[curr_index+42] = 0xFF;
                frame_buffer[curr_index+43] = 0xFF;
                frame_buffer[curr_index+44] = 0xFF;
            }
            if (pixel16 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel16;
                global_buffer[non_white].x = j+15;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+15;
                 local_max_x = j+15;
                 non_white_found = true;
                frame_buffer[curr_index+45] = 0xFF;
                frame_buffer[curr_index+46] = 0xFF;
                frame_buffer[curr_index+47] = 0xFF;
            }
            }
            if (word17 != 0xFFFFFFFF || word18 != 0xFFFFFFFF || word19 != 0xFFFFFFFF)
            {
                unsigned pixel17 = (word17 & 0xFFFFFF);
                unsigned pixel18 = ((word17 & 0xFF000000) >> 24) | (word18 & 0xFFFF) << 8;
                unsigned pixel19 = ((word18 & 0xFFFF0000) >> 16) | (word19 &0xFF) << 16;
                unsigned pixel20 = (word19 & 0xFFFFFF00) >> 8;
            if (pixel17 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel17;
                global_buffer[non_white].x = j+16;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+16;
                 local_max_x = j+16;
                 non_white_found = true;
                frame_buffer[curr_index+48] = 0xFF;
                frame_buffer[curr_index+49] = 0xFF;
                frame_buffer[curr_index+50] = 0xFF;
            }
            if (pixel18 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel18;
                global_buffer[non_white].x = j+17;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+17;
                 local_max_x = j+17;
                 non_white_found = true;
                frame_buffer[curr_index+51] = 0xFF;
                frame_buffer[curr_index+52] = 0xFF;
                frame_buffer[curr_index+53] = 0xFF;
            }
            if (pixel19 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel19;
                global_buffer[non_white].x = j+18;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+18;
                 local_max_x = j+18;
                 non_white_found = true;
                frame_buffer[curr_index+54] = 0xFF;
                frame_buffer[curr_index+55] = 0xFF;
                frame_buffer[curr_index+56] = 0xFF;
            }
            if (pixel20 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel20;
                global_buffer[non_white].x = j+19;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+19;
                 local_max_x = j+19;
                 non_white_found = true;
                frame_buffer[curr_index+57] = 0xFF;
                frame_buffer[curr_index+58] = 0xFF;
                frame_buffer[curr_index+59] = 0xFF;
            }
            }
            if (word21 != 0xFFFFFFFF || word22 != 0xFFFFFFFF || word23 != 0xFFFFFFFF)
            {
                unsigned pixel21 = (word21 & 0xFFFFFF);
                unsigned pixel22 = ((word21 & 0xFF000000) >> 24) | (word22 & 0xFFFF) << 8;
                unsigned pixel23 = ((word22 & 0xFFFF0000) >> 16) | (word23 &0xFF) << 16;
                unsigned pixel24 = (word23 & 0xFFFFFF00) >> 8;
            if (pixel21 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel21;
                global_buffer[non_white].x = j+20;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+20;
                 local_max_x = j+20;
                 non_white_found = true;
                frame_buffer[curr_index+60] = 0xFF;
                frame_buffer[curr_index+61] = 0xFF;
                frame_buffer[curr_index+62] = 0xFF;
            }
            if (pixel22 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel22;
                global_buffer[non_white].x = j+21;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+21;
                 local_max_x = j+21;
                 non_white_found = true;
                frame_buffer[curr_index+63] = 0xFF;
                frame_buffer[curr_index+64] = 0xFF;
                frame_buffer[curr_index+65] = 0xFF;
            }
            if (pixel23 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel23;
                global_buffer[non_white].x = j+22;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+22;
                 local_max_x = j+22;
                 non_white_found = true;
                frame_buffer[curr_index+66] = 0xFF;
                frame_buffer[curr_index+67] = 0xFF;
                frame_buffer[curr_index+68] = 0xFF;
            }
            if (pixel24 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel24;
                global_buffer[non_white].x = j+23;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+23;
                 local_max_x = j+23;
                 non_white_found = true;
                frame_buffer[curr_index+69] = 0xFF;
                frame_buffer[curr_index+70] = 0xFF;
                frame_buffer[curr_index+71] = 0xFF;
            }
            }
            if (word25 != 0xFFFFFFFF || word26 != 0xFFFFFFFF || word27 != 0xFFFFFFFF)
            {
                unsigned pixel25 = (word25 & 0xFFFFFF);
                unsigned pixel26 = ((word25 & 0xFF000000) >> 24) | (word26 & 0xFFFF) << 8;
                unsigned pixel27 = ((word26 & 0xFFFF0000) >> 16) | (word27 &0xFF) << 16;
                unsigned pixel28 = (word27 & 0xFFFFFF00) >> 8;
            if (pixel25 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel25;
                global_buffer[non_white].x = j+24;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+24;
                 local_max_x = j+24;
                 non_white_found = true;
                frame_buffer[curr_index+72] = 0xFF;
                frame_buffer[curr_index+73] = 0xFF;
                frame_buffer[curr_index+74] = 0xFF;
            }
            if (pixel26 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel26;
                global_buffer[non_white].x = j+25;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+25;
                 local_max_x = j+25;
                 non_white_found = true;
                frame_buffer[curr_index+75] = 0xFF;
                frame_buffer[curr_index+76] = 0xFF;
                frame_buffer[curr_index+77] = 0xFF;
            }
            if (pixel27 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel27;
                global_buffer[non_white].x = j+26;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+26;
                 local_max_x = j+26;
                 non_white_found = true;
                frame_buffer[curr_index+78] = 0xFF;
                frame_buffer[curr_index+79] = 0xFF;
                frame_buffer[curr_index+80] = 0xFF;
            }
            if (pixel28 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel28;
                global_buffer[non_white].x = j+27;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+27;
                 local_max_x = j+27;
                 non_white_found = true;
                frame_buffer[curr_index+81] = 0xFF;
                frame_buffer[curr_index+82] = 0xFF;
                frame_buffer[curr_index+83] = 0xFF;
            }
            }
            if (word29 != 0xFFFFFFFF || word30 != 0xFFFFFFFF || word31 != 0xFFFFFFFF)
            {
                unsigned pixel29 = (word29 & 0xFFFFFF);
                unsigned pixel30 = ((word29 & 0xFF000000) >> 24) | (word30 & 0xFFFF) << 8;
                unsigned pixel31 = ((word30 & 0xFFFF0000) >> 16) | (word31 &0xFF) << 16;
                unsigned pixel32 = (word31 & 0xFFFFFF00) >> 8;
            if (pixel29 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel29;
                global_buffer[non_white].x = j+28;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+28;
                 local_max_x = j+28;
                 non_white_found = true;
                frame_buffer[curr_index+84] = 0xFF;
                frame_buffer[curr_index+85] = 0xFF;
                frame_buffer[curr_index+86] = 0xFF;
            }
            if (pixel30 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel30;
                global_buffer[non_white].x = j+29;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+29;
                 local_max_x = j+29;
                 non_white_found = true;
                frame_buffer[curr_index+87] = 0xFF;
                frame_buffer[curr_index+88] = 0xFF;
                frame_buffer[curr_index+89] = 0xFF;
            }
            if (pixel31 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel31;
                global_buffer[non_white].x = j+30;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+30;
                 local_max_x = j+30;
                 non_white_found = true;
                frame_buffer[curr_index+90] = 0xFF;
                frame_buffer[curr_index+91] = 0xFF;
                frame_buffer[curr_index+92] = 0xFF;
            }
            if (pixel32 != 0xFFFFFF)
            {
                global_buffer[non_white].rgb = pixel32;
                global_buffer[non_white].x = j+31;
                global_buffer[non_white].y = i;
                non_white++;
                 if (local_min_x == -1)
                     local_min_x = j+31;
                 local_max_x = j+31;
                 non_white_found = true;
                frame_buffer[curr_index+93] = 0xFF;
                frame_buffer[curr_index+94] = 0xFF;
                frame_buffer[curr_index+95] = 0xFF;
            }

            }
             if (non_white_found)
             {
                 if (i < min_y)
                     min_y = i;
                 if (i > max_y)
                     max_y = i;
                 if (local_min_x < min_x)
                     min_x = local_min_x;
                 if (local_max_x > max_x)
                     max_x = local_max_x;
             }
        }
        for (; j < width; j++)
        {
            unsigned column_offset = j * 3;
            unsigned curr_index = row_offset + column_offset;
            unsigned pixel1;
            if (j < width - 1)
            {
                pixel1 = *(unsigned *)(frame_buffer + curr_index) & 0xFFFFFF;
            }
            else 
            {
                pixel1 = (*(unsigned short*) (frame_buffer + curr_index) & 0xFFFF) | (frame_buffer[curr_index+2] << 16);
            }

            if(pixel1 != 0xFFFFFF)
            {
                if (i < min_y)
                    min_y = i;
                if (i > max_y)
                    max_y = i;
                if (j < min_x)
                    min_x = j;
                if (j > max_x)
                    max_x = j;

                *(unsigned short *)(frame_buffer + curr_index) = 0xFFFF;
                frame_buffer[curr_index+2] = 0xFF;

                global_buffer[non_white].rgb = pixel1;
                global_buffer[non_white].x = j;
                global_buffer[non_white].y = i;
                non_white++;
            }
        }

    }

    // pixel_t * global_buffer_new = realloc(global_buffer, sizeof(pixel_t) * non_white);
    // if (global_buffer_new == NULL) 
    // {
    //     printf("Failed to reallocate!");
    //     exit(-1);
    // }

    // if (global_buffer_new != global_buffer)
    // {
    //     global_buffer = global_buffer_new;
    // }    
    
    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
//        printf("Processing sensor value #%d: %s, %d\n", sensorValueIdx, sensor_values[sensorValueIdx].key,
//               sensor_values[sensorValueIdx].value);
        char *sensor_key = sensor_values[sensorValueIdx].key;
        register char sensor_key_0 = sensor_key[0];
        register char sensor_key_1 = sensor_key[1];
        if (sensor_key_0 == KEY_D)
        {
            global_shift_matrix.m13 += sensor_values[sensorValueIdx].value;
//            printBMP(width, height, frame_buffer);
        } else if (sensor_key_0 == KEY_A) {
            global_shift_matrix.m13 -= sensor_values[sensorValueIdx].value;
//            printBMP(width, height, frame_buffer);
        } else if (sensor_key_0 == KEY_W) {
            global_shift_matrix.m23 -= sensor_values[sensorValueIdx].value;  
//            printBMP(width, height, frame_buffer);
        } else if (sensor_key_0 == KEY_S) {
            global_shift_matrix.m23 += sensor_values[sensorValueIdx].value;  
//            printBMP(width, height, frame_buffer);
        } else if (sensor_key_1 == KEY_CW) {
            unsigned rotate_iteration = sensor_values[sensorValueIdx].value & 0x3;
            if (rotate_iteration == 1)
            {
                int temp_m11 = global_shift_matrix.m11;
                int temp_m12 = global_shift_matrix.m12;
                int temp_m13 = global_shift_matrix.m13;
                global_shift_matrix.m11 = -global_shift_matrix.m21;
                global_shift_matrix.m12 = -global_shift_matrix.m22;
                global_shift_matrix.m13 = width - 1 - global_shift_matrix.m23;
                global_shift_matrix.m21 = temp_m11;
                global_shift_matrix.m22 = temp_m12;
                global_shift_matrix.m23 = temp_m13;
            }
            else if (rotate_iteration == 2)
            {
                global_shift_matrix.m11 = -global_shift_matrix.m11;
                global_shift_matrix.m12 = -global_shift_matrix.m12;
                global_shift_matrix.m13 = width - 1 - global_shift_matrix.m13;
                global_shift_matrix.m21 = -global_shift_matrix.m21;
                global_shift_matrix.m22 = -global_shift_matrix.m22;
                global_shift_matrix.m23 = width - 1 - global_shift_matrix.m23;
            }
            else if (rotate_iteration == 3)
            {
                int temp_m11 = global_shift_matrix.m11;
                int temp_m12 = global_shift_matrix.m12;
                int temp_m13 = global_shift_matrix.m13;
                global_shift_matrix.m11 = global_shift_matrix.m21;
                global_shift_matrix.m12 = global_shift_matrix.m22;
                global_shift_matrix.m13 = global_shift_matrix.m23;
                global_shift_matrix.m21 = -temp_m11;
                global_shift_matrix.m22 = -temp_m12;
                global_shift_matrix.m23 = width - 1 - temp_m13;
            }
//            printBMP(width, height, frame_buffer);
        } else if (sensor_key_1 == KEY_CCW) {
            unsigned rotate_iteration = sensor_values[sensorValueIdx].value & 0x3;
            if (rotate_iteration != 0)
                rotate_iteration = 4 - rotate_iteration;
            if (rotate_iteration == 1)
            {
                int temp_m11 = global_shift_matrix.m11;
                int temp_m12 = global_shift_matrix.m12;
                int temp_m13 = global_shift_matrix.m13;
                global_shift_matrix.m11 = -global_shift_matrix.m21;
                global_shift_matrix.m12 = -global_shift_matrix.m22;
                global_shift_matrix.m13 = width - 1 - global_shift_matrix.m23;
                global_shift_matrix.m21 = temp_m11;
                global_shift_matrix.m22 = temp_m12;
                global_shift_matrix.m23 = temp_m13;
            }
            else if (rotate_iteration == 2)
            {
                global_shift_matrix.m11 = -global_shift_matrix.m11;
                global_shift_matrix.m12 = -global_shift_matrix.m12;
                global_shift_matrix.m13 = width - 1 - global_shift_matrix.m13;
                global_shift_matrix.m21 = -global_shift_matrix.m21;
                global_shift_matrix.m22 = -global_shift_matrix.m22;
                global_shift_matrix.m23 = width - 1 - global_shift_matrix.m23;
            }
            else if (rotate_iteration == 3)
            {
                int temp_m11 = global_shift_matrix.m11;
                int temp_m12 = global_shift_matrix.m12;
                int temp_m13 = global_shift_matrix.m13;
                global_shift_matrix.m11 = global_shift_matrix.m21;
                global_shift_matrix.m12 = global_shift_matrix.m22;
                global_shift_matrix.m13 = global_shift_matrix.m23;
                global_shift_matrix.m21 = -temp_m11;
                global_shift_matrix.m22 = -temp_m12;
                global_shift_matrix.m23 = width - 1 - temp_m13;
            }
//            printBMP(width, height, frame_buffer);
        } else if (sensor_key_1 == KEY_MX) {
            global_shift_matrix.m21 = -global_shift_matrix.m21;
            global_shift_matrix.m22 = -global_shift_matrix.m22;
            global_shift_matrix.m23 = width - 1 - global_shift_matrix.m23;
//            printBMP(width, height, frame_buffer);
        } else if (sensor_key_1 == KEY_MY) {
            global_shift_matrix.m11 = -global_shift_matrix.m11;
            global_shift_matrix.m12 = -global_shift_matrix.m12;
            global_shift_matrix.m13 = width - 1 - global_shift_matrix.m13;
//            printBMP(width, height, frame_buffer);
        }
        processed_frames += 1;
        if (processed_frames == 25) {
            register unsigned m11_r = global_shift_matrix.m11;
            register unsigned m12_r = global_shift_matrix.m12;
            register unsigned m13_r = global_shift_matrix.m13;
            register unsigned m21_r = global_shift_matrix.m21;
            register unsigned m22_r = global_shift_matrix.m22;
            register unsigned m23_r = global_shift_matrix.m23;


            convertFrame(frame_buffer, width, &global_shift_matrix, non_white, global_buffer);
            verifyFrame(frame_buffer, width, height, grading_mode);
            // 4 points
            // A (min x, min y), B(max_x, min y), C(min x, max y) D(max x, max y)
            if (!(m11_r == 1 && m12_r == 0 && m13_r == 0 && m21_r == 0 && m22_r == 1 && m23_r == 0))
            {
                unsigned new_ax = min_x * m11_r + min_y * m12_r + m13_r;
                unsigned new_ay = min_x * m21_r + min_y * m22_r + m23_r;

                unsigned new_bx = max_x * m11_r + min_y * m12_r + m13_r;
                unsigned new_by = max_x * m21_r + min_y * m22_r + m23_r;

                unsigned new_cx = min_x * m11_r + max_y * m12_r + m13_r;
                unsigned new_cy = min_x * m21_r + max_y * m22_r + m23_r;

                unsigned new_dx = max_x * m11_r + max_y * m12_r + m13_r;
                unsigned new_dy = max_x * m21_r + max_y * m22_r + m23_r;

                min_x = min(min(new_ax, new_bx), new_cx);
                min_y = min(min(new_ay, new_by), new_cy);
                max_x = max(max(new_ax, new_bx), new_cx);
                max_y = max(max(new_ay, new_by), new_cy);
            }

            unsigned entry_per_row = (max_x - min_x + 1) * 3;
            unsigned char* starting_column_offset = (unsigned char*)frame_buffer + min_x * 3;
            for (unsigned row = min_y; row <= max_y; row++)
            {
                unsigned row_offset = row * width * 3;
                memset(starting_column_offset + row_offset, 0xFF, entry_per_row);
            }

            global_shift_matrix.m11 = 1;
            global_shift_matrix.m12 = 0;
            global_shift_matrix.m13 = 0;
            global_shift_matrix.m21 = 0;
            global_shift_matrix.m22 = 1;
            global_shift_matrix.m23 = 0;
            processed_frames = 0;
        }
    }
    free(global_buffer);
    global_buffer = NULL;
    return;
}
