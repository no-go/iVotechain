#include <cmath>
#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>

#include <cstdio>
#include <string>
#include "src/Tools.hpp"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "src/gen-cpp/RegOffice.h"
#include "src/gen-cpp/BallotBox.h"


#define DIGESTBYTES   512
#define PUBFILE       "/tmp/keyfile.pub"
#define ROFFICEHOST   "localhost"
#define ROFFICEPORT   9090
#define BALLOTBOXHOST "localhost"
#define BALLOTBOXPORT 9091

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace std;
using namespace CryptoPP;


int main(int argc, char* argv[]) {
    
    if (argc < 5) {
        printf("usage: %s [id] [name] [pseudonym] [vote]\n", argv[0]);
        return 1;
    }
    
    AutoSeededRandomPool rng;
    int32_t id         = atoi(argv[1]);
    string identName   = argv[2];
    string pseudonym   = argv[3];
    string voting      = argv[4];
    printf("CardId:      %d\n", id);
    printf("identName:   %s\n", identName.c_str());
    printf("pseudonym:   %s\n", pseudonym.c_str());
    printf("Vote:        %s\n", voting.c_str());
    
    // a ugly kind of nounce, but pseudonym is realy a kind of anonymous :o)
    voting += (string)"|" + pseudonym;
    
    Ident ident;
    ident.id = id;
    ident.name = identName;
    
    RSA::PublicKey pubKey;
    // do not make this mistake: decode is not decrypt ;o)
    // this function just load the pubkey file into the pubkey variable 
    Tools::DecodePublicKey(PUBFILE, pubKey);

    const Integer& n = pubKey.GetModulus();
    const Integer& e = pubKey.GetPublicExponent();

    SecByteBlock p((const byte*) pseudonym.data(), pseudonym.size());

    // H(p) as Integer
    Integer hp = Tools::fullDomainHash(p, DIGESTBYTES);

    // build a blinding symetric cryptation
    // n,e are coming from registration Office pub key!
    Integer r;
    do {
        // Attention! How random is this?
        r.Randomize(rng, Integer::One(), n - Integer::One());
    } while (!RelativelyPrime(r, n));

    // Blinding factor
    Integer b = a_exp_b_mod_c(r, e, n);

    // blinding the message
    Integer blinded = a_times_b_mod_c(hp, b, n);

    // Start RPC (thrift) stuff, to communicate with registation office
    shared_ptr<TTransport> socket1(new TSocket(ROFFICEHOST, ROFFICEPORT));
    shared_ptr<TTransport> transport1(new TBufferedTransport(socket1));
    shared_ptr<TProtocol> protocol1(new TBinaryProtocol(transport1));
    RegOfficeClient regOffice(protocol1);
    transport1->open();
    string back;
    // a synchrone RPC request
    regOffice.sign(back, ident, Tools::convertBase64(blinded));
    transport1->close();
    if (back == (string) "fail") {
        printf("Voter %s with %d is not (again) allowed!\n", ident.name.c_str(), ident.id);
        return 1;
    }
    Integer ss = Tools::convertBase64(back);

    // check the signature coming from the registration office: pub(priv(blinded)) = blinded
    Integer check = pubKey.ApplyFunction(ss);
    if (check != blinded) {
        printf("voter cross-check failed!\n");
        return 1;
    }
    
    // un-blind the signature
    Integer s = a_times_b_mod_c(ss, r.InverseMod(n), n);
    printf(
        "unblinded signature from registration office:\n"
        "%s\n", Tools::convertBase64(s).c_str()
    );

    // RPC to BallotBox has to verify pseudonym and gets the pub crypted voting
    shared_ptr<TTransport> socket2(new TSocket(BALLOTBOXHOST, BALLOTBOXPORT));
    shared_ptr<TTransport> transport2(new TBufferedTransport(socket2));
    shared_ptr<TProtocol> protocol2(new TBinaryProtocol(transport2));
    BallotBoxClient ballotBox(protocol2);

    transport2->open();
    // a synchrone RPC request
    bool ok = ballotBox.vote(
        pseudonym,
        Tools::convertBase64(s),
        Tools::convertBase64( Tools::enCrypt(voting, pubKey) )
    );
    transport2->close();

    if (ok == true) {
        printf("Voted.\n");
    } else {
        printf("Vote verified failed in ballot box!\n");
    }

    return 0;
}
