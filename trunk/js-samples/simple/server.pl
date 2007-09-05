use IO::Socket;

my $sock = new IO::Socket::INET (
								 LocalHost => '127.0.0.1',
								 LocalPort => '64446',
								 Proto => 'tcp',
								 Listen => 1,
								 Reuse => 1,
								);
die "Could not create socket: $!\n" unless $sock;
print "Waiting for incoming connection...\n";
while (1)
{
	my $new_sock = $sock->accept();
	printf( "Incoming connection from ('%s', %d)\n",  $new_sock->peerhost(), $new_sock->peerport() );
	for ( my $i=1; $i<5; $i++ )
	{
		my $strLine = <$new_sock>;
		print $strLine;
		$new_sock->send( "Hi there $i\n" );
	}
	close($new_sock);
}
close($sock);
    
