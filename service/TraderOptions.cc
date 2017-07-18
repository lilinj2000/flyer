// Copyright (c) 2010
// All rights reserved.

#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>


#include "TraderOptions.hh"

namespace flyer {

TraderOptions::TraderOptions():
    options_("FlyerTraderOptions"),
    is_logged(false),
    is_fastmode(true) {
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
       "investor id")
      ("flyer_trader.is_logged", po::value<bool>(&is_logged),
       "is Logged")
      ("flyer_trader.is_fastmode", po::value<bool>(&is_fastmode),
       "is FastMode");

  return;
}

TraderOptions::~TraderOptions() {
}

po::options_description* TraderOptions::configOptions() {
  return &options_;
}

}  // namespace flyer
