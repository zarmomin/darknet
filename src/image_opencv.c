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
void show_image_cv_ipl(image p, const char *name, IplImage *disp)
{
    int x,y,k;
    if(p.c == 3) rgbgr_image(p);
    //normalize_image(copy);

    char buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    sprintf(buff, "%s", name);

    int step = disp->widthStep;
    cvNamedWindow(buff, CV_WINDOW_NORMAL);
    //cvMoveWindow(buff, 100*(windows%10) + 200*(windows/10), 100*(windows%10));
    for(y = 0; y < p.h; ++y){
        for(x = 0; x < p.w; ++x){
            for(k= 0; k < p.c; ++k){
                disp->imageData[y*step + x*p.c + k] = (unsigned char)(get_pixel(p,x,y,k)*255);
            }
        }
    }
    if(0){
        int w = 448;
        int h = w*p.h/p.w;
        if(h > 1000){
            h = 1000;
            w = h*p.w/p.h;
        }
        IplImage *buffer = disp;
        disp = cvCreateImage(cvSize(w, h), buffer->depth, buffer->nChannels);
        cvResize(buffer, disp, CV_INTER_LINEAR);
        cvReleaseImage(&buffer);
    }
    cvShowImage(buff, disp);
}
