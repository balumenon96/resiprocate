#if !defined(FlowManager_hxx)
#define FlowManager_hxx

#include "MediaStream.hxx"
#include "FlowManagerException.hxx"
#include "dtls_wrapper/DtlsFactory.hxx"

#include <openssl/crypto.h>
#include <openssl/ssl.h>

#include <map>

using namespace reTurn;

namespace flowmanager
{

/**
  This class represents the Flow Manager.  It is responsible for sending/receiving
  media and performing the necessary NAT traversal.  
  
  Threading Notes:  This class implements a thread
  to manage the asyncrouns reTurn client library calls.  Essentially all
  asyncrounous operations for all Flows will be called from this one thread.

  Author: Scott Godin (sgodin AT SipSpectrum DOT com)
*/
class IOServiceThread;

class FlowManager
{
public:  
   FlowManager();  // throws FlowManagerException
   virtual ~FlowManager();

   // This API assumes that RTCP localBinding is always the same as RTP binding but add one to the port number
   // We can add a new API in the future to accomodate, custom RTCP bindings as required
   MediaStream* createMediaStream(MediaStreamHandler& mediaStreamHandler,
                                  const StunTuple& localBinding, 
                                  bool rtcpEnabled = true,
                                  MediaStream::NatTraversalMode natTraversalMode = MediaStream::NoNatTraversal,
                                  const char* natTraversalServerHostname = 0, 
                                  unsigned short natTraversalServerPort = 0, 
                                  const char* stunUsername = 0,
                                  const char* stunPassword = 0);

   void initializeDtlsFactory(const char* certAor);
   dtls::DtlsFactory* getDtlsFactory() { return mDtlsFactory; }

protected: 

private:
   static int createCert (const resip::Data& pAor, int expireDays, int keyLen, X509*& outCert, EVP_PKEY*& outKey );
   static void srtpEventHandler(srtp_event_data_t *data);

   // Member variables used to manager asio io service thread
   asio::io_service mIOService;
   IOServiceThread* mIOServiceThread;
   asio::io_service::work* mIOServiceWork;
   asio::ssl::context mSslContext;
   X509* mClientCert;
   EVP_PKEY* mClientKey;
   dtls::DtlsFactory* mDtlsFactory;
};

}

#endif


/* ====================================================================

 Original contribution Copyright (C) 2008 Plantronics, Inc.
 Provided under the terms of the Vovida Software License, Version 2.0.

 The Vovida Software License, Version 2.0 
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution. 
 
 THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 DAMAGE.

 ==================================================================== */