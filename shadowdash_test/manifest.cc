#include "manifest.h"
#include <iostream>

using namespace shadowdash;

void manifest() {
	std::cout << "manifest start" << std::endl;
	rule{"compile", {
		bind(command, {"g++", " ", "-c", " ", "in"_v, " ", "-o", " ", "out"_v}),
		bind(depfile, {"out"_v, ".d"}),
		bind(deps, {"gcc"})
	}};
	std::cout << "compile rule done" << std::endl;

	rule{"link", {
		bind(command, {"g++", " ", "in"_v, " ", "-o", " ", "out"_v}),
	}};
	std::cout << "link rule done" << std::endl;

	build(list{ {str {{"hello.o"}}} },
		list{{}},
		"compile",
		list{ {str{{"hello.cc"}}} },
		list{{}},
		list{{}},
		{  }
	);
	std::cout << "build1 done" << std::endl;

	build(list{ {str {{"hello"}}} },
		list{{}},
		"link",
		list{ {str{{"hello.o"}}} },
		list{{}},
		list{{}},
		{  }
	);
	std::cout << "build2 done" << std::endl;

	build(list{ {str {{"hello.d"}}} },
		list{{}},
		"phony",
		list{{}},
		list{{}},
		list{{}},
		{  }
	);
	std::cout << "build3 done" << std::endl;

	std::cout << "manifest end" << std::endl;
}