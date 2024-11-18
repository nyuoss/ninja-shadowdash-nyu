#include "manifest.h"

using namespace shadowdash;

extern "C" {
    buildGroup manifest() {

        let(ninja_required_version, {"1.5"});

        auto C_COMPILER__zlib_ = rule{{
            bind(command, {"/usr/bin/cc","DEFINES"_v,"INCLUDES"_v,"FLAGS"_v,"-MD","-MT","out"_v,"-MF","DEP_FILE"_v,"-o","out"_v,"-c","in"_v})
        }};

        auto C_SHARED_LIBRARY_LINKER__zlib_ = rule{{
            bind(command, {"PRE_LINK"_v,"&&","/usr/bin/cc","-fPIC","LANGUAGE_COMPILE_FLAGS"_v,"ARCH_FLAGS"_v,"LINK_FLAGS"_v,"-shared","SONAME_FLAG"_v,"-o","TARGET_FILE"_v,"in"_v,"LINK_PATH"_v,"LINK_LIBRARIES"_v,"&&","POST_BUILD"_v})
        }};

        auto CMAKE_SYMLINK_LIBRARY = rule{{
            bind(command, {"/usr/bin/cmake","-E","cmake_symlink_library","in"_v,"SONAME"_v,"out"_v,"&&","POST_BUILD"_v})
        }};

        auto C_COMPILER__zlibstatic_ = rule{{
            bind(command, {"/usr/bin/cc","DEFINES"_v,"INCLUDES"_v,"FLAGS"_v,"-MD","-MT","out"_v,"-MF","DEP_FILE"_v,"-o","out"_v,"-c","in"_v})
        }};

        auto C_STATIC_LIBRARY_LINKER__zlibstatic_ = rule{{
            bind(command, {"PRE_LINK"_v,"&&","/usr/bin/cmake","-E","rm","-f","TARGET_FILE"_v,"&&","/usr/bin/ar","qc","TARGET_FILE"_v,"LINK_FLAGS"_v,"in"_v,"&&","/usr/bin/ranlib","TARGET_FILE"_v,"&&","POST_BUILD"_v})
        }};

        auto C_COMPILER__example_ = rule{{
            bind(command, {"/usr/bin/cc","DEFINES"_v,"INCLUDES"_v,"FLAGS"_v,"-MD","-MT","out"_v,"-MF","DEP_FILE"_v,"-o","out"_v,"-c","in"_v})
        }};

        auto C_EXECUTABLE_LINKER__example_ = rule{{
            bind(command, {"PRE_LINK"_v,"&&","/usr/bin/cc","FLAGS"_v,"LINK_FLAGS"_v,"in"_v,"-o","TARGET_FILE"_v,"LINK_PATH"_v,"LINK_LIBRARIES"_v,"&&","POST_BUILD"_v})
        }};

        auto C_COMPILER__minigzip_ = rule{{
            bind(command, {"/usr/bin/cc","DEFINES"_v,"INCLUDES"_v,"FLAGS"_v,"-MD","-MT","out"_v,"-MF","DEP_FILE"_v,"-o","out"_v,"-c","in"_v})
        }};

        auto C_EXECUTABLE_LINKER__minigzip_ = rule{{
            bind(command, {"PRE_LINK"_v,"&&","/usr/bin/cc","FLAGS"_v,"LINK_FLAGS"_v,"in"_v,"-o","TARGET_FILE"_v,"LINK_PATH"_v,"LINK_LIBRARIES"_v,"&&","POST_BUILD"_v})
        }};

        auto C_COMPILER__example64_ = rule{{
            bind(command, {"/usr/bin/cc","DEFINES"_v,"INCLUDES"_v,"FLAGS"_v,"-MD","-MT","out"_v,"-MF","DEP_FILE"_v,"-o","out"_v,"-c","in"_v})
        }};

        auto C_EXECUTABLE_LINKER__example64_ = rule{{
            bind(command, {"PRE_LINK"_v,"&&","/usr/bin/cc","FLAGS"_v,"LINK_FLAGS"_v,"in"_v,"-o","TARGET_FILE"_v,"LINK_PATH"_v,"LINK_LIBRARIES"_v,"&&","POST_BUILD"_v})
        }};

        auto C_COMPILER__minigzip64_ = rule{{
            bind(command, {"/usr/bin/cc","DEFINES"_v,"INCLUDES"_v,"FLAGS"_v,"-MD","-MT","out"_v,"-MF","DEP_FILE"_v,"-o","out"_v,"-c","in"_v})
        }};

        auto C_EXECUTABLE_LINKER__minigzip64_ = rule{{
            bind(command, {"PRE_LINK"_v,"&&","/usr/bin/cc","FLAGS"_v,"LINK_FLAGS"_v,"in"_v,"-o","TARGET_FILE"_v,"LINK_PATH"_v,"LINK_LIBRARIES"_v,"&&","POST_BUILD"_v})
        }};

        auto CUSTOM_COMMAND = rule{{
            bind(command, {"COMMAND"_v})
        }};

        auto RERUN_CMAKE = rule{{
            bind(command, {"/usr/bin/cmake","--regenerate-during-build","-S/home/opensource/zlib","-B/home/opensource/zlib"})
        }};

        auto CLEAN = rule{{
            bind(command, {"/usr/bin/ninja","FILE_ARG"_v,"-t","clean","TARGETS"_v})
        }};

        auto HELP = rule{{
            bind(command, {"/usr/bin/ninja","-t","targets"})
        }};

        let(cmake_ninja_workdir, {"/home/opensource/zlib/"});

        auto build1 = build(list{{str{{"cmake_object_order_depends_target_zlib"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"CMakeFiles/zlib.dir"}} } },
                list{{}},
                list{{}},
                {});

        auto build2 = build(list{{str{{"CMakeFiles/zlib.dir/adler32.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/adler32.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/adler32.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build3 = build(list{{str{{"CMakeFiles/zlib.dir/compress.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/compress.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/compress.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build4 = build(list{{str{{"CMakeFiles/zlib.dir/crc32.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/crc32.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/crc32.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build5 = build(list{{str{{"CMakeFiles/zlib.dir/deflate.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/deflate.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/deflate.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build6 = build(list{{str{{"CMakeFiles/zlib.dir/gzclose.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/gzclose.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/gzclose.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build7 = build(list{{str{{"CMakeFiles/zlib.dir/gzlib.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/gzlib.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/gzlib.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build8 = build(list{{str{{"CMakeFiles/zlib.dir/gzread.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/gzread.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/gzread.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build9 = build(list{{str{{"CMakeFiles/zlib.dir/gzwrite.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/gzwrite.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/gzwrite.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build10 = build(list{{str{{"CMakeFiles/zlib.dir/inflate.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/inflate.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/inflate.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build11 = build(list{{str{{"CMakeFiles/zlib.dir/infback.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/infback.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/infback.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build12 = build(list{{str{{"CMakeFiles/zlib.dir/inftrees.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/inftrees.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/inftrees.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build13 = build(list{{str{{"CMakeFiles/zlib.dir/inffast.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/inffast.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/inffast.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build14 = build(list{{str{{"CMakeFiles/zlib.dir/trees.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/trees.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/trees.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build15 = build(list{{str{{"CMakeFiles/zlib.dir/uncompr.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/uncompr.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/uncompr.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build16 = build(list{{str{{"CMakeFiles/zlib.dir/zutil.c.o"}}}},
                list{{}},
                C_COMPILER__zlib_,
                list{{ 		str{{"/home/opensource/zlib/zutil.c"}}, 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-DZLIB_DLL -D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlib.dir/zutil.c.o.d"}),
                bind(FLAGS, {"-fPIC"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlib.dir"})
                });

        auto build17 = build(list{{str{{"libz.so.1.3.1.1-motley"}}}},
                list{{}},
                C_SHARED_LIBRARY_LINKER__zlib_,
                list{{ 		str{{"CMakeFiles/zlib.dir/adler32.c.o"}}, 		str{{"CMakeFiles/zlib.dir/compress.c.o"}}, 		str{{"CMakeFiles/zlib.dir/crc32.c.o"}}, 		str{{"CMakeFiles/zlib.dir/deflate.c.o"}}, 		str{{"CMakeFiles/zlib.dir/gzclose.c.o"}}, 		str{{"CMakeFiles/zlib.dir/gzlib.c.o"}}, 		str{{"CMakeFiles/zlib.dir/gzread.c.o"}}, 		str{{"CMakeFiles/zlib.dir/gzwrite.c.o"}}, 		str{{"CMakeFiles/zlib.dir/inflate.c.o"}}, 		str{{"CMakeFiles/zlib.dir/infback.c.o"}}, 		str{{"CMakeFiles/zlib.dir/inftrees.c.o"}}, 		str{{"CMakeFiles/zlib.dir/inffast.c.o"}}, 		str{{"CMakeFiles/zlib.dir/trees.c.o"}}, 		str{{"CMakeFiles/zlib.dir/uncompr.c.o"}}, 		str{{"CMakeFiles/zlib.dir/zutil.c.o"}} } },
                list{{}},
                list{{}},
                {	bind(LINK_FLAGS, {"-Wl,--version-script,\"/home/opensource/zlib/zlib.map\""}),
                bind(OBJECT_DIR, {"CMakeFiles/zlib.dir"}),
                bind(POST_BUILD, {":"}),
                bind(PRE_LINK, {":"}),
                bind(SONAME, {"libz.so.1"}),
                bind(SONAME_FLAG, {"-Wl,-soname,"}),
                bind(TARGET_FILE, {"libz.so.1.3.1.1-motley"}),
                bind(TARGET_PDB, {"z.so.dbg"})
                });

        auto build18 = build(list{{str{{"libz.so.1"}}, str{{"libz.so"}}}},
                list{{}},
                CMAKE_SYMLINK_LIBRARY,
                list{{ 		str{{"libz.so.1.3.1.1-motley"}} } },
                list{{}},
                list{{}},
                {	bind(POST_BUILD, {":"})
                });

        auto build19 = build(list{{str{{"cmake_object_order_depends_target_zlibstatic"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"CMakeFiles/zlibstatic.dir"}} } },
                list{{}},
                list{{}},
                {});

        auto build20 = build(list{{str{{"CMakeFiles/zlibstatic.dir/adler32.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/adler32.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/adler32.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build21 = build(list{{str{{"CMakeFiles/zlibstatic.dir/compress.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/compress.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/compress.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build22 = build(list{{str{{"CMakeFiles/zlibstatic.dir/crc32.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/crc32.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/crc32.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build23 = build(list{{str{{"CMakeFiles/zlibstatic.dir/deflate.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/deflate.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/deflate.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build24 = build(list{{str{{"CMakeFiles/zlibstatic.dir/gzclose.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/gzclose.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/gzclose.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build25 = build(list{{str{{"CMakeFiles/zlibstatic.dir/gzlib.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/gzlib.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/gzlib.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build26 = build(list{{str{{"CMakeFiles/zlibstatic.dir/gzread.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/gzread.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/gzread.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build27 = build(list{{str{{"CMakeFiles/zlibstatic.dir/gzwrite.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/gzwrite.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/gzwrite.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build28 = build(list{{str{{"CMakeFiles/zlibstatic.dir/inflate.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/inflate.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/inflate.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build29 = build(list{{str{{"CMakeFiles/zlibstatic.dir/infback.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/infback.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/infback.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build30 = build(list{{str{{"CMakeFiles/zlibstatic.dir/inftrees.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/inftrees.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/inftrees.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build31 = build(list{{str{{"CMakeFiles/zlibstatic.dir/inffast.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/inffast.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/inffast.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build32 = build(list{{str{{"CMakeFiles/zlibstatic.dir/trees.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/trees.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/trees.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build33 = build(list{{str{{"CMakeFiles/zlibstatic.dir/uncompr.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/uncompr.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/uncompr.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build34 = build(list{{str{{"CMakeFiles/zlibstatic.dir/zutil.c.o"}}}},
                list{{}},
                C_COMPILER__zlibstatic_,
                list{{ 		str{{"/home/opensource/zlib/zutil.c"}}, 		str{{"cmake_object_order_depends_target_zlibstatic"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/zlibstatic.dir/zutil.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/zlibstatic.dir"})
                });

        auto build35 = build(list{{str{{"libz.a"}}}},
                list{{}},
                C_STATIC_LIBRARY_LINKER__zlibstatic_,
                list{{ 		str{{"CMakeFiles/zlibstatic.dir/adler32.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/compress.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/crc32.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/deflate.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/gzclose.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/gzlib.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/gzread.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/gzwrite.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/inflate.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/infback.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/inftrees.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/inffast.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/trees.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/uncompr.c.o"}}, 		str{{"CMakeFiles/zlibstatic.dir/zutil.c.o"}} } },
                list{{}},
                list{{}},
                {	bind(OBJECT_DIR, {"CMakeFiles/zlibstatic.dir"}),
                bind(POST_BUILD, {":"}),
                bind(PRE_LINK, {":"}),
                bind(TARGET_FILE, {"libz.a"}),
                bind(TARGET_PDB, {"z.a.dbg"})
                });

        auto build36 = build(list{{str{{"cmake_object_order_depends_target_example"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {});

        auto build37 = build(list{{str{{"CMakeFiles/example.dir/test/example.c.o"}}}},
                list{{}},
                C_COMPILER__example_,
                list{{ 		str{{"/home/opensource/zlib/test/example.c"}}, 		str{{"cmake_object_order_depends_target_example"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/example.dir/test/example.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/example.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/example.dir/test"})
                });

        auto build38 = build(list{{str{{"example"}}}},
                list{{}},
                C_EXECUTABLE_LINKER__example_,
                list{{ 		str{{"CMakeFiles/example.dir/test/example.c.o"}}, 		str{{"libz.so.1.3.1.1-motley"}}, 		str{{"libz.so"}}, 		str{{"libz.so"}} } },
                list{{}},
                list{{}},
                {	bind(LINK_LIBRARIES, {"-Wl,-rpath,/home/opensource/zlib  libz.so.1.3.1.1-motley"}),
                bind(OBJECT_DIR, {"CMakeFiles/example.dir"}),
                bind(POST_BUILD, {":"}),
                bind(PRE_LINK, {":"}),
                bind(TARGET_FILE, {"example"}),
                bind(TARGET_PDB, {"example.dbg"})
                });

        auto build39 = build(list{{str{{"cmake_object_order_depends_target_minigzip"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {});

        auto build40 = build(list{{str{{"CMakeFiles/minigzip.dir/test/minigzip.c.o"}}}},
                list{{}},
                C_COMPILER__minigzip_,
                list{{ 		str{{"/home/opensource/zlib/test/minigzip.c"}}, 		str{{"cmake_object_order_depends_target_minigzip"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/minigzip.dir/test/minigzip.c.o.d"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/minigzip.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/minigzip.dir/test"})
                });

        auto build41 = build(list{{str{{"minigzip"}}}},
                list{{}},
                C_EXECUTABLE_LINKER__minigzip_,
                list{{ 		str{{"CMakeFiles/minigzip.dir/test/minigzip.c.o"}}, 		str{{"libz.so.1.3.1.1-motley"}}, 		str{{"libz.so"}}, 		str{{"libz.so"}} } },
                list{{}},
                list{{}},
                {	bind(LINK_LIBRARIES, {"-Wl,-rpath,/home/opensource/zlib  libz.so.1.3.1.1-motley"}),
                bind(OBJECT_DIR, {"CMakeFiles/minigzip.dir"}),
                bind(POST_BUILD, {":"}),
                bind(PRE_LINK, {":"}),
                bind(TARGET_FILE, {"minigzip"}),
                bind(TARGET_PDB, {"minigzip.dbg"})
                });

        auto build42 = build(list{{str{{"cmake_object_order_depends_target_example64"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {});

        auto build43 = build(list{{str{{"CMakeFiles/example64.dir/test/example.c.o"}}}},
                list{{}},
                C_COMPILER__example64_,
                list{{ 		str{{"/home/opensource/zlib/test/example.c"}}, 		str{{"cmake_object_order_depends_target_example64"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/example64.dir/test/example.c.o.d"}),
                bind(FLAGS, {"-D_FILE_OFFSET_BITS=64"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/example64.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/example64.dir/test"})
                });

        auto build44 = build(list{{str{{"example64"}}}},
                list{{}},
                C_EXECUTABLE_LINKER__example64_,
                list{{ 		str{{"CMakeFiles/example64.dir/test/example.c.o"}}, 		str{{"libz.so.1.3.1.1-motley"}}, 		str{{"libz.so"}}, 		str{{"libz.so"}} } },
                list{{}},
                list{{}},
                {	bind(LINK_LIBRARIES, {"-Wl,-rpath,/home/opensource/zlib  libz.so.1.3.1.1-motley"}),
                bind(OBJECT_DIR, {"CMakeFiles/example64.dir"}),
                bind(POST_BUILD, {":"}),
                bind(PRE_LINK, {":"}),
                bind(TARGET_FILE, {"example64"}),
                bind(TARGET_PDB, {"example64.dbg"})
                });

        auto build45 = build(list{{str{{"cmake_object_order_depends_target_minigzip64"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"cmake_object_order_depends_target_zlib"}} } },
                list{{}},
                list{{}},
                {});

        auto build46 = build(list{{str{{"CMakeFiles/minigzip64.dir/test/minigzip.c.o"}}}},
                list{{}},
                C_COMPILER__minigzip64_,
                list{{ 		str{{"/home/opensource/zlib/test/minigzip.c"}}, 		str{{"cmake_object_order_depends_target_minigzip64"}} } },
                list{{}},
                list{{}},
                {	bind(DEFINES, {"-D_LARGEFILE64_SOURCE=1"}),
                bind(DEP_FILE, {"CMakeFiles/minigzip64.dir/test/minigzip.c.o.d"}),
                bind(FLAGS, {"-D_FILE_OFFSET_BITS=64"}),
                bind(INCLUDES, {"-I/home/opensource/zlib"}),
                bind(OBJECT_DIR, {"CMakeFiles/minigzip64.dir"}),
                bind(OBJECT_FILE_DIR, {"CMakeFiles/minigzip64.dir/test"})
                });

        auto build47 = build(list{{str{{"minigzip64"}}}},
                list{{}},
                C_EXECUTABLE_LINKER__minigzip64_,
                list{{ 		str{{"CMakeFiles/minigzip64.dir/test/minigzip.c.o"}}, 		str{{"libz.so.1.3.1.1-motley"}}, 		str{{"libz.so"}}, 		str{{"libz.so"}} } },
                list{{}},
                list{{}},
                {	bind(LINK_LIBRARIES, {"-Wl,-rpath,/home/opensource/zlib  libz.so.1.3.1.1-motley"}),
                bind(OBJECT_DIR, {"CMakeFiles/minigzip64.dir"}),
                bind(POST_BUILD, {":"}),
                bind(PRE_LINK, {":"}),
                bind(TARGET_FILE, {"minigzip64"}),
                bind(TARGET_PDB, {"minigzip64.dbg"})
                });

        auto build48 = build(list{{str{{"CMakeFiles/test.util"}}}},
                list{{}},
                CUSTOM_COMMAND,
                list{{  } },
                list{{}},
                list{{}},
                {	bind(COMMAND, {"cd /home/opensource/zlib && /usr/bin/ctest --force-new-ctest-process"}),
                bind(DESC, {"Running tests..."}),
                bind(pool, {"console"}),
                bind(restat, {"1"})
                });

        auto build49 = build(list{{str{{"test"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"CMakeFiles/test.util"}} } },
                list{{}},
                list{{}},
                {});

        auto build50 = build(list{{str{{"CMakeFiles/edit_cache.util"}}}},
                list{{}},
                CUSTOM_COMMAND,
                list{{  } },
                list{{}},
                list{{}},
                {	bind(COMMAND, {"cd /home/opensource/zlib && /usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available."}),
                bind(DESC, {"No interactive CMake dialog available..."}),
                bind(restat, {"1"})
                });

        auto build51 = build(list{{str{{"edit_cache"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"CMakeFiles/edit_cache.util"}} } },
                list{{}},
                list{{}},
                {});

        auto build52 = build(list{{str{{"CMakeFiles/rebuild_cache.util"}}}},
                list{{}},
                CUSTOM_COMMAND,
                list{{  } },
                list{{}},
                list{{}},
                {	bind(COMMAND, {"cd /home/opensource/zlib && /usr/bin/cmake --regenerate-during-build -S/home/opensource/zlib -B/home/opensource/zlib"}),
                bind(DESC, {"Running CMake to regenerate build system..."}),
                bind(pool, {"console"}),
                bind(restat, {"1"})
                });

        auto build53 = build(list{{str{{"rebuild_cache"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"CMakeFiles/rebuild_cache.util"}} } },
                list{{}},
                list{{}},
                {});

        auto build54 = build(list{{str{{"list_install_components"}}}},
                list{{}},
                rule::phony,
                list{{  } },
                list{{}},
                list{{}},
                {});

        auto build55 = build(list{{str{{"CMakeFiles/install.util"}}}},
                list{{}},
                CUSTOM_COMMAND,
                list{{ 		str{{"all"}} } },
                list{{}},
                list{{}},
                {	bind(COMMAND, {"cd /home/opensource/zlib && /usr/bin/cmake -P cmake_install.cmake"}),
                bind(DESC, {"Install the project..."}),
                bind(pool, {"console"}),
                bind(restat, {"1"})
                });

        auto build56 = build(list{{str{{"install"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"CMakeFiles/install.util"}} } },
                list{{}},
                list{{}},
                {});

        auto build57 = build(list{{str{{"CMakeFiles/install/local.util"}}}},
                list{{}},
                CUSTOM_COMMAND,
                list{{ 		str{{"all"}} } },
                list{{}},
                list{{}},
                {	bind(COMMAND, {"cd /home/opensource/zlib && /usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake"}),
                bind(DESC, {"Installing only the local directory..."}),
                bind(pool, {"console"}),
                bind(restat, {"1"})
                });

        auto build58 = build(list{{str{{"install/local"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"CMakeFiles/install/local.util"}} } },
                list{{}},
                list{{}},
                {});

        auto build59 = build(list{{str{{"CMakeFiles/install/strip.util"}}}},
                list{{}},
                CUSTOM_COMMAND,
                list{{ 		str{{"all"}} } },
                list{{}},
                list{{}},
                {	bind(COMMAND, {"cd /home/opensource/zlib && /usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake"}),
                bind(DESC, {"Installing the project stripped..."}),
                bind(pool, {"console"}),
                bind(restat, {"1"})
                });

        auto build60 = build(list{{str{{"install/strip"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"CMakeFiles/install/strip.util"}} } },
                list{{}},
                list{{}},
                {});

        auto build61 = build(list{{str{{"zlib"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"libz.so"}} } },
                list{{}},
                list{{}},
                {});

        auto build62 = build(list{{str{{"zlibstatic"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"libz.a"}} } },
                list{{}},
                list{{}},
                {});

        auto build63 = build(list{{str{{"all"}}}},
                list{{}},
                rule::phony,
                list{{ 		str{{"libz.so"}}, 		str{{"libz.a"}}, 		str{{"example"}}, 		str{{"minigzip"}}, 		str{{"example64"}}, 		str{{"minigzip64"}} } },
                list{{}},
                list{{}},
                {});

        auto build64 = build(list{{str{{"build.ninja"}}}},
                list{{}},
                RERUN_CMAKE,
                list{{ 		str{{"/usr/share/cmake-3.22/Modules/CMakeCInformation.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/CMakeCommonLanguageInclude.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/CMakeGenericSystem.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/CMakeInitializeConfigs.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/CMakeLanguageInformation.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/CMakeSystemSpecificInformation.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/CMakeSystemSpecificInitialize.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/CheckCSourceCompiles.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/CheckFunctionExists.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/CheckIncludeFile.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/CheckIncludeFileCXX.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/CheckTypeSize.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/Compiler/CMakeCommonCompilerMacros.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/Compiler/GNU-C.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/Compiler/GNU.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/Internal/CheckSourceCompiles.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/Platform/Linux-GNU-C.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/Platform/Linux-GNU.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/Platform/Linux.cmake"}}, 		str{{"/usr/share/cmake-3.22/Modules/Platform/UnixPaths.cmake"}}, 		str{{"CMakeCache.txt"}}, 		str{{"CMakeFiles/3.22.1/CMakeCCompiler.cmake"}}, 		str{{"CMakeFiles/3.22.1/CMakeSystem.cmake"}}, 		str{{"CMakeLists.txt"}}, 		str{{"zconf.h.cmakein"}}, 		str{{"zlib.pc.cmakein"}} } },
                list{{}},
                list{{}},
                {	bind(pool, {"console"})
                });

        auto build65 = build(list{{str{{"/usr/share/cmake-3.22/Modules/CMakeCInformation.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/CMakeCommonLanguageInclude.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/CMakeGenericSystem.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/CMakeInitializeConfigs.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/CMakeLanguageInformation.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/CMakeSystemSpecificInformation.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/CMakeSystemSpecificInitialize.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/CheckCSourceCompiles.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/CheckFunctionExists.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/CheckIncludeFile.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/CheckIncludeFileCXX.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/CheckTypeSize.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/Compiler/CMakeCommonCompilerMacros.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/Compiler/GNU-C.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/Compiler/GNU.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/Internal/CheckSourceCompiles.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/Platform/Linux-GNU-C.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/Platform/Linux-GNU.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/Platform/Linux.cmake"}}, str{{"/usr/share/cmake-3.22/Modules/Platform/UnixPaths.cmake"}}, str{{"CMakeCache.txt"}}, str{{"CMakeFiles/3.22.1/CMakeCCompiler.cmake"}}, str{{"CMakeFiles/3.22.1/CMakeSystem.cmake"}}, str{{"CMakeLists.txt"}}, str{{"zconf.h.cmakein"}}, str{{"zlib.pc.cmakein"}}}},
                list{{}},
                rule::phony,
                list{{  } },
                list{{}},
                list{{}},
                {});

        auto build66 = build(list{{str{{"clean"}}}},
                list{{}},
                CLEAN,
                list{{  } },
                list{{}},
                list{{}},
                {});

        auto build67 = build(list{{str{{"help"}}}},
                list{{}},
                HELP,
                list{{  } },
                list{{}},
                list{{}},
                {});
        return buildGroup({build1, build2, build3, build4, build5, build6, build7, build8, build9, build10, build11, build12, build13, build14, build15, build16, build17, build18, build19, build20, build21, build22, build23, build24, build25, build26, build27, build28, build29, build30, build31, build32, build33, build34, build35, build36, build37, build38, build39, build40, build41, build42, build43, build44, build45, build46, build47, build48, build49, build50, build51, build52, build53, build54, build55, build56, build57, build58, build59, build60, build61, build62, build63, build64, build65, build66, build67});
    }}
