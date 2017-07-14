#include "TraderOptions.hh"

#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>

namespace flyer {

TraderOptions::TraderOptions():
    options_("FlyerTraderOptions") {
  namespace po = boost::program_options;

  options_.add_options()
      ("flyer_trader.flow_path", po::value<std::string>(&flow_path), 
       "flow path")
      ("flyer_trader.front_address", po::value<std::string>(&front_address), 
       "front address")
      ("flyer_trader.broker_id", po::value<std::string>(&broker_id), 
       "broker id")
      ("flyer_trader.user_id", po::value<std::string>(&user_id), 
       "user id")
      ("flyer_trader.password", po::value<std::string>(&password), 
       "password")
      ("flyer_trader.investor_id", po::value<std::string>(&investor_id), 
       "investor_id")
      ("flyer_trader.is_auth_force", po::value<bool>(&is_auth_force),
       "is auth force")
      ("flyer_trader.user_product_info", po::value<std::string>(&user_product_info), 
       "user product info")
      ("flyer_trader.auth_code", po::value<std::string>(&auth_code), 
       "auth code")
      ;

  return;
}

TraderOptions::~TraderOptions() {
}

po::options_description* TraderOptions::configOptions() {
  return &options_;
}

};  
