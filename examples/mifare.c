#include "mifare.h"

#include <string.h>

#include "chips/pn53x.h"

extern const byte_t pncmd_initiator_exchange_data       [265];

/**
 * @brief Execute a MIFARE Classic Command
 * @return Returns true if action was successfully performed; otherwise returns false.
 * @param pmp Some commands need additional information. This information should be supplied in the mifare_param union.
 *
 * The specified MIFARE command will be executed on the tag. There are different commands possible, they all require the destination block number.
 * @note There are three different types of information (Authenticate, Data and Value).
 *
 * First an authentication must take place using Key A or B. It requires a 48 bit Key (6 bytes) and the UID. 
 * They are both used to initialize the internal cipher-state of the PN53X chip (http://libnfc.org/hardware/pn53x-chip).
 * After a successful authentication it will be possible to execute other commands (e.g. Read/Write). 
 * The MIFARE Classic Specification (http://www.nxp.com/acrobat/other/identification/M001053_MF1ICS50_rev5_3.pdf) explains more about this process.
 */
bool nfc_initiator_mifare_cmd(nfc_device_t* pnd, const mifare_cmd mc, const uint8_t ui8Block, mifare_param* pmp)
{
  byte_t abtRx[MAX_FRAME_LEN];
  size_t szRxLen;
  size_t szParamLen;
  byte_t abtCmd[sizeof(pncmd_initiator_exchange_data)];
  memcpy(abtCmd,pncmd_initiator_exchange_data,sizeof(pncmd_initiator_exchange_data));

  // Make sure we are dealing with a active device
  if (!pnd->bActive) return false;

  abtCmd[2] = 0x01;     // Use first target/card
  abtCmd[3] = mc;       // The MIFARE Classic command
  abtCmd[4] = ui8Block; // The block address (1K=0x00..0x39, 4K=0x00..0xff)

  switch (mc)
  {
    // Read and store command have no parameter
    case MC_READ:
    case MC_STORE:
      szParamLen = 0;
    break;

    // Authenticate command
    case MC_AUTH_A:
    case MC_AUTH_B:
      szParamLen = sizeof(mifare_param_auth);
    break;

    // Data command
    case MC_WRITE:
      szParamLen = sizeof(mifare_param_data);
    break;

    // Value command
    case MC_DECREMENT:
    case MC_INCREMENT:
    case MC_TRANSFER:
      szParamLen = sizeof(mifare_param_value);
    break;

    // Please fix your code, you never should reach this statement
    default:
      return false;
    break;
  }

  // When available, copy the parameter bytes
  if (szParamLen) memcpy(abtCmd+5,(byte_t*)pmp,szParamLen);

  // Fire the mifare command
  if (!pn53x_transceive(pnd,abtCmd,5+szParamLen,abtRx,&szRxLen)) return false;

  // When we have executed a read command, copy the received bytes into the param
  if (mc == MC_READ && szRxLen == 17) memcpy(pmp->mpd.abtData,abtRx+1,16);

  // Command succesfully executed
  return true;
}
