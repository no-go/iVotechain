/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef BallotBox_H
#define BallotBox_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "SimpleEvote_types.h"



#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class BallotBoxIf {
 public:
  virtual ~BallotBoxIf() {}
  virtual bool vote(const std::string& pseudonym, const std::string& signature, const std::string& pubCryptVote) = 0;
  virtual void endElection(std::vector<std::string> & _return, const std::string& privBase64Key) = 0;
};

class BallotBoxIfFactory {
 public:
  typedef BallotBoxIf Handler;

  virtual ~BallotBoxIfFactory() {}

  virtual BallotBoxIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(BallotBoxIf* /* handler */) = 0;
};

class BallotBoxIfSingletonFactory : virtual public BallotBoxIfFactory {
 public:
  BallotBoxIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<BallotBoxIf>& iface) : iface_(iface) {}
  virtual ~BallotBoxIfSingletonFactory() {}

  virtual BallotBoxIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(BallotBoxIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<BallotBoxIf> iface_;
};

class BallotBoxNull : virtual public BallotBoxIf {
 public:
  virtual ~BallotBoxNull() {}
  bool vote(const std::string& /* pseudonym */, const std::string& /* signature */, const std::string& /* pubCryptVote */) {
    bool _return = false;
    return _return;
  }
  void endElection(std::vector<std::string> & /* _return */, const std::string& /* privBase64Key */) {
    return;
  }
};

typedef struct _BallotBox_vote_args__isset {
  _BallotBox_vote_args__isset() : pseudonym(false), signature(false), pubCryptVote(false) {}
  bool pseudonym :1;
  bool signature :1;
  bool pubCryptVote :1;
} _BallotBox_vote_args__isset;

class BallotBox_vote_args {
 public:

  BallotBox_vote_args(const BallotBox_vote_args&);
  BallotBox_vote_args& operator=(const BallotBox_vote_args&);
  BallotBox_vote_args() : pseudonym(), signature(), pubCryptVote() {
  }

  virtual ~BallotBox_vote_args() throw();
  std::string pseudonym;
  std::string signature;
  std::string pubCryptVote;

  _BallotBox_vote_args__isset __isset;

  void __set_pseudonym(const std::string& val);

  void __set_signature(const std::string& val);

  void __set_pubCryptVote(const std::string& val);

