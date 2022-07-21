#include <cstdlib>
#include <iostream>

#include "../stb_image.h"


int main()
{
  int width{};
  int height{};
  int nr_channels{};

  auto data = stbi_load("container.jpg", &width, &height, &nr_channels, 0);

  std::cout << width << ", " << height << std::endl;

  return EXIT_SUCCESS;
}