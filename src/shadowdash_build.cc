#include <iostream>

#include "ninja.h"

using namespace std;
using namespace ninja;

void manifest();

State* ninja_state;

NORETURN void shadowdash_compile(int argc, char** argv) {
    METRIC_RECORD(".shadowdash parse");
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
    cout << "runbuild done" << endl;

    if (g_metrics)
      ninja.DumpMetrics();
    exit(result);
    // exit(0);
}

int main(int argc, char** argv) {
    // Call the manifest function and get the returned value
    // shadowdash::buildGroup builds = manifest();
    
    // cout << "in shadowdash, ready to print build" << endl;
    // for(const shadowdash::build build : builds.builds) {
        // cout << "build" << endl;
        // for(const shadowdash::str& str : build.outputs_.values_) {
        //     cout << "outputs" << endl;
        //     for (const shadowdash::Token& token: str.tokens_) {
        //         cout << "token: " << token.value_ << endl;
        //     }
        // }
    //     cout << build << endl;
    // }
    // cout << "done printing build" << endl;

    shadowdash_compile(argc, argv);
    // return 0;
}
