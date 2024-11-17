#include <iostream>

#include "ninja.h"

using namespace std;
using namespace ninja;

void manifest();

State* ninja_state;

NORETURN void shadowdash_compile(int argc, char** argv) {
    // a bunch of ninja init stuff
    const char* ninja_command = argv[0];
    BuildConfig config;
    Options options = {};

    int exit_code = ReadFlags(&argc, &argv, &options, &config);
    if (exit_code >= 0)
        exit(exit_code);

    Status* status = Status::factory(config);

    NinjaMain ninja(ninja_command, config);

    string* err = new string();

    // ninja init done, start building graph

    ninja_state = &ninja.state_;
    {
        METRIC_RECORD(".shadowdash parse");
        manifest();
    }
    
    if (!ninja.EnsureBuildDirExists())
      exit(1);

    if (!ninja.OpenBuildLog() || !ninja.OpenDepsLog())
      exit(1);

    // // graph building done, start actual building
    int result = ninja.RunBuild(argc, argv, status);

    if (g_metrics)
      ninja.DumpMetrics();
    exit(result);
    // exit(0);
}

int main(int argc, char** argv) {
    shadowdash_compile(argc, argv);
}
