// Copyright (c) 2010
// All rights reserved.

#include "gtest/gtest.h"
#include "service/Log.hh"
#include "service/TraderServiceImpl.hh"

namespace flyer {

class TraderServiceImplTest : public ::testing::Test {
 public:
  TraderServiceImplTest() {
  }

  void SetUp() {
    options_.reset(TraderService::createOptions());
    std::unique_ptr<soil::Config> config(
        soil::Config::create());
    config->configFile() = "trader.cfg";
    config->registerOptions(options_.get());
    config->loadConfig();

    FLYER_LOG_INIT("log.cfg");

    cond_.reset(soil::STimer::create());

    service_.reset(
        TraderService::createService(
            options_.get(),
            nullptr));
  }

  void TearDown() {
  }

 protected:
  std::unique_ptr<TraderService> service_;
  std::unique_ptr<soil::Options> options_;
  std::unique_ptr<soil::STimer> cond_;
};

TEST_F(TraderServiceImplTest, loginTest) {
  SUCCEED();
}

TEST_F(TraderServiceImplTest, orderOpenBuyTest) {
  std::string instru = "IF1510";
  double price = 2889;
  int volume = 1;

  service_->orderOpenBuy(instru, price, volume);

  cond_->wait(2000);

  SUCCEED();
}

TEST_F(TraderServiceImplTest, queryAccountTest) {
  service_->queryAccount();

  cond_->wait(2000);

  SUCCEED();
}

}  // namespace flyer
