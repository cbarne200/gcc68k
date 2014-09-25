
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __gnu_javax_imageio_jpeg_JPEGScan__
#define __gnu_javax_imageio_jpeg_JPEGScan__

#pragma interface

#include <java/lang/Object.h>
extern "Java"
{
  namespace gnu
  {
    namespace javax
    {
      namespace imageio
      {
        namespace jpeg
        {
            class JPEGComponent;
            class JPEGScan;
        }
      }
    }
  }
}

class gnu::javax::imageio::jpeg::JPEGScan : public ::java::lang::Object
{

public:
  JPEGScan();
  JPEGScan(jint, jint);
private:
  void recalculateDimensions();
public:
  virtual void addComponent(jbyte, jbyte, jbyte, jbyte);
  virtual ::gnu::javax::imageio::jpeg::JPEGComponent * getComponentByID(jbyte);
  virtual ::gnu::javax::imageio::jpeg::JPEGComponent * get(jint);
  virtual jint getX(jbyte);
  virtual jint getY(jbyte);
  virtual jint getMaxV();
  virtual jint getMaxH();
  virtual void setWidth(jint);
  virtual void setHeight(jint);
  virtual jint size();
  virtual jint sizeComponentBlocks();
private:
  jint __attribute__((aligned(__alignof__( ::java::lang::Object)))) maxHeight;
  jint maxWidth;
  jint maxV;
  jint maxH;
  jint numOfComponents;
  jint numOfComponentBlocks;
  ::java::util::ArrayList * components;
public:
  static ::java::lang::Class class$;
};

#endif // __gnu_javax_imageio_jpeg_JPEGScan__
