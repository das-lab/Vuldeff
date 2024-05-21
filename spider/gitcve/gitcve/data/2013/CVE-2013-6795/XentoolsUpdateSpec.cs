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
    public class XentoolsUpdateSpec
    {
        private IAgentUpdateMessageHandler _agentUpdateMessageHandler;
        private IDownloader _downloader;
        private IUnzipper _unzipper;
        private ISleeper _sleeper;
        private IChecksumValidator _checksumValidator;
        private IInstaller _installer;
        private IFinalizer _finalizer;
        private XentoolsUpdate _xentoolsUpdate;
        private string _agentUpdateInfo;
        private IConnectionChecker _connectionChecker;
        private IServiceRestarter _serviceRestarter;
        private ILogger _logger;
        private MockRepository _mockRepo;

        [SetUp]
        public void Setup()
        {
            _mockRepo = new MockRepository();
            _agentUpdateInfo = "http://something.com/file.zip,544564abc453de787ad";

            _downloader = MockRepository.GenerateMock<IDownloader>();
            _checksumValidator = MockRepository.GenerateMock<IChecksumValidator>();
            _unzipper = MockRepository.GenerateMock<IUnzipper>();
            _installer = MockRepository.GenerateMock<IInstaller>();
            _finalizer = MockRepository.GenerateMock<IFinalizer>();
            _connectionChecker = MockRepository.GenerateMock<IConnectionChecker>();
            _sleeper = MockRepository.GenerateMock<ISleeper>();
            _logger = MockRepository.GenerateMock<ILogger>();
            _serviceRestarter = _mockRepo.StrictMock<IServiceRestarter>();
            _agentUpdateMessageHandler = new AgentUpdateMessageHandler();

            _logger.Stub(x => x.Log(Arg<string>.Is.Anything));

            _xentoolsUpdate = new XentoolsUpdate(_sleeper, _downloader, _checksumValidator, _unzipper, _installer, _finalizer, _serviceRestarter, _connectionChecker, _agentUpdateMessageHandler, _logger);

        }

        [Test]
        public void should_update_xentools()
        {
            _sleeper.Expect(x => x.Sleep(Arg<int>.Is.Anything));
            _connectionChecker.Expect(x => x.Check());
            _downloader.Expect(x => x.Download("http://something.com/file.zip", Constants.XenToolsReleasePackage));
            _checksumValidator.Expect(x => x.Validate("544564abc453de787ad", Constants.XenToolsReleasePackage));
            _unzipper.Expect(x => x.Unzip(Arg<string>.Is.Anything, Arg<string>.Is.Anything, Arg<string>.Is.Anything));
            _installer.Expect(x => x.Install(new Dictionary<string, string>
                                                 {
                                                     {
                                                         Constants.XenToolsUnzipPath,
                                                         "/S /norestart /D=" + Constants.XenToolsPath
                                                         }
                                                 }));
            _finalizer.Expect(x => x.Finalize(new List<string>
                                                  {
                                                      Constants.XenToolsUnzipPath, 
                                                      Constants.XenToolsReleasePackage
                                                  }
                                                 ));
            _serviceRestarter.Expect(x => x.Restart("xensvc"));
            _serviceRestarter.Expect(x => x.Restart("XenServerVssProvider"));
            _serviceRestarter.Expect(x => x.ServiceExists("XenServerVssProvider")).Return(true);
            _mockRepo.ReplayAll();
            _xentoolsUpdate.Execute(_agentUpdateInfo);
            _mockRepo.VerifyAll();
        }

        [Test]
        public void should_not_restart_vss_provider_if_service_does_not_exist()
        {
            _serviceRestarter.Expect(x => x.Restart("xensvc"));
            _serviceRestarter.Expect(x => x.Restart("XenServerVssProvider")).Repeat.Never();
            _serviceRestarter.Expect(x => x.ServiceExists("XenServerVssProvider")).Return(false);

            _mockRepo.ReplayAll();
            _xentoolsUpdate.Execute(_agentUpdateInfo);
            _mockRepo.VerifyAll();
        }

        [Test]
        public void should_throw_UnsuccessfulCommandExecutionException_if_connection_to_updater_service_fails()
        {
            _sleeper.Expect(x => x.Sleep(Arg<int>.Is.Anything));
            _connectionChecker.Stub(x => x.Check())
                .Throw(new UnsuccessfulCommandExecutionException("error message", new ExecutableResult { ExitCode = "1" }));
            var result = _xentoolsUpdate.Execute(_agentUpdateInfo);
            Assert.That(result.ExitCode, Is.EqualTo("1"));
            Assert.That(result.Error[0], Is.EqualTo("Update failed"));
        }
    }
}