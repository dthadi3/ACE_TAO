// $Id$

#include "tao/tao_internals.h"

#if !defined(__ACE_INLINE__)
#  include "tao_internals.i"
#endif /* __ACE_INLINE__ */

#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "tao/default_server.h"
#include "tao/default_client.h"

int TAO_Internal::service_open_count_ = 0;

int
TAO_Internal::fake_service_entries_i (void)
{
#if defined(TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
#define FAKE_SVC_ENTRY(svcname, svctype, argc, argv) \
  do \
    { \
      ACE_Service_Object *obj = _make_##svctype (); \
      obj->init (argc, argv); \
      ACE_Service_Repository::instance()->insert \
        (new ACE_Service_Type (svcname,\
                                 new ACE_Service_Object_Type\
                                 (obj, svcname, \
                                  (ACE_Service_Type::DELETE_OBJ \
                                   | ACE_Service_Type::DELETE_THIS)), \
                                  0, 1));\
    }\
  while (0) //;

  char *rfactory_args[] = { "-ORBresources", "global" };
  FAKE_SVC_ENTRY ("Resource_Factory",
                  TAO_Resource_Factory,
                  0,
                  rfactory_args);
  
  char *client_args[] = { 0 };
  FAKE_SVC_ENTRY ("Client_Strategy_Factory",
                  TAO_Default_Client_Strategy_Factory,
                  0,
                  client_args);
  
  char* server_args[] = {
    "-ORBconcurrency", "reactive",
    "-ORBdemuxstrategy", "dynamic", "-ORBtablesize", "128" };
  FAKE_SVC_ENTRY ("Server_Strategy_Factory",
                  TAO_Default_Server_Strategy_Factory,
                  sizeof server_args / sizeof server_args[0],
                  server_args);
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

  return 0;
}
