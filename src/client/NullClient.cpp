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

#include "client/NullClient.h"

#include "client/ClientError.h"
#include "client/QSError.h"

namespace QS {

namespace Client {

namespace {
ClientError<QSError> GoodState() {
  return ClientError<QSError>(QSError::GOOD, false);
}
}  // namespace

using std::string;

ClientError<QSError> NullClient::DeleteFile(const string &filePath) {
  return GoodState();
}

ClientError<QSError> NullClient::DeleteDirectory(const string &dirPath) {
  return GoodState();
}

ClientError<QSError> NullClient::MakeFile(const std::string &filePath) {
  return GoodState();
}

ClientError<QSError> NullClient::MakeDirectory(const std::string &dirPath) {
  return GoodState();
}

ClientError<QSError> NullClient::RenameFile(const std::string &filePath) {
  return GoodState();
}

ClientError<QSError> NullClient::RenameDirectory(const std::string &dirPath) {
  return GoodState();
}

ClientError<QSError> NullClient::DownloadFile(const std::string &filePath) {
  return GoodState();
}

ClientError<QSError> NullClient::DownloadDirectory(const std::string &dirPath) {
  return GoodState();
}

ClientError<QSError> NullClient::UploadFile(const std::string &filePath) {
  return GoodState();
}

ClientError<QSError> NullClient::UploadDirectory(const std::string &dirPath) {
  return GoodState();
}

ClientError<QSError> NullClient::ReadFile(const std::string &filePath) {
  return GoodState();
}

ClientError<QSError> NullClient::ListDirectory(const std::string &dirPath) {
  return GoodState();
}

ClientError<QSError> NullClient::WriteFile(const std::string &filePath) {
  return GoodState();
}

ClientError<QSError> NullClient::WriteDirectory(const std::string &dirPath) {
  return GoodState();
}

ClientError<QSError> NullClient::Stat(const std::string &path,
                                      time_t modifiedSince) {
  return GoodState();
}

}  // namespace Client
}  // namespace QS
