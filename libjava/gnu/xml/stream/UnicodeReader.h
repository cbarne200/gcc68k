
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __gnu_xml_stream_UnicodeReader__
#define __gnu_xml_stream_UnicodeReader__

#pragma interface

#include <java/lang/Object.h>
#include <gcj/array.h>

extern "Java"
{
  namespace gnu
  {
    namespace xml
    {
      namespace stream
      {
          class UnicodeReader;
      }
    }
  }
}

class gnu::xml::stream::UnicodeReader : public ::java::lang::Object
{

public: // actually package-private
  UnicodeReader(::java::io::Reader *);
public:
  virtual void mark(jint);
  virtual void reset();
  virtual jint read();
  virtual jint read(JArray< jint > *, jint, jint);
  virtual void close();
  static JArray< jint > * toCodePointArray(::java::lang::String *);
public: // actually package-private
  ::java::io::Reader * __attribute__((aligned(__alignof__( ::java::lang::Object)))) in;
public:
  static ::java::lang::Class class$;
};

#endif // __gnu_xml_stream_UnicodeReader__