  bool operator == (const BallotBox_vote_args & rhs) const
  {
    if (!(pseudonym == rhs.pseudonym))
      return false;
    if (!(signature == rhs.signature))
      return false;
    if (!(pubCryptVote == rhs.pubCryptVote))
      return false;
    return true;
  }
  bool operator != (const BallotBox_vote_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const BallotBox_vote_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class BallotBox_vote_pargs {
 public:


  virtual ~BallotBox_vote_pargs() throw();
  const std::string* pseudonym;
  const std::string* signature;
  const std::string* pubCryptVote;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _BallotBox_vote_result__isset {
  _BallotBox_vote_result__isset() : success(false) {}
  bool success :1;
} _BallotBox_vote_result__isset;

class BallotBox_vote_result {
 public:

  BallotBox_vote_result(const BallotBox_vote_result&);
  BallotBox_vote_result& operator=(const BallotBox_vote_result&);
  BallotBox_vote_result() : success(0) {
  }

  virtual ~BallotBox_vote_result() throw();
  bool success;

  _BallotBox_vote_result__isset __isset;

  void __set_success(const bool val);

  bool operator == (const BallotBox_vote_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const BallotBox_vote_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const BallotBox_vote_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _BallotBox_vote_presult__isset {
  _BallotBox_vote_presult__isset() : success(false) {}
  bool success :1;
} _BallotBox_vote_presult__isset;

class BallotBox_vote_presult {
 public:


  virtual ~BallotBox_vote_presult() throw();
  bool* success;

  _BallotBox_vote_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _BallotBox_endElection_args__isset {
  _BallotBox_endElection_args__isset() : privBase64Key(false) {}
  bool privBase64Key :1;
} _BallotBox_endElection_args__isset;

class BallotBox_endElection_args {
 public:

  BallotBox_endElection_args(const BallotBox_endElection_args&);
  BallotBox_endElection_args& operator=(const BallotBox_endElection_args&);
  BallotBox_endElection_args() : privBase64Key() {
  }

  virtual ~BallotBox_endElection_args() throw();
  std::string privBase64Key;

  _BallotBox_endElection_args__isset __isset;

  void __set_privBase64Key(const std::string& val);

  bool operator == (const BallotBox_endElection_args & rhs) const
  {
    if (!(privBase64Key == rhs.privBase64Key))
      return false;
    return true;
  }
  bool operator != (const BallotBox_endElection_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const BallotBox_endElection_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class BallotBox_endElection_pargs {
 public:


  virtual ~BallotBox_endElection_pargs() throw();
  const std::string* privBase64Key;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _BallotBox_endElection_result__isset {
  _BallotBox_endElection_result__isset() : success(false) {}
  bool success :1;
} _BallotBox_endElection_result__isset;

class BallotBox_endElection_result {
 public:

  BallotBox_endElection_result(const BallotBox_endElection_result&);
  BallotBox_endElection_result& operator=(const BallotBox_endElection_result&);
  BallotBox_endElection_result() {
  }

  virtual ~BallotBox_endElection_result() throw();
  std::vector<std::string>  success;

  _BallotBox_endElection_result__isset __isset;

  void __set_success(const std::vector<std::string> & val);

  bool operator == (const BallotBox_endElection_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const BallotBox_endElection_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const BallotBox_endElection_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _BallotBox_endElection_presult__isset {
  _BallotBox_endElection_presult__isset() : success(false) {}
  bool success :1;
} _BallotBox_endElection_presult__isset;

class BallotBox_endElection_presult {
 public:


  virtual ~BallotBox_endElection_presult() throw();
  std::vector<std::string> * success;

  _BallotBox_endElection_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class BallotBoxClient : virtual public BallotBoxIf {
 public:
  BallotBoxClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  BallotBoxClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  bool vote(const std::string& pseudonym, const std::string& signature, const std::string& pubCryptVote);
  void send_vote(const std::string& pseudonym, const std::string& signature, const std::string& pubCryptVote);
  bool recv_vote();
  void endElection(std::vector<std::string> & _return, const std::string& privBase64Key);
  void send_endElection(const std::string& privBase64Key);
  void recv_endElection(std::vector<std::string> & _return);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class BallotBoxProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<BallotBoxIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (BallotBoxProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_vote(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_endElection(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  BallotBoxProcessor(::apache::thrift::stdcxx::shared_ptr<BallotBoxIf> iface) :
    iface_(iface) {
    processMap_["vote"] = &BallotBoxProcessor::process_vote;
    processMap_["endElection"] = &BallotBoxProcessor::process_endElection;
  }

  virtual ~BallotBoxProcessor() {}
};

class BallotBoxProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  BallotBoxProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< BallotBoxIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< BallotBoxIfFactory > handlerFactory_;
};

class BallotBoxMultiface : virtual public BallotBoxIf {
 public:
  BallotBoxMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<BallotBoxIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~BallotBoxMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<BallotBoxIf> > ifaces_;
  BallotBoxMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<BallotBoxIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  bool vote(const std::string& pseudonym, const std::string& signature, const std::string& pubCryptVote) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->vote(pseudonym, signature, pubCryptVote);
    }
    return ifaces_[i]->vote(pseudonym, signature, pubCryptVote);
  }

  void endElection(std::vector<std::string> & _return, const std::string& privBase64Key) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->endElection(_return, privBase64Key);
    }
    ifaces_[i]->endElection(_return, privBase64Key);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class BallotBoxConcurrentClient : virtual public BallotBoxIf {
 public:
  BallotBoxConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  BallotBoxConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  bool vote(const std::string& pseudonym, const std::string& signature, const std::string& pubCryptVote);
  int32_t send_vote(const std::string& pseudonym, const std::string& signature, const std::string& pubCryptVote);
  bool recv_vote(const int32_t seqid);
  void endElection(std::vector<std::string> & _return, const std::string& privBase64Key);
  int32_t send_endElection(const std::string& privBase64Key);
  void recv_endElection(std::vector<std::string> & _return, const int32_t seqid);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif



#endif
