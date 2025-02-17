#include "ServiceImpl.hpp"
#include <iostream>

namespace sample {

void ServiceComponentCA02::Modified(
  const std::shared_ptr<ComponentContext>& /*context*/,
  const std::shared_ptr<cppmicroservices::AnyMap>& configuration)
{
  throw std::runtime_error("Modified method exception");
  std::lock_guard<std::mutex> lock(propertiesLock);
  properties = configuration;
}

cppmicroservices::AnyMap ServiceComponentCA02::GetProperties()
{
  std::lock_guard<std::mutex> lock(propertiesLock);
  return *properties;
}

}
