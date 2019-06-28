#include <cmath>
#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>

#include <algorithm>
#include <string>
#include <vector>
#include <cstdio>

#include "src/Tools.hpp"
#include "src/gen-cpp/BallotBox.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#define DIGESTBYTES   512
#define PUBFILE       "/tmp/keyfile.pub"
#define BALLOTBOXPORT 9091

using namespace std;
using namespace CryptoPP;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

vector<string> cryptedVotes;
vector<string> voted;

class BallotBoxHandler : virtual public BallotBoxIf {
private:
    RSA::PrivateKey _privKey; // if the election ends, we get this private key be votes() RPC request
    RSA::PublicKey * _pubKey;
    
public:
    BallotBoxHandler() = default;
    
    BallotBoxHandler(RSA::PublicKey * pubKey) {
        _pubKey = pubKey;
    }

    bool vote(const string & pseudonym, const string & signature, const string & pubCryptVote) {
        Integer v = _pubKey->ApplyFunction(Tools::convertBase64(signature));
        SecByteBlock p((const byte*) pseudonym.data(), pseudonym.size());
        Integer v2 = Tools::fullDomainHash(p, DIGESTBYTES);
        
        if( find(voted.begin(), voted.end(), pseudonym) != voted.end() ) {
            printf("pseudonym still voted!\n");
            return false;
        }
        if (v == v2) {
            printf("Vote: %s\n", pubCryptVote.c_str());
            cryptedVotes.push_back(pubCryptVote);
            voted.push_back(pseudonym);
            return true;
        } else {
            printf("signature verify failed!\n");
            return false;
        }
    }
    
    void endElection(vector<string> & _return, const string & privBase64Key) {
        StringSource sors(privBase64Key, true, new Base64Decoder);
        _privKey.BERDecode(sors);
        for (auto str : cryptedVotes) {
            Integer cv = Tools::convertBase64(str);
            string v = Tools::deCrypt(cv, _privKey);
            printf("%s\n", v.c_str());
            _return.push_back(v);
        }
    }
};

int main(int argc, char **argv) {
    int port = BALLOTBOXPORT;
    RSA::PublicKey pubKey;
    // read the public key file to verify signature
    Tools::DecodePublicKey(PUBFILE, pubKey);

    printf(
        "BallotBox on port %d with public key '%s'.\n",
        BALLOTBOXPORT,
        PUBFILE
    );
    
    ::apache::thrift::stdcxx::shared_ptr<BallotBoxHandler> handler(new BallotBoxHandler(&pubKey));
    ::apache::thrift::stdcxx::shared_ptr<TProcessor> processor(new BallotBoxProcessor(handler));
    ::apache::thrift::stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    ::apache::thrift::stdcxx::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    ::apache::thrift::stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    return 0;
}

