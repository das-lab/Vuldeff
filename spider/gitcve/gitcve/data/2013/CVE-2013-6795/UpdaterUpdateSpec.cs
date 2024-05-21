using System.Collections.Generic;
using NUnit.Framework;
using NUnit.Framework.SyntaxHelpers;
using Rackspace.Cloud.Server.Agent.Actions;
using Rackspace.Cloud.Server.Agent.Commands;
using Rackspace.Cloud.Server.Agent.Configuration;
using Rackspace.Cloud.Server.Common.Logging;
using Rhino.Mocks;

namespace Rackspace.Cloud.Server.Agent.Specs
{
    [TestFixture]
    public class UpdaterUpdateSpec
    {
        private IAgentUpdateMessageHandler _agentUpdateMessageHandler;
        private IDownloader _downloader;
        private IUnzipper _unzipper;
        private ISleeper _sleeper;
        private IChecksumValidator _checksumValidator;
        private IFileCopier _fileCopier;
        private IFinalizer _finalizer;
        private UpdaterUpdate _updaterUpdate;
        private string _agentUpdateInfo;
        private IConnectionChecker _connectionChecker;
        private IServiceStopper _serviceStopper;
        private IServiceStarter _serviceStarter;
        private ILogger _logger;

        [SetUp]
        public void Setup()
        {
            _agentUpdateInfo = "http://something.com/file.zip,544564abc453de787ad";

            _downloader = MockRepository.GenerateMock<IDownloader>();
            _checksumValidator = MockRepository.GenerateMock<IChecksumValidator>();
            _unzipper = MockRepository.GenerateMock<IUnzipper>();
            _fileCopier = MockRepository.GenerateMock<IFileCopier>();
            _finalizer = MockRepository.GenerateMock<IFinalizer>();
            _connectionChecker = MockRepository.GenerateMock<IConnectionChecker>();
            _sleeper = MockRepository.GenerateMock<ISleeper>();
            _logger = MockRepository.GenerateMock<ILogger>();
            _serviceStopper = MockRepository.GenerateMock<IServiceStopper>();
            _serviceStarter = MockRepository.GenerateMock<IServiceStarter>();
            _agentUpdateMessageHandler = new AgentUpdateMessageHandler();

            _logger.Stub(x => x.Log(Arg<string>.Is.Anything));

            _updaterUpdate = new UpdaterUpdate(_sleeper, _downloader, _checksumValidator, _unzipper, _fileCopier, _finalizer, _serviceStopper, _serviceStarter, _connectionChecker, _agentUpdateMessageHandler, _logger);

        }

        [Test]
        public void should_update_xentools()
        {
            _sleeper.Expect(x => x.Sleep(Arg<int>.Is.Anything));
            _connectionChecker.Expect(x => x.Check());
            _downloader.Expect(x => x.Download("http://something.com/file.zip", Constants.XenToolsReleasePackage));
            _checksumValidator.Expect(x => x.Validate("544564abc453de787ad", Constants.XenToolsReleasePackage));
            _unzipper.Expect(x => x.Unzip(Arg<string>.Is.Anything, Arg<string>.Is.Anything, Arg<string>.Is.Anything));
            _serviceStopper.Expect(x => x.Stop("RackspaceCloudServersAgentUpdater"));
            _fileCopier.Expect(x => x.CopyFiles(Constants.UpdaterUnzipPath, Constants.UpdaterPath, _logger));
            _finalizer.Expect(x => x.Finalize(new List<string>
                                                  {
                                                      Constants.UpdaterUnzipPath, 
                                                      Constants.UpdaterReleasePackage
                                                  }
                                                 ));
            _serviceStarter.Expect(x => x.Start("RackspaceCloudServersAgentUpdater"));

            _updaterUpdate.Execute(_agentUpdateInfo);
        }

        [Test]
        public void should_throw_UnsuccessfulCommandExecutionException_if_connection_to_updater_service_fails()
        {
            _sleeper.Expect(x => x.Sleep(Arg<int>.Is.Anything));
            _connectionChecker.Stub(x => x.Check())
                .Throw(new UnsuccessfulCommandExecutionException("error message", new ExecutableResult { ExitCode = "1" }));
            var result = _updaterUpdate.Execute(_agentUpdateInfo);
            Assert.That(result.ExitCode, Is.EqualTo("1"));
            Assert.That(result.Error[0], Is.EqualTo("Update failed"));
        }
    }
}
