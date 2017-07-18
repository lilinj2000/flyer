// Copyright (c) 2010
// All rights reserved.

#ifndef FLYER_TRADER_SERVICE_IMPL_HH
#define FLYER_TRADER_SERVICE_IMPL_HH

#include <string>
#include <memory>
#include <atomic>

#include "flyer/TraderService.hh"
#include "SgitFtdcTraderApi.h"

#include "soil/STimer.hh"

namespace flyer {

class TraderOptions;
class TraderSpiImpl;

class TraderServiceImpl : public TraderService {
 public:
  TraderServiceImpl(soil::Options* options, TraderServiceCallback* callback);
  virtual ~TraderServiceImpl();

  virtual std::string tradingDay();

  virtual int orderOpenBuy(
      const std::string& instru,
      double price, int volume);

  virtual int orderOpenBuyFAK(
      const std::string& instru,
      double price, int volume);

  virtual int orderOpenBuyFOK(
      const std::string& instru,
      double price, int volume);

  virtual int orderOpenSell(
      const std::string& instru,
      double price, int volume);

  virtual int orderOpenSellFAK(
      const std::string& instru,
      double price, int volume);

  virtual int orderOpenSellFOK(
      const std::string& instru,
      double price, int volume);

  virtual int orderCloseBuy(
      const std::string& instru,
      double price, int volume);

  virtual int orderCloseSell(
      const std::string& instru,
      double price, int volume);

  virtual int queryAccount();

  void ready();

  void login();

  void initSession(CSgitFtdcRspUserLoginField* pRspUserLogin);

  void wait(const std::string& hint = "");

  void notify();

  TraderServiceCallback* callback() {
    return callback_;
  }

  TraderOptions* options() {
    return options_;
  }

 protected:
  CSgitFtdcInputOrderField* orderField(
      int* order_ref);

  void orderGo(CSgitFtdcInputOrderField* req);

 private:
  TraderOptions* options_;
  CSgitFtdcTraderApi* trader_api_;
  std::unique_ptr<TraderSpiImpl> trader_spi_;
  TraderServiceCallback* callback_;
  std::atomic_int request_id_;
  std::unique_ptr<soil::STimer> cond_;
  int front_id_;
  int session_id_;
  std::atomic_int max_order_ref_;
};

}  // namespace flyer

#endif  // FLYER_TRADER_SERVICE_IMPL_HH
