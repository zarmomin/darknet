#ifndef IMAGE_OPENCV_H
#define IMAGE_OPENCV_H

#include "matrix.h"

#ifdef __cplusplus
#include <opencv2/core.hpp>
inline cv::Mat image_to_mat(image img)
{
    int channels = img.c;
    int width = img.w;
    int height = img.h;
    cv::Mat mat = cv::Mat(height, width, CV_8UC(channels));
    int step = mat.step;

    for (int y = 0; y < img.h; ++y) {
        for (int x = 0; x < img.w; ++x) {
            for (int c = 0; c < img.c; ++c) {
                float val = img.data[c*img.h*img.w + y*img.w + x];
                mat.data[y*step + x*img.c + c] = (unsigned char)(val * 255);
            }
        }
    }
    return mat;
}
// ----------------------------------------

inline image mat_to_image(cv::Mat mat)
{
    int w = mat.cols;
    int h = mat.rows;
    int c = mat.channels();
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)mat.data;
    int step = mat.step;
    for (int y = 0; y < h; ++y) {
        for (int k = 0; k < c; ++k) {
            for (int x = 0; x < w; ++x) {
                //uint8_t val = mat.ptr<uint8_t>(y)[c * x + k];
                //uint8_t val = mat.at<Vec3b>(y, x).val[k];
                //im.data[k*w*h + y*w + x] = val / 255.0f;

                im.data[k*w*h + y*w + x] = data[y*step + x*c + k] / 255.0f;
            }
        }
    }
    return im;
}
extern "C" {
#endif

#ifdef OPENCV

// declaration
typedef struct mat_cv mat_cv;
typedef struct cap_cv cap_cv;
typedef struct write_cv write_cv;

// cv::Mat
mat_cv *load_image_mat_cv(const char *filename, int flag);
image load_image_cv(char *filename, int channels);
image load_image_resize(char *filename, int w, int h, int c, image *im);
int get_width_mat(mat_cv *mat);
int get_height_mat(mat_cv *mat);
void release_mat(mat_cv **mat);

image mat_to_image_cv(mat_cv *mat);

// Window
void create_window_cv(char const* window_name, int full_screen, int width, int height);
void destroy_all_windows_cv();
int wait_key_cv(int delay);
int wait_until_press_key_cv();
void make_window(char *name, int w, int h, int fullscreen);
void show_image_cv(image p, const char *name);

void show_image_mat(mat_cv *mat_ptr, const char *name);

// Video Writer
write_cv *create_video_writer(char *out_filename, char c1, char c2, char c3, char c4, int fps, int width, int height, int is_color);
void write_frame_cv(write_cv *output_video_writer, mat_cv *mat);
void release_video_writer(write_cv **output_video_writer);

// Video Capture
cap_cv* get_capture_video_stream(const char *path);
cap_cv* get_capture_webcam(int index);
void release_capture(cap_cv* cap);

mat_cv* get_capture_frame_cv(cap_cv *cap);
int get_stream_fps_cpp_cv(cap_cv *cap);
double get_capture_property_cv(cap_cv *cap, int property_id);
double get_capture_frame_count_cv(cap_cv *cap);
int set_capture_property_cv(cap_cv *cap, int property_id, double value);
int set_capture_position_frame_cv(cap_cv *cap, int index);

// ... Video Capture
image get_image_from_stream_cpp(cap_cv *cap);
image get_image_from_stream_resize(cap_cv *cap, int w, int h, int c, mat_cv** in_img, int dont_close);
image get_image_from_stream_letterbox(cap_cv *cap, int w, int h, int c, mat_cv** in_img, int dont_close);


// Image Saving
void save_cv_png(mat_cv *img, const char *name);
void save_cv_jpg(mat_cv *img, const char *name);

// Draw Detection
void draw_detections_cv_v3(mat_cv* show_img, detection *dets, int num, float thresh, char **names, image **alphabet, int classes, int ext_output);

// Draw Loss & Accuracy chart
mat_cv* draw_train_chart(float max_img_loss, int max_batches, int number_of_lines, int img_size, int dont_show);
void draw_train_loss(mat_cv* img, int img_size, float avg_loss, float max_img_loss, int current_batch, int max_batches,
    float precision, int draw_precision, char *accuracy_name, int dont_show, int mjpeg_port);

// Data augmentation
image image_data_augmentation(mat_cv* mat, int w, int h,
    int pleft, int ptop, int swidth, int sheight, int flip,
    float dhue, float dsat, float dexp,
    int blur, int num_boxes, float *truth);

// blend two images with (alpha and beta)
void blend_images_cv(image new_img, float alpha, image old_img, float beta);

// Show Anchors
void show_acnhors(int number_of_boxes, int num_of_clusters, float *rel_width_height_array, model anchors_data, int width, int height);

static float get_pixel(image m, int x, int y, int c)
{
  assert(x < m.w && y < m.h && c < m.c);
  return m.data[c*m.h*m.w + y*m.w + x];
}

#else   // OPENCV

int wait_key_cv(int delay);
int wait_until_press_key_cv();
void destroy_all_windows_cv();

#endif  // OPENCV

#ifdef __cplusplus
}
#endif

#ifndef __cplusplus
#ifdef OPENCV
#include "box.h"
#include "darknet.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
void ipl_into_image(IplImage* src, image im);
image ipl_to_image(IplImage* src);
void show_image_cv_ipl(image p, const char *name, IplImage *disp);
#endif
#endif

#endif // IMAGE_OPENCV_H
