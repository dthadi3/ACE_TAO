// $Id:
#ifndef BB_PROXY_H
#define BB_PROXY_H

#include "BandwidthBroker/BandwidthBrokerC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"

#include <list>
#include <string>

using namespace mil::darpa::arms::mlrm;
using namespace mil::darpa::arms::mlrm::BandwidthBroker;

class FlowRequest
{
  public:
    explicit FlowRequest (AdmissionControl_ptr bb_ref,
                          const AdmissionControl::FlowInfo &f,
                          CommonDef::QOSRequired qos_req);
    ~FlowRequest () throw ();
    AdmissionControl::AdmissionControlResult send_request ();
    int commit ();
    int rollback ();
    long get_dscp () const;

  private:
    AdmissionControl_var BB_ref_;
    AdmissionControl::FlowInfo_var flowinfo_;
    CommonDef::QOSRequired qos_req_;
    AdmissionControl::SchedulingAction sched_action_;
    CORBA::String_var flowtoken_;
    CORBA::Long dscp_;
    AdmissionControl::AdmissionControlResult adm_ctrl_result_;
};

class BB_Proxy
{
  public:

    typedef std::list <FlowRequest *> CommandList;

    BB_Proxy ();
    ~BB_Proxy () throw ();
    bool resolve (CORBA::ORB_ptr orb);
    int flow_request (const AdmissionControl::FlowInfo &f, CommonDef::QOSRequired qos_req, long &dscp);
    int commit ();
    int rollback ();
    static void del (const FlowRequest *) throw ();

  protected:
    void populate_name (CosNaming::Name &name);

  private:
    bool resolved_;
    std::string BB_iorfile_;
    std::string BB_nameserv_context_;
    AdmissionControl_var BB_ref_;
    CommandList BB_commands_;
};




#endif // BB_PROXY_H
