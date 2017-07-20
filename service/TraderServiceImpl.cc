// Copyright (c) 2010
// All rights reserved.

#include "TraderServiceImpl.hh"
#include "Log.hh"
#include "TraderOptions.hh"
#include "TraderSpiImpl.hh"
#include "soil/Macro.hh"
#include "SgitFtdcUserApiStructPrint.hh"

namespace flyer {

TraderServiceImpl::TraderServiceImpl(
    soil::Options* options,
    TraderServiceCallback* callback):
    trader_api_(nullptr),
    callback_(callback),
    request_id_(0),
    front_id_(-1),
    session_id_(-1),
    max_order_ref_(-1) {
  FLYER_TRACE <<"TraderServiceImpl::TraderServiceImpl()";

  cond_.reset(soil::STimer::create());

  options_ = dynamic_cast<TraderOptions*>(options);
  trader_api_ =
      CSgitFtdcTraderApi::CreateFtdcTraderApi(
          options_->flow_path.data());
  trader_spi_.reset(new TraderSpiImpl(this));
  trader_api_->RegisterSpi(trader_spi_.get());

  // SGIT_TERT_QUICK
  // SGIT_TERT_RESUME
  trader_api_->SubscribePrivateTopic(Sgit_TERT_QUICK);
  trader_api_->SubscribePublicTopic(Sgit_TERT_QUICK);
  trader_api_->RegisterFront(const_cast<char*>(options_->front_address.data()));

  trader_api_->Init(options_->is_logged,
                    options_->is_fastmode);

  wait("login");

  wait();  // for ready, just sleep 2 second
}

TraderServiceImpl::~TraderServiceImpl() {
  FLYER_TRACE <<"TraderServiceImpl::~TraderServiceImpl()";

  trader_api_->RegisterSpi(nullptr);
  trader_api_->Release();
  trader_api_ = nullptr;
}

std::string TraderServiceImpl::tradingDay() {
  FLYER_TRACE <<"TraderServiceImpl::tradingDate()";

  return trader_api_->GetTradingDay();
}

int TraderServiceImpl::orderOpenBuy(
    const std::string& instru,
    double price, int volume) {
  FLYER_TRACE <<"TraderServiceImpl::orderOpenBuy()";

  FLYER_DEBUG <<"instru: " <<instru
            <<"\t price: " <<price
            <<"\t volume: " <<volume;

  int order_ref = -1;

  std::unique_ptr<CSgitFtdcInputOrderField> req(
      orderField(&order_ref));

  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          InstrumentID,
          instru.data());
  req->Direction = Sgit_FTDC_D_Buy;
  req->LimitPrice = price;
  req->VolumeTotalOriginal = volume;

  try {
    orderGo(req.get());
  } catch (...) {
    throw std::runtime_error("order open buy failed.");
  }

  return order_ref;
}

int TraderServiceImpl::orderOpenBuyFAK(
    const std::string& instru,
    double price, int volume) {
  FLYER_TRACE <<"TraderServiceImpl::orderOpenBuyFAK()";

  FLYER_DEBUG <<"instru: " <<instru
            <<"\t price: " <<price
            <<"\t volume: " <<volume;

  int order_ref = -1;

  std::unique_ptr<CSgitFtdcInputOrderField> req(
      orderField(&order_ref));

  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          InstrumentID,
          instru.data());
  req->Direction = Sgit_FTDC_D_Buy;
  req->LimitPrice = price;
  req->VolumeTotalOriginal = volume;

  req->TimeCondition = Sgit_FTDC_TC_IOC;

  try {
    orderGo(req.get());
  } catch (...) {
    throw std::runtime_error("order open buy FAK failed.");
  }

  return order_ref;
}

int TraderServiceImpl::orderOpenBuyFOK(
    const std::string& instru,
    double price, int volume) {
  FLYER_TRACE <<"TraderServiceImpl::orderOpenBuyFOK()";

  FLYER_DEBUG <<"instru: " <<instru
            <<"\t price: " <<price
            <<"\t volume: " <<volume;

  int order_ref = -1;

  std::unique_ptr<CSgitFtdcInputOrderField> req(
      orderField(&order_ref));

  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          InstrumentID,
          instru.data());
  req->Direction = Sgit_FTDC_D_Buy;
  req->LimitPrice = price;
  req->VolumeTotalOriginal = volume;

  req->TimeCondition = Sgit_FTDC_TC_IOC;
  req->VolumeCondition = Sgit_FTDC_VC_CV;

  try {
    orderGo(req.get());
  } catch (...) {
    throw std::runtime_error("order open buy FOK failed.");
  }

  return order_ref;
}

