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
using System.Reflection;
using System.Timers;
using Rackspace.Cloud.Server.Agent.Interfaces;
using Rackspace.Cloud.Server.Common.Logging;
using StructureMap;
using StructureMap.Configuration.DSL;

namespace Rackspace.Cloud.Server.Agent.Service {
    public class ServerClass {
        private readonly ILogger _logger;
        private ITimer _timer;

        public ServerClass(ILogger logger) {
            _logger = logger;
        }

        public void Onstart() {
            _logger.Log("Agent Service Starting ...");
            _logger.Log("Agent Version: " + Assembly.GetExecutingAssembly().GetName().Version);

            const int TIMER_INTERVAL_IS_SIX_SECONDS = 6000;

            _timer = new ProdTimer { Interval = TIMER_INTERVAL_IS_SIX_SECONDS };
            _timer.Elapsed(TimerElapsed);
            _timer.Enabled = true;

            StructureMapConfiguration.UseDefaultStructureMapConfigFile = false;
            StructureMapConfiguration.BuildInstancesOf<ITimer>().TheDefaultIs(Registry.Object(_timer));
            IoC.Register();
        }

        public void Onstop() {
            LogManager.ShouldBeLogging = true;
            _logger.Log("Agent Service Stopping ...");
            _timer.Enabled = false;
        }

        private void TimerElapsed(object sender, ElapsedEventArgs e) {
            try {
                ObjectFactory.GetInstance<ServiceWork>().Do();
            } catch (Exception ex) {
                _logger.Log("Exception was : " + ex.Message + "\nStackTrace Was: " + ex.StackTrace);
            }
        }
    }
}