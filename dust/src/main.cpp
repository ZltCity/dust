#include <iostream>

#include <dust/render/backend.hpp>

int main(int argc, char **argv)
{
	auto backends = dust::render::getSupportedBackends();

	for (auto &back: backends)
	{
		switch (back)
		{
			case dust::render::Backend::Vulkan:
				std::cout << "Vulkan is supported!" << std::endl;
				break;
		}
	}

    return 0;
}
