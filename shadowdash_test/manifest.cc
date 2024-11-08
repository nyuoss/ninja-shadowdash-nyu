#include "manifest.h"
#include <iostream>

using namespace shadowdash;

extern "C" {
	buildGroup manifest() {
		std::cout << "manifest start" << std::endl;
		static auto compile = rule{{
			bind(command, {"g++", " ", "-c", " ", in, " ", "-o", " ", out, " ", "-MD", " ", "-MF", " ", "depfile"_v}),
			bind(depfile, {out, ".d"}),
			bind(deps, {"gcc"})
		}};
		std::cout << "compile rule done" << std::endl;

		static auto link = rule{{
			bind(command, {"g++", " ", in, " ", "-o", " ", out}),
		}};
		std::cout << "link rule done" << std::endl;

		static auto build1 = build(list{ {str {{"hello.o"}}} },
			list{{}},
			compile,
			list{ {str{{"hello.cc"}}} },
			list{{}},
			list{{}},
			{  }
		);
		std::cout << "build1 done" << std::endl;

		static auto build2 = build(list{ {str {{"hello"}}} },
			list{{}},
			link,
			list{ {str{{"hello.o"}}} },
			list{{}},
			list{{}},
			{  }
		);
		std::cout << "build2 done" << std::endl;

		static auto build3 = build(list{ {str {{"hello.d"}}} },
			list{{}},
			rule::phony,
			list{{}},
			list{{}},
			list{{}},
			{  }
		);
		std::cout << "build3 done" << std::endl;

		// std::cout << "print build1" << std::endl;
		// std::cout << build1 << std::endl;
		// std::cout << "done printing build1" << std::endl;

		// std::cout << "print build2" << std::endl;
		// std::cout << build2 << std::endl;
		// std::cout << "done printing build2" << std::endl;

		// std::cout << "print build3" << std::endl;
		// std::cout << build3 << std::endl;
		// std::cout << "done printing build3" << std::endl;

		std::cout << "manifest end" << std::endl;
		return buildGroup({ &build1, &build2, &build3 });
	}
}