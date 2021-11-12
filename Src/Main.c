/** 
 * \file Main.c
 *
 * Application entry point.
 * This file was automatically generated by IngeDev and must be filled out
 * by the developer.
 */

#include "SDK30.H"
#include "etat.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Prototype declaration to be used with INCENDO (replacing 'more_function' declared in SDK file 'etat.h').
// This new prototype can be used with SDK version >= 6.5.
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// extern int more_function_ext(NO_SEGMENT no, S_ETATOUT *in, void *out);

/** 
 * Application has to call ServiceRegister for each service with a same 
 *  address Main and using predefined service number.
 * \param    size (I-) data size.
 * \param    data (I-) contains data needed between the Manager and application regarding services.
 * 
 * \return service call status.
 *
 * \see sdk30.h
 *      etat.h
 */
int Main(unsigned int size, StructPt *data)
{
  NO_SEGMENT No;
  int ret = FCT_OK;

  // Service call management
  No = ApplicationGetCurrent(); // Return the application number
  switch (data->service)
  {
  case GIVE_YOUR_DOMAIN: // Return application domain to Manager
    ret = give_your_domain(No, NULL, &data->Param.GiveYourType.param_out);
    break;

  case AFTER_RESET: // Activated on each terminal reset
    ret = after_reset(No, NULL, &data->Param.AfterReset.param_out);
    break;

  case IS_NAME: // Activated when Manager wants to get application name
    ret = is_name(No, NULL, &data->Param.IsName.param_out);
    break;

  case IS_STATE: // Activated at boot and every minute to check if application is initialized
    ret = is_state(No, NULL, &data->Param.IsState.param_out);
    break;

  case IDLE_MESSAGE: // Activated when Manager goes back to idle, to display its message
    idle_message(No, NULL, NULL);
    break;

  case MORE_FUNCTION: // Activated on "F" key and dedicated to navigation menus
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // NOTE: other prototype variant 'int more_function_ext(NO_SEGMENT no, S_ETATOUT *in, void *out)'
    //       can be used with INCENDO.
    // This other prototype is used if the application manages more than one application name.
    // The 'S_ETATOUT' structure allows to know the name selected by the user after pressing the "F" key.
    // This new prototype can be used with SDK version >= 6.5.
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ret = more_function(No, NULL, NULL);
    break;

  case KEYBOARD_EVENT: // Activated when key is pressed
    ret = keyboard_event(No, &data->Param.KeyboardEvent.param_in,
        &data->Param.KeyboardEvent.param_out);
    break;

  case STATE: // Activated on "F" key: Consultation->State, to print terminal content receipt
    ret = state(No, NULL, NULL);
    break;

  case CONSULT: // Activated on "F" key: Consultation->Transactions, to print transaction totals receipt
    ret = consult(No, NULL, NULL);
    break;

  case MCALL: // Activated on "F" key: Consultation->Call->Planning of Call, to print host call planning receipt
    ret = mcall(No, NULL, NULL);
    break;

  case IS_TIME_FUNCTION: // Activated every minute, do you need the peripherals at the next call of time_function()?
    ret = is_time_function(No, NULL, &data->Param.IsTimeFunction.param_out);
    break;

  case TIME_FUNCTION: // Activated every minute, to execute automatic periodic functions
    ret = time_function(No, NULL, NULL);
    break;

  case IS_CHANGE_INIT: // Activated on "F" key: Initialization->Parameters->List, Conditions for changing Manager parameters?
    ret = is_change_init(No, NULL, &data->Param.IsChangeInit.param_out);
    break;

  case MODIF_PARAM: // Activated on "F" key: Initialization->Parameters->List, Manager reports parameters changed.
    ret = modif_param(No, &data->Param.ModifParam.param_in, NULL);
    break;

  case IS_EVOL_PG: // Activated on "F" key: Evolution->Load->Local or RemoteLoad, Conditions for application downloading?
    ret = is_evol_pg(No, NULL, &data->Param.IsEvolPg.param_out);
    break;

  case IS_DELETE: // Activated on "F" key: Deletion, Conditions for application deletion?
    ret = is_delete(No, NULL, &data->Param.IsDelete.param_out);
    break;

  case FILE_RECEIVED: // Activated each time Manager received a file from a "parameters" downloading session
    ret = file_received(No, &data->Param.FileReceived.param_in, NULL);
    break;

  case MESSAGE_RECEIVED: // Activated each time Manager received a message in its own mailbox for this application
    ret = message_received(No, &data->Param.MessageReceived.param_in, NULL);
    break;

  case IS_CARD_SPECIFIC: // Activated when card inserted card swiped or manually entry, do you want to process the card?
    ret = is_card_specific(No, &data->Param.IsCardSpecific.param_in,
        &data->Param.IsCardSpecific.param_out);
    break;

  case CARD_INSIDE: // Activated when the card is specific, the application process the card in transparent mode
    ret = card_inside(No, &data->Param.CardInside.param_in,
        &data->Param.CardInside. param_out);
    break;

  case IS_FOR_YOU_AFTER:
    ret = is_for_you_after(No, &data->Param.IsForYouAfter.param_in,
        &data->Param.IsForYouAfter.param_out);
    break;

  case DEBIT_NON_EMV:
    ret = debit_non_emv(No, &data->Param.DebitNonEmv.param_in,
        &data->Param.DebitNonEmv.param_out);
    break;

  case IS_FOR_YOU_BEFORE: // Activated when chip card inserted, ask application to recognise the chip card in order to a candidate
  case TIME_FUNCTION_CHAINE: // French Bank Domain
  case GIVE_INFOS_CX: // French Bank Domain
  case FALL_BACK:
  case DEBIT_OVER:
  case AUTO_OVER:
  case IS_ORDER: // French Health Care Domain
  case ORDER: // French Health Care Domain
  case IS_SUPPR_PG: // French Health Care Domain
  case IS_INSTALL_PG: // French Health Care Domain
  case GET_ORDER: // French Health Care Domain
  case IS_LIBELLE: // French Health Care Domain
  case EVOL_CONFIG: // French Bank Domain
  case GIVE_MONEY: // French Bank Domain
  case COM_EVENT:
  case MODEM_EVENT:
  case GIVE_INTERFACE:
  case IS_BIN_CB: // French Bank Domain
  case GIVE_AID:
  case IS_CARD_EMV_FOR_YOU:
  case DEBIT_EMV:
  case SELECT_FUNCTION: // French Bank Domain
  case SELECT_FUNCTION_EMV: // French Bank Domain
  default:
    break;
  }

  return ret;
}

