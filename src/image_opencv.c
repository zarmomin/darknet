#include "image_opencv.h"

void ipl_into_image(IplImage* src, image im) {
  unsigned char *data = (unsigned char *)src->imageData;
  int h = src->height;
  int w = src->width;
  int c = src->nChannels;
  int step = src->widthStep;
  int i, j, k;

  for(i = 0; i < h; ++i){
      for(k= 0; k < c; ++k){
          for(j = 0; j < w; ++j){
              im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
          }
      }
  }
}
image ipl_to_image(IplImage* src) {

  int h = src->height;
  int w = src->width;
  int c = src->nChannels;
  image out = make_image(w, h, c);
  ipl_into_image(src, out);
  return out;
}
void image_to_ipl(image p, IplImage *disp) {
  int x,y,k;
  if(p.c == 3) rgbgr_image(p);

  int step = disp->widthStep;

  for(y = 0; y < p.h; ++y){
      for(x = 0; x < p.w; ++x){
          for(k= 0; k < p.c; ++k){
              disp->imageData[y*step + x*p.c + k] = (unsigned char)(get_pixel(p,x,y,k)*255);
          }
      }
  }
}
void show_image_cv_ipl(const char *name, IplImage *disp) {
    char buff[256];
    cvNamedWindow(buff, CV_WINDOW_NORMAL);
    sprintf(buff, "%s", name);
    cvShowImage(buff, disp);
}
