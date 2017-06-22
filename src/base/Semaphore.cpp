// +-------------------------------------------------------------------------
// | Copyright (C) 2017 Yunify, Inc.
// +-------------------------------------------------------------------------
// | Licensed under the Apache License, Version 2.0 (the "License");
// | You may not use this work except in compliance with the License.
// | You may obtain a copy of the License in the LICENSE file, or at:
// |
// | http://www.apache.org/licenses/LICENSE-2.0
// |
// | Unless required by applicable law or agreed to in writing, software
// | distributed under the License is distributed on an "AS IS" BASIS,
// | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// | See the License for the specific language governing permissions and
// | limitations under the License.
// +-------------------------------------------------------------------------

#include "base/Semaphore.h"

namespace QS {

namespace Threading {

using std::mutex;
using std::unique_lock;

void Semaphore::notify_one() {
  {
    unique_lock<mutex> lock(m_mutex);
    ++m_count;
  }
  m_conditionVariable.notify_one();
}

void Semaphore::notify_all() {
  {
    unique_lock<mutex> lock(m_mutex);
    ++m_count;
  }
  m_conditionVariable.notify_all();
}

void Semaphore::wait() {
  unique_lock<mutex> lock(m_mutex);
  m_conditionVariable.wait(lock, [this] { return m_count > 0; });
  --m_count;
}

}  // namespace Threading
}  // namespace QS
