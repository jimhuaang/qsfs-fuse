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

#ifndef _QSFS_FUSE_INCLUDE_CLIENT_CONFIGURATION_H_  // NOLINT
#define _QSFS_FUSE_INCLUDE_CLIENT_CONFIGURATION_H_  // NOLINT

#include <stddef.h>

namespace QS {

namespace Client {

size_t GetMaxCacheSize() {
  // TODO(Jim) : read form config file
  const int maxCacheSize = 104857600;  // 100 * 1024 *1024 (100M)
  return maxCacheSize;
}

}  // namespace Client
}  // namespace QS

// NOLINTNEXTLIN
#endif  //_QSFS_FUSE_INCLUDE_CLIENT_CONFIGURATION_H_
