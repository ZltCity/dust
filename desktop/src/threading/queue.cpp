#include "queue.hpp"

namespace dust::threading
{

QueueStopped::QueueStopped() : runtime_error("Queue has stopped.")
{}

} // namespace dust::threading