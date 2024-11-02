#include "../manifest.h"

using namespace shadowdash;

void manifest() {
  
  let(flags, "-O3");

  auto compile = rule{ {
      bind(command, "g++", "flags"_v, "-c", "in"_v, "-o", "out"_v),  //
  } };

  auto link = rule{ {
      bind(command, "g++", "in"_v, "-o", "out"_v),  //
  } };

  build(list{ str{ "hello.o" } },   //
        {},                         //
        compile,                    //
        list{ str{ "hello.cc" } },  //
        {},                         //
        {},                         //
        { bind(flags, "-O2") }      //
  );

  build(list{ str{ "hello" } },    //
        {},                        //
        link,                      //
        list{ str{ "hello.o" } },  //
        {},                        //
        {},                        //
        {}                         //
  );

  default(str("hello"));
}
