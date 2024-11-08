#include "manifest.h"
#include <iostream>

using namespace shadowdash;

Token operator"" _l(const char* value, std::size_t len) {
    return Token(Token::Type::LITERAL, std::string(value, len));
}

Token operator"" _v(const char* value, std::size_t len) {
    return Token(Token::Type::VAR, std::string(value, len));
}

rule phony = rule::phony;

buildGroup manifest() {
	std::cout << "manifest start" << std::endl;
	static auto compile = rule{"compile", {
		bind(command, {"g++", " ", "-c", " ", "in"_v, " ", "-o", " ", "out"_v}),
		bind(depfile, {"out"_v, ".d"}),
		bind(deps, {"gcc"})
	}};
	std::cout << "compile rule done" << std::endl;

	static auto link = rule{"link", {
		bind(command, {"g++", " ", "in"_v, " ", "-o", " ", "out"_v}),
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
		phony,
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
	return buildGroup({ build1, build2, build3 });
}