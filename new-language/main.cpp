#include "manifest.h"

int main() {

    shadowdash::ShadowDash shadowDash;

    BUILDDIR("build");

    VAR(cflags, "-Wall -Wextra");
    VAR(ldflags, "-lm");

    PHONY(clean, "clean_rule");
    RULE(clean_rule,
         shadowdash::constant("rm", 2),
         shadowdash::constant("-f", 2),
         shadowdash::constant("*.o", 3),
         shadowdash::constant("main", 4)
    );
    RULE(cc,
         shadowdash::constant("g++", 3),
         shadowdash::variable("cflags", 6),
         shadowdash::constant("-c", 2),
         shadowdash::variable(shadowdash::in, 2),
         shadowdash::constant("-o", 2),
         shadowdash::variable(shadowdash::out, 3)
    );

    RULE(link,
         shadowdash::constant("g++", 3),
         shadowdash::variable(shadowdash::in, 2),
         shadowdash::variable("ldflags", 7),
         shadowdash::constant("-o", 2),
         shadowdash::variable(shadowdash::out, 3)
    );

    BUILD("foo.o", "cc", {"foo.cpp"}, {"foo.h"});
    BUILD("bar.o", "cc", {"bar.cpp"}, {"bar.h"});
    BUILD("baz.o", "cc", {"baz.cpp"}, {"foo.h", "bar.h"});
    BUILD("main", "link", {"foo.o", "bar.o", "baz.o"});


    DEFAULT("clean");

    shadowDash.executeBuild();

    return 0;
}
