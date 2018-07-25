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

#include "BundleThread.h"

#include "cppmicroservices/BundleActivator.h"
#include "cppmicroservices/FrameworkEvent.h"

#include "cppmicroservices/util/String.h"

#include "BundlePrivate.h"
#include "CoreBundleContext.h"

#include <future>

namespace cppmicroservices {

const int BundleThread::OP_IDLE = 0;
const int BundleThread::OP_BUNDLE_EVENT = 1;
const int BundleThread::OP_START = 2;
const int BundleThread::OP_STOP = 3;

#ifdef US_ENABLE_THREADING_SUPPORT
const std::chrono::milliseconds BundleThread::KEEP_ALIVE(1000);

BundleThread::BundleThread(CoreBundleContext* ctx)
  : be(BundleEvent::BUNDLE_INSTALLED, nullptr)
  , op()
  , doRun(true)
{
  th.v = std::thread(&BundleThread::Run, this, ctx);
}
#else
BundleThread::BundleThread(CoreBundleContext*)
  : be(BundleEvent::BUNDLE_INSTALLED, nullptr)
{
}
#endif

BundleThread::~BundleThread()
{
  Quit();
}

void BundleThread::Quit()
{
#ifdef US_ENABLE_THREADING_SUPPORT
  doRun = false;
  op.NotifyAll();
  auto l = th.Lock(); US_UNUSED(l);
  if (th.v.joinable()) th.v.join();
#endif
}

void BundleThread::Run(CoreBundleContext* fwCtx)
{
#ifdef US_ENABLE_THREADING_SUPPORT
  while (doRun)
  {
    std::promise<bool> pr;
    int operation = OP_IDLE;
    BundlePrivate* bundle = nullptr;
    BundleEventInternal bev{ BundleEvent::BUNDLE_INSTALLED, nullptr };

    while (doRun)
    {
      auto l = op.Lock(); US_UNUSED(l);
      if (doRun && op.operation == OP_IDLE)
      {
        op.WaitFor(l, KEEP_ALIVE);
      }

      if (!doRun) return;
      if (op.operation != OP_IDLE)
      {
        pr = std::move(op.pr);
        operation = op.operation;
        bundle = op.bundle;
        bev = be.Exchange(BundleEventInternal{ BundleEvent::BUNDLE_INSTALLED, nullptr });
        break;
      }

      // If we arrive here, we reached our keep-alive limit without
      // an operation being queued. However, in the mean time, some
      // other thread may have picked this thread from the bundleThreads.value
      // list and calling StartAndWait(). So we only terminate this thread
      // (by returning) if it was not taken from the bundleThreads list to
      // do more work. Otherwise, we just keep running for another keep alive
      // cycle.
      {
        auto l2 = fwCtx->bundleThreads.Lock(); US_UNUSED(l2);
        auto iter = std::find(fwCtx->bundleThreads.value.begin(), fwCtx->bundleThreads.value.end(), this->shared_from_this());
        if (iter != fwCtx->bundleThreads.value.end())
        {
          fwCtx->bundleThreads.zombies.push_back(*iter);
          fwCtx->bundleThreads.value.erase(iter);
          return;
        }
      }
    }

    if (!doRun) return;

    std::exception_ptr tmpres;
    try
    {
      switch (operation)
      {
      case OP_BUNDLE_EVENT:
        fwCtx->listeners.BundleChanged(MakeBundleEvent(bev));
        break;
      case OP_START:
        tmpres = bundle->Start0();
        break;
      case OP_STOP:
        tmpres = bundle->Stop1();
        break;
      }
    }
    catch (...)
    {
      fwCtx->listeners.SendFrameworkEvent(FrameworkEvent(FrameworkEvent::Type::FRAMEWORK_ERROR,
                                                            MakeBundle(bundle->shared_from_this()),
                                                            bundle->symbolicName,
                                                            std::current_exception()));
    }

    op.operation = OP_IDLE;
    if (tmpres)
    {
      pr.set_exception(tmpres);
    }
    else
    {
      pr.set_value(true);
    }

    // lock the resolver in order to synchronize with StartAndWait
    fwCtx->resolver.Lock(), fwCtx->resolver.NotifyAll();
  }
#else
  US_UNUSED(fwCtx);
#endif
}

void BundleThread::Join()
{
#ifdef US_ENABLE_THREADING_SUPPORT
  auto l = th.Lock(); US_UNUSED(l);
  if (th.v.joinable())
  {
    th.v.join();
  }
#endif
}

void BundleThread::BundleChanged(const BundleEventInternal& be, UniqueLock& resolveLock)
{
  this->be.Store(be);
  StartAndWait(be.bundle.get(), OP_BUNDLE_EVENT, resolveLock);
}

std::exception_ptr BundleThread::CallStart0(BundlePrivate* b, UniqueLock& resolveLock)
{
  return StartAndWait(b, OP_START, resolveLock);
}

std::exception_ptr BundleThread::CallStop1(BundlePrivate* b, UniqueLock& resolveLock)
{
  return StartAndWait(b, OP_STOP, resolveLock);
}

std::exception_ptr BundleThread::StartAndWait(BundlePrivate* b, int operation, UniqueLock& resolveLock)
{
#ifdef US_ENABLE_THREADING_SUPPORT
  std::future<bool> res;
  {
    auto l = op.Lock(); US_UNUSED(l);
    op.pr = std::promise<bool>();
    op.bundle = b;
    op.operation = operation;
    res = op.pr.get_future();
  }
  op.NotifyAll();

  b->coreCtx->resolver.WaitFor(resolveLock, std::chrono::milliseconds::zero(), [&res]{
    return res.valid() &&
        res.wait_for(std::chrono::milliseconds::zero()) == US_FUTURE_READY;
  });

  b->coreCtx->bundleThreads.Lock(), b->coreCtx->bundleThreads.value.push_front(this->shared_from_this());
  b->ResetBundleThread();
  try
  {
    res.get();
    return nullptr;
  }
  catch (...)
  {
    return std::current_exception();
  }
#else
  US_UNUSED(resolveLock);
  std::exception_ptr tmpres;
  try
  {
    switch (operation)
    {
    case OP_BUNDLE_EVENT:
      b->coreCtx->listeners.BundleChanged(MakeBundleEvent(this->be.Exchange(BundleEventInternal{ BundleEvent::BUNDLE_INSTALLED, nullptr })));
      break;
    case OP_START:
      tmpres = b->Start0();
      break;
    case OP_STOP:
      tmpres = b->Stop1();
      break;
    }
  }
  catch (...)
  {
    b->coreCtx->listeners.SendFrameworkEvent(FrameworkEvent(FrameworkEvent::Type::FRAMEWORK_ERROR,
                                                            MakeBundle(b->shared_from_this()),
                                                            b->symbolicName,
                                                            std::current_exception()));
  }
  return tmpres;
#endif
}

bool BundleThread::operator==(const std::thread::id& id) const
{
#ifdef US_ENABLE_THREADING_SUPPORT
  return (th.Lock(), th.v.get_id() == id);
#else
  return std::this_thread::get_id() == id;
#endif
}

}
