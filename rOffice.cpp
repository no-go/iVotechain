#include <cmath>
#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "src/Tools.hpp"
#include "src/gen-cpp/RegOffice.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#define DIGESTBYTES   512
#define ALLOWEDFILE   "allowed.txt"
#define PRIVATEFILE   "/tmp/keyfile.priv"
#define PUBFILE       "/tmp/keyfile.pub"
#define ROFFICEHOST   "localhost"
#define ROFFICEPORT   9090

using namespace std;
using namespace CryptoPP;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

vector<Ident> allowedVoter;

class RegOfficeHandler : virtual public RegOfficeIf {

private:
    RSA::PrivateKey * _privKey;
    RSA::PublicKey * _pubKey;
    
public:
    RegOfficeHandler() = default;
    
    RegOfficeHandler(RSA::PrivateKey * privKey, RSA::PublicKey * pubKey) {
        _privKey = privKey;
        _pubKey = pubKey;
    }
    
    void sign(string & _return, const Ident & ident, const string & blindedHashedPsydonyme) {
        int findid = -1;
        
        // search the ident/voter in allowed voter array
        for (unsigned int i=0; i < allowedVoter.size(); ++i) {
            if (allowedVoter[i].id == ident.id && allowedVoter[i].name == ident.name) {
                findid = i;
                break;
            }
        }
        if (findid >= 0 && allowedVoter[findid].used == false) {
            AutoSeededRandomPool rng;
            Integer mm = Tools::convertBase64(blindedHashedPsydonyme);
            Integer ss = _privKey->CalculateInverse(rng, mm);
            _return = Tools::convertBase64(ss);
            allowedVoter[findid].used = true; // we want only one pseudonym+signature per voter!
        } else {
            printf("%s is not allowed (anymore)!\n", ident.name.c_str());
            _return = "fail";
        }
    }
};

int main(int argc, char **argv) {
    int port = ROFFICEPORT;
    AutoSeededRandomPool prng;
    RSA::PrivateKey privKey;

    printf(
        "Registration Office generating private and public RSA keys with %d bits.\n"
        "%s\n"
        "%s\n",
        DIGESTBYTES * 8,
        PRIVATEFILE,
        PUBFILE
    );
    
    // generate and store keys
    privKey.GenerateRandomWithKeySize(prng, DIGESTBYTES * 8); // bits
    RSA::PublicKey pubKey(privKey);
    Tools::EncodePrivateKey(PRIVATEFILE, privKey);
    Tools::EncodePublicKey(PUBFILE, pubKey);
    
    // read file with allowed voters
    string line;
    ifstream infile(ALLOWEDFILE);
    while (getline(infile, line)) {
        if (line.size() < 3) continue; // ignore "empty" lines
        istringstream iss(line);
        Ident ident;
        if (!(iss >> ident.id >> ident.name)) { 
            printf("Error processing %s !\n", ALLOWEDFILE);
            return 1;
        }
        allowedVoter.push_back(ident);
    }
    
    printf("Registration Office starting on port %d\n", port);

    ::apache::thrift::stdcxx::shared_ptr<RegOfficeHandler> handler(new RegOfficeHandler(&privKey, &pubKey));
    ::apache::thrift::stdcxx::shared_ptr<TProcessor> processor(new RegOfficeProcessor(handler));
    ::apache::thrift::stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    ::apache::thrift::stdcxx::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    ::apache::thrift::stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    return 0;
}

