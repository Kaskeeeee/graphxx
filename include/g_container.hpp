#pragma once

#include <memory>

namespace graph {
namespace container {
template <typename It> class Container {
public:
  template <typename T>
  Container(T &&obj)
      : object(std::make_shared<Model<T>>(std::forward<T>(obj))) {}

  It begin() { return object->begin(); }
  It end() { return object->end(); }

  struct Concept {
    virtual ~Concept() {}
    virtual It begin() = 0;
    virtual It end() = 0;
  };

  template <typename T> struct Model : Concept {
    Model(const T &t) : object(t) {}
    It begin() override { return object.begin(); }
    It end() override { return object.end(); }

  private:
    T object;
  };

  std::shared_ptr<Concept> object;
};
} // namespace container
} // namespace graph