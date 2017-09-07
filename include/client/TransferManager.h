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

#ifndef _QSFS_FUSE_INCLUDED_CLIENT_TRANSFERMANAGER_H_  // NOLINT
#define _QSFS_FUSE_INCLUDED_CLIENT_TRANSFERMANAGER_H_  // NOLINT

#include <stdint.h>
#include "data/ResourceManager.h"

#include <iostream>
#include <memory>

#include "filesystem/Configure.h"

namespace QS {

namespace Data {
class ResourceManager;
}  // namespace Data

namespace FileSystem {
class Drive;
}  // namespace FileSystem

namespace Threading {
class ThreadPool;
}  // namespace Threading

namespace Client {

class Client;
class TransferHandle;

struct TransferManagerConfigure {
  // Maximum size of the working buffers to use
  uint64_t m_bufferMaxHeapSize =
      QS::FileSystem::Configure::GetDefaultTransferMaxBufHeapSize();

  // Memory size allocated for one transfer buffer
  // If you are uploading large files(e.g. larger than 50GB), this needs to be
  // specified to be a size larger. And keeping in mind that you may need to
  // increase your max heap size if you plan on increasing buffer size.
  uint64_t m_bufferSize =
      QS::FileSystem::Configure::GetDefaultTransferMaxBufSize();

  // Maximum number of file transfers to run in parallel.
  size_t m_maxParallelTransfers =
      QS::FileSystem::Configure::GetDefaultMaxParallelTransfers();

  TransferManagerConfigure(
      uint64_t bufMaxHeapSize =
          QS::FileSystem::Configure::GetDefaultTransferMaxBufHeapSize(),
      uint64_t bufSize =
          QS::FileSystem::Configure::GetDefaultTransferMaxBufSize(),
      size_t maxParallelTransfers =
          QS::FileSystem::Configure::GetDefaultMaxParallelTransfers())
      : m_bufferMaxHeapSize(bufMaxHeapSize),
        m_bufferSize(bufSize),
        m_maxParallelTransfers(maxParallelTransfers) {}
};

class TransferManager {
 public:
  TransferManager(const TransferManagerConfigure &config);
  TransferManager(TransferManager &&) = delete;
  TransferManager(const TransferManager &) = delete;
  TransferManager &operator=(TransferManager &&) = delete;
  TransferManager &operator=(const TransferManager &) = delete;
  virtual ~TransferManager();

 public:
  // Download a file
  //
  // @param  : file path, file offset, size, bufStream
  // @return : transfer handle
  virtual std::shared_ptr<TransferHandle> DownloadFile(
      const std::string &filePath, off_t offset, uint64_t size,
      std::shared_ptr<std::iostream> bufStream) = 0;

  // Retry a failed download
  //
  // @param  : transfer handle to retry, bufStream
  // @return : transfer handle after been retried
  virtual std::shared_ptr<TransferHandle> RetryDownload(
      const std::shared_ptr<TransferHandle> &handle,
      std::shared_ptr<std::iostream> bufStream) = 0;

  // Upload a file
  //
  // @param  : file path, file size
  // @return : transfer handle
  virtual std::shared_ptr<TransferHandle> UploadFile(
      const std::string &filePath, uint64_t fileSize) = 0;

  // Retry a failed upload
  //
  // @param  : tranfser handle to retry
  // @return : transfer handle after been retried
  virtual std::shared_ptr<TransferHandle> RetryUpload(
      const std::shared_ptr<TransferHandle> &handle) = 0;

  // Abort a multipart upload
  //
  // @param  : tranfer handle to abort
  // @return : void
  //
  // By default, multipart upload will remain in a Failed state if they fail,
  // or a Cancelled state if they were cancelled. Leaving failed state around
  // still costs the owner of the bucket money. If you know you will not going
  // to retry it, abort the multipart upload request after cancelled or failed.
  virtual void AbortMultipartUpload(
      const std::shared_ptr<TransferHandle> &handle) = 0;

 public:
  uint64_t GetBufferMaxHeapSize() const {
    return m_configure.m_bufferMaxHeapSize;
  }
  uint64_t GetBufferSize() const { return m_configure.m_bufferSize; }
  size_t GetMaxParallelTransfers() const {
    return m_configure.m_maxParallelTransfers;
  }
  size_t GetBufferCount() const;

 protected:
  std::shared_ptr<Client> GetClient() { return m_client; }
  std::unique_ptr<QS::Threading::ThreadPool> &GetExecutor() {
    return m_executor;
  }
  std::unique_ptr<QS::Data::ResourceManager> &GetBufferManager() {
    return m_bufferManager;
  }

 private:
  void SetClient(const std::shared_ptr<Client> &client);

 private:
  void InitializeResources();

 private:
  TransferManagerConfigure m_configure;
  std::unique_ptr<QS::Data::ResourceManager> m_bufferManager;

  // This executor is used in a different context with the client used one.
  std::unique_ptr<QS::Threading::ThreadPool> m_executor;
  std::shared_ptr<Client> m_client;

  friend class QS::FileSystem::Drive;
};

}  // namespace Client
}  // namespace QS

// NOLINTNEXTLINE
#endif  // _QSFS_FUSE_INCLUDED_CLIENT_TRANSFERMANAGER_H_
