#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fmt/format.h>


int main()
{
  glm::vec4 vec{ 1.0f, 0.0f, 0.0f, 1.0f };
  glm::mat4 trans{ 1.0f };

  trans = glm::translate(trans, glm::vec3{ 1.0f, 1.0f, 0.0f });
  vec = trans * vec;

  fmt::print("{}, {}, {}\n", vec.x, vec.y, vec.z);

  return EXIT_SUCCESS;
}