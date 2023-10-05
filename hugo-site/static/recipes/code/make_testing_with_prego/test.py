import hamcrest
import prego

class TestNcat(prego.TestCase):
	def test_server(self):

		prego.context.port = 2000
		
		server = prego.Task(desc='ncat server', detach=True)
		cmd = server.command('ncat -l -p $port')
		server.assert_that(cmd.stdout.content, hamcrest.contains_string('testing'))

		client = prego.Task(desc='ncat client')
		client.wait_that(server, prego.running())
		client.command('ncat -c "echo testing" localhost $port')		
