#ifndef _TOOLS_HPP
#define _TOOLS_HPP 1

#include <cmath>
#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/files.h>
#include <cryptopp/base64.h>

using namespace std;
using namespace CryptoPP;

namespace Tools {

    Integer fullDomainHash(const SecByteBlock inData, int digestByteWish) {
        digestByteWish = (digestByteWish/4) *3;
        
        //sha 256 bit = 64bytes blocksize, but only 32bytes digestsize
        SHA256 hash;
        SecByteBlock outData(NULL, digestByteWish);
        int digestLength = hash.DigestSize()*8; // 32*8 = 128bit
        int targetLength = digestByteWish*8; // bits
        SecByteBlock tmp(digestLength/8); // 32byte array
        
        int cycles = ceil((double)targetLength/(double)digestLength);
        
        // priv/pub keys with more than 32*256 are not realistic, but we use a 2nd byte to make such a big hash
        for (int i=0; i<cycles; ++i) {
            SecByteBlock dummy(inData.data(), inData.size()+2);
            dummy[dummy.size()-2] = (byte) (i>>8);
            dummy[dummy.size()-1] = (byte) i;

            hash.Update((const byte*)dummy.data(), dummy.size()); // the regular hash but with added block number to the data 
            hash.TruncatedFinal(tmp, tmp.size()); // creates and cuts the relevant 32bytes
            
            outData += tmp; // this concates the bytes
        }
        Integer dummy(outData.data(), outData.size());
        return dummy;
    }

    /**
     * base64 makes the CryptoPP::Integer transfer easier
     * 
     * @param val CryptoPP::Integer to be converted
     * @return the base64 result as string
     */
    string convertBase64(const Integer& val) {
        string back;
        Base64Encoder encoder;
        SecByteBlock buff;
        
        buff.resize(val.MinEncodedSize());
        val.Encode(&buff[0], buff.size());
        encoder.Put((const byte*) buff.data(), buff.size());
        encoder.MessageEnd();
        back.resize(encoder.MaxRetrievable());
        encoder.Get((byte*)&back[0], back.size());
        return back;
    }

    /**
     * convert base64 back to CryptoPP::Integer
     * 
     * @param val base64 version of a CryptoPP::Integer
     * @return the Integer result
     */
    Integer convertBase64(const string& val) {
        StringSource ssource(val, true, new Base64Decoder);
        Integer back(ssource, ssource.MaxRetrievable());
        return back;
    }
    
    void Decode(const string & filename, BufferedTransformation & bt) {
        FileSource file(filename.c_str(), true);
        file.TransferTo(bt);
        bt.MessageEnd();
    }
    
    void Encode(const string& filename, const BufferedTransformation& bt) {
        FileSink file(filename.c_str());
        bt.CopyTo(file);
        file.MessageEnd();
    }

    // load key
    void DecodePrivateKey(const string & filename, RSA::PrivateKey & key) {
        ByteQueue queue;
        Decode(filename, queue);
        key.BERDecodePrivateKey(queue, false, queue.MaxRetrievable());
    }
    
    // load key
    void DecodePublicKey(const string & filename, RSA::PublicKey & key) {
        ByteQueue queue;
        Decode(filename, queue);
        key.BERDecodePublicKey(queue, false, queue.MaxRetrievable());
    }

    // save key
    void EncodePrivateKey(const string& filename, const RSA::PrivateKey& key) {
        ByteQueue queue;
        key.DEREncodePrivateKey(queue);
        Encode(filename, queue);
    }

    // save key
    void EncodePublicKey(const string& filename, const RSA::PublicKey& key) {
        ByteQueue queue;
        key.DEREncodePublicKey(queue);
        Encode(filename, queue);
    }
    
    Integer enCrypt(const string & message, const RSA::PublicKey & pubKey) {
        SecByteBlock m((const byte*) message.data(), message.size());
        Integer mm(m.data(), m.size());
        return pubKey.ApplyFunction(mm);
    }
    
    string deCrypt(const Integer & cryptedMessage, const RSA::PrivateKey & privKey) {
        string recovered;
        AutoSeededRandomPool rng;
        Integer rev = privKey.CalculateInverse(rng, cryptedMessage);
        recovered.resize(rev.MinEncodedSize());
        rev.Encode((byte *) recovered.data(), recovered.size());
        return recovered;
    }
}

#endif
