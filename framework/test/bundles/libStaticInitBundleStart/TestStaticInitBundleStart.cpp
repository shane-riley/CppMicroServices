/*=============================================================================

  Library: CppMicroServices

  Copyright (c) The CppMicroServices developers. See the COPYRIGHT
  file at the top-level directory of this distribution and at
  https://github.com/CppMicroServices/CppMicroServices/COPYRIGHT .

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#include "cppmicroservices/Bundle.h"
#include "cppmicroservices/BundleActivator.h"
#include "cppmicroservices/BundleContext.h"
#include "cppmicroservices/GlobalConfig.h"
#include "cppmicroservices/util/FileSystem.h"
#include "cppmicroservices/Framework.h"
#include "cppmicroservices/FrameworkEvent.h"
#include "cppmicroservices/FrameworkFactory.h"

#include "TestUtils.h"
#include <TestingConfig.h>

#include <algorithm>
#include <iostream>
#include <thread>

namespace cppmicroservices {

auto globalFunc = []() {
        std::string libToStart(cppmicroservices::testing::LIB_PATH + util::DIR_SEP + US_LIB_PREFIX + "TestBundleA" + US_LIB_EXT);
        auto f = cppmicroservices::FrameworkFactory().NewFramework();
        f.Start();
        auto bundles = f.GetBundleContext().InstallBundles(libToStart);
        bundles[0].Start();

        f.Stop();
        f.WaitForStop(std::chrono::milliseconds::zero());
        return true;
}();

class TestStaticInitBundleStartActivator : public BundleActivator
{
public:
  TestStaticInitBundleStartActivator() {}
  ~TestStaticInitBundleStartActivator() {}

  void Start(BundleContext) { }
  void Stop(BundleContext) { }
};

}

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(cppmicroservices::TestStaticInitBundleStartActivator)
