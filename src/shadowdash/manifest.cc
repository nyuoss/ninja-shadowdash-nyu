#include "manifest.h"

using namespace shadowdash;

BuildsReturn manifest() {
  let(flags, "-O3");

  let(pool_depth, "4");

  auto heavy_object_pool = pool_(bind(depth, "pool_depth"_v));

  auto heavy_object_pool2 = pool_(binding("depth", {"pool_depth"_v}));

// old way of defining rules: 
//   auto compile = rule( "compile", {
//       bind(command, "g++", "flags"_v, "-c", in, "-o", out),  
//       bind(pool, "heavy_object_pool"_v)
//   } );

// now we can use macro to define
    make_rule(compile, 
        bind(command, "g++", "flags"_v, "-c", in, "-o", out),  
        bind(pool, "heavy_object_pool"_v)
    );

//   old way of defining link: 
//   auto link = rule( "link", {
//       bind(command, "g++", in, "-o", out), 
//   } );

    make_rule(link,
        bind(command, "g++", in, "-o", out)
    );

  auto build_c = build(list{ str{ "hello.o" } }, 
      {},
      compile,
      list{ str{ "hello.cc" } },
      {},
      {},
      { bind(flags, "-O2"), 
        bind(pool, "console"_v) }
  );

  auto build_l = build(list{ str{ "hello" } },
      {},
      link,
      list{ str{ "hello.o" } },
      {},
      {},
      {}
  );

  auto build_p = build(list{ str{ "dummy" } },
      {},
      phony,
      {},
      {},
      {},
      {}
  );

  default_(str{ "hello" });
  default_(list{ str{ "foo1" }, str{ "foo2" } });

  return {build_c, build_l, build_p};
}