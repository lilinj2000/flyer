// Copyright (c) 2010
// All rights reserved.

#include "TraderSpiImpl.hh"
#include "TraderServiceImpl.hh"
#include "TraderOptions.hh"
#include "Log.hh"

#include "SgitFtdcUserApiStructPrint.hh"

namespace flyer {

TraderSpiImpl::TraderSpiImpl(TraderServiceImpl* service):
    service_(service) {
  FLYER_TRACE <<"TraderSpiImpl::TraderSpiImpl()";
}

TraderSpiImpl::~TraderSpiImpl() {
  FLYER_TRACE <<"TraderSpiImpl::~TraderSpiImpl()";
}

/////////////////////////////////////////
// impl from CSgitFtdcSpi
/////////////////////////////////////////
void TraderSpiImpl::OnFrontConnected() {
  FLYER_TRACE <<"TraderSpiImpl::OnFrontConnected()";

  service_->login();
}

void TraderSpiImpl::OnFrontDisconnected(int nReason) {
  FLYER_TRACE <<"TraderSpiImpl::OnFrontDisconnected()";

  FLYER_INFO <<"OnFrontDisconnected, the Reason is " <<std::hex <<nReason;
}

void TraderSpiImpl::OnRspUserLogin(
    CSgitFtdcRspUserLoginField *pRspUserLogin,
    CSgitFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast) {
  FLYER_TRACE <<"TraderSpiImpl::OnRspUserLogin()";

  try {
    checkRspInfo(pRspInfo);

    FLYER_PDU <<*pRspUserLogin;

    service_->initSession(pRspUserLogin);

    if (bIsLast) {
      service_->ready();

      service_->notify();
    }
  } catch (std::exception& e) {
    FLYER_ERROR <<"exception cached: " <<e.what();
  }
}

void TraderSpiImpl::OnRspQryTradingAccount(
    CSgitFtdcTradingAccountField *pTradingAccount,
    CSgitFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast) {
  FLYER_TRACE <<"TraderSpiImpl::OnRspQryTradingAccount()";

  try {
    checkRspInfo(pRspInfo);

    FLYER_PDU <<*pTradingAccount;
  } catch (...) {
  }
}

void TraderSpiImpl::OnRspError(
    CSgitFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast) {
  FLYER_TRACE <<"TraderSpiImpl::OnRspError()";

  if (pRspInfo) {
    FLYER_PDU <<*pRspInfo;
  }
}

void TraderSpiImpl::OnRspOrderInsert(
    CSgitFtdcInputOrderField *pInputOrder,
    CSgitFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast) {
  FLYER_TRACE <<"TraderSpiImpl::OnRspOrderInsert()";

  try {
    checkRspInfo(pRspInfo);
  } catch (std::exception& e) {
    FLYER_ERROR <<e.what();
  }

  if (pInputOrder && service_->callback()) {
    FLYER_PDU <<*pInputOrder;

    int order_ref = std::stoi(pInputOrder->OrderRef);

    service_->callback()->onRspOrderInsert(order_ref);
  }
}

void TraderSpiImpl::OnRtnOrder(
    CSgitFtdcOrderField *pOrder,
    CSgitFtdcRspInfoField *pRspInfo) {
  FLYER_TRACE <<"TraderSpiImpl::OnRtnOrder()";

  int error_id = -1;
  std::string err_msg;
  if (pRspInfo) {
    FLYER_PDU <<*pRspInfo;
    error_id = pRspInfo->ErrorID;
    err_msg = pRspInfo->ErrorMsg;
  }

  if (pOrder) {
    FLYER_PDU <<*pOrder;
  }

  if (service_->callback()) {
    int order_ref = std::stoi(pOrder->OrderRef);

    service_->callback()->onRtnOrder(order_ref, error_id, err_msg);
  }
}

void TraderSpiImpl::OnRtnTrade(
    CSgitFtdcTradeField *pTrade) {
  FLYER_TRACE <<"TraderSpiImpl::OnRtnTrade()";

  if (pTrade) {
    FLYER_PDU <<*pTrade;
  }

  if (service_->callback()) {
    int order_ref = std::stoi(pTrade->OrderRef);

    double price = pTrade->Price;
    double volume = pTrade->Volume;

    service_->callback()->onRtnTrade(order_ref, price, volume);
  }
}

void TraderSpiImpl::OnErrRtnOrderInsert(
    CSgitFtdcInputOrderField *pInputOrder,
    CSgitFtdcRspInfoField *pRspInfo) {
  FLYER_TRACE <<"TraderSpiImpl::OneErrRtnOrderInsert()";

  try {
    checkRspInfo(pRspInfo);

    FLYER_PDU <<*pInputOrder;
  } catch (...) {
  }
}

void TraderSpiImpl::checkRspInfo(
    CSgitFtdcRspInfoField *pRspInfo) {
  FLYER_TRACE <<"TraderSpiImpl::checkRspInfo()";

  if (pRspInfo) {
    FLYER_PDU <<*pRspInfo;
  }

  bool result = ((pRspInfo) && (pRspInfo->ErrorID != 0));

  if (result) {
    std::stringstream err_stream;
    err_stream <<"ErrorID=" <<pRspInfo->ErrorID <<","
               <<"ErrorMsg=" <<pRspInfo->ErrorMsg;

    throw std::runtime_error(err_stream.str());
  }
}

}  // namespace flyer
