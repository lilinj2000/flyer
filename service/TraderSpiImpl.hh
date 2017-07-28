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

  virtual void OnFrontDisconnected(char *pErrMsg);

  virtual void OnRspUserLogin(
      CSgitFtdcRspUserLoginField *pRspUserLogin,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspUserLogout(
      CSgitFtdcUserLogoutField *pUserLogout,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspUserPasswordUpdate(
      CSgitFtdcUserPasswordUpdateField *pUserPasswordUpdate,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspOrderInsert(
      CSgitFtdcInputOrderField *pInputOrder,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspOrderAction(
      CSgitFtdcInputOrderActionField *pInputOrderAction,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspQryOrder(
      CSgitFtdcOrderField *pOrder,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspQryTradingAccount(
      CSgitFtdcTradingAccountField *pTradingAccount,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspQryInvestor(
      CSgitFtdcInvestorField *pInvestor,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspQryInstrument(
      CSgitFtdcInstrumentField *pInstrument,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRtnOrder(
      CSgitFtdcOrderField *pOrder,
      CSgitFtdcRspInfoField *pRspInfo);

  virtual void OnRtnTrade(CSgitFtdcTradeField *pTrade);

  virtual void OnRtnInstrumentStatus(
      CSgitFtdcInstrumentStatusField *pInstrumentStatus);

  virtual void OnRspQryInvestorPositionDetail(
      CSgitFtdcInvestorPositionDetailField *pInvestorPositionDetail,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void OnRspQryInvestorPosition(
      CSgitFtdcInvestorPositionField *pInvestorPosition,
      CSgitFtdcRspInfoField *pRspInfo,
      int nRequestID, bool bIsLast);

  virtual void onRspMBLQuot(
      CSgitMBLQuotData *pMBLQuotData,
      CSgitFtdcRspInfoField *pRspMsg,
      int nRequestID, bool bIsLast);

 protected:
    void checkRspInfo(CSgitFtdcRspInfoField *pRspInfo);

 private:
  TraderServiceImpl* service_;
};

}  // namespace flyer

#endif  // FLYER_TRADER_SPI_IMPL_HH
