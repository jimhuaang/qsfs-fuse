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

#include "qingstor/Options.h"

#include <assert.h>
#include <stddef.h>  // for offsetof
#include <string.h>  // for strdup

#include "base/Exception.h"
#include "base/LogLevel.h"
#include "client/Protocol.h"
#include "client/RetryStrategy.h"
#include "client/Zone.h"
#include "qingstor/Configure.h"
#include "qingstor/IncludeFuse.h"  // for fuse.h

namespace QS {

namespace QingStor {

using QS::Exception::QSException;

namespace Parser {

namespace {
static struct options {
  // We can't set default values for the char* fields here
  // because fuse_opt_parse would attempt to free() them
  // when user specifies different values on command line.
  const char *bucket;
  const char *mountPoint;
  const char *zone;
  const char *host;
  const char *protocol;
  unsigned port = QS::Client::DefaultPort::HTTPS;
  unsigned retries = QS::Client::Retry::DefaultMaxRetries;
  const char *addtionalAgent;
  const char *logDirectory;
  const char *logLevel;        // INFO, WARN, ERROR, FATAL
  int nonEmpty = 0;            // default mount point is empty
  int foreground = 0;          // default not foreground
  int singleThread = 0;        // default multi-thread
  int debug = 0;               // default no debug
  int showHelp = 0;
  int showVersion = 0;
} options;

#define OPTION(t, p) \
  { t, offsetof(struct options, p), 1 }

static const struct fuse_opt optionSpec[] = {
    OPTION("-b=%s", bucket),         OPTION("--bucket=%s",   bucket),
    OPTION("-m=%s", mountPoint),     OPTION("--mount=%s",    mountPoint),
    OPTION("-z=%s", zone),           OPTION("--zone=%s",     zone),
    OPTION("-h=%s", host),           OPTION("--host=%s",     host),
    OPTION("-p=%s", protocol),       OPTION("--protocol=%s", protocol),
    OPTION("-t=%u", port),           OPTION("--port=%u",     port),
    OPTION("-r=%u", retries),        OPTION("--retries=%u",  retries),
    OPTION("-a=%s", addtionalAgent), OPTION("--agent=%s",    addtionalAgent),
    OPTION("-l=%s", logDirectory),   OPTION("--logdir=%s",   logDirectory),
    OPTION("-e=%s", logLevel),       OPTION("--loglevel=%s", logLevel),
    OPTION("-n",    nonEmpty),       OPTION("--nonempty",    nonEmpty),
    OPTION("-f",    foreground),     OPTION("--foreground",  foreground),
    OPTION("-s",    singleThread),   OPTION("--single",      singleThread),
    OPTION("-d",    debug),          OPTION("--debug",       debug),
    OPTION("--help",showHelp),       OPTION("--version",     showVersion),
    FUSE_OPT_END
};

}  // namespace

void Parse(int argc, char **argv) {
  auto &qsOptions =QS::QingStor::Options::Instance();
  qsOptions.SetFuseArgs(argc, argv);

  // Set defaults for const char*. 
  // we have to use strdup so that fuse_opt_parse
  // can free the defaults if other values are specified.
  options.bucket         = strdup("");
  options.mountPoint     = strdup("");
  options.zone           = strdup(QS::Client::Zone::PEK_3A);
  options.host           = strdup(QS::Client::Host::BASE);
  options.protocol       = strdup(QS::Client::Protocol::HTTPS);
  options.addtionalAgent = strdup("");
  options.logDirectory   = strdup(
      QS::QingStor::Configure::GetDefaultLogDirectory().c_str());
  options.logLevel       = strdup(
      QS::Logging::GetLogLevelName(QS::Logging::LogLevel::Info).c_str());

  auto & args = qsOptions.GetFuseArgs();
  if (0 != fuse_opt_parse(&args, &options, optionSpec, NULL)) {
    throw QSException("Error while parsing command line options.");
  }

  qsOptions.SetBucket(options.bucket);
  qsOptions.SetMountPoint(options.mountPoint);
  qsOptions.SetZone(options.zone);
  qsOptions.SetHost(options.host);
  qsOptions.SetProtocol(options.protocol);
  qsOptions.SetPort(options.port);
  qsOptions.SetRetries(options.retries);
  qsOptions.SetAdditionalAgent(options.addtionalAgent);
  qsOptions.SetLogDirectory(options.logDirectory);
  qsOptions.SetLogLevel(QS::Logging::GetLogLevelByName(options.logLevel));
  qsOptions.SetMountPointNonEmpty(options.nonEmpty != 0);
  qsOptions.SetForeground(options.foreground != 0);
  qsOptions.SetSingleThread(options.singleThread != 0);
  qsOptions.SetDebug(options.debug != 0);
  qsOptions.SetShowHelp(options.showHelp != 0);
  qsOptions.setShowVerion(options.showVersion !=0);

  // Put signals for fuse_main.
  // TODO(jim): should we put program name
  // and put bucket name
  //fuse_opt_free_args(&args);
  if (qsOptions.IsShowHelp()) {
    assert(fuse_opt_add_arg(&args, "--help") == 0);
  }
  if (qsOptions.IsShowVersion()) {
    assert(fuse_opt_add_arg(&args, "--verion") == 0);
  }
  if (qsOptions.IsForeground()) {
    assert(fuse_opt_add_arg(&args, "-f") == 0);
  }
  if (qsOptions.IsSingleThread()) {
    assert(fuse_opt_add_arg(&args, "-s") == 0);
  }
  if (qsOptions.IsDebug()) {
    assert(fuse_opt_add_arg(&args, "-d") == 0);
  }
}

}  // namespace Parser
}  // namespace QingStor
}  // namespace QS
