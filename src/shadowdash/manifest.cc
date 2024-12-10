#include "manifest.h"

using namespace shadowdash;

BuildsReturn manifest() {
    // example of defining a variable named "flags"
    // the value "-03" is a literal token
    let(flags, "-O3");      // original language: flags = -03
    let(pool_depth, "4");   // original language: pool_depth = 4
    // variable examples

    // examples of creating pool objects:
    auto heavy_object_pool = pool_(bind(depth, "pool_depth"_v));
    // passing the argument via macro
    auto heavy_object_pool2 = pool_(binding("depth", {"pool_depth"_v}));       
    // passing the argument via creating a binding object
    /*
    in original language:
    pool heavy_object_pool:
        depth = pool_depth
    */
    // pool examples

    // examples of creating rules in the new language
    make_rule(compile, 
        bind(command, "g++", "flags"_v, "-c", in, "-o", out),  
        bind(pool, "heavy_object_pool"_v)
    );
    /*
    in original language:
    rule compile:
        command = gcc $flags -c $in -o $out
        pool = heavy_object_pool
    */

    make_rule(link,
        bind(command, "g++", in, "-o", out)
    );
    // rule examples

    // examples of creating builds in the new language:
    auto build_c = build(list{ str{ "hello.o" } }, 
        {},
        compile,
        list{ str{ "hello.cc" } },
        {},
        {},
        { bind(flags, "-O2"), 
        bind(pool, "console"_v) }
    );
    /*
    in original language:
    build hello.o : compile hello.cc
        flags = -02
        pool = console
    */

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
    // build examples

    // examples of adding defaults in new language
    default_(str{ "hello" });
    default_(list{ str{ "foo1" }, str{ "foo2" } });
    /*
    in original language:
    default hello
    default foo1 foo2
    */
    // default examples
    return {build_c, build_l, build_p}; // returns all the builds
}