int TraderServiceImpl::orderOpenSell(
    const std::string& instru,
    double price, int volume) {
  FLYER_TRACE <<"TraderServiceImpl::orderOpenSell()";

  FLYER_DEBUG <<"instru: " <<instru
            <<"\t price: " <<price
            <<"\t volume: " <<volume;

  int order_ref = -1;

  std::unique_ptr<CSgitFtdcInputOrderField> req(
      orderField(&order_ref));

  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          InstrumentID,
          instru.data());
  req->Direction = Sgit_FTDC_D_Sell;
  req->LimitPrice = price;
  req->VolumeTotalOriginal = volume;

  try {
    orderGo(req.get());
  } catch (...) {
    throw std::runtime_error("order open sell failed.");
  }

  return order_ref;
}

int TraderServiceImpl::orderOpenSellFAK(
    const std::string& instru,
    double price, int volume) {
  FLYER_TRACE <<"TraderServiceImpl::orderOpenSellFAK()";

  FLYER_DEBUG <<"instru: " <<instru
            <<"\t price: " <<price
            <<"\t volume: " <<volume;

  int order_ref = -1;

  std::unique_ptr<CSgitFtdcInputOrderField> req(
      orderField(&order_ref));

  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          InstrumentID,
          instru.data());
  req->Direction = Sgit_FTDC_D_Sell;
  req->LimitPrice = price;
  req->VolumeTotalOriginal = volume;

  req->TimeCondition = Sgit_FTDC_TC_IOC;

  try {
    orderGo(req.get());
  } catch (...) {
    throw std::runtime_error("order open sell FAK failed.");
  }

  return order_ref;
}

int TraderServiceImpl::orderOpenSellFOK(
    const std::string& instru,
    double price, int volume) {
  FLYER_TRACE <<"TraderServiceImpl::orderOpenSellFOK()";

  FLYER_DEBUG <<"instru: " <<instru
            <<"\t price: " <<price
            <<"\t volume: " <<volume;

  int order_ref = -1;

  std::unique_ptr<CSgitFtdcInputOrderField> req(
      orderField(&order_ref));

  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          InstrumentID,
          instru.data());
  req->Direction = Sgit_FTDC_D_Sell;
  req->LimitPrice = price;
  req->VolumeTotalOriginal = volume;

  req->TimeCondition = Sgit_FTDC_TC_IOC;
  req->VolumeCondition = Sgit_FTDC_VC_CV;

  try {
    orderGo(req.get());
  } catch (...) {
    throw std::runtime_error("order open sell FOK failed.");
  }

  return order_ref;
}

int TraderServiceImpl::orderCloseBuy(
    const std::string& instru,
    double price, int volume) {
  FLYER_TRACE <<"TraderServiceImpl::orderCloseBuy()";

  FLYER_DEBUG <<"instru: " <<instru
            <<"\t price: " <<price
            <<"\t volume: " <<volume;

  int order_ref = -1;

  std::unique_ptr<CSgitFtdcInputOrderField> req(
      orderField(&order_ref));

  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          InstrumentID,
          instru.data());
  req->Direction = Sgit_FTDC_D_Buy;
  req->LimitPrice = price;
  req->VolumeTotalOriginal = volume;
  req->CombOffsetFlag[0] = Sgit_FTDC_OF_CloseToday;

  try {
    orderGo(req.get());
  } catch (...) {
    throw std::runtime_error("order close buy failed.");
  }

  return order_ref;
}

int TraderServiceImpl::orderCloseSell(
    const std::string& instru,
    double price, int volume) {
  FLYER_TRACE <<"TraderServiceImpl::orderCloseSell()";

  FLYER_DEBUG <<"instru: " <<instru
            <<"\t price: " <<price
            <<"\t volume: " <<volume;

  int order_ref = -1;

  std::unique_ptr<CSgitFtdcInputOrderField> req(
      orderField(&order_ref));

  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          InstrumentID,
          instru.data());
  req->Direction = Sgit_FTDC_D_Sell;
  req->LimitPrice = price;
  req->VolumeTotalOriginal = volume;
  req->CombOffsetFlag[0] = Sgit_FTDC_OF_CloseToday;

  try {
    orderGo(req.get());
  } catch (...) {
    throw std::runtime_error("order close sell failed.");
  }

  return order_ref;
}

