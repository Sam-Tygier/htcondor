#include "condor_common.h"
#include "condor_sinful.h"

#include <stdio.h>

//
// For now, we're only testing the addrs-related functionality.
//

bool verbose = false;
#define REQUIRE( condition ) \
	if(! ( condition )) { \
		fprintf( stderr, "Failed requirement '%s' on line %d.\n", #condition, __LINE__ ); \
		return 1; \
	} else if( verbose ) { \
		fprintf( stdout, "Passed requirement '%s' on line %d.\n", #condition, __LINE__ ); \
	}

int main( int, char ** ) {
	Sinful s;
	Sinful t;

	// FIXME: This whole test needs to be reworked.
#if defined( NEW_SCHOOL_SINFULS )

	//
	// Test, for 0, 1, 2, and 3 condor_sockaddrs: that the vector is not NULL,
	// that the vector has the correct number of elements, that the vector's
	// elements values are correct (and in the correct order); and that the
	// string form of the sinful is correct; and that the parser correctly
	// handles the sinful string (by repeating the vector tests); and that
	// the string->sinful->string loop also works (by repeating the string
	// test).  Then repeat the 0 test after clearing the Sinful of addrs.
	//
	// We then also test adding three condor_sockaddrs in a row, just in
	// case one-at-a-time testing somehow hides a problem; this also tests
	// that adding condor_sockaddrs after clearing the Sinful works.
	//


	std::vector< condor_sockaddr > * v = s.getAddrs();
	REQUIRE( v != NULL );
	REQUIRE( v->size() == 0 );
	delete v; v = NULL;

	char const * sinfulString = NULL;
	sinfulString = s.getSinful();
	REQUIRE( sinfulString == NULL );

	REQUIRE( ! s.hasAddrs() );


	condor_sockaddr sa;
	bool ok = sa.from_ip_and_port_string( "1.2.3.4:5" );
	REQUIRE( ok );
	s.addAddrToAddrs( sa );

	v = s.getAddrs();
	REQUIRE( v != NULL );
	REQUIRE( v->size() == 1 );
	REQUIRE( (*v)[0] == sa );
	delete v; v = NULL;

	sinfulString = s.getSinful();
	REQUIRE( sinfulString != NULL );
	REQUIRE( strcmp( sinfulString, "<?addrs=1.2.3.4:5>" ) == 0 );

	t = Sinful( sinfulString );
	v = t.getAddrs();
	REQUIRE( v != NULL );
	REQUIRE( v->size() == 1 );
	REQUIRE( (*v)[0] == sa );
	delete v; v = NULL;

	sinfulString = t.getSinful();
	REQUIRE( sinfulString != NULL );
	REQUIRE( strcmp( sinfulString, "<?addrs=1.2.3.4:5>" ) == 0 );

	REQUIRE( s.hasAddrs() );


	condor_sockaddr sa2;
	ok = sa2.from_ip_and_port_string( "5.6.7.8:9" );
	REQUIRE( ok );
	s.addAddrToAddrs( sa2 );

	v = s.getAddrs();
	REQUIRE( v != NULL );
	REQUIRE( v->size() == 2 );
	REQUIRE( (*v)[0] == sa );
	REQUIRE( (*v)[1] == sa2 );
	delete v; v = NULL;

	sinfulString = s.getSinful();
	REQUIRE( sinfulString != NULL );
	REQUIRE( strcmp( sinfulString, "<?addrs=1.2.3.4:5+5.6.7.8:9>" ) == 0 );

	t = Sinful( sinfulString );
	v = t.getAddrs();
	REQUIRE( v != NULL );
	REQUIRE( v->size() == 2 );
	REQUIRE( (*v)[0] == sa );
	REQUIRE( (*v)[1] == sa2 );
	delete v; v = NULL;

	sinfulString = t.getSinful();
	REQUIRE( sinfulString != NULL );
	REQUIRE( strcmp( sinfulString, "<?addrs=1.2.3.4:5+5.6.7.8:9>" ) == 0 );

	REQUIRE( s.hasAddrs() );


	condor_sockaddr sa3;
	ok = sa3.from_ip_and_port_string( "[1:3:5:7::a]:13" );
	REQUIRE( ok );
	s.addAddrToAddrs( sa3 );

	v = s.getAddrs();
	REQUIRE( v != NULL );
	REQUIRE( v->size() == 3 );
	REQUIRE( (*v)[0] == sa );
	REQUIRE( (*v)[1] == sa2 );
	REQUIRE( (*v)[2] == sa3 );
	delete v; v = NULL;

	sinfulString = s.getSinful();
	REQUIRE( sinfulString != NULL );
	REQUIRE( strcmp( sinfulString, "<?addrs=1.2.3.4:5+5.6.7.8:9+[1:3:5:7::a]:13>" ) == 0 );

	t = Sinful( sinfulString );
	v = t.getAddrs();
	REQUIRE( v != NULL );
	REQUIRE( v->size() == 3 );
	REQUIRE( (*v)[0] == sa );
	REQUIRE( (*v)[1] == sa2 );
	REQUIRE( (*v)[2] == sa3 );
	delete v; v = NULL;

	sinfulString = t.getSinful();
	REQUIRE( sinfulString != NULL );
	REQUIRE( strcmp( sinfulString, "<?addrs=1.2.3.4:5+5.6.7.8:9+[1:3:5:7::a]:13>" ) == 0 );

	REQUIRE( s.hasAddrs() );


	s.clearAddrs();
	v = s.getAddrs();
	REQUIRE( v != NULL );
	REQUIRE( v->size() == 0 );

	sinfulString = s.getSinful();
	REQUIRE( sinfulString != NULL );
	REQUIRE( strcmp( sinfulString, "<>" ) == 0 );

	t = Sinful( sinfulString );
	v = t.getAddrs();
	REQUIRE( v != NULL );
	REQUIRE( v->size() == 0 );

	sinfulString = t.getSinful();
	REQUIRE( sinfulString != NULL );
	REQUIRE( strcmp( sinfulString, "<>" ) == 0 );

	REQUIRE( ! s.hasAddrs() );


	s.addAddrToAddrs( sa );
	s.addAddrToAddrs( sa2 );
	s.addAddrToAddrs( sa3 );

	v = s.getAddrs();
	REQUIRE( v != NULL );
	REQUIRE( v->size() == 3 );
	REQUIRE( (*v)[0] == sa );
	REQUIRE( (*v)[1] == sa2 );
	REQUIRE( (*v)[2] == sa3 );
	delete v; v = NULL;

	sinfulString = s.getSinful();
	REQUIRE( sinfulString != NULL );
	REQUIRE( strcmp( sinfulString, "<?addrs=1.2.3.4:5+5.6.7.8:9+[1:3:5:7::a]:13>" ) == 0 );

	t = Sinful( sinfulString );
	v = t.getAddrs();
	REQUIRE( v != NULL );
	REQUIRE( v->size() == 3 );
	REQUIRE( (*v)[0] == sa );
	REQUIRE( (*v)[1] == sa2 );
	REQUIRE( (*v)[2] == sa3 );
	delete v; v = NULL;

	sinfulString = t.getSinful();
	REQUIRE( sinfulString != NULL );
	REQUIRE( strcmp( sinfulString, "<?addrs=1.2.3.4:5+5.6.7.8:9+[1:3:5:7::a]:13>" ) == 0 );

	REQUIRE( s.hasAddrs() );
#endif /* defined( NEW_SCHOOL_SINFULS ) */

	return 0;
}
