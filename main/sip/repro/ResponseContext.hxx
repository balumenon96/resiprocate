#if !defined(RESIP_RESPONSE_CONTEXT_HXX)
#define RESIP_RESPONSE_CONTEXT_HXX 

#include "resiprocate/os/HashMap.hxx"
#include "resiprocate/NameAddr.hxx"

namespace resip
{
class SipMessage;
}

namespace repro
{

class RequestContext;

class ResponseContext
{
   public:
      ResponseContext(RequestContext& parent);

      /// Called by RequestContext after every event other than a response
      void processEvent();
      void processCandidates();
      void processPendingTargets();
      
      /// Called by RequestContext after every response is received
      void processResponse(const resip::SipMessage& response);
      
   private:
      RequestContext& mRequestContext;
      
      class CompareStatus  : public std::binary_function<const resip::SipMessage&, const resip::SipMessage&, bool>  
      {
         public:
            bool operator()(const resip::SipMessage& lhs, const resip::SipMessage& rhs) const;
      };      
      
      class CompareQ  : public std::binary_function<const resip::NameAddr&, const resip::NameAddr&, bool>
      {
         public:
            bool operator()(const resip::NameAddr& lhs, const resip::NameAddr& rhs) const;
      };      
      
      typedef std::set<resip::NameAddr, CompareQ> PendingTargetSet;
      PendingTargetSet mPendingTargetSet;

      HashSet<resip::NameAddr> mTargetSet;
      
      typedef enum
      {
         PendingTarget,
         Trying,
         Proceeding,
         WaitingToCancel
      } Status;

      struct Branch
      {
            Status status;
            resip::Uri uri;
      };
      typedef HashMap<resip::Data, Branch> TransactionMap;
      TransactionMap mClientTransactions;
};

}
#endif

/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */
