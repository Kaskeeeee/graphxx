#include "g_exceptions.hpp"

namespace graph {
namespace exceptions {
char *OutOfIdsException::what() { return "Out of IDs exception"; };
} // namespace exceptions
} // namespace graph