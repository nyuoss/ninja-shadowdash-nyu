#include <dlfcn.h>
#include <iostream>

#include "ninja.h"

using namespace std;

shadowdash::buildGroup manifest();

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

    ninja::shadowdash_compile(argc, argv, manifest);
}
