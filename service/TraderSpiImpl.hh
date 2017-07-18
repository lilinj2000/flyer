// Copyright (c) 2010
// All rights reserved.

#ifndef FLYER_TRADER_SPI_IMPL_HH
#define FLYER_TRADER_SPI_IMPL_HH


#include "SgitFtdcTraderApi.h"

namespace flyer {

class TraderServiceImpl;

class TraderSpiImpl : public CSgitFtdcTraderSpi {
 public:
  explicit TraderSpiImpl(TraderServiceImpl* service);

  virtual ~TraderSpiImpl();

  // interface from CSgitFtdcTraderSpi
  virtual void OnFrontConnected();

  virtual void OnFrontDisconnected(int nReason);

  virtual void OnRspUserLogin(
      CSgitFtdcRspUserLoginField *pRspUserLogin,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspOrderInsert(
      CSgitFtdcInputOrderField *pInputOrder,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspQryTradingAccount(
      CSgitFtdcTradingAccountField *pTradingAccount,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspError(
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRtnOrder(CSgitFtdcOrderField *pOrder);

  virtual void OnRtnTrade(CSgitFtdcTradeField *pTrade);

  virtual void OnErrRtnOrderInsert(
      CSgitFtdcInputOrderField *pInputOrder,
      CSgitFtdcRspInfoField *pRspInfo);

 protected:
    void checkRspInfo(CSgitFtdcRspInfoField *pRspInfo);

 private:
  TraderServiceImpl* service_;
};

}  // namespace flyer

#endif  // FLYER_TRADER_SPI_IMPL_HH
