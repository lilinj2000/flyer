#ifndef FLYER_TRADER_OPTIONS_HH
#define FLYER_TRADER_OPTIONS_HH

#include "soil/Config.hh"

#include <string>

namespace flyer {

namespace po = boost::program_options;

class TraderOptions : public soil::Options {
 public:
  TraderOptions();
  virtual ~TraderOptions();

  virtual po::options_description* configOptions();

  std::string flow_path;
  std::string front_address;
  
  std::string broker_id;
  std::string user_id;
  std::string password;

  std::string investor_id;

  bool is_auth_force;
  std::string user_product_info;
  std::string auth_code;

 private:
  boost::program_options::options_description options_;
};

}  


#endif 