int TraderServiceImpl::queryAccount() {
  FLYER_TRACE <<"TraderServiceImpl::queryAccount()";

  CSgitFtdcQryTradingAccountField req;
  memset(&req, 0x0, sizeof(req));

  S_INPUT(&req,
         CSgitFtdcQryTradingAccountField,
         BrokerID,
         options_->broker_id.data());
  S_INPUT(&req,
          CSgitFtdcQryTradingAccountField,
          InvestorID,
          options_->investor_id.data());

  FLYER_PDU <<req;

  int result = trader_api_->ReqQryTradingAccount(&req, ++request_id_);

  if (result != 0) {
    FLYER_ERROR <<"return code " <<result;
    throw std::runtime_error("query account failed.");
  }
}

void TraderServiceImpl::initSession(
    CSgitFtdcRspUserLoginField* pRspUserLogin) {
  front_id_ = pRspUserLogin->FrontID;
  session_id_ = pRspUserLogin->SessionID;

  max_order_ref_ = std::stoi(pRspUserLogin->MaxOrderRef);
}

void TraderServiceImpl::ready() {
  FLYER_TRACE <<"TraderServiceImpl::ready()";

  trader_api_->Ready();
}

void TraderServiceImpl::login() {
  FLYER_TRACE <<"TraderServiceImpl::login()";

  CSgitFtdcReqUserLoginField req;
  memset(&req, 0x0, sizeof(req));

  S_INPUT(&req,
          CSgitFtdcReqUserLoginField,
          TradingDay,
          tradingDay().data());
  S_INPUT(&req,
          CSgitFtdcReqUserLoginField,
          BrokerID,
          options_->broker_id.data());
  S_INPUT(&req,
          CSgitFtdcReqUserLoginField,
          UserID,
          options_->user_id.data());
  S_INPUT(&req,
          CSgitFtdcReqUserLoginField,
          Password,
          options_->password.data());

  FLYER_PDU <<req;

  int result = trader_api_->ReqUserLogin(&req, ++request_id_);

  if (result != 0) {
    FLYER_ERROR <<"return code " <<result;
    throw std::runtime_error("login failed.");
  }
}

void TraderServiceImpl::wait(const std::string& hint) {
  if (cond_->wait(2000)) {
    if (!hint.empty()) {
      throw std::runtime_error(hint + " time out");
    }
  }
}

void TraderServiceImpl::notify() {
  cond_->notifyAll();
}

CSgitFtdcInputOrderField* TraderServiceImpl::orderField(int* order_ref) {
  std::unique_ptr<CSgitFtdcInputOrderField> req(new CSgitFtdcInputOrderField());

  *order_ref = ++max_order_ref_;

  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          BrokerID,
          options_->broker_id.data());
  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          InvestorID,
          options_->investor_id.data());

  char OrderRef[13];
  snprintf(OrderRef, sizeof(OrderRef), "%012d", *order_ref);
  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          OrderRef,
          OrderRef);
  S_INPUT(req.get(),
          CSgitFtdcInputOrderField,
          UserID,
          options_->user_id.data());
  req->OrderPriceType = Sgit_FTDC_OPT_LimitPrice;

  // req->CombOffsetFlag[0] = SGIT_FTDC_OF_Open;
  req->CombOffsetFlag[0] = Sgit_FTDC_OF_Open;
  req->CombHedgeFlag[0] = Sgit_FTDC_OPT_AnyPrice;
  req->TimeCondition = Sgit_FTDC_TC_GFD;
  req->VolumeCondition = Sgit_FTDC_VC_AV;
  req->MinVolume = 1;
  req->ContingentCondition = Sgit_FTDC_CC_Immediately;
  req->ForceCloseReason = Sgit_FTDC_FCC_NotForceClose;
  req->IsAutoSuspend = 0;

  return req.release();
}

void TraderServiceImpl::orderGo(CSgitFtdcInputOrderField* req) {
  FLYER_TRACE <<"TraderServiceImpl::orderGo()";
  FLYER_PDU <<*req;

  int result = trader_api_->ReqOrderInsert(req, ++request_id_);

  if (result != 0) {
    FLYER_ERROR <<"return code " <<result;
    throw;
  }
}

soil::Options* TraderService::createOptions() {
  return new TraderOptions();
}

TraderService* TraderService::createService(
    soil::Options* options,
    TraderServiceCallback* callback) {
  return new TraderServiceImpl(options, callback);
}

}  // namespace flyer
