#include <cmath>
#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>

#include <cstdio>
#include <vector>
#include <string>
#include "src/Tools.hpp"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "src/gen-cpp/BallotBox.h"

#define PRIVATEFILE   "/tmp/keyfile.priv"
#define BALLOTBOXHOST "localhost"
#define BALLOTBOXPORT 9091

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace std;
using namespace CryptoPP;


int main(int argc, char* argv[]) {
    vector<string> votes;
    RSA::PrivateKey privKey;
    string privBase64Key;
    
    // read the private key. We got it, because election is finish
    Tools::DecodePrivateKey(PRIVATEFILE, privKey);
    Base64Encoder privKeySink(new StringSink(privBase64Key));
    privKey.DEREncode(privKeySink);
    privKeySink.MessageEnd();
    
    shared_ptr<TTransport> socket(new TSocket(BALLOTBOXHOST, BALLOTBOXPORT));
    shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    BallotBoxClient ballotBox(protocol);

    transport->open();
    ballotBox.endElection(votes, privBase64Key);
    transport->close();
    
    printf("Election ends, because "
        "we got the private key from the registration office. "
        "I am Schoenenborn, and I have to analyse this:\n\n"
    );
    
    for (auto str : votes) printf("%s\n", str.c_str());
    return 0;
}
