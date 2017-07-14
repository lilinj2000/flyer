#include "gtest/gtest.h"

#include "service/ZeroLog.hh"
#include "service/TraderServiceImpl.hh"

namespace zero
{

class TraderServiceImplTest : public ::testing::Test
{
 public:
  TraderServiceImplTest()
  {
  }
  
  void SetUp()
  {
    options_.reset( TraderService::createOptions() );

    std::auto_ptr<soil::Config> config( soil::Config::create() );
    config->configFile() = "zero.cfg";
    config->registerOptions( options_.get() );
    config->loadConfig();

    ZERO_LOG_INIT("log.cfg");

    cond_.reset( soil::STimer::create() );

    service_.reset( TraderService::createService(options_.get(), NULL) );

  }

  void TearDown()
  {
  }

 protected:
  std::auto_ptr<TraderService> service_;
  
  std::auto_ptr<soil::Options> options_;
  
  std::auto_ptr<soil::STimer> cond_;

};

TEST_F(TraderServiceImplTest, loginTest)
{
  ASSERT_TRUE( true );
}

TEST_F(TraderServiceImplTest, orderOpenBuyTest)
{

  std::string instru = "IF1510";
  double price = 2889;
  int volume = 1;
  
  service_->orderOpenBuy(instru, price, volume);

  cond_->wait(2000);

  ASSERT_TRUE(true);
}

TEST_F(TraderServiceImplTest, orderOpenBuyFAKTest)
{

  std::string instru = "IF1510";
  double price = 3000;
  int volume = 1;
  
  service_->orderOpenBuyFAK(instru, price, volume);

  cond_->wait(2000);

  ASSERT_TRUE(true);
}

TEST_F(TraderServiceImplTest, orderOpenBuyFOKTest)
{

  std::string instru = "cu1603";
  double price = 32660;
  int volume = 10;
  
  service_->orderOpenBuyFOK(instru, price, volume);

  cond_->wait(2000);

  ASSERT_TRUE(true);
}

TEST_F(TraderServiceImplTest, orderOpenSellFOKTest)
{

  std::string instru = "cu1603";
  double price = 35330;
  int volume = 10;
  
  service_->orderOpenSellFOK(instru, price, volume);

  cond_->wait(2000);

  ASSERT_TRUE(true);
}

TEST_F(TraderServiceImplTest, queryAccountTest)
{

  service_->queryAccount();

  cond_->wait(2000);

  ASSERT_TRUE(true);
}

}; 
