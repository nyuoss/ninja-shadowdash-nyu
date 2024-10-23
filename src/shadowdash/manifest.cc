#include "manifest.h"

using namespace shadowdash;

void manifest() {
  let(flags, "-O3");

  auto heavy_object_pool = 4;

  auto compile = rule{ {
      bind(command, "g++", "flags"_v, "-c", in, "-o", out),  
      bind(pool, "heavy_object_pool"_p)
  } };

  auto link = rule{ {
      bind(command, "g++", in, "-o", out),  //
  } };

  build(list{ str{ "hello.o" } },   //
        {},                         //
        compile,                    //
        list{ str{ "hello.cc" } },  //
        {},                         //
        {},                         //
        { 
          bind(flags, "-O2"),
          bind(pool, ""_p)
        }      //
  );

  build(list{ str{ "hello" } },    //
        {},                        //
        link,                      //
        list{ str{ "hello.o" } },  //
        {},                        //
        {},                        //
        {}                         //
  );

  default_(list{ str{ "hello" } });
}