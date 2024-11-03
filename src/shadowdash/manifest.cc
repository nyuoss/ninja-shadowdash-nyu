#include "manifest.h"

using namespace shadowdash;

void manifest() {
  let(flags, "-O3");

  let(pool_depth, "4");

  pool_ heavy_object_pool(bind(depth, "pool_depth"_v));

  auto compile = rule( {
      bind(command, "g++", "flags"_v, "-c", in, "-o", out),  
      bind(pool, "heavy_object_pool"_v)
  } );

  auto link = rule( {
      bind(command, "g++", in, "-o", out), 
  } );

  auto build_c = build(list{ str{ "hello.o" } }, 
        {},
        compile,
        list{ str{ "hello.cc" } },
        {},
        {},
        { bind(flags, "-O2") }
  );

  auto build_l= build(list{ str{ "hello" } },
        {},
        link,
        list{ str{ "hello.o" } },
        {},
        {},
        {}
  );

  auto build_p= build(list{ str{ "dummy" } },
        {},
        phony,
        {},
        {},
        {},
        {}
  );

  default_(list{ str{ "hello" } });
  default_(list{ str{ "foo1" }, str{"foo2"} });
}