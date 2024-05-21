// Copyright 2011 OpenStack LLC.
// All Rights Reserved.
//
//    Licensed under the Apache License, Version 2.0 (the "License"); you may
//    not use this file except in compliance with the License. You may obtain
//    a copy of the License at
//
//         http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
//    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
//    License for the specific language governing permissions and limitations
//    under the License.

using System;
using System.IO;
using System.Threading;
using Rackspace.Cloud.Server.Agent.UpdaterService.Commands;
using Rackspace.Cloud.Server.Common.AgentUpdate;
using Rackspace.Cloud.Server.Common.Communication;
using Rackspace.Cloud.Server.Common.Configuration;
using Rackspace.Cloud.Server.Common.Logging;

namespace Rackspace.Cloud.Server.Agent.UpdaterService {
    public class AgentUpdater : MarshalByRefObject, IAgentUpdater {
        private readonly Logger _logger;
        private AgentUpdateInfo _agentUpdateInfo;

        private AgentUpdater() {
            _logger = new Logger();
        }

        public void DoUpdate(AgentUpdateInfo agentUpdateInfo) {
            _agentUpdateInfo = agentUpdateInfo;
            _logger.Log(String.Format("Received from Agent the following data:\r\nURL:{0}\r\nCHECKSUM:{1}, will resume in a minute", _agentUpdateInfo.url, _agentUpdateInfo.signature));

            new Timer(TimerElapsed,null,60000,0);
        }

        private void TimerElapsed(object sender) {
            try {
                new Do(_agentUpdateInfo)
                    .Add<IBackupAgent>()
                    .Add<IAgentDownload>()
                    .Add<ICheckSumCheck>()
                    .Add<IStopAgentService>()
                    .Add<IInstallAgentService>()
                    .Add<IStartAgentService>()
                    .Add<IFinalize>()
                    .Run()
                    ;
            } catch (Exception ex) {
                _logger.Log("Exception was : " + ex.Message + "\nStackTrace Was: " + ex.StackTrace);
                _logger.Log("Rolling back the update operation...reinstalling the old version of the agent.");
                
                if (!Directory.Exists(Constants.AgentServiceBackupPath)) return;
                
                new StopAgentService(_logger).Execute(_agentUpdateInfo);
                Utility.CopyFiles(Constants.AgentServiceBackupPath, SvcConfiguration.AgentPath, _logger);
                new StartAgentService(_logger).Execute(_agentUpdateInfo);
            }
        }
    }
}
