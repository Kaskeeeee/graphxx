#pragma once

namespace graph {

namespace container {

template <typename It> class Container {
public:
  template <typename T> // (3)
  Container(T &&obj)
      : object(std::make_shared<Model<T>>(std::forward<T>(obj))) {}

  It begin() const { // (4)
    return object->begin();
  }

  It end() const { return object->end(); }

  struct Concept { // (5)
    virtual ~Concept() {}
    virtual begin() const = 0;
    virtual end() const = 0;
  };

  template <typename T> // (6)
  struct Model : Concept {
    Model(const T &t) : object(t) {}
    It begin() const override { return object.begin(); }
    It end() const override { return object.end(); }

  private:
    T object;
  };

  std::shared_ptr<const Concept> object;
};
} // namespace container
} // namespace